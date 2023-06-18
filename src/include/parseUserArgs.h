// header file for parseUserArgs.cc

#include <iostream>
#include <vector>
#include <array>
#include <string.h>

void errorAndExit(std::string errorMsg);
void parseUserArgs(int argc, char **argv, std::array<std::string,11> argList, std::vector<std::string> *srcNameVec, std::vector<std::string> *noIncSrcNameVec);
int getNextArgs(int argc, char **argv, int i, std::string argName, std::string errMsg, std::vector<std::string> *argumentVecPtr);
