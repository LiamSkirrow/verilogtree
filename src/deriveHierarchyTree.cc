
#include "include/deriveHierarchyTree.h"

/* NOTE: does Verilog allow nested modules? Like...
-> if this is standard Verilog, create a GitHub issue for this and just deal with it in a future minor release
module mod0(...);
    module mod1(...);
        mod2 mod2_inst(...);
    endmodule
endmodule
*/

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

// TODO: replace hardcoded iterations with C++ STL iterators ??? 

// *** NOTE:
// I do not like the current tokenising code, maybe refactor it from here.

// TODO: include a --superdebug flag as well for a ton of debugging information
//       enabling --superdebug should also enable --debug automatically in the args parsing stage

// *** TODO: do I need to handle tab characters? Not handling newlines for now!
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

// TODO: might be cool to include how many lines of verilog/systemverilog were parsed at the very end, add to debug output?

void parseRtl(std::vector<std::string> rtlFiles, std::vector<Node> *parentNodeVecPtr, std::regex parentNodeRegexStr, std::regex childNodeRegexStr, std::map<std::string, Node> *pNodeMapPtr, bool debug){

    std::fstream rtlFileObj;
    std::string line;
    std::string moduleName;
    std::string childModuleName;
    std::string childInstName;
    std::vector<std::string> tokenisedString;
    std::vector<std::string> *tokenisedStringPtr = &tokenisedString;
    std::smatch matchObjParent;
    std::smatch matchObjChild;
    Node tmpNode;
    Node *tmpNodePtr;

    // std::string pNodeName;

    std::map<std::string, Node> tmpNodeMap;

    int parentNodeSize;

    tmpNodePtr = &tmpNode;

    for(int i = 0; i < rtlFiles.size(); i++){
        // open the next file in the list
        rtlFileObj.open(rtlFiles.at(i));
        // read line-by-line and apply the regex search pattern
        for(std::string line; getline(rtlFileObj, line); ){
            // std::cout << "line: " << line << std::endl;
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
                    std::cout << "Parent node found in file " << rtlFiles.at(i) << ": \"" << matchObjParent.str() << "\" with module name: \"";
                    std::cout << moduleName << "\" " << std::endl;
                }
                // create the parent Node object
                Node curr;
                curr.setModuleName(moduleName);
                parentNodeVecPtr->push_back(curr);
                // add an entry to the hash table of parent nodes
                // pNodeMapPtr->insert(std::pair<std::string, Node>(moduleName, curr));
                tmpNodeMap[moduleName] = curr;
            }
            // found a child node
            else if(matchObjChild.size() == 1){
                // tokenise the child node string, splitting on arbitrary number of space chars
                tokenisedStringPtr->clear();
                tokeniseString(matchObjChild.str(), tokenisedStringPtr, false);
                // create the child Node object
                Node curr;
                curr.setModuleName(tokenisedStringPtr->at(0));
                curr.setInstName(tokenisedStringPtr->at(1));
                // all child nodes are instantiated, only parent nodes are not
                // I was hoping this would eliminate the need for the first for loop in elaborateHierarchyTree ?
                // curr.setIsInstantiated();
                
                // the last parent node is the current one, push the associated child nodes
                tmpNodePtr = &parentNodeVecPtr->back();
                // pNodeName = tmpNodePtr->getModuleName();
                tmpNodePtr->pushChildNode(curr);

                // add/update the entry in the hash table of the respective parent node
                // pNodeMapPtr->insert(std::pair<std::string, Node>(moduleName, *tmpNodePtr));
                tmpNodeMap[moduleName] = *tmpNodePtr;

                // parentNodeSize = parentNodeVecPtr->size();
                // // get a reference to the last entry
                // tmpNodePtr = &parentNodeVecPtr->at(parentNodeSize-1);
                // tmpNodePtr->pushChildNode(curr);
            }
        }
        rtlFileObj.close();
        *pNodeMapPtr = tmpNodeMap;
    }
}

// TODO: the -L flag could be used to simply limit how many levels the tree goes, rather than
//       only printing so far...

// TODO: add "stage 1","stage 2" in debug output text and format it nicely with blank spaces between each section

// recursively go down the hierarchy of Nodes and assign child nodes
void constructTreeRecursively(Node *pNodePtr, Tree *hTreePtr, bool debug){

    Node cNode;
    Node *cNodePtr;
    Node tmpNode;

    cNodePtr = &cNode;

    if(pNodePtr->getChildNodeAtIndex(0)->getModuleName() == "mod0"){
        // pNodePtr->getChildNodeAtIndex(0)->setModuleName("mod0Moodified");
    }

    for(int i = 0; i < pNodePtr->getChildNodesSize(); i++){
        cNodePtr = pNodePtr->getChildNodeAtIndex(i);
        tmpNode = *hTreePtr->getMapElem(cNodePtr->getModuleName());
        *cNodePtr = tmpNode;
        if(debug){
            std::cout << "  At level: " << pNodePtr->getModuleName() << " w/ # children: " << pNodePtr->getChildNodesSize() << std::endl;
            std::cout << "  Child   : " << cNodePtr->getModuleName() << " w/ # children: " << cNodePtr->getChildNodesSize() << std::endl;
        }
        
        // FIXME: might not be adding the leaf nodes to the structure via *cNodePtr = tmpNode;
        //        this might be the reason why the final level of the hierarchy isn't being reached in the printout
        
        if(cNodePtr->getChildNodesSize() > 0){
            if(debug){
                std::cout << "recursing..." << std::endl;
            }
            constructTreeRecursively(cNodePtr, hTreePtr, debug);
        }
    }
    if(debug){
        std::cout << "going up..." << std::endl;
    }

    // std::cout << std::endl << "Check in recursion: " << hTreePtr->getTreeRootNodeAtIndex(1)->getChildNodeAtIndex(0)->getChildNodesSize() << std::endl << std::endl;

}

