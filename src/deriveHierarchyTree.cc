
#include "include/deriveHierarchyTree.h"

std::map<std::string, Node>::iterator Tree::getMapEnd(){
    return this->parentNodeMap.end();
}

std::map<std::string, Node>::iterator Tree::findNodeInMap(Node pNode){
    return this->parentNodeMap.find(pNode.getModuleName());
}

Node * Tree::getMapElem(std::string key){
    Node *pNodePtr;
    pNodePtr = &this->parentNodeMap.at(key);
    return pNodePtr;
}

Node * Tree::getTreeRootNodeAtIndex(int index){
    Node *pNodePtr;
    pNodePtr = &this->treeRoots.at(index);
    return pNodePtr;
}

int Tree::getTreeRootSize(){
    return this->treeRoots.size();
}

void Tree::pushTreeRoot(Node pNode){
    this->treeRoots.push_back(pNode);
}

void Tree::setMap(std::map<std::string, Node> pNodeMap){
    this->parentNodeMap = pNodeMap;
}

void Tree::setParentNodes(std::vector<Node> pNodes){
    this->parentNodes = pNodes;
}

Node * Tree::getParentNodeAtIndex(int index){
    Node *pNodePtr;
    pNodePtr = &this->parentNodes.at(index);
    return pNodePtr;
}

int Tree::getParentNodesSize(){
    return this->parentNodes.size();
}

bool Node::getIsInstantiated(){
    return this->isInstantiated;
}

// this means that the instance is instantiated by a higher level module
// used to determine the top level module(s) in the input rtl files
void Node::setIsInstantiated(){
    this->isInstantiated = true;
}

void Node::setModuleName(std::string str){
    this->moduleName = str;
}

void Node::setInstName(std::string str){
    this->instName = str;
}

std::string Node::getInstName(){
    return this->instName;
}

std::string Node::getModuleName(){
    return this->moduleName;
}

void Node::pushChildNode(Node cNode){
    this->childNodes.push_back(cNode);
}

Node * Node::getChildNodeAtIndex(int index){
    Node *cNodePtr;
    cNodePtr = &this->childNodes.at(index);
    return cNodePtr;
}

int Node::getChildNodesSize(){
    return this->childNodes.size();
}

// *** NOTE:
// I do not like the current tokenising code, maybe refactor it from here.

// given the raw regex match from the RTL files, strip whitespace and store the two text words
void tokeniseString(std::string str, std::vector<std::string> *tokenisedStringPtr, bool superDebug){
    // loop through str and push the sub-words to tokenisedStringPtr
    int indexStart = 0;
    bool indexStartAssigned = false;
    std::string substr;
    std::size_t found;
    for(int i = 0; i < str.size(); i++){
        if(str[i] != ' ' && !indexStartAssigned){
            indexStart = i;
            indexStartAssigned = true;
        }
        else if((str[i] == '(' || str[i] == '#' || str[i] == ' ') && indexStartAssigned){
            substr = str.substr(indexStart, i);

            if(superDebug) { std::cout << "substr before trim: " << '<' << substr << '>' << std::endl; }
            for( ; ; ){
                found = substr.find_first_of(" #(");
                // found an unwanted char, delete it
                if(found != std::string::npos){
                    substr.erase(found);
                }
                else{
                    break;
                }
            }
            if(superDebug) { std::cout << "substr after trim: " << '<' << substr << '>' << std::endl; }
            tokenisedStringPtr->push_back(substr);
            indexStart = 0;
            indexStartAssigned = false;
        }
    }
}

// TODO: print out somewhere in the debug output, the total number of modules found. This is a good sanity check number
// TODO: need a systematic way to find false modules (they appear as child nodes but not parent nodes!) and print this out in an error message...
//       v0.1.0 should be pretty full proof and should not print out any false modules

