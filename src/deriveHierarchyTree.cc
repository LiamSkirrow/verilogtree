
#include "include/deriveHierarchyTree.h"

/* NOTE: does Verilog allow nested modules? Like...
-> if this is standard Verilog, create a GitHub issue for this and just deal with it in a future minor release
module mod0(...);
    module mod1(...);
        mod2 mod2_inst(...);
    endmodule
endmodule
*/

void parseRtl(std::vector<std::string> rtlFiles, std::vector<ParentNode> *parentNodeVecPtr, std::regex parentNodeRegexStr, std::regex childNodeRegexStr, bool debug){
    // - loop through the RTL files and whenever we get a regex hit for a parent node...
    // parentNodeVecPtr->push_back(new ParentNode); // is this correct use of 'new'?
    // - then keep scanning for child nodes, when we get a regex hit...
    // ChildNode curr; curr.moduleName...; curr.instanceName...;
    // parentNodeVecPtr->back().childNodes.push_back(curr);
    // - continue doing this for the rest of the files and parentNodeVecPtr should become a vector
    //   of all the parent nodes in the RTL codebase, each with a record of their corresponding child nodes

    std::fstream rtlFileObj;
    std::string line;
    std::smatch matchObj;

    for(int i = 0; i < rtlFiles.size(); i++){
        // open the next file in the list
        rtlFileObj.open(rtlFiles.at(i));
        // read line-by-line and apply the regex search pattern
        for(std::string line; getline(rtlFileObj, line); ){
            // std::cout << "line: " << line << std::endl;
            // check for a parent-node match

            std::regex_search(line, matchObj, parentNodeRegexStr);
            if(matchObj.size() == 1){
                if(debug){
                    std::cout << "Parent node found in file " << rtlFiles.at(i) << ": " << matchObj.str() << std::endl;
                }
                // split based on spaces
                
                ParentNode curr;
                parentNodeVecPtr->push_back(curr);


            }



        }
        rtlFileObj.close();
    }


}

void constructHierarchyTree(Tree *hTreePtr, std::vector<ParentNode> *parentNodeVecPtr){

}

Tree *deriveHierarchyTree(std::vector<std::string> rtlFiles, std::regex parentNodeRegexStr, std::regex childNodeRegexStr, bool debug){

    Tree hTree;
    Tree *hTreePtr;
    std::vector<ParentNode> parentNodeVec;
    std::vector<ParentNode> *parentNodeVecPtr;

    parentNodeVecPtr = &parentNodeVec;

    // TODO: to check the regex parsing strings, pass in a TON of real-world open source RTL files
    //       and print out the internal database of parent nodes and child nodes, should be 
    //       easy to find any errors or verify correct working functionality this way

    // what about modules defined on the same line separated by a ; ?
    // -> mod0 mod0_inst(); mod1 mod1_inst(); ????

    // just remember to document the supported syntax for module declarations
    // include examples in README...

    // parse the RTL according to the regex strings. Create distinct parent-child node groups
    parseRtl(rtlFiles, parentNodeVecPtr, parentNodeRegexStr, childNodeRegexStr, debug);

    // consider whether the tree construction algorithm should be performed using a function or class methods for the Tree class

    // now (recursively?) replace all child nodes with parent nodes to construct the tree
    constructHierarchyTree(hTreePtr, parentNodeVecPtr);

    return hTreePtr;

}
