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
    std::vector<std::string> noIncFiles;
    std::string codeVersion = "v0.0.0";
    std::string lang        = "verilog";
    std::string level       = "-1";
    bool debug              = false;
};

struct Arguments parseUserArgs(int argc, char **argv, std::array<std::string,12> argList);
void errorAndExit(std::string errorMsg);
int getNextArgs(int argc, char **argv, int i, std::string argName, std::string errMsg, std::vector<std::string> *argumentVecPtr);
