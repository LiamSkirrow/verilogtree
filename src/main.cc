// main source file for verilogtree

#include "include/parseUserArgs.h"
#include "include/deriveHierarchyTree.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
// #include <regex>
#include <string.h>   // need this?

// move into own file to declutter main.cc, since I will add debug output for the next stages etc
void dumpArgsStruct(struct Arguments args){
    std::cout << "Supplied Verilog files: " << args.rtlFiles.size() << std::endl;
    for(int i = 0; i < args.rtlFiles.size(); i++){
        std::cout << "    " << args.rtlFiles.at(i) << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Ignored modules: " << args.noIncModules.size() << std::endl;
    for(int i = 0; i < args.noIncModules.size(); i++){
        std::cout << "    " << args.noIncModules.at(i) << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Top level modules: " << args.topModules.size() << std::endl;
    for(int i = 0; i < args.topModules.size(); i++){
        std::cout << "    " << args.topModules.at(i) << std::endl;
    }
    std::cout << std::endl;

    std::cout << "level arg: " << std::endl;
    std::cout << "    " << args.level;
    std::cout << std::endl;

    std::cout << "max hierarchy arg: " << std::endl;
    std::cout << "    " << args.maxHierarchyLevel;
    std::cout << std::endl;

    std::cout << "lang arg: " << std::endl;
    std::cout << "    " << args.lang;
    std::cout << std::endl << std::endl;

    // std::cout << "Supplied debug arg: " << std::endl;
    // std::cout << "    " << args.debug;
    // std::cout << std::endl;
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

void printTreeRecursively(Node pNode, int depth, int count, bool indentationDone, bool finalChild, bool printInstName, std::vector<std::string> noIncModules){

    Node cNode;
    int cNodeLen;
    std::string character;
    std::string instNamePlaceholder;
    
    // level arg: may need to return count, and update it at end of each function call
    // calling each function up the recursive chain to end

    // check if we've reached the user specified max depth
    if(count == depth){
        return;
    }

    for(int i = 0; i < pNode.getChildNodesSize(); i++){
        // print out the correct level of indentation
        for(int j = 0; j < 4*(count-1); j++){
            if((j % 4) == 0){
                if(indentationDone){
                    std::cout << ' ';
                }
                else{
                    if(j == 0 || !finalChild){
                        std::cout << "│";
                    }
                    else{
                        std::cout << " ";
                    }
                }
            }
            else
                std::cout << ' ';
        }
        cNode = *pNode.getChildNodeAtIndex(i);
        for(int j = 0; j < 3; j++){
            if(j == 0){
                // if we've reached the final module at this level
                if(i == pNode.getChildNodesSize()-1){
                    std::cout << "└";
                }
                // if we still have more modules to print out at this level
                else{
                    std::cout << "├";
                }
            }
            else
                std::cout << "─";
        }
        instNamePlaceholder = (printInstName ? cNode.getInstName() : (std::string)" ");
        std::cout << ' ';
        std::cout << cNode.getModuleName() << " " << instNamePlaceholder << std::endl;
        if(cNode.getChildNodesSize() > 0){
            if(i == pNode.getChildNodesSize()-1){
                if(count==1){
                    indentationDone = true;
                }
                finalChild = true;
            }
            printTreeRecursively(cNode, depth, count+1, indentationDone, finalChild, printInstName, noIncModules);
        }
    }
}

void printTree(Tree hierarchyTree, struct Arguments args){

    int treeRootSize = hierarchyTree.getTreeRootSize();
    int pNodeNumChilds;
    Node pNode;
    Node *cNodePtr;

    // how to do this non-recursively?

    if(args.algorithm == "recursive"){
        for(int i = 0; i < treeRootSize; i++){
            std::cout << std::endl;
            
            // TODO: what if it's empty, pNode doesn't exist? Need to account for this edge case

            // find a root parent node
            pNode = *hierarchyTree.getTreeRootNodeAtIndex(i);
            pNodeNumChilds = pNode.getChildNodesSize();
            std::cout << pNode.getModuleName() << std::endl;
            printTreeRecursively(pNode, atoi(args.level.c_str()), 1, false, false, args.printInstName, args.noIncModules);
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
    
    std::cout << std::endl;

    // print a summary of the ignored modules as a sanity reminder
    if(args.noIncModules.size() > 0){
        std::cout << "Ignoring childs of following module(s): ";
        for(int j = 0; j < args.noIncModules.size(); j++){
            std::cout << args.noIncModules.at(j) << " ";
        }
        std::cout << std::endl;
    }

    // print a summary of the top level modules as a reminder
    if(args.topModules.size() > 0){
        std::cout << "User specified top-level modules: ";
        for(int j = 0; j < args.topModules.size(); j++){
            std::cout << args.topModules.at(j) << " ";
        }
        std::cout << std::endl;
    }
}

int main(int argc, char **argv){

    struct Arguments args;
    struct RegexStrings regexStrings;
    Tree hierarchyTree;
    Tree *hierarchyTreePtr;
    // std::regex parentNodeRegexStr;
    // std::regex childNodeRegexStr;

    hierarchyTreePtr = &hierarchyTree;

    // TODO: figure out what to do with --iterative and --recursive? Probably not going to implement these afterall
    // accepted list of arguments, must remember to update number in parentheses!!!
    std::array<std::string,20> argListFlags = 
                              {"-h",             // display usage and flags information
                               "--help",
                               "-f",             // file paths
                               "--filelist",     // txt filelist, one file path per line
                               "-v",             // version string
                               "--version",
                               "-L",             // print up to this many levels of hierarchy
                               "--level",
                               "-m",             // define the maximum recursion depth when constructing hierarchy tree
                               "--max-hierarchy",
                               "-n",             // don't print out this/these module names
                               "--ignore-modules",
                               "-T",             // manually specify top-level module(s)
                               "--top",
                               "--lang",         // one of either [verilog ^ vhdl]
                               "--debug",        // more verbose output, print out internal variables
                               "--super-debug",  // print out even more debug output
                               "--iterative",    // print out hierarchy iteratively rather than recursively (uses less memory for large hierarchies)
                               "--recursive",    // print out hierarchy recursively (default)
                               "--no-inst-name" // do not print out the instance names 
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

    // parentNodeRegexStr = "^\\s*module\\s+\\w+\\s*#?\\s*\\(";
    // TODO: pretty sure the module instantiations could have a # in them, need to add that to regex...
    // childNodeRegexStr  = "^\\s*\\w+\\s+\\w+\\s*\\(";

    // regex strings to match one-line module declarations/instantiations
    regexStrings.parentNodeRegexStr = "^\\s*module\\s+\\w+\\s*#?\\s*\\(";
    regexStrings.childNodeRegexStr  = "^\\s*\\w+\\s+\\w+\\s*\\(";
    // regex strings to match multi-line module declarations/instantiations
    regexStrings.parentNodeRegexStrModuleWord        = "^\\s*module\\s*";
    regexStrings.parentNodeRegexStrModuleWordAndName = "^\\s*module\\s+\\w+\\s*";
    regexStrings.parentNodeRegexStrModuleName        = "^\\s*\\w+\\s*";
    regexStrings.parentNodeRegexStrModuleParenthesis = "^\\s*#?\\s*\\(";

    // NOTE: should I also be storing the hpaths to each module? This may be more efficient to do *while*
    //       the tree is being constructed rather than having to traverse the tree DFS-style to figure
    //       out the hpaths

    // now parse the Verilog/VHDL, searching for the key phrases and generate the logical hierarchy
    // this is the main algorithm to configure the tree
    hierarchyTree = deriveHierarchyTree(hierarchyTreePtr, args.rtlFiles, regexStrings, args.debug, args.superDebug, args.noIncModules, args.maxHierarchyLevel, args.topModules);

    // display the tree structure of the RTL
    printTree(hierarchyTree, args);

    if(args.debug){
        std::cout << "Successfully reached end of program!" << std::endl;
    }

    return 0;
}
