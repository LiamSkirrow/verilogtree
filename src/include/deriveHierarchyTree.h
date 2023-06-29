// header file for parseUserArgs.cc

#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <regex>
#include <cstddef>

// child nodes
// class ChildNode{
//     std::string moduleName;
//     std::string instanceName;

//     public:
//         void setModuleName(std::string str);
//         void setInstName(std::string str);
//         std::string getModuleName();
//         std::string getInstName();
// };

// node representing a Verilog module
class Node{
    std::vector<Node> childNodes;
    std::string moduleName;
    std::string instName;
    bool isInstantiated = false;

    public:
        void setModuleName(std::string name);
        void setInstName(std::string str);
        std::string getModuleName();
        std::string getInstName();
        void pushChildNode(Node cNode);
        Node * getChildNodeAtIndex(int index);
        int getChildNodesSize();
        void setIsInstantiated();
        bool getIsInstantiated();
};

// main tree abstract data type
class Tree{
    std::vector<Node> parentNodes;
    std::map<std::string, Node> parentNodeMap;

    // the ParentNode representing the top level module
    std::vector<Node> treeRoots;

    public:
        void setParentNodes(std::vector<Node> pNodes);
        Node * getParentNodeAtIndex(int index);
        int getParentNodesSize();
        void setMap(std::map<std::string, Node> pNodeMap);
        Node * getMapElem(std::string key);
        void pushTreeRoot(Node pNode);            // should this be a pointer to a ParentNode ???
        Node * getTreeRootNodeAtIndex(int index);   // should this be a pointer to a ParentNode ???
        int getTreeRootSize();
};

Tree deriveHierarchyTree(Tree *hierarchyTreePtr, std::vector<std::string> rtlFiles, std::regex parentNodeRegexStr, std::regex childNodeRegexStr, bool debug);
void parseRtl(std::vector<std::string> rtlFiles, std::vector<Node> *parentNodeVecPtr, std::regex parentNodeRegexStr, std::regex childNodeRegexStr, std::map<std::string, Node> *pNodeMapPtr, bool debug);
void elaborateHierarchyTree(Tree *hTreePtr);
void tokenizeString(std::string str, std::string *tokenisedStringPtr);
void constructTreeRecursively(std::vector<Node> *pNodePtr, Tree *hTreePtr);
