#include "includes/lexer.hpp"
#include <boost/regex.hpp>
#include <string.h>
#include <iostream>

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
            {"struct", TokenType::STRUCT}};

    std::map<std::string, TokenType> double_operators =
        {
            {"==", TokenType::EQUIVALENCE},
            {"!=", TokenType::NOT_EQUAL},
            {">=", TokenType::MORE_EQUAL},
            {"<=", TokenType::LESS_EQUAL},
            {"+=", TokenType::INCREMENT},
            {"-=", TokenType::DECREMENT}};

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
            {'<', TokenType::LESS_THAN}};

public:
    Lexer(std::string content)
    {
        this->content = content;
        this->lineNumber = 0;
        this->columnNumber = 0;
    }

    std::vector<Token> init()
    {
        std::vector<std::string> lines = split(this->content, "\n");
        lines.erase(std::remove(lines.begin(), lines.end(), ""), lines.end());

        // netsed level
        int level = 0;
        std::vector<Token> tmp_tokens;
        for (std::string line : lines)
        {
            std::tie(tmp_tokens, level) = this->tokenize(line, level);
            this->tokens.insert(this->tokens.end(), tmp_tokens.begin(), tmp_tokens.end());
        }
    }

private:
    void advance()
    {
        if (this->cursor < this->line.size())
        {
            this->cursor += 1;
            this->character = this->line[this->cursor];
            this->columnNumber + 1;
        }
        else
        {
            this->cursor += 1;
        }
    }

    Token collect_identidier()
    {
        std::string value = "";
        while (this->cursor <= this->line.size() && isalpha(this->character))
        {
            value += this->character;
            this->advance();
        }

        if (this->keywords.count(value))
        {
            return {value, {this->lineNumber, this->columnNumber}, this->keywords[value]};
        }
        else
        {
            return {value, {this->lineNumber, this->columnNumber}, TokenType::ID};
        }
    }

    Token collect_number()
    {
        std::string value = "";
        while (this->cursor <= this->line.size() && isdigit(this->character) or this->character == '.')
        {
            value += this->character;
            this->advance();
        }
        if (value[-1] == '.')
        {
            value.pop_back();
        } // remove any trailing dots
        return {value, {this->lineNumber, this->columnNumber}, TokenType::NUMBER};
    }

    Token collect_string(char close)
    {
        std::string value = "";
        while (this->cursor < this->line.size())
        {
            this->advance();
            if (this->character != close)
            {
                value += this->character;
            }
            else
            {
                this->advance();
                break;
            }
        }
        return {value, {this->lineNumber, this->columnNumber}, TokenType::STRING};
    }

    std::string characterAt(int index, int lookahead = 1)
    {
        return this->line.substr(index, index + lookahead);
    }

    std::tuple<std::vector<Token>, int> tokenize(std::string line, int level)
    {
        this->lineNumber++;
        this->columnNumber = 0;

        // replace spaces with tabs
        boost::regex re("    ");
        this->line = boost::regex_replace(line, re, '\t');
        this->cursor = 0;
        this->character = this->line[this->cursor];
        this->level = level;
        std::vector<Token> tokens = {};

        while (this->cursor < this->line.size())
        {
            if (this->character == '#')
            {
                Token t = {"", {this->lineNumber, this->columnNumber}, TokenType::COMMENT};

                return {{t}, level};
            }
            else if (level < std::count(this->line.begin(), this->line.end(), '\t'))
            {
                tokens.push_back({"\t", {this->lineNumber, this->columnNumber}, TokenType::INDENT});
                level += 1;
                this->advance();
            }
            else if (level > std::count(this->line.begin(), this->line.end(), '\t'))
            {
                tokens.push_back({"\t", {this->lineNumber, this->columnNumber}, TokenType::DEDENT});
                level -= 1;
                this->advance();
            }
            else if (isalpha(this->character))
            {
                tokens.push_back(this->collect_identidier());
            }
            else if (isdigit(this->character))
            {
                tokens.push_back(this->collect_number());
            }
            else if (this->character == '"' || this->character == '\'')
            {
                tokens.push_back(this->collect_string(this->character));
            }
            else if (this->double_operators.count(this->line.substr(this->cursor, this->cursor + 2)))
            {
                std::string value = this->line.substr(this->cursor, this->cursor + 2);
                tokens.push_back({value, {this->lineNumber, this->columnNumber}, this->double_operators[value]});
                this->advance();
                this->advance();
            }
            else if (this->single_operators.count(this->character))
            {
                tokens.push_back({"" + this->character, {this->lineNumber, this->columnNumber}, this->single_operators[this->character]});
                this->advance();
            }
            else
            {
                this->advance();
            };
        };
        tokens.push_back({"", {this->lineNumber, this->columnNumber}, TokenType::EOL});
        return {tokens, level};
    };
};