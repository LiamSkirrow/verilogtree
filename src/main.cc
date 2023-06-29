// main source file for verilogtree

#include "include/parseUserArgs.h"
#include "include/deriveHierarchyTree.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <regex>
#include <string.h>   // need this?

// struct Arguments{
//     std::vector<std::string> rtlFiles;
//     std::vector<std::string> noIncFiles;
//     std::string codeVersion = "v0.0.0";
//     std::string lang        = "verilog";
//     std::string level       = "-1";
//     bool debug              = false;
// };

// move into own file to declutter main.cc, since I will add debug output for the next stages etc
void dumpArgsStruct(struct Arguments args){
    std::cout << "Supplied Verilog files for vector size: " << args.rtlFiles.size() << std::endl;
    for(int i = 0; i < args.rtlFiles.size(); i++){
        std::cout << "    " << args.rtlFiles.at(i) << std::endl;
    }
    std::cout << std::endl;

    // TODO: need to replace all mentions of no-include files with no-include modules!
    std::cout << "Supplied no-include files for vector size: " << args.noIncFiles.size() << std::endl;
    for(int i = 0; i < args.noIncFiles.size(); i++){
        std::cout << "    " << args.noIncFiles.at(i) << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Supplied level arg: " << std::endl;
    std::cout << "    " << args.level;
    std::cout << std::endl;

    std::cout << "Supplied lang arg: " << std::endl;
    std::cout << "    " << args.lang;
    std::cout << std::endl;

    std::cout << "Supplied debug arg: " << std::endl;
    std::cout << "    " << args.debug;
    std::cout << std::endl;
}

// sanity check that input argument files exist
void checkFilesExist(struct Arguments args){
    std::fstream fileHandler;

    for(int i = 0; i < args.rtlFiles.size(); i++){
        fileHandler.open(args.rtlFiles.at(i));
        if(!fileHandler){
            std::cout << "File: " << args.rtlFiles.at(i) << " not found" << std::endl;
            fileHandler.close();
            exit(-1);
        }
        fileHandler.close();
    }
}

// TODO: have the option to highlight specific modules with a certain colour in the output
// TODO: add an iterative (non-recursive) print out input arg to have a low-memory consumption mode
// TODO: always print module name, optionally print out instance name as well
// TODO: (stretch feature) actually print out a proper visual tree in the middle of the screen branching downwards left and right
//       see if there's a library that can do this automatically
// TODO: include a setting to print out a blank line between lines of hierarchy, space it out a bit more?

void printTreeRecursively(Node pNode, int depth, int count){

    Node cNode;
    int cNodeLen;
    
    // level arg: may need to return count, and update it at end of each function call
    // calling each function up the recursive chain to end

    // currently not recursing all the way to the end... stops one level too shallow...

    for(int i = 0; i < pNode.getChildNodesSize(); i++){
        cNode = *pNode.getChildNodeAtIndex(i);
        // cNodeLen = cNode.getChildNodesSize();
        // print out the correct level of indentation
        std::cout << '|';
        for(int j = 0; j < 4*(count-1)-1; j++){
            std::cout << ' ';
        }
        for(int j = 0; j < 3; j++){
            std::cout << '-';
        }
        std::cout << ' ';
        // FIXME: where are the instance names in the console output?
        std::cout << cNode.getModuleName() << " " << cNode.getInstName() << std::endl;
        // std::cout << "Child's child: " << cNode.getChildNodeAtIndex(0)->getModuleName() << ": " << cNodeLen << std::endl;
        if(cNode.getChildNodesSize() > 0){
            printTreeRecursively(cNode, depth, ++count);
        }
    }
}

void printTree(Tree hierarchyTree, struct Arguments args){

    int treeRootSize = hierarchyTree.getTreeRootSize();
    int pNodeNumChilds;
    Node pNode;
    Node *cNodePtr;

    // how to do this non-recursively?

    // std::cout << "----------" << std::endl;

    if(args.algorithm == "recursive"){
        for(int i = 0; i < treeRootSize; i++){
            
            // TODO: what if it's empty, pNode doesn't exist? Need to account for this edge case

            // find a root parent node
            pNode = *hierarchyTree.getTreeRootNodeAtIndex(i);
            pNodeNumChilds = pNode.getChildNodesSize();
            std::cout << pNode.getModuleName() << std::endl;
            // if(pNodeNumChilds > 0){
            printTreeRecursively(pNode, atoi(args.level.c_str()), 1);
            // }
            // std::cout << "----------" << std::endl;
        }
    }
    else if(args.algorithm == "iterative"){
        std::cout << "iterative selected" << std::endl;
    }
    else{
        std::cout << ("Internal error!");
        std::cout << "Please report this on GitHub!" << std::endl;
        exit(-1);
    }

}

int main(int argc, char **argv){

    struct Arguments args;
    Tree hierarchyTree;
    Tree *hierarchyTreePtr;
    std::regex parentNodeRegexStr;
    std::regex childNodeRegexStr;

    hierarchyTreePtr = &hierarchyTree;

    // accepted list of arguments, must remember to update number in parentheses!!!
    std::array<std::string,14> argListFlags = 
                              {"-h",            // display usage and flags information
                               "--help",
                               "-f",            // file paths
                               "--filelist",    // txt filelist, one file path per line
                               "-v",            // version string
                               "--version",
                               "-L",            // print up to this many levels of hierarchy
                               "--level",
                               "-n",            // don't print out this/these module names/instance names
                               "--no-include",
                               "--lang",        // one of either [verilog ^ vhdl]
                               "--debug",       // more verbose output, print out internal variables
                               "--iterative",    // print out hierarchy iteratively rather than recursively (uses less memory for large hierarchies)
                               "--recursive"    // print out hierarchy recursively (default)
                              };

    // call user input parser function here
    args = parseUserArgs(argc, argv, argListFlags);
    
    // this is enabled by supplying the --debug argument
    if(args.debug){
        dumpArgsStruct(args);
    }

    // sanity check, make sure the files exist in the file system
    checkFilesExist(args);

    // select the correct regex patterns depending on the language selected
    // https://regex101.com/ is my best friend for this kind of stuff -> https://regex101.com/r/CzzlTF/1
    // parentNodeRegexStr = (args.lang == "verilog") ? "^\ *module*\s*\w*\s*\(" : "          ";
    // childNodeRegexStr  = (args.lang == "verilog") ? "^\ *\w*\s*\w*\s*\("     : "          ";
    //                                               \_______Verilog______/     \__VHDL__/

    parentNodeRegexStr = "^\\ *module\\s*\\w*\\s*#*\\(";
    childNodeRegexStr  = "^\\ *\\w*\\s*\\w*\\s*\\(";

    // NOTE: should I also be storing the hpaths to each module? This may be more efficient to do *while*
    //       the tree is being constructed rather than having to traverse the tree DFS-style to figure
    //       out the hpaths

    // now parse the Verilog/VHDL, searching for the key phrases and generate the logical hierarchy
    // this is the main algorithm to configure the tree
    hierarchyTree = deriveHierarchyTree(hierarchyTreePtr, args.rtlFiles, parentNodeRegexStr, childNodeRegexStr, args.debug);

    // display the tree structure of the RTL
    printTree(hierarchyTree, args);

    // TODO: what happens if I give it a module that doesn't exist in the RTL?
    // FIXME: BUG: this program will segfault if a circular hierarchy is given -> eg mod0 instantiates itself or something...

    if(args.debug){
        std::cout << std::endl << "Successfully reached end of program!" << std::endl << std::endl;
    }

    // TODO: should include a list of which modules were no-included at the end...

    return 0;
}
