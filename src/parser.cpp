#include "includes/lexer.hpp"
#include "includes/ast_nodes.hpp"
#include "includes/parser.hpp"
#include <iostream>
class Parser
{

    std::vector<Token> tokens;
    int index;
    Token current_token;
    static std::map<std::string, int> precedence_map;

public:
    Parser(std::vector<Token> tokens)
    {
        this->tokens = tokens;
        this->index = 0;
        this->current_token = this->tokens[this->index];
        this->precedence_map = {
            {"^", 80},
            {"*", 40},
            {"/", 40},
            {"+", 20},
            {"-", 20}};
    };

    Program parse()
    {
        LineColumn span_start = this->current_token.pointer;
        std::vector<Statement> program = this->parse_statements();
        LineColumn span_end = this->current_token.pointer;
        return Program(program, Span(span_start, span_end));
    };

private:
    // utility

    // looks ahead to see if there is a sequence of tokens
    bool lookahead(std::vector<TokenType> tokens)
    {
        for (int i = 0; i < tokens.size(); i++)
        {
            if (!(tokens[i] == this->tokens[this->index + i].type))
            {
                return false;
            }
        }

        return true;
    };

    void consume(int repetitions = 1)
    {
        for (int i = 0; i<repetitions; i++)
        {
            if (this->current_token.type != TokenType::EoF)
            {
                ++this->index;
                this->current_token = this->tokens[this->index];
            }
        }
    };

    void check_and_consume(TokenType expected_token_type)
    {
        if (this->current_token.type != expected_token_type)
        {
            throw "Unexpected token: {} \nExpected: {}";
        }
        this->consume();
    };

    int check_precedence();

    bool check_if_func_def();

    // Statements

    StatementBlock parse_statement_block()
    {
        LineColumn span_start = this->current_token.pointer;
        this->check_and_consume(TokenType::INDENT);
        std::vector<Statement> statements = this->parse_statements(TokenType::DEDENT);
        LineColumn span_end = this->current_token.pointer;
        return StatementBlock(statements, Span(span_start, span_end));
    };

    std::vector<Statement> parse_statements(TokenType end_token = TokenType::EoF)
    {
        std::vector<Statement> statements;
        while(this->current_token.type != end_token)
        {
            statements.push_back(this->parse_statement());
        };
        return statements;
    };

    Statement parse_statement()
    {
        if (lookahead({TokenType::ID, TokenType::TYPE_DECL, TokenType::ID, TokenType::ASSIGN}) || lookahead({TokenType::ID, TokenType::ASSIGN}))
        {
            return parse_assignment();
        }
        else if (lookahead({TokenType::ID, TokenType::LPAREN}))
        {
            return parse_function_defenition();
        }
        else if (this->current_token.type == TokenType::RETURN)
        {
            return parse_return_statement();
        }
        else if (this->current_token.type == TokenType::FOR)
        {
            return parse_for_loop();
        }
        else if (this->current_token.type == TokenType::WHILE)
        {
            return parse_while_loop();
        }
        else 
        {
            return parse_expression();
        }
    };

    Assignment parse_assignment()
    {
        LineColumn span_start = this->current_token.pointer;
        std::string name = this->current_token.value;
        std::string type = "";
        consume();
        if (current_token.type==TokenType::TYPE_DECL)
        {
            consume();
            type = this->current_token.value;
        }
        consume(); // consume =
        Expression rhs = parse_expression();
        LineColumn span_end = this->current_token.pointer;
        Assignment assignment = Assignment(name, type, rhs, Span(span_start, span_end));
        check_and_consume(TokenType::EoL);
        return assignment;
    };
    
    FunctionDefenition parse_function_defenition()
    {
        LineColumn span_start = this->current_token.pointer;
        FunctionPrototype prototype = parse_function_prototype();
        if (this->current_token.type == TokenType::ASSIGN)
        {
            consume();
            Expression body = parse_expression();
            check_and_consume(TokenType::EoL);
            LineColumn span_end = this->current_token.pointer;
            return FunctionDefenition(prototype, body, Span(span_start, span_end));
        }
        else if (lookahead({TokenType::EoL, TokenType::INDENT}))
        {
            consume();
            StatementBlock body = parse_statement_block();
            LineColumn span_end = this->current_token.pointer;
            return FunctionDefenition(prototype, body, Span(span_start, span_end));
        }
        else
        {
            throw "something";
        }
    };