void parseRtl(std::vector<std::string> rtlFiles, std::vector<Node> *parentNodeVecPtr, std::regex parentNodeRegexStr, std::regex childNodeRegexStr, std::map<std::string, Node> *pNodeMapPtr, bool debug){

    std::fstream rtlFileObj;
    std::string line;
    std::string moduleName;
    std::string instName;
    std::string childModuleName;
    std::string childInstName;
    std::vector<std::string> tokenisedString;
    std::vector<std::string> *tokenisedStringPtr = &tokenisedString;
    std::smatch matchObjParent;
    std::smatch matchObjChild;
    Node tmpNode;
    Node *tmpNodePtr;

    std::map<std::string, Node> tmpNodeMap;

    int parentNodeSize;
    int numlines = 0;
    bool caughtFalseModule = false;

    tmpNodePtr = &tmpNode;

    for(int i = 0; i < rtlFiles.size(); i++){
        // open the next file in the list
        rtlFileObj.open(rtlFiles.at(i));
        // read line-by-line and apply the regex search pattern
        for(std::string line; getline(rtlFileObj, line); numlines++){
            // caughtFalseModule = false;
            
            // check for a parent-node match
            std::regex_search(line, matchObjParent, parentNodeRegexStr);
            std::regex_search(line, matchObjChild,  childNodeRegexStr);
            // found a parent node
            if(matchObjParent.size() == 1){
                // tokenise the parent node string, splitting on arbitrary number of space chars
                tokenisedStringPtr->clear();
                tokeniseString(matchObjParent.str(), tokenisedStringPtr, false);
                moduleName = tokenisedStringPtr->at(1);

                if(debug){
                    std::cout << "Module definition found in file " << rtlFiles.at(i) << ": \"" << matchObjParent.str() << "\" with module name: \"";
                    std::cout << moduleName << "\" " << std::endl;
                }
                // create the parent Node object
                Node curr0;
                curr0.setModuleName(moduleName);
                parentNodeVecPtr->push_back(curr0);
                // add an entry to the hash table of parent nodes
                // pNodeMapPtr->insert(std::pair<std::string, Node>(moduleName, curr0));
                tmpNodeMap[moduleName] = curr0;
            }
            // found a child node
            else if(matchObjChild.size() == 1){
                // tokenise the child node string, splitting on arbitrary number of space chars
                tokenisedStringPtr->clear();
                tokeniseString(matchObjChild.str(), tokenisedStringPtr, false);
                // moduleName = tokenisedStringPtr->at(0);
                instName = tokenisedStringPtr->at(1);
                
                caughtFalseModule = (tokenisedStringPtr->at(0) == "unique") && (tokenisedStringPtr->at(1) == "case" || tokenisedStringPtr->at(1) == "casez");

                if(!caughtFalseModule){
                    if(debug){
                        std::cout << "  Instantiated module \"" << tokenisedStringPtr->at(0) << "\" found in file " << rtlFiles.at(i) << ": \"" << matchObjChild.str() << "\" with instance name: \"";
                        std::cout << instName << "\" " << std::endl;
                    }

                    // create the child Node object
                    Node curr1;
                    curr1.setModuleName(tokenisedStringPtr->at(0));
                    curr1.setInstName(tokenisedStringPtr->at(1));
                    // curr1.setModuleName(moduleName);
                    // curr1.setInstName(instName);

                    // all child nodes are instantiated, only parent nodes are not
                    // I was hoping this would eliminate the need for the first for loop in elaborateHierarchyTree ?
                    // curr.setIsInstantiated();
                    
                    // the last parent node is the current one, push the associated child nodes
                    tmpNodePtr = &parentNodeVecPtr->back();
                    // pNodeName = tmpNodePtr->getModuleName();
                    tmpNodePtr->pushChildNode(curr1);

                    // add/update the entry in the hash table of the respective parent node
                    // pNodeMapPtr->insert(std::pair<std::string, Node>(moduleName, *tmpNodePtr));
                    tmpNodeMap[moduleName] = *tmpNodePtr;
                }
            }
        }
        rtlFileObj.close();
        *pNodeMapPtr = tmpNodeMap;
    }
    if(debug){
        std::cout << parentNodeVecPtr->size() << " modules found, " << numlines << " lines parsed..." << std::endl;
    }
}

