// header file for parseUserArgs.cc

#include <iostream>
#include <vector>
#include <array>

// child nodes
class ChildNode{
    std::string moduleName;
    std::string instanceName;

    void addModule(std::string mName, std::string iName);
};

// parent nodes
class ParentNode{
    std::vector<ChildNode> childNodes;

    void pushChildNode(ChildNode cNode);
};

// main tree abstract data type
class Tree{
    // each of these parent nodes is the *root* node of a separate/physically isolated tree
    std::vector<ParentNode> parentNodes;

    void pushParentNode(ParentNode pNode);
};

Tree deriveHierarchyTree(std::vector<std::string> rtlFiles, std::string parentNodeRegexStr, std::string childNodeRegexStr);