// main algorithm for elaborating the tree of parent nodes
void elaborateHierarchyTree(Tree *hTreePtr, bool debug){
    // algorithm:
    // - the hash table allows for a lookup of a module, given the module name. Returns the ParentNode object
    // - iterate over the hTree parent nodes and assign child nodes as instantiated using the isInstantiated bool
    // - can then figure out the top level modules by looping through and finding ones with isInstantiated = false
    // - can then enter into main algorithm loop where you iteratively go down the hierarchy, assembling the tree

    Node pNode;
    Node *pNodePtr;
    Node cNode;
    Node *cNodePtr;
    Node tmpNode;
    Node *tmpNodePtr;
    int pNodeNumChilds;
    int parentNodeSize = hTreePtr->getParentNodesSize();
    int childNodeSize;
    int treeRootSize;
    int cNodeIndex = 0;
    bool testInstantiated;

    pNodePtr = &pNode;
    cNodePtr = &cNode;
    tmpNodePtr = &tmpNode;

    // TODO: the below methods have to return a pointer, rather than a copy
    
    for(int i = 0; i < parentNodeSize; i++){
        pNodePtr = hTreePtr->getParentNodeAtIndex(i);
        pNodeNumChilds = pNodePtr->getChildNodesSize();
        // std::cout << "Parent Module Name: " << pNodePtr->getModuleName() << ", num children: " << pNodeNumChilds << std::endl;
        for(int j = 0; j < pNodeNumChilds; j++){
            cNodePtr = pNodePtr->getChildNodeAtIndex(j);
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

            constructTreeRecursively(pNodePtr, hTreePtr, debug);
            
        }
    }

    std::cout << std::endl << "Check: " << hTreePtr->getTreeRootNodeAtIndex(0)->getChildNodeAtIndex(0)->getChildNodesSize() << std::endl << std::endl;

}

// top level function, dispatch the rtl parsing and tree construction functions, return the Tree to main
Tree deriveHierarchyTree(Tree *hTreePtr, std::vector<std::string> rtlFiles, std::regex parentNodeRegexStr, std::regex childNodeRegexStr, bool debug){

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

    // TODO: to check the regex parsing strings, pass in a TON of real-world open source RTL files
    //       and print out the internal database of parent nodes and child nodes, should be 
    //       easy to find any errors or verify correct working functionality this way
    // TODO: add ^^^ to an issue with the label 'Testing'

    // what about modules defined on the same line separated by a ; ?
    // -> mod0 mod0_inst(); mod1 mod1_inst(); ????
    // just remember to document the supported syntax for module declarations
    // include examples in README...

    // parse the RTL according to the regex strings. Create distinct parent-child node groups
    parseRtl(rtlFiles, parentNodeVecPtr, parentNodeRegexStr, childNodeRegexStr, pNodeMapPtr, debug);

    std::string debugModuleName;
    std::string debugInstName;

    if(debug){
        for(int i = 0; i < parentNodeVecPtr->size(); i++){
            std::cout << "Parent Node Name: " << parentNodeVecPtr->at(i).getModuleName() << std::endl;
            for(int j = 0; j < parentNodeVecPtr->at(i).getChildNodesSize(); j++){
                debugModuleName = parentNodeVecPtr->at(i).getChildNodeAtIndex(j)->getModuleName();
                debugInstName   = parentNodeVecPtr->at(i).getChildNodeAtIndex(j)->getInstName();
                std::cout << "    Child Node Module  : " << debugModuleName << std::endl;
                std::cout << "    Child Node Instance: " << debugInstName << std::endl;
                // std::cout << "    Num Children:        " << parentNodeVecPtr->at(i).getChildNodeAtIndex(j)->getChildNodesSize() << std::endl;
                std::cout << "    Num Children:        " << pNodeMap[parentNodeVecPtr->at(i).getChildNodeAtIndex(j)->getModuleName()].getChildNodesSize() << std::endl;
            }
        }
    }

    // assign the parent nodes vector to the main tree
    hTreePtr->setParentNodes(*parentNodeVecPtr);
    hTreePtr->setMap(*pNodeMapPtr);

    // now (recursively?) replace all child nodes with parent nodes to construct the tree
    elaborateHierarchyTree(hTreePtr, debug);

    // TODO: add a 'code refactoring' label 

    return *hTreePtr;

}
