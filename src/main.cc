// main file for verilogtree

#include <iostream>
#include <vector>
#include <array>

#include <string.h>

// is this bad practice?
using namespace std;

// include this somewhere else since it's just sitting in global scope atm
struct Arguments{
    vector<string> rtlFiles;
    vector<string> noIncFiles;
    string codeVersion = "v0.0.0";
    string lang        = "verilog";
    int level          = -1;
} args;

void errorAndExit(string errorMsg){
    // TODO: need to do some error handling rather than cout
    // cout << endl << "Usage: verilogtree [--filelist <files.txt> | -f file1.v file2.v ...]" << endl << endl;
    cout << endl << errorMsg << endl << endl;
    cout << "Type: 'man verilogtree' or 'verilogtree --help' for help" << endl;
    exit(-1);
}

int main(int argc, char **argv){

    // TODO: move all of the input parsing code into its own file and call parseInput() function
    //       to simplify this main function

    // accepted list of arguments, must remember to update number in parentheses!!!
    array<string,11> argList = {"-h",            // display usage and flags information
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

    int isEqual;
    int fileNum;
    bool includedVerilog = false;
    vector<string> srcNameVec;

    // check args and arg number
    if(argc <= 1){
        errorAndExit((string)"Usage: verilogtree [--filelist <files.txt> | -f file1.v file2.v ...]");
    }

    // TODO: make this more efficient...
    // parse args, make sure they match what's expected
    for(int i = 1; i < argc; i++){
        isEqual = 0;
        // loop through argv list and perform a lookup of argList to determine whether the args are correct
        for(int j = 0; j < argList.size(); j++){
            isEqual = (argv[i] == argList.at(j));
            // if the arg matches a known argument, exit the loop
            if(isEqual) break;
        }
        // exit the program if a supplied argument doesn't match
        if(!isEqual){
            errorAndExit((string)"Invalid argument detected: \"" + (string)argv[i] + (string)"\"");
        }
        // FIXME:
        // replace the entire above code with simply "if(argv[i] in argList)"... that's literally it!
        // FIXME:

        // at this point, the argument read matches one of the accepted args, proceed with getting input
        // -_- switch statements don't work with strings... thank you C++ :)

        if(argv[i] == (string)"-v" || argv[i] == (string)"--version"){
            cout << "verilogtree " << args.codeVersion << endl;
            exit(0);
        }

        if(argv[i] == (string)"-h" || argv[i] == (string)"--help"){
            // printHelp();
        } else if(argv[i] == (string)"-f"){
            // TODO: make this into a function since it's replicated across -f and --filelist
            includedVerilog = true;
            // check the next N strings of argv to get the Verilog filepaths
            for(int fileNum = 0; ; fileNum++){
                // -f is the final argument
                if(i == argc-1){
                    // no following file names, invalid so throw error
                    if(fileNum == 0){
                        errorAndExit((string)"Argument -f must be followed by paths to RTL files");
                    }
                    break;
                }
                // next argv entry begins with '-' and is therefore another argument, no filenames supplied
                if(argv[i+1][0] == '-'){
                    // no following file names, invalid so throw error
                    if(fileNum == 0){
                        errorAndExit((string)"Argument -f must be followed by paths to RTL files");
                    }
                    break;
                }
                srcNameVec.push_back(argv[i+1]);
                // advance i along to next arg index, fileNum counter also advances
                i++;
            }
            
        } else if(argv[i] == (string)"--filelist"){
            includedVerilog = true;
            // check the next string of argv for the filelist name, open it and read the contents

        } else if(argv[i] == (string)"-L" || argv[i] == (string)"--level"){

        } else if(argv[i] == (string)"-n" || argv[i] == (string)"--no-include"){

        } else if(argv[i] == (string)"--lang"){

        } /* TODO: add another argument that determines whether both the module name and inst name is printed or just module name */ else {   // this should never be reached...
            cout << "Bug found! Need to add " << argv[i] << " to argument parser!" << endl;
            // TODO: include the url of the verilogtree github repo
            cout << "Please report this on GitHub!" << endl;
            exit(-1);
        }
    }
    
    // cout << "Supplied Verilog files: ";
    // for(int i = 0; i < srcNameVec.size(); i++){
    //     cout << srcNameVec.at(i) << ' ';
    // }
    // cout << endl;

    // if the args are valid, but didn't include either -f or --filelist, exit
    if(!includedVerilog){
        errorAndExit((string)"Usage: verilogtree [--filelist <files.txt> | -f file1.v file2.v ...]");
    }

    // - create a vector to store the filename paths (read from cmd line or from filelist txt file)
    // - loop through and make sure that all the files exist *first* before you start parsing the text.
    // - once all files are verified to exist, then begin opening the files (one at a time) and start the regex search sequence...

    return 0;
}