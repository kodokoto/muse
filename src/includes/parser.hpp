#pragma once

#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"
#include "ast.hpp"
#include <map>

class Parser
{
    std::vector<Token> tokens;
    int index;
    Token current_token;
    std::map<std::string, int> precedence_map = 
    {
        {"^", 80},
        {"*", 40},
        {"/", 40},
        {"+", 20},
        {"-", 20},
        {"==", 10},
        {"!=", 10},
        {">=", 10},
        {"<=", 10},
        {">", 10},
        {"<", 10},
        {"and", 12},
        {"or", 12},
        {"not", 12},
        {"in", 12},
    };

    public:

    Parser(std::vector<Token> tokens);

    std::unique_ptr<Program> parse();

    private:

    // utility

    bool lookahead(std::vector<TokenType> tokens);

    std::string consume(int repetition=1);

    void check_and_consume(TokenType expected_token_type);

    int check_precedence();

    bool check_if_func_def();

    bool check_if_list_comp();

    bool check_if_generator();

    Span current_span();

    // Statements

    std::unique_ptr<StatementBlock> parse_statement_block();

    std::vector<std::unique_ptr<Statement>> parse_statements(TokenType end_token = TokenType::EoF);
    
    std::unique_ptr<Statement> parse_statement();

    std::unique_ptr<Assignment> parse_assignment();

    std::unique_ptr<Statement> parse_return();

    std::unique_ptr<FunctionDefenition> parse_function_defenition();

    std::unique_ptr<FunctionPrototype> parse_function_prototype();

    std::unique_ptr<Parameter> parse_parameter();

    std::unique_ptr<Statement> parse_return_statement();

    std::unique_ptr<IfStatement> parse_if_statement();

    std::unique_ptr<WhileLoop> parse_while_loop();

    std::unique_ptr<ForLoop> parse_for_loop();

    // Expressions

    std::unique_ptr<Expression> parse_expression();

    std::unique_ptr<Expression> parse_primary();

    std::unique_ptr<Expression> parse_unary();

    std::unique_ptr<Expression> parse_parenthesis();

    std::unique_ptr<FunctionCall> parse_function_call();

    std::unique_ptr<Expression> parse_binary_expression(int precedence, std::unique_ptr<Expression> lhs);

    std::unique_ptr<ListComprehension> parse_list_comprehension();

    std::unique_ptr<Generator> parse_generator();

    std::unique_ptr<Iterable> parse_iterable();

    std::unique_ptr<Identifier> parse_identifier();

    std::unique_ptr<Slice> parse_iterable_slice();

    std::unique_ptr<Subscription> parse_iterable_subscription();

    std::unique_ptr<Value> parse_attribute_reference();

    // literals

    std::unique_ptr<NumericLiteral> parse_numeric_literal();

    std::unique_ptr<BooleanLiteral> parse_boolean_literal();

    std::unique_ptr<StringLiteral> parse_string_literal();

    std::unique_ptr<List> parse_list();

    std::unique_ptr<IterableLiteral> parse_literal_iterable();


};

#endif