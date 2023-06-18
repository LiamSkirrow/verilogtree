// main file for verilogtree

#include "include/parseUserArgs.h"
#include <iostream>
#include <vector>
#include <array>
#include <string.h>

// just for debugging ;)
void dumpArgsStruct(struct Arguments args){
    std::cout << "Supplied Verilog files for vector size: " << args.rtlFiles.size() << std::endl;
    for(int i = 0; i < args.rtlFiles.size(); i++){
        std::cout << args.rtlFiles.at(i) << ' ';
    }
    std::cout << std::endl;

    std::cout << "Supplied no include files for vector size: " << args.noIncFiles.size() << std::endl;
    for(int i = 0; i < args.noIncFiles.size(); i++){
        std::cout << args.noIncFiles.at(i) << ' ';
    }
    std::cout << std::endl;

    std::cout << "Supplied level arg: ";
    std::cout << args.level;
    std::cout << std::endl;

    std::cout << "Supplied lang arg: ";
    std::cout << args.lang;
    std::cout << std::endl;
}

int main(int argc, char **argv){

    struct Arguments args;

    // accepted list of arguments, must remember to update number in parentheses!!!
    std::array<std::string,11> argListFlags = 
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
                               "--lang"         // one of either [verilog ^ vhdl]
                              };

    // call user input parser function here
    args = parseUserArgs(argc, argv, argListFlags);
    
    // uncomment for debugging: print out user supplied filenames and arguments
    dumpArgsStruct(args);

    // - create a vector to store the filename paths (read from cmd line or from filelist txt file)
    // - loop through and make sure that all the files exist *first* before you start parsing the text.
    // - once all files are verified to exist, then begin opening the files (one at a time) and start the regex search sequence...

    return 0;
}