    FunctionPrototype parse_function_prototype()
    {
        LineColumn span_start = this->current_token.pointer;
        std::string name = this->current_token.value;
        std::string return_type = "";
        consume(2);
        std::vector<Parameter> parameters = {};
        while (this->current_token.type!=TokenType::RPAREN)
        {
            parameters.push_back(parse_parameter());
            if (this->current_token.type == TokenType::COMMA)
            {   
                consume();
            }
        }
        consume();

        if (this->current_token.type == TokenType::TYPE_DECL)
        {
            consume();
            return_type = this->current_token.value;
            check_and_consume(TokenType::ID);
        }
        
        LineColumn span_end = this->current_token.pointer;
        return FunctionPrototype(name, parameters, return_type, Span(span_start, span_end));
    };

    Parameter parse_parameter()
    {        
        LineColumn span_start = this->current_token.pointer;

        std::string name = this->current_token.value;
        check_and_consume(TokenType::ID);
        check_and_consume(TokenType::TYPE_DECL);
        std::string type = this->current_token.value;
        check_and_consume(TokenType::ID);
        LineColumn span_end = this->current_token.pointer;

        return Parameter(name, type, Span(span_start, span_end));
    };


    Statement parse_return_statement()
    {
        consume();
        return parse_expression();
    };


    IfStatement parse_if_statement()
    {
        LineColumn span_start = this->current_token.pointer;
        consume();
        BooleanExpression conditional = parse_boolean_expression();
        check_and_consume(TokenType::EoL);
        StatementBlock ifBlock = parse_statement_block();
        LineColumn span_end = this->current_token.pointer;
        StatementBlock elseBlock = {{}, Span(this->current_token.pointer, this->current_token.pointer)};

        if (lookahead({TokenType::ELSE, TokenType::IF}))
        {
            consume();
            IfStatement nestedIf = parse_if_statement();
            elseBlock = {{nestedIf}, nestedIf.span};
        } else if (lookahead({TokenType::ELSE, TokenType::EoL, TokenType::INDENT}))
        {
            consume();
            elseBlock = parse_statement_block();
        }
        LineColumn span_end = this->current_token.pointer;

        return IfStatement(conditional, ifBlock, elseBlock, Span(span_start, span_end));
    };


    WhileLoop parse_while_loop()
    {
        LineColumn span_start = this->current_token.pointer;
        consume();

        BooleanExpression conditional = parse_boolean_expression();
        check_and_consume(TokenType::EoL);
        StatementBlock block = parse_statement_block();
        LineColumn span_end = this->current_token.pointer;
        return WhileLoop(conditional, block, Span(span_start, span_end));
    };

    ForLoop parse_for_loop()
    {
        LineColumn span_start = this->current_token.pointer;
        consume();
        std::vector<Identifier> ids = {};
        Value iterable = {{}};
        while(!lookahead({TokenType::IN}))
        {
            ids.push_back(parse_identifier());
            if (lookahead({TokenType::COMMA}))
            {
                consume();
            }
            else 
            {
                break;
            }
        }
        check_and_consume(TokenType::IN);
        if (lookahead({TokenType::LSQUARE}))
        {
            iterable = parse_iterable();
        } 
        else 
        {
            iterable = parse_identifier();
        }
        check_and_consume(TokenType::EoL);
        StatementBlock body = parse_statement_block();

        LineColumn span_end = this->current_token.pointer;
        return ForLoop(ids, iterable, body, Span(span_start, span_end));

    };

    // Expressions

    Expression parse_expression();

    Expression parse_primary();

    Expression parse_unary();

    Expression parse_parenthesis();

    FunctionCall parse_function_call();

    BinaryExpression parse_binary_expression();

    BooleanExpression parse_boolean_expression();

    ListComprehension parse_list_comprehension();

    Generator parse_generator();

    Iterable parse_iterable();

    Identifier parse_identifier()
    {
        Identifier id = Identifier(this->current_token.value, Span(this->current_token.pointer, this->current_token.pointer));
        check_and_consume(TokenType::ID);
        return id;
    };
};