// main source file for verilogtree

#include "include/parseUserArgs.h"
#include "include/deriveHierarchyTree.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string.h>

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

int main(int argc, char **argv){

    struct Arguments args;
    Tree hierarchyTree;
    Tree *hierarchyTreePtr;
    std::string parentNodeRegexStr;
    std::string childNodeRegexStr;

    // accepted list of arguments, must remember to update number in parentheses!!!
    std::array<std::string,12> argListFlags = 
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
                               "--debug"        // more verbose output, print out internal variables
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
    // https://regex101.com/ is my best friend for this kind of stuff -> https://regex101.com/r/HrzsFQ/2
    parentNodeRegexStr = (args.lang == "verilog") ? "^\ *module*\s*\w*\s*\(" : "          ";
    childNodeRegexStr  = (args.lang == "verilog") ? "^\ *\w*\s*\w*\s*\("     : "          ";
    //                                               \_______Verilog______/     \__VHDL__/

    // NOTE: should I also be storing the hpaths to each module? This may be more efficient to do *while*
    //       the tree is being constructed rather than having to traverse the tree DFS-style to figure
    //       out the hpaths

    // now parse the Verilog/VHDL, searching for the key phrases and generate the logical hierarchy
    // this is the main algorithm to configure the tree
    hierarchyTreePtr = deriveHierarchyTree(args.rtlFiles, parentNodeRegexStr, childNodeRegexStr);

    // display the tree structure of the RTL
    // printTree(hierarchyTree, args);

    return 0;
}