// recursively go down the hierarchy of Nodes and assign child nodes
void constructTreeRecursively(Node *pNodePtr, Tree *hTreePtr, bool debug, std::vector<std::string> noIncModules, int level){

    Node cNode;
    Node *cNodePtr;
    Node tmpNode;
    // will need to manually update instance name when swapping child for parent node
    std::string tmpInstName;
    cNodePtr = &cNode;
    bool skip = false;

    // check if we've reached the arbitrary maximum depth of 100, to avoid circular hierarchy segfault
    if(level >= 100){
        std::cout << std::endl << "*** Error!" << std::endl;
        std::cout << "Maximum hierarchy depth reached, possible ciruclar hierarchy? Run again with '--debug' flag for more information" << std::endl << std::endl;
        std::cout << "Exiting..." << std::endl;
        exit(-1);
    }

    for(int i = 0; i < pNodePtr->getChildNodesSize(); i++){
        cNodePtr = pNodePtr->getChildNodeAtIndex(i);
        // TODO: this would be more efficient as a lookup table rather than a linear search
        // check if the current module is a module to ignore
        for(int k = 0; k < noIncModules.size(); k++){
            if(cNodePtr->getModuleName() == noIncModules.at(k)){
                skip = true;
                break;
            }
        }
        // if we get the signal, skip this iteration of the loop and don't add to tree
        if(skip){
            skip = false;
            // std::cout << "Skipping..." << std::endl;
            continue;
        }
        tmpInstName = cNodePtr->getInstName();
        tmpNode = *hTreePtr->getMapElem(cNodePtr->getModuleName());
        *cNodePtr = tmpNode;
        // in overwriting the child node with a parent node, we lose the instance name so update it here
        cNodePtr->setInstName(tmpInstName);
        if(debug){
            std::cout << "  At level: " << pNodePtr->getModuleName() << " w/ # children: " << pNodePtr->getChildNodesSize() << std::endl;
            std::cout << "  Child   : " << cNodePtr->getModuleName() << " w/ # children: " << cNodePtr->getChildNodesSize() << std::endl;
        }
        if(cNodePtr->getChildNodesSize() > 0){
            if(debug){
                std::cout << "Recursing..." << std::endl;
            }
            constructTreeRecursively(cNodePtr, hTreePtr, debug, noIncModules, ++level);
        }
    }
    if(debug){
        std::cout << "Going up..." << std::endl;
    }
}

// main algorithm for elaborating the tree of parent nodes
void elaborateHierarchyTree(Tree *hTreePtr, bool debug, std::vector<std::string> noIncModules){
    // algorithm:
    // - the hash table allows for a lookup of a module, given the module name. Returns the ParentNode object
    // - iterate over the hTree parent nodes and assign child nodes as instantiated using the isInstantiated bool
    // - can then figure out the top level modules by looping through and finding ones with isInstantiated = false
    // - can then enter into main algorithm loop where you recursively go down the hierarchy, assembling the tree

    Node pNode;
    Node *pNodePtr;
    Node cNode;
    Node *cNodePtr;
    Node tmpNode;
    Node *tmpNodePtr;
    // create an iterator for the map
    std::map<std::string, Node>::iterator iter;
    int pNodeNumChilds;
    int parentNodeSize = hTreePtr->getParentNodesSize();
    int childNodeSize;
    int treeRootSize;
    int cNodeIndex = 0;
    bool testInstantiated;

    pNodePtr = &pNode;
    cNodePtr = &cNode;
    tmpNodePtr = &tmpNode;

    // TODO: could alternatively just loop until we find a module that should not be included, and simply 'delete' it 
    //       from the parent node vector
    
    for(int i = 0; i < parentNodeSize; i++){
        pNodePtr = hTreePtr->getParentNodeAtIndex(i);
        pNodeNumChilds = pNodePtr->getChildNodesSize();
        for(int j = 0; j < pNodeNumChilds; j++){
            cNodePtr = pNodePtr->getChildNodeAtIndex(j);
            // check if the module exists in the map, if not generate an error and exit
            iter = hTreePtr->findNodeInMap(*cNodePtr);
            if(iter == hTreePtr->getMapEnd()){
                std::cout << "*** Internal Error: Tried performing a lookup for a module that doesn't exist! " << std::endl;
                std::cout << "Make sure all RTL files are supplied to verilogtree" << std::endl;
                std::cout << std::endl << "Please report on GitHub: https://github.com/LiamSkirrow/verilogtree/" << std::endl;
                exit(-1);
            }
            // mark the parent node as instantiated
            hTreePtr->getMapElem(cNodePtr->getModuleName())->setIsInstantiated();
        }
    }

    // find the top level modules and push to Tree's tree root vector
    for(int i = 0; i < parentNodeSize; i++){
        pNodePtr = hTreePtr->getParentNodeAtIndex(i);
        testInstantiated = hTreePtr->getMapElem(pNodePtr->getModuleName())->getIsInstantiated();
        if(!testInstantiated){
            // now add the top level modules to the Tree's root nodes vector
            hTreePtr->pushTreeRoot(*pNodePtr);
        }
    }
    // assemble the final tree, starting at the tree roots
    treeRootSize = hTreePtr->getTreeRootSize();
    // std::cout << "tree root size: " << treeRootSize << std::endl;
    for(int i = 0; i < treeRootSize; i++){
        pNodePtr = hTreePtr->getTreeRootNodeAtIndex(i);
        pNodeNumChilds = pNodePtr->getChildNodesSize();
        if(debug){
            std::cout << "Tree Root: " << pNodePtr->getModuleName() << " w/ # children: " << pNodeNumChilds << std::endl;
        }
        // if not stub tree node with no children
        if(pNodeNumChilds > 0){
            // just use recursion, it's waaaaay easier... can refactor it later if memory usage becomes a concern
            // check with a monitor program to view ram usage, just out of curiosity.
            // - replace below with function call
            // - within the function, have a for loop that iterates over the child nodes, and if a child node exists,
            //   then recurse down, and keep recursing until a node with no children is found (leaf node) at which point, return
            // - after returning upwards, the next child node at that level will be entered into ... etc until the tree
            //   is constructed DFS style

            constructTreeRecursively(pNodePtr, hTreePtr, debug, noIncModules, 0);
        }
    }
}

