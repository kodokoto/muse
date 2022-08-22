#include "includes/lexer.hpp"
#include <boost/regex.hpp>
#include <string.h>
#include <iostream>

Lexer::Lexer(std::string content)
{
    this->content = content;
    this->lineNumber = 0;
    this->columnNumber = 0;
}

std::vector<Token> Lexer::init()
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
    this->tokens.push_back({"EoF",{tokens[-1].pointer.line, 0}, TokenType::EoF});
    return this->tokens;
}

void Lexer::advance()
{
    if (this->cursor < this->line.size())
    {
        this->cursor += 1;
        this->character = this->line[this->cursor];
        this->columnNumber += 1;
    }
    else
    {
        this->cursor += 1;
    }
}

void Lexer::goBack()
{
    this->cursor -= 1;
}

Token Lexer::collect_identifier()
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

Token Lexer::collect_number()
{
    std::string value = "";
    while (this->cursor <= this->line.size() && isdigit(this->character) or this->character == '.')
    {
        char c = this->character;
        char cs = value.back();
        if (this->character=='.' && value.back()=='.') {
            this->goBack();
            break;
        }
        value += this->character;
        this->advance();
    }
    if (value.back() == '.')
    {
        value.pop_back();
    } // remove any trailing dots
    return {value, {this->lineNumber, this->columnNumber}, TokenType::NUMBER};
}

Token Lexer::collect_string(char close)
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

std::string Lexer::characterAt(int index, int lookahead)
{
    return this->line.substr(index, index + lookahead);
}

std::tuple<std::vector<Token>, int> Lexer::tokenize(std::string line, int level)
{
    this->lineNumber++;
    this->columnNumber = 0;

    // replace spaces with tabs
    boost::regex re("    ");
    std::string s = boost::regex_replace(line, re, "\t");
    this->line = s;
    this->cursor = 0;
    this->character = this->line[this->cursor];
    this->level = level;
    std::vector<Token> tokens = {};

    while (this->cursor < this->line.size())
    {
        int diff = level - std::count(this->line.begin(), this->line.end(), '\t');
        if (this->character == '#')
        {
            Token t = {"COMMENT", {this->lineNumber, this->columnNumber}, TokenType::COMMENT};

            return {{t}, level};
        }
        else if (level < std::count(this->line.begin(), this->line.end(), '\t'))
        {
            tokens.push_back({"INDENT", {this->lineNumber, this->columnNumber}, TokenType::INDENT});
            level += 1;
            this->advance();
        }
        else if (level > std::count(this->line.begin(), this->line.end(), '\t'))
        {
            tokens.push_back({"DEDENT", {this->lineNumber, this->columnNumber}, TokenType::DEDENT});
            level -= 1;
            // this->advance();
        }
        else if (isalpha(this->character))
        {
            tokens.push_back(this->collect_identifier());
        }
        else if (isdigit(this->character))
        {
            tokens.push_back(this->collect_number());
        }
        else if (this->character == '"' || this->character == '\'')
        {
            tokens.push_back(this->collect_string(this->character));
        }
        else if (this->double_operators.count(this->line.substr(this->cursor, 2)))
        {
            std::string value = this->line.substr(this->cursor, 2);
            tokens.push_back({value, {this->lineNumber, this->columnNumber}, this->double_operators[value]});
            this->advance();
            this->advance();
        }
        else if (this->single_operators.count(this->character))
        {
            std::string s;
            s += this->character;
            tokens.push_back({s, {this->lineNumber, this->columnNumber}, this->single_operators[this->character]});
            this->advance();
        }
        else
        {
            this->advance();
        };
    };
    tokens.push_back({"EOL", {this->lineNumber, this->columnNumber}, TokenType::EoL});
    return {tokens, level};
};
