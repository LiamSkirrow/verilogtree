// header file for parseUserArgs.cc

#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <regex>
#include <cstddef>

#define MULTI_LINE_0 0  // one liner
#define MULTI_LINE_1 1  // module keyword and module name on same line
#define MULTI_LINE_2 2  // module keyword, module name and #/( all on different lines

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
        void clearChildNodes();
};

// main tree abstract data type
class Tree{
    std::vector<Node> parentNodes;
    std::map<std::string, Node> parentNodeMap;

    // the ParentNode representing the top level module
    std::vector<Node> treeRoots;

    public:
        void setParentNodes(std::vector<Node> pNodes);
        bool getParentNodeExistence(std::string str);
        Node * getParentNodeAtIndex(int index);
        int getParentNodesSize();
        void setMap(std::map<std::string, Node> pNodeMap);
        Node * getMapElem(std::string key);
        void pushTreeRoot(Node pNode);
        Node * getTreeRootNodeAtIndex(int index);
        std::map<std::string, Node>::iterator findNodeInMap(Node pNode);
        std::map<std::string, Node>::iterator getMapEnd();
        int getTreeRootSize();
};

struct RegexStrings{
    // regex strings to match one-line module declarations/instantiations
    std::regex parentNodeRegexStr;
    std::regex childNodeRegexStr;
    // regex strings to match multi-line module declarations/instantiations
    std::regex parentNodeRegexStrModuleWord;
    std::regex parentNodeRegexStrModuleName;
    std::regex parentNodeRegexStrModuleWordAndName;
    // TODO: could even potentially split the # and the ( over newlines
    std::regex parentNodeRegexStrModuleParenthesis;
    // TODO:
    // std::regex childNodeRegexStr  = "^\\s*\\w+\\s+\\w+\\s*\\(";

};

Tree deriveHierarchyTree(Tree *hierarchyTreePtr, std::vector<std::string> rtlFiles, RegexStrings regexStrings, bool debug, bool superDebug, std::vector<std::string> noIncModules, int maxHierarchyLevel, std::vector<std::string> topModules);
void parseRtl(std::vector<std::string> rtlFiles, std::vector<Node> *parentNodeVecPtr, RegexStrings regexStrings, std::map<std::string, Node> *pNodeMapPtr, bool debug);
void elaborateHierarchyTree(Tree *hTreePtr, bool debug, bool superDebug, std::vector<std::string> noIncModules, std::vector<std::string> topModules, int maxHierarchyLevel);
void tokeniseString(std::string str, std::vector<std::string> *tokenisedStringPtr, bool superDebug, int string_format);
void constructTreeRecursively(Node *pNodePtr, Tree *hTreePtr, bool debug, bool superDebug, std::vector<std::string> noIncModules, int level, int maxHierarchyLevel);