// top level function, dispatch the rtl parsing and tree construction functions, return the Tree to main
Tree deriveHierarchyTree(Tree *hTreePtr, std::vector<std::string> rtlFiles, std::regex parentNodeRegexStr, std::regex childNodeRegexStr, bool debug, std::vector<std::string> noIncModules){

    Tree hTree;
    // Tree *hTreePtr;
    std::vector<Node> parentNodeVec;
    std::vector<Node> *parentNodeVecPtr;
    // a hash table of Nodes, looked up using their module name
    std::map<std::string, Node> pNodeMap;
    std::map<std::string, Node> *pNodeMapPtr;

    parentNodeVecPtr = &parentNodeVec;
    hTreePtr         = &hTree;
    pNodeMapPtr      = &pNodeMap;


    // parse the RTL according to the regex strings. Create distinct parent-child node groups
    parseRtl(rtlFiles, parentNodeVecPtr, parentNodeRegexStr, childNodeRegexStr, pNodeMapPtr, debug);

    // int pNodeMapSize = pNodeMap[parentNodeVecPtr->at(0).getModuleName()].getChildNodesSize();
    std::string debugModuleName;
    std::string debugInstName;

    // FIXME: when the below for loop is not executed (debug == false) the pNodeMap lookup never happens, this is what is causing the 
    //        discrepency between running on the ibex rtl codebase with --debug either defined or not defined

    if(debug){
        for(int i = 0; i < parentNodeVecPtr->size(); i++){
            std::cout << "Parent Node Name: " << parentNodeVecPtr->at(i).getModuleName() << std::endl;
            for(int j = 0; j < parentNodeVecPtr->at(i).getChildNodesSize(); j++){
                debugModuleName = parentNodeVecPtr->at(i).getChildNodeAtIndex(j)->getModuleName();
                debugInstName   = parentNodeVecPtr->at(i).getChildNodeAtIndex(j)->getInstName();
                std::cout << "    Child Node Module  : " << debugModuleName << std::endl;
                std::cout << "    Child Node Instance: " << debugInstName << std::endl;
                std::cout << "    Num Children:        " << pNodeMap[parentNodeVecPtr->at(i).getChildNodeAtIndex(j)->getModuleName()].getChildNodesSize() << std::endl;
            }
        }
    }
    
    // assign the parent node's vector to the main tree
    hTreePtr->setParentNodes(*parentNodeVecPtr);
    hTreePtr->setMap(*pNodeMapPtr);

    // now (recursively?) replace all child nodes with parent nodes to construct the tree
    elaborateHierarchyTree(hTreePtr, debug, noIncModules);

    return *hTreePtr;

}
