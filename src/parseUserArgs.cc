// parse the user input, handling invalid errors as we go

#include "include/parseUserArgs.h"

void errorAndExit(std::string errorMsg){
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

struct Arguments parseUserArgs(int argc, char **argv, std::array<std::string,20> argList){

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
        // replace the entire above code with simply "if(argv[i] in argList)"... that's literally it!
        // TODO: make this more efficient...

        // at this point, the argument read matches one of the accepted args, proceed with getting input
        // -_- switch statements don't work with strings... thank you C++ :)

        argumentVecPtr->clear();

        if(argv[i] == (std::string)"-v" || argv[i] == (std::string)"--version"){
            std::cout << "verilogtree " << args.codeVersion << std::endl;
            exit(0);
        }

        if(argv[i] == (std::string)"-h" || argv[i] == (std::string)"--help"){
            printHelp();
            exit(0);
        } 
        else if(argv[i] == (std::string)"-f"){
            includedVerilog = true;
            // check the next N strings of argv to get the Verilog filepaths, increment i accordingly
            i = getNextArgs(argc, argv, i, argv[i], (std::string)"path(s) to RTL files", argumentVecPtr);
            // now give the args structure the relevant filenames
            args.rtlFiles = *argumentVecPtr;
        } 
        else if(argv[i] == (std::string)"--filelist"){
            includedVerilog = true;
            // check the next string of argv for the filelist name, open it and read the contents
            i = getNextArgs(argc, argv, i, argv[i], (std::string)"path to filelist", argumentVecPtr);
            if(argumentVecPtr->size() != 1){
                errorAndExit((std::string)"Argument --filelist must have only one proceeding value");
            }
            // make sure filelist exists
            std::fstream filelistObj;
            filelistObj.open(argumentVecPtr->at(0));
            if(!filelistObj){
                errorAndExit((std::string)"File " + argumentVecPtr->at(0) + " does not exist");
            }
            // read the contents of filelist into argumentVecPtr
            std::string filelistName = argumentVecPtr->at(0);
            std::fstream fileHandler;
            argumentVecPtr->clear();

            for(std::string filelistEntry; getline(filelistObj, filelistEntry); ){
                fileHandler.open(filelistEntry);
                if(!fileHandler){
                    errorAndExit((std::string)"Supplied file in filelist: \"" + filelistEntry + "\" does not exist");
                }
                fileHandler.close();
                argumentVecPtr->push_back(filelistEntry);
            }

            args.rtlFiles = *argumentVecPtr;
            filelistObj.close();
        }
        else if(argv[i] == (std::string)"-L" || argv[i] == (std::string)"--level"){
            // check the next string of argv to get the level, increment i accordingly
            i = getNextArgs(argc, argv, i, argv[i], (std::string)"a positive numeric value", argumentVecPtr);
            // check only one value is given
            if(argumentVecPtr->size() != 1){
                errorAndExit((std::string)"Argument -L/--level must have only one proceeding value");
            }
            // assert(argumentVecPtr->at(0))
            args.level = argumentVecPtr->at(0);
        } 
        else if(argv[i] == (std::string)"-m" || argv[i] == (std::string)"--max-hierarchy"){
            // check the next string of argv to get the max number of levels to recurse down, increment i accordingly
            i = getNextArgs(argc, argv, i, argv[i], (std::string)"a positive numeric value", argumentVecPtr);
            // check only one value is given
            if(argumentVecPtr->size() != 1){
                errorAndExit((std::string)"Argument -m/--max-hierarchy must have only one proceeding value");
            }
            // assert(argumentVecPtr->at(0))
            args.maxHierarchyLevel = atoi(argumentVecPtr->at(0).c_str());
        } 
        else if(argv[i] == (std::string)"-n" || argv[i] == (std::string)"--ignore-modules"){
            // check the next N strings of argv to get the don't include filepaths, increment i accordingly
            i = getNextArgs(argc, argv, i, argv[i], (std::string)"names of modules to ignore", argumentVecPtr);
            // now give the args structure the relevant filenames
            args.noIncModules = *argumentVecPtr;
        } 
        else if(argv[i] == (std::string)"-T" || argv[i] == (std::string)"--top"){
            // check the next N strings of argv to get the top level module(s), increment i accordingly
            i = getNextArgs(argc, argv, i, argv[i], (std::string)"names of top-level modules to include", argumentVecPtr);
            // now give the args structure the relevant filenames
            args.topModules = *argumentVecPtr;
        } 
        else if(argv[i] == (std::string)"--lang"){
            // check the next string of argv to get the language, increment i accordingly
            i = getNextArgs(argc, argv, i, argv[i], (std::string)"one of [verilog | vhdl]", argumentVecPtr);
            if(argumentVecPtr->size() != 1){
                errorAndExit((std::string)"Argument --lang must be one of [verilog | vhdl]");
            }
            // assert(argumentVecPtr->at(0) == "verilog" || argumentVecPtr->at(0) == "vhdl");
            
            // ******* TEMPORARY *******
            if(argumentVecPtr->at(0) == "vhdl"){
                std::cout << "******* ERROR *******" << std::endl << std::endl;
                std::cout << "VHDL not yet supported! Please see this GitHub issue for progress on VHDL support:" << std::endl;
                std::cout << "https://github.com/LiamSkirrow/verilogtree/issues/2" << std::endl;
                std::cout << std::endl << "******* ERROR *******" << std::endl;
                exit(-1);
            }
            // ******* TEMPORARY *******
            args.lang = argumentVecPtr->at(0);

        } else if(argv[i] == (std::string)"--debug"){
            args.debug = true;
        } else if(argv[i] == (std::string)"--super-debug"){
            // set both levels of debug output to true -> therefore --super-debug implies --debug 
            args.superDebug = true;
            args.debug      = true;
        } else if(argv[i] == (std::string)"--iterative"){
            args.algorithm = "iterative";
        } 
        else if(argv[i] == (std::string)"--recursive"){
            args.algorithm = "recursive";   // default
        }
        else if(argv[i] == (std::string)"--no-inst-name"){
            args.printInstName = false;
        }
        else {   // this should never be reached...
            std::cout << "Bug found! Need to add " << argv[i] << " to argument parser!" << std::endl;
            std::cout << "Please report this on GitHub! https://github.com/LiamSkirrow/verilogtree" << std::endl;
            exit(-1);
        }
    }

    // if the args are valid, but didn't include either -f or --filelist, exit
    if(!includedVerilog){
        errorAndExit((std::string)"Usage: verilogtree [--filelist <files.txt> | -f file1.v file2.v ...]");
    }

    return args;
}

