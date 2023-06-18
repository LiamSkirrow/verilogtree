
#include "include/deriveHierarchyTree.h"

/* NOTE: does Verilog allow nested modules? Like...
-> if this is standard Verilog, create a GitHub issue for this and just deal with it in a future minor release
module mod0(...);
    module mod1(...);
        mod2 mod2_inst(...);
    endmodule
endmodule
*/

void parseRtl(std::vector<std::string> rtlFiles, std::vector<ParentNode> *parentNodeVecPtr){
    // - loop through the RTL files and whenever we get a regex hit for a parent node...
    // parentNodeVecPtr->push_back(new ParentNode); // is this correct use of 'new'?
    // - then keep scanning for child nodes, when we get a regex hit...
    // ChildNode curr; curr.moduleName...; curr.instanceName...;
    // parentNodeVecPtr->back().childNodes.push_back(curr);
    // - continue doing this for the rest of the files and parentNodeVecPtr should become a vector
    //   of all the parent nodes in the RTL codebase, each with a record of their corresponding child nodes


}

void constructHierarchyTree(Tree *hTreePtr, std::vector<ParentNode> *parentNodeVecPtr){

}

Tree *deriveHierarchyTree(std::vector<std::string> rtlFiles, std::string parentNodeRegexStr, std::string childNodeRegexStr){

    Tree hTree;
    Tree *hTreePtr;
    std::vector<ParentNode> parentNodeVec;
    std::vector<ParentNode> *parentNodeVecPtr;

    // parse the RTL according to the regex strings. Create distinct parent-child node groups
    parseRtl(rtlFiles, parentNodeVecPtr);

    // now (recursively?) replace all child nodes with parent nodes to construct the tree
    constructHierarchyTree(hTreePtr, parentNodeVecPtr);

    return hTreePtr;

}
