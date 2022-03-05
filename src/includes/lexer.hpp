#pragma once

#include <string>
#include <vector>
#include <tuple>
#include "includes/io.hpp"
#include "includes/token.hpp"

class Lexer
{
    std::vector<Token> tokens;
    std::string content;
    int lineNumber;
    int columnNumber;
    std::string line;
    int cursor;
    char character;
    int level;
    std::map<std::string, TokenType> keywords;
    std::map<std::string, TokenType> double_operators;
    std::map<char, TokenType> single_operators;

public:
    Lexer(std::string content);

    std::vector<Token> init();

private:
    void advance();

    Token collect_identifier();

    Token collect_number();

    Token collect_string();

    std::tuple<std::vector<Token>, int> tokenize(std::string line, int level);
};