#pragma once

#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include "LineColumn.hpp"

enum class TokenType
{
    // core

    ID,
    COMMENT,
    INDENT,
    DEDENT,
    EoL,
    EoF,

    // Literals

    NUMBER,
    STRING,
    BOOL,
    NONE,

    // keywords

    IF,
    ELSE,
    FOR,
    IN,
    WHILE,
    RETURN,
    CLASS,
    STRUCT,

    // arithmetic operators

    PLUS,
    MINUS,
    MULT,
    DIV,
    INTDIV,
    EXPO,
    MODULO,
    INCREMENT,
    DECREMENT,

    // relational Operators

    ASSIGN,
    EQUIVALENCE,
    LESS_THAN,
    MORE_THAN,
    LESS_EQUAL,
    MORE_EQUAL,
    NOT_EQUAL,

    // Logical operator

    AND,
    OR,
    NOT,

    // special carachters

    COLON,
    COMMA,
    PERIOD,
    TYPE_DECL,
    LPAREN,
    RPAREN,
    LSQUARE,
    RSQUARE,
    LCURLY,
    RCURLY,
    ELIPSIS
};

struct Token
{
    std::string value;
    LineColumn pointer;
    TokenType type;
};

#endif 
