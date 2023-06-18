// parse the user input, handling invalid errors as we go

#include "include/parseUserArgs.h"

void errorAndExit(std::string errorMsg){
    // TODO: need to do some error handling rather than cout
    std::cout << std::endl << errorMsg << std::endl << std::endl;
    std::cout << "Type 'verilogtree --help' or 'man verilogtree' for help" << std::endl;
    exit(-1);
}

int getNextArgs(int argc, char **argv, int i, std::string argName, std::string errMsg, std::vector<std::string> *argumentVecPtr){
    // int index = 0;
    for(int fileNum = 0; ; fileNum++){
        // -f is the final argument
        if(i == argc-1){
            // no following file names, invalid so throw error
            if(fileNum == 0){
                errorAndExit((std::string)"Argument " + argName + " must be followed by " + errMsg);  //paths to RTL files
            }
            break;
        }
        // next argv entry begins with '-' and is therefore another argument, no filenames supplied
        if(argv[i+1][0] == '-'){
            // no following file names, invalid so throw error
            if(fileNum == 0){
                errorAndExit((std::string)"Argument " + argName + " must be followed by " + errMsg);  //paths to RTL files
            }
            break;
        }
        argumentVecPtr->push_back(argv[i+1]);
        // advance i along to next arg index, fileNum counter also advances
        i++;
    }
    return i;
}

// TODO: include error handling for case where user includes the same argument more than once.
//       have a bool for each arg and check that it's not already true when handling the arg

struct Arguments parseUserArgs(int argc, char **argv, std::array<std::string,11> argList){

    int isEqual;
    bool includedVerilog = false;
    std::vector<std::string> argumentVec;
    std::vector<std::string> *argumentVecPtr = &argumentVec;
    Arguments args;

    // check args and arg number
    if(argc <= 1){
        errorAndExit((std::string)"Usage: verilogtree [--filelist <files.txt> | -f file1.v file2.v ...]");
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
            errorAndExit((std::string)"Invalid argument detected: \"" + (std::string)argv[i] + (std::string)"\"");
        }
        // FIXME:
        // replace the entire above code with simply "if(argv[i] in argList)"... that's literally it!
        // FIXME:

        // at this point, the argument read matches one of the accepted args, proceed with getting input
        // -_- switch statements don't work with strings... thank you C++ :)

        argumentVecPtr->clear();

        if(argv[i] == (std::string)"-v" || argv[i] == (std::string)"--version"){
            std::cout << "verilogtree " << args.codeVersion << std::endl;
            exit(0);
        }

        if(argv[i] == (std::string)"-h" || argv[i] == (std::string)"--help"){
            // printHelp();
            exit(0);
        } 
        else if(argv[i] == (std::string)"-f"){
            // TODO: make this into a function since it's replicated across -f and --filelist
            includedVerilog = true;
            // check the next N strings of argv to get the Verilog filepaths, increment i accordingly
            i = getNextArgs(argc, argv, i, argv[i], (std::string)"path(s) to RTL files", argumentVecPtr);
            // now give the args structure the relevant filenames
            args.rtlFiles = *argumentVecPtr;
        } 
        else if(argv[i] == (std::string)"--filelist"){
            includedVerilog = true;
            // check the next string of argv for the filelist name, open it and read the contents

        } 
        else if(argv[i] == (std::string)"-L" || argv[i] == (std::string)"--level"){
            // check the next string of argv to get the level, increment i accordingly
            i = getNextArgs(argc, argv, i, argv[i], (std::string)"a numeric value", argumentVecPtr);
            if(argumentVecPtr->size() != 1){
                errorAndExit((std::string)"Argument -L must have only one proceeding value");
            }
            // TODO: assert -L is numeric value
            // assert(argumentVecPtr->at(0))
            args.level = argumentVecPtr->at(0);
        } 
        else if(argv[i] == (std::string)"-n" || argv[i] == (std::string)"--no-include"){
            // check the next N strings of argv to get the don't include filepaths, increment i accordingly
            i = getNextArgs(argc, argv, i, argv[i], (std::string)"path(s) to RTL files", argumentVecPtr);
            // now give the args structure the relevant filenames
            args.noIncFiles = *argumentVecPtr;
        } 
        else if(argv[i] == (std::string)"--lang"){
            // check the next string of argv to get the language, increment i accordingly
            i = getNextArgs(argc, argv, i, argv[i], (std::string)"one of [verilog | vhdl]", argumentVecPtr);
            if(argumentVecPtr->size() != 1){
                errorAndExit((std::string)"Argument --lang must be one of [verilog | vhdl]");
            }
            // assert(argumentVecPtr->at(0) == "verilog" || argumentVecPtr->at(0) == "vhdl");
            
            // ******* TEMPORARY *******
            // TODO: add GitHub issue for adding VHDL support
            if(argumentVecPtr->at(0) == "vhdl"){
                std::cout << "******* ERROR *******" << std::endl << std::endl;
                std::cout << "VHDL not yet supported! Please see this GitHub issue for progress on VHDL support:" << std::endl;
                std::cout << "<insert URL here>" << std::endl;
                std::cout << std::endl << "******* ERROR *******" << std::endl;
                exit(-1);
            }
            // ******* TEMPORARY *******
            args.lang = argumentVecPtr->at(0);

        } /* TODO: add another argument that determines whether both the module name and inst name is printed or just module name */ 
        else {   // this should never be reached...
            std::cout << "Bug found! Need to add " << argv[i] << " to argument parser!" << std::endl;
            // TODO: include the url of the verilogtree github repo
            std::cout << "Please report this on GitHub!" << std::endl;
            exit(-1);
        }
    }

    // if the args are valid, but didn't include either -f or --filelist, exit
    if(!includedVerilog){
        errorAndExit((std::string)"Usage: verilogtree [--filelist <files.txt> | -f file1.v file2.v ...]");
    }

    return args;
}