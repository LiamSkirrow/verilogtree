
#include "include/deriveHierarchyTree.h"

/* NOTE: does Verilog allow nested modules? Like...
-> if this is standard Verilog, create a GitHub issue for this and just deal with it in a future minor release
module mod0(...);
    module mod1(...);
        mod2 mod2_inst(...);
    endmodule
endmodule
*/

bool Node::getIsInstantiated(){
    return this->isInstantiated;
}

// this means that the instance is instantiated by a higher level module
// used to determine the top level module(s) in the input rtl files
void Node::setIsInstantiated(){
    this->isInstantiated = true;
}

Node * Tree::getMapElem(std::string key){
    Node *pNodePtr;
    pNodePtr = &this->parentNodeMap.at(key);
    return pNodePtr;
}

Node Tree::getTreeRootNodeAtIndex(int index){
    return this->treeRoots.at(index);
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

Node * Tree::getParentNodeAtIndex(int index){
    Node *pNodePtr;
    pNodePtr = &this->parentNodes.at(index);
    return pNodePtr;
}

int Tree::getParentNodesSize(){
    return this->parentNodes.size();
}

void Tree::setParentNodes(std::vector<Node> pNodes){
    this->parentNodes = pNodes;
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
                pNodeMapPtr->insert(std::pair<std::string, Node>(moduleName, curr));
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
                parentNodeVecPtr->back().pushChildNode(curr);
                
            }
        }
        rtlFileObj.close();
    }
}

// main algorithm for elaborating the tree of parent nodes
void elaborateHierarchyTree(Tree *hTreePtr){
    // algorithm:
    // - the hash table allows for a lookup of a module, given the module name. Returns the ParentNode object
    // - iterate over the hTree parent nodes and assign child nodes as instantiated using the isInstantiated bool
    // - can then figure out the top level modules by looping through and finding ones with isInstantiated = false
    // - can then enter into main algorithm loop where you iteratively go down the hierarchy, assembling the tree

    Node pNode;
    Node *pNodePtr;
    Node cNode;
    Node *cNodePtr;
    int pNodeNumChilds;
    int parentNodeSize = hTreePtr->getParentNodesSize();
    int childNodeSize;
    int treeRootSize;
    bool testInstantiated;

    pNodePtr = &pNode;
    cNodePtr = &cNode;

    // TODO: the below methods have to return a pointer, rather than a copy
    
    for(int i = 0; i < parentNodeSize; i++){
        pNodePtr = hTreePtr->getParentNodeAtIndex(i);
        pNodeNumChilds = pNodePtr->getChildNodesSize();
        // std::cout << "Parent Module Name: " << pNodePtr->getModuleName() << ", num children: " << pNodeNumChilds << std::endl;
        for(int j = 0; j < pNodeNumChilds; j++){
            cNodePtr = pNodePtr->getChildNodeAtIndex(j);
            // mark the parent node as instantiated
            // std::cout << "  Child Module Name: " << cNodePtr->getModuleName() << std::endl;
            hTreePtr->getMapElem(cNodePtr->getModuleName())->setIsInstantiated();
            // std::cout << "  Map lookup: " << hTreePtr->getMapElem(cNodePtr->getModuleName())->getModuleName();
            // std::cout << ", instantiated: " << hTreePtr->getMapElem(cNodePtr->getModuleName())->getIsInstantiated() << std::endl;
        }
    }

    // find the top level modules and push to Tree's tree root vector
    for(int i = 0; i < parentNodeSize; i++){
        pNodePtr = hTreePtr->getParentNodeAtIndex(i);
        testInstantiated = hTreePtr->getMapElem(pNodePtr->getModuleName())->getIsInstantiated();
        if(!testInstantiated){
            std::cout << "Detected top level module: " << pNodePtr->getModuleName() << std::endl;
            // now add the top level modules to the Tree's root nodes vector
            hTreePtr->pushTreeRoot(*pNodePtr);
        }
        // else{
        //     std::cout << "Detected child module: " << pNodePtr->getModuleName() << std::endl;
        // }
    }

    // assemble the final tree, starting at the tree roots
    treeRootSize = hTreePtr->getTreeRootSize();
    // std::cout << "tree root size: " << treeRootSize << std::endl;
    for(int i = 0; i < treeRootSize; i++){
        pNode = hTreePtr->getTreeRootNodeAtIndex(i);
        pNodeNumChilds = pNode.getChildNodesSize();
        for(int j = 0; j < pNodeNumChilds; j++){
            // TODO:
            // TODO: UP TO HERE !!!
            // TODO:
        }
    }

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

    if(debug){
        for(int i = 0; i < parentNodeVecPtr->size(); i++){
            std::cout << "Parent Node Name: " << parentNodeVecPtr->at(i).getModuleName() << std::endl;
            for(int j = 0; j < parentNodeVecPtr->at(i).getChildNodesSize(); j++){
                std::cout << "    Child Node Module  : " << parentNodeVecPtr->at(i).getChildNodeAtIndex(j)->getModuleName() << std::endl;
                std::cout << "    Child Node Instance: " << parentNodeVecPtr->at(i).getChildNodeAtIndex(j)->getInstName() << std::endl;
            }
        }
    }

    // assign the parent nodes vector to the main tree
    hTreePtr->setParentNodes(*parentNodeVecPtr);
    hTreePtr->setMap(*pNodeMapPtr);

    // now (recursively?) replace all child nodes with parent nodes to construct the tree
    elaborateHierarchyTree(hTreePtr);

    // TODO: add a 'code refactoring' label 

    return *hTreePtr;

}
