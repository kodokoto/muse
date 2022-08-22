#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "includes/parser.hpp"
#include "includes/lexer.hpp"


int main() {

    std::ifstream inFile;
    inFile.open("src/examples/test.mu"); 

    std::stringstream strStream;
    strStream << inFile.rdbuf(); 
    std::string content = strStream.str(); 
    Lexer l = Lexer(content);
    std::vector<Token> tokens = l.init();
    // for (Token t : tokens) {
    //     std::cout << t.value << std::endl;
    // }
    Parser parser = Parser(tokens);
    std::cout <<  parser.parse()->toJSON(0);
    return 0;
}