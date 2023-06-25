// header file for parseUserArgs.cc

#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <regex>
#include <cstddef>

// child nodes
class ChildNode{
    std::string moduleName;
    std::string instanceName;

    public:
        void setModuleName(std::string str);
        void setInstName(std::string str);
        std::string getModuleName();
        std::string getInstName();
};

// parent nodes
class ParentNode{
    std::vector<ChildNode> childNodes;
    std::string moduleName;
    bool isInstantiated = false;

    public:
        void pushChildNode(ChildNode cNode);
        void setModuleName(std::string name);
        std::string getModuleName();
        ChildNode getChildNodeAtIndex(int index);
        int getChildNodesSize();
        void setIsInstantiated();
        bool getIsInstantiated();
};

// main tree abstract data type
class Tree{
    std::vector<ParentNode> parentNodes;
    std::map<std::string, ParentNode> parentNodeMap;

    // the ParentNode representing the top level module
    std::vector<ParentNode> treeRoots;

    public:
        void setParentNodes(std::vector<ParentNode> pNodes);
        ParentNode getParentNodeAtIndex(int index);
        int getParentNodesSize();
        void setMap(std::map<std::string, ParentNode> pNodeMap);
        ParentNode getMapElem(std::string key);
};

Tree *deriveHierarchyTree(std::vector<std::string> rtlFiles, std::regex parentNodeRegexStr, std::regex childNodeRegexStr, bool debug);
void parseRtl(std::vector<std::string> rtlFiles, std::vector<ParentNode> *parentNodeVecPtr, std::regex parentNodeRegexStr, std::regex childNodeRegexStr, std::map<std::string, ParentNode> *pNodeMapPtr, bool debug);
void elaborateHierarchyTree(Tree *hTreePtr);
void tokenizeString(std::string str, std::string *tokenisedStringPtr);
