// header file for parseUserArgs.cc

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <string.h>
#include <assert.h>

struct Arguments{
    std::vector<std::string> rtlFiles;
    std::vector<std::string> noIncModules;
    std::string codeVersion  = "v0.1.0-dev";
    std::vector<std::string> topModules;
    std::string lang         = "verilog";
    std::string level        = "-1";
    std::string algorithm    = "recursive";
    bool printInstName       = true;
    bool debug               = false;
    bool superDebug          = false;
    int maxHierarchyLevel    = 100;
};

struct Arguments parseUserArgs(int argc, char **argv, std::array<std::string,20> argList);
void errorAndExit(std::string errorMsg);
int getNextArgs(int argc, char **argv, int i, std::string argName, std::string errMsg, std::vector<std::string> *argumentVecPtr);
void printHelp();
