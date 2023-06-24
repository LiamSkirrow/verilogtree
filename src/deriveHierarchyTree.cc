
#include "include/deriveHierarchyTree.h"

/* NOTE: does Verilog allow nested modules? Like...
-> if this is standard Verilog, create a GitHub issue for this and just deal with it in a future minor release
module mod0(...);
    module mod1(...);
        mod2 mod2_inst(...);
    endmodule
endmodule
*/

void ChildNode::setModuleName(std::string str){
    this->moduleName = str;
}

void ChildNode::setInstName(std::string str){
    this->instanceName = str;
}

std::string ChildNode::getModuleName(){
    return this->moduleName;
}

std::string ChildNode::getInstName(){
    return this->instanceName;
}

void ParentNode::setModuleName(std::string str){
    this->moduleName = str;
}

std::string ParentNode::getModuleName(){
    return this->moduleName;
}

void ParentNode::pushChildNode(ChildNode cNode){
    this->childNodes.push_back(cNode);
}

ChildNode ParentNode::getChildNodeAtIndex(int index){
    return this->childNodes.at(index);
}

int ParentNode::getChildNodesSize(){
    return this->childNodes.size();
}

// *** NOTE:
// I do not like the current tokenising code, maybe refactor it from here.

// *** TODO: do I need to handle tab characters? Not handling newlines for now!
// given the raw regex match from the RTL files, strip whitespace and store the two text words
void tokeniseString(std::string str, std::vector<std::string> *tokenisedStringPtr, bool debug){
    // loop through str and push the sub-words to tokenisedStringPtr
    int indexStart = 0;
    bool indexStartAssigned = false;
    std::string substr;
    std::size_t found;
    for(int i = 0; i < str.size(); i++){
        if(str[i] != ' ' && !indexStartAssigned){
            indexStart = i;
            indexStartAssigned = true;
        }
        else if((str[i] == '(' || str[i] == '#' || str[i] == ' ') && indexStartAssigned){
            substr = str.substr(indexStart, i);
            
            // TODO: too much debug output, simply print out the names of the parent node module names
            //       and child node module/inst names, in deriveHierarchyTree

            if(debug) { std::cout << "substr before trim: " << '<' << substr << '>' << std::endl; }
            for( ; ; ){
                found = substr.find_first_of(" #(");
                // found an unwanted char, delete it
                if(found != std::string::npos){
                    substr.erase(found);
                }
                else{
                    break;
                }
            }
            if(debug) { std::cout << "substr after trim: " << '<' << substr << '>' << std::endl; }
            tokenisedStringPtr->push_back(substr);
            indexStart = 0;
            indexStartAssigned = false;
        }
    }
}

void parseRtl(std::vector<std::string> rtlFiles, std::vector<ParentNode> *parentNodeVecPtr, std::regex parentNodeRegexStr, std::regex childNodeRegexStr, bool debug){

    std::fstream rtlFileObj;
    std::string line;
    std::string moduleName;
    std::string childModuleName;
    std::string childInstName;
    std::vector<std::string> tokenisedString;
    std::vector<std::string> *tokenisedStringPtr = &tokenisedString;
    std::smatch matchObjParent;
    std::smatch matchObjChild;

    for(int i = 0; i < rtlFiles.size(); i++){
        // open the next file in the list
        rtlFileObj.open(rtlFiles.at(i));
        // read line-by-line and apply the regex search pattern
        for(std::string line; getline(rtlFileObj, line); ){
            // std::cout << "line: " << line << std::endl;
            // check for a parent-node match

            std::regex_search(line, matchObjParent, parentNodeRegexStr);
            std::regex_search(line, matchObjChild,  childNodeRegexStr);
            // found a parent node
            if(matchObjParent.size() == 1){
                // tokenise the parent node string, splitting on arbitrary number of space chars
                tokenisedStringPtr->clear();
                tokeniseString(matchObjParent.str(), tokenisedStringPtr, debug);
                moduleName = tokenisedStringPtr->at(1);

                if(debug){
                    std::cout << "Parent node found in file " << rtlFiles.at(i) << ": \"" << matchObjParent.str() << "\" with module name: \"";
                    std::cout << moduleName << "\" " << std::endl;
                }
                // create the ParentNode object
                ParentNode curr;
                curr.setModuleName(moduleName);                
                parentNodeVecPtr->push_back(curr);
            }
            // found a child node
            else if(matchObjChild.size() == 1){
                // tokenise the child node string, splitting on arbitrary number of space chars
                tokenisedStringPtr->clear();
                tokeniseString(matchObjChild.str(), tokenisedStringPtr, debug);

                ChildNode curr;
                curr.setModuleName(tokenisedStringPtr->at(0));
                curr.setInstName(tokenisedStringPtr->at(1));
                
                // the last parent node is the current one, push the associated child nodes
                parentNodeVecPtr->back().pushChildNode(curr);
                
            }
        }
        rtlFileObj.close();
    }
}

void constructHierarchyTree(Tree *hTreePtr, std::vector<ParentNode> *parentNodeVecPtr){

}

Tree *deriveHierarchyTree(std::vector<std::string> rtlFiles, std::regex parentNodeRegexStr, std::regex childNodeRegexStr, bool debug){

    Tree hTree;
    Tree *hTreePtr;
    std::vector<ParentNode> parentNodeVec;
    std::vector<ParentNode> *parentNodeVecPtr;

    parentNodeVecPtr = &parentNodeVec;

    // TODO: to check the regex parsing strings, pass in a TON of real-world open source RTL files
    //       and print out the internal database of parent nodes and child nodes, should be 
    //       easy to find any errors or verify correct working functionality this way

    // what about modules defined on the same line separated by a ; ?
    // -> mod0 mod0_inst(); mod1 mod1_inst(); ????

    // just remember to document the supported syntax for module declarations
    // include examples in README...

    // parse the RTL according to the regex strings. Create distinct parent-child node groups
    parseRtl(rtlFiles, parentNodeVecPtr, parentNodeRegexStr, childNodeRegexStr, debug);

    for(int i = 0; i < parentNodeVecPtr->size(); i++){
        std::cout << "Parent Node Name: " << parentNodeVecPtr->at(i).getModuleName() << std::endl;
        for(int j = 0; j < parentNodeVecPtr->at(i).getChildNodesSize(); j++){
            std::cout << "   Child Node Module: " << parentNodeVecPtr->at(i).getChildNodeAtIndex(j).getModuleName() << std::endl;
            std::cout << "   Child Node Instance: " << parentNodeVecPtr->at(i).getChildNodeAtIndex(j).getInstName() << std::endl;
        }
    }

    // consider whether the tree construction algorithm should be performed using a function or class methods for the Tree class

    // now (recursively?) replace all child nodes with parent nodes to construct the tree
    constructHierarchyTree(hTreePtr, parentNodeVecPtr);

    return hTreePtr;

}
