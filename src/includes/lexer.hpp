#pragma once

#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <vector>
#include <tuple>
#include <map>
#include "io.hpp"
#include "token.hpp"

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
    std::map<std::string, TokenType> keywords =
        {
            {"if", TokenType::IF},
            {"else", TokenType::ELSE},
            {"for", TokenType::FOR},
            {"in", TokenType::IN},
            {"while", TokenType::WHILE},
            {"return", TokenType::RETURN},
            {"true", TokenType::BOOL},
            {"false", TokenType::BOOL},
            {"null", TokenType::NONE},
            {"and", TokenType::AND},
            {"or", TokenType::OR},
            {"not", TokenType::NOT},
            {"class", TokenType::CLASS},
            {"struct", TokenType::STRUCT}
        };

    std::map<std::string, TokenType> double_operators =
        {
            {"==", TokenType::EQUIVALENCE},
            {"!=", TokenType::NOT_EQUAL},
            {">=", TokenType::MORE_EQUAL},
            {"<=", TokenType::LESS_EQUAL},
            {"+=", TokenType::INCREMENT},
            {"-=", TokenType::DECREMENT},
            {"..", TokenType::ELIPSIS}
        };

    std::map<char, TokenType> single_operators =
        {
            {'+', TokenType::PLUS},
            {'-', TokenType::MINUS},
            {'*', TokenType::MULT},
            {'/', TokenType::DIV},
            {'^', TokenType::EXPO},
            {'%', TokenType::MODULO},
            {'=', TokenType::ASSIGN},
            {':', TokenType::TYPE_DECL},
            {',', TokenType::COMMA},
            {'.', TokenType::PERIOD},
            {'(', TokenType::LPAREN},
            {')', TokenType::RPAREN},
            {'[', TokenType::LSQUARE},
            {']', TokenType::RSQUARE},
            {'{', TokenType::LCURLY},
            {'}', TokenType::RCURLY},
            {'>', TokenType::MORE_THAN},
            {'<', TokenType::LESS_THAN}
        };
    
public:
    Lexer(std::string content);

    std::vector<Token> init();

private:
    void advance();

    void goBack();

    Token collect_identifier();

    Token collect_number();

    Token collect_string(char close);

    std::string characterAt(int index, int lookahead = 1);

    std::tuple<std::vector<Token>, int> tokenize(std::string line, int level);
};

#endif 