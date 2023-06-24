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

    public:
        void pushChildNode(ChildNode cNode);
        void setModuleName(std::string name);
        std::string getModuleName();
        ChildNode getChildNodeAtIndex(int index);
        int getChildNodesSize();
};

// main tree abstract data type
class Tree{
    //public: // apparently public variables aren't good practice??? Any way around this???
    // each of these parent nodes is the *root* node of a separate/physically isolated tree
    std::vector<ParentNode> parentNodes;

    void pushParentNode(ParentNode pNode);
};

Tree *deriveHierarchyTree(std::vector<std::string> rtlFiles, std::regex parentNodeRegexStr, std::regex childNodeRegexStr, bool debug);
void parseRtl(std::vector<std::string> rtlFiles, std::vector<ParentNode> *parentNodeVecPtr, std::regex parentNodeRegexStr, std::regex childNodeRegexStr, bool debug);
void constructHierarchyTree(Tree *hTreePtr, std::vector<ParentNode> *parentNodeVecPtr);
void tokenizeString(std::string str, std::string *tokenisedStringPtr);