void printHelp(){
    std::string helpString = "verilogtree arguments:                     \n\
                                                                         \n\
    Usage: verilogtree [--filelist <files.txt> | -f file1.v file2.v ...] \n\
                                                                         \n\
    -v / --version             Print current version string.                       \n\
    -h / --help                Print this message.                                 \n\
    -f                         Pass in RTL files directly one after the other.     \n\
    --filelist                 Pass in a filelist containing the paths to the      \n\
                               files to be parsed. File paths must each be on a    \n\
                               new line.                                           \n\
    -L / --level               Specify the maximum hierarchy depth to be printed   \n\
                               out.                                                \n\
    -m / --max-hierarchy       Specify the maximum hierarchy depth to be recursed  \n\
                               down. This is in place to avoid seg faults in the   \n\
                               case of a circular hierarchy bug.                   \n\
    -n / --ignore-modules      List modules whose child modules shall be ignored   \n\
                               when generating console output. Multiple modules    \n\
                               can be listed one after the other.                  \n\
    -T / --top                 List modules that shall be treated as top level     \n\
                               modules.                                            \n\
    --lang                     Specify either of 'verilog' or 'vhdl' as the target \n\
                               language. Currently, in verilogtree v0.1.x, only    \n\
                               'verilog' is supported.                             \n\
    --debug                    Produce debugging output. Useful for bug tracing    \n\
    --super-debug              Produce more extensive debugging output. Passing    \n\
                               this flag implies --debug.                          \n\
    --no-inst-name             Do not print out the module's instance names. This  \n\
                               gives cleaner output.                               \n\
    ";

    std::cout << helpString << std::endl;
}
