// main file for verilogtree

#include "include/parseUserArgs.h"
#include <iostream>
#include <vector>
#include <array>
#include <string.h>

int main(int argc, char **argv){

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

    // pass by reference
    std::vector<std::string> srcNameVec;
    std::vector<std::string> *srcNameVecPtr = &srcNameVec;

    // call user input parser function here
    parseUserArgs(argc, argv, argListFlags, srcNameVecPtr);
    
    // uncomment for debugging: print out user supplied filenames
        // std::cout << "Supplied Verilog files: ";
        // for(int i = 0; i < srcNameVecPtr->size(); i++){
        //     std::cout << srcNameVecPtr->at(i) << ' ';
        // }
        // std::cout << std::endl;

    // - create a vector to store the filename paths (read from cmd line or from filelist txt file)
    // - loop through and make sure that all the files exist *first* before you start parsing the text.
    // - once all files are verified to exist, then begin opening the files (one at a time) and start the regex search sequence...

    return 0;
}