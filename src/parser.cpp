
#include "includes/lexer.hpp"
#include "includes/ast.hpp"
#include "includes/parser.hpp"
#include "includes/span.hpp"
#include <iostream>

Parser::Parser(std::vector<Token> tokens)
{
    this->tokens = tokens;
    this->index = 0;
    this->current_token = this->tokens[this->index];
};

std::unique_ptr<Program> Parser::parse()
{
    LineColumn span_start = this->current_token.pointer;
    std::vector<std::unique_ptr<Statement>> program = parse_statements();
    LineColumn span_end = this->current_token.pointer;
    return std::make_unique<Program>(std::move(program), (Span){span_start, span_end});
};

// utility

// looks ahead to see if there is a sequence of tokens
bool Parser::lookahead(std::vector<TokenType> tokens)
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

std::string Parser::consume(int repetitions)
{
    std::string res = this->current_token.value;
    for (int i = 0; i<repetitions; i++)
    {
        if (this->current_token.type != TokenType::EoF)
        {
            ++this->index;
            this->current_token = this->tokens[this->index];
        }
    }
    return res;
};

void Parser::check_and_consume(TokenType expected_token_type)
{
    if (this->current_token.type != expected_token_type && this->current_token.type != TokenType::EoF)
    {
        throw std::runtime_error("Unexpected token: {" + this->current_token.value + "}");
    }
    this->consume();
};

int Parser::check_precedence()
{
    std::map<std::string, int>::iterator iter = precedence_map.find(current_token.value);
    if (iter != precedence_map.end() )
    {
        return iter->second;
    } 
    else {
        return -1;
    }
};

bool Parser::check_if_func_def() 
{

    int i = this->index;
    while (tokens[i].type != TokenType::RPAREN)
    {
        i++;
    }
    ++i;
    if (tokens[i].type == TokenType::ASSIGN || tokens[i].type == TokenType::TYPE_DECL)
    {
        return true;
    }
    else if (tokens[i].type == TokenType::EoL && tokens[i+1].type == TokenType::INDENT)
    {
        return true;
    }
    else 
    {
        return false;
    }
};
 
bool Parser::check_if_list_comp() 
{ 
    int i = this->index;
    while(tokens[i].type != TokenType::RSQUARE) 
    {
        if (tokens[i].type == TokenType::FOR) 
            return true;
        ++i;
    }
    return false;
};

bool Parser::check_if_generator() 
{ 
    int i = this->index;
    while(tokens[i].type != TokenType::RSQUARE) 
    {
        if (tokens[i].type == TokenType::ELIPSIS) 
            return true;
        ++i;
    }
    return false;
};

Span Parser::current_span()
{
    return {this->current_token.pointer, this->current_token.pointer};
};

// Statements

std::unique_ptr<StatementBlock> Parser::parse_statement_block()
{
    LineColumn span_start = this->current_token.pointer;
    this->check_and_consume(TokenType::INDENT);
    std::vector<std::unique_ptr<Statement>> statements = this->parse_statements(TokenType::DEDENT);
    this->consume(); // DEDENT or EoF
    LineColumn span_end = this->current_token.pointer;
    return std::make_unique<StatementBlock>(std::move(statements), (Span){span_start, span_end});
};

std::vector<std::unique_ptr<Statement>> Parser::parse_statements(TokenType end_token)
{
    std::vector<std::unique_ptr<Statement>> statements;
    while(this->current_token.type != end_token && this->current_token.type != TokenType::EoF)
    {
        statements.push_back(this->parse_statement());
    };
    return statements;
};

std::unique_ptr<Statement> Parser::parse_statement()
{
    if (lookahead({TokenType::ID, TokenType::TYPE_DECL, TokenType::ID, TokenType::ASSIGN}) || lookahead({TokenType::ID, TokenType::ASSIGN}))
    {
        return parse_assignment();
    }
    else if (lookahead({TokenType::ID, TokenType::LPAREN}))
    {
        if (check_if_func_def())
        {
            return parse_function_defenition();
        } else {
            std::unique_ptr<Expression> e = parse_expression();
            check_and_consume(TokenType::EoL);
            return e;
        }
    }
    else if (this->current_token.type == TokenType::RETURN)
    {
        return parse_return_statement();
    }
    else if (this->current_token.type == TokenType::IF)
    {
        return parse_if_statement();
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

std::unique_ptr<Assignment> Parser::parse_assignment()
{
    LineColumn span_start = this->current_token.pointer;
    std::string name = this->current_token.value;
    std::string type = "";
    consume();
    if (current_token.type==TokenType::TYPE_DECL)
    {
        consume(); // consume :
        type = this->current_token.value;
        consume(); // consume typedef
    }
    consume(); // consume =
    std::unique_ptr<Expression> rhs = parse_expression();
    LineColumn span_end = this->current_token.pointer;
    std::unique_ptr<Assignment> assignment = std::make_unique<Assignment>(name, type, std::move(rhs), (Span){span_start, span_end});
    check_and_consume(TokenType::EoL);
    return assignment;
};

std::unique_ptr<FunctionDefenition> Parser::parse_function_defenition()
{
    LineColumn span_start = this->current_token.pointer;
    std::unique_ptr<FunctionPrototype> prototype = parse_function_prototype();
    if (this->current_token.type == TokenType::ASSIGN)
    {
        std::unique_ptr<Statement> body = parse_return_statement();
        LineColumn span_end = this->current_token.pointer;
        return std::make_unique<FunctionDefenition>(std::move(prototype), std::move(body), (Span){span_start, span_end});
    }
    else if (lookahead({TokenType::EoL, TokenType::INDENT}))
    {
        consume();
        std::unique_ptr<StatementBlock> body = parse_statement_block();
        LineColumn span_end = this->current_token.pointer;
        return std::make_unique<FunctionDefenition>(std::move(prototype), std::move(body), (Span){span_start, span_end});
    }
    else
    {
        throw "something";
    }
};

std::unique_ptr<FunctionPrototype> Parser::parse_function_prototype()
{
    LineColumn span_start = this->current_token.pointer;
    std::string name = this->current_token.value;
    std::string return_type = "";
    consume(2);
    std::vector<std::unique_ptr<Parameter>> parameters = {};
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
    return std::make_unique<FunctionPrototype>(name, std::move(parameters), return_type, (Span){span_start, span_end});
};

std::unique_ptr<Parameter> Parser::parse_parameter()
{        
    LineColumn span_start = this->current_token.pointer;

    std::string name = this->current_token.value;
    check_and_consume(TokenType::ID);
    check_and_consume(TokenType::TYPE_DECL);
    std::string type = this->current_token.value;
    check_and_consume(TokenType::ID);
    LineColumn span_end = this->current_token.pointer;
    return std::make_unique<Parameter>(name, type, (Span){span_start, span_end});
};


std::unique_ptr<Statement> Parser::parse_return_statement()
{
    LineColumn span_start = this->current_token.pointer;
    consume();
    std::unique_ptr<Expression> expression = parse_expression();
    LineColumn span_end = this->current_token.pointer;
    check_and_consume(TokenType::EoL);
    return std::make_unique<ReturnStatement>(std::move(expression), (Span){span_start, span_end});
};


std::unique_ptr<IfStatement> Parser::parse_if_statement()
{
    LineColumn span_start = this->current_token.pointer;
    consume();
    std::unique_ptr<Expression> conditional = parse_expression();
    check_and_consume(TokenType::EoL);
    std::unique_ptr<StatementBlock> ifBlock = parse_statement_block();
    LineColumn span_end = this->current_token.pointer;
    std::unique_ptr<StatementBlock> elseBlock = std::make_unique<StatementBlock>((std::vector<std::unique_ptr<Statement>>){}, current_span());

    if (lookahead({TokenType::ELSE, TokenType::IF}))
    {
        consume();
        std::unique_ptr<IfStatement> nestedIf = parse_if_statement();
        Span span = nestedIf->span;
        std::vector<std::unique_ptr<Statement>> statements;
        statements.push_back(std::move(nestedIf));
        elseBlock = std::make_unique<StatementBlock>(std::move(statements), span);
    } else if (lookahead({TokenType::ELSE, TokenType::EoL, TokenType::INDENT}))
    {
        consume(2);
        elseBlock = parse_statement_block();
    }
    span_end = this->current_token.pointer;

    return std::make_unique<IfStatement>(std::move(conditional), std::move(ifBlock), std::move(elseBlock), (Span){span_start, span_end});
};


std::unique_ptr<WhileLoop> Parser::parse_while_loop()
{
    LineColumn span_start = this->current_token.pointer;
    consume();

    std::unique_ptr<Expression> conditional = parse_expression();
    check_and_consume(TokenType::EoL);
    std::unique_ptr<StatementBlock> block = parse_statement_block();
    LineColumn span_end = this->current_token.pointer;
    return std::make_unique<WhileLoop>(std::move(conditional), std::move(block), (Span){span_start, span_end});
};

std::unique_ptr<ForLoop> Parser::parse_for_loop()
{
    LineColumn span_start = this->current_token.pointer;
    consume();
    std::vector<std::unique_ptr<Identifier>> ids = {};
    std::unique_ptr<Value> iterable;
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
    std::unique_ptr<StatementBlock> body = parse_statement_block();

    LineColumn span_end = this->current_token.pointer;
    return std::make_unique<ForLoop>(std::move(ids), std::move(iterable), std::move(body), (Span){span_start, span_end});

};

// Expressions

std::unique_ptr<Expression> Parser::parse_expression()
{
    std::unique_ptr<Expression> lhs = parse_unary();
    return parse_binary_expression(0, std::move(lhs));
};

std::unique_ptr<Expression> Parser::parse_primary()
{
    if (lookahead({TokenType::ID, TokenType::LPAREN}))
    {
        return parse_function_call();
    }
    else if (lookahead({TokenType::ID, TokenType::PERIOD}))
    {
        return parse_attribute_reference();
    }
    else if (lookahead({TokenType::LPAREN}))
    {
        return parse_parenthesis();
    }
    else if (lookahead({TokenType::LSQUARE}))
    {
        return parse_iterable();
    }
    else if (lookahead({TokenType::NUMBER}))
    {
        return parse_numeric_literal();
    }
    else if (lookahead({TokenType::STRING}))
    {
        return parse_string_literal();
    }
    else if (lookahead({TokenType::BOOL}))
    {
        return parse_boolean_literal();
    }
    else if (lookahead({TokenType::ID}))
    {
        return parse_identifier();
    }
    else 
    {
        throw std::runtime_error("Unexpected token " + this->current_token.value);
    }
};

std::unique_ptr<Expression> Parser::parse_unary()
{
    // this function is for unary operators
    return parse_primary();
};

std::unique_ptr<Expression> Parser::parse_binary_expression(int precedence, std::unique_ptr<Expression> lhs)
{
    int current_precedence = check_precedence();
    while(true)
    {
        LineColumn span_start = current_token.pointer;
        current_precedence = check_precedence();
        if(current_precedence < precedence)
            return lhs;
        std::string op = this->current_token.value;
        consume();
        std::unique_ptr<Expression> rhs = parse_unary();
        int next_precedence = check_precedence();
        if (current_precedence < next_precedence)
        {
            rhs = parse_binary_expression(current_precedence + 1, std::move(rhs));
        }
        LineColumn span_end = current_token.pointer;
        lhs = std::make_unique<BinaryExpression>(std::move(lhs), std::move(rhs), op, (Span){span_start, span_end});
    }
};


std::unique_ptr<Expression> Parser::parse_parenthesis()
{
    consume();
    std::unique_ptr<Expression> expression = parse_expression();
    check_and_consume(TokenType::RPAREN);
    return expression;
};

std::unique_ptr<FunctionCall> Parser::parse_function_call()
{
    LineColumn span_start = this->current_token.pointer;
    std::string name = this->current_token.value;
    check_and_consume(TokenType::ID);
    check_and_consume(TokenType::LPAREN);
    std::vector<std::unique_ptr<Expression>> args = {};
    while(!lookahead({TokenType::RPAREN}))
    {
        args.push_back(parse_expression());
        if (!lookahead({TokenType::COMMA}))
        {
            break;
        }
        consume();
    }
    check_and_consume(TokenType::RPAREN);
    LineColumn span_end = this->current_token.pointer;

    return std::make_unique<FunctionCall>(name, std::move(args), (Span){span_start, span_end});
};

std::unique_ptr<ListComprehension> Parser::parse_list_comprehension()
{
    LineColumn span_start = this->current_token.pointer;
    consume();
    std::unique_ptr<Expression> result = parse_expression();
    check_and_consume(TokenType::FOR);
    std::vector<std::unique_ptr<Identifier>> ids = {};
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
    std::unique_ptr<Value> iterable;

    if (lookahead({TokenType::LSQUARE}))
    {
        iterable = parse_iterable();
    } 
    else 
    {
        iterable = parse_identifier();
    }

    std::unique_ptr<Expression> filter = std::make_unique<BooleanLiteral>(true, current_span());
    if (lookahead({TokenType::IF}))
    {
        consume();
        filter = parse_expression();
    }
    check_and_consume(TokenType::RSQUARE);
    LineColumn span_end = this->current_token.pointer;
    return std::make_unique<ListComprehension>(std::move(result), std::move(ids), std::move(iterable), std::move(filter), (Span){span_start, span_end});
};

std::unique_ptr<Generator> Parser::parse_generator()
{
    LineColumn span_start = this->current_token.pointer;
    consume();
    std::unique_ptr<Expression> start = parse_expression();
    std::unique_ptr<Expression> step = std::make_unique<NumericLiteral>(1, current_span());
    if (lookahead({TokenType::COMMA}))
    {
        consume();
        step = parse_expression();
    }
    check_and_consume(TokenType::ELIPSIS);
    std::unique_ptr<Expression> stop = parse_expression();
    check_and_consume(TokenType::RSQUARE);
    LineColumn span_end = this->current_token.pointer;

    return std::make_unique<Generator>(std::move(start), std::move(step), std::move(stop), (Span){span_start, span_end});
};

std::unique_ptr<IterableLiteral> Parser::parse_literal_iterable()
{
    LineColumn span_start = this->current_token.pointer;
    consume();
    std::vector<std::unique_ptr<Expression>> values;
    while (this->current_token.type != TokenType::RSQUARE)
    {
        values.emplace_back(std::move(parse_expression()));
        if (current_token.type == TokenType::COMMA)
        {
            consume();
        }
    }
    LineColumn span_end = this->current_token.pointer;
    consume();
    return std::make_unique<IterableLiteral>(std::move(values), (Span){span_start, span_end});

}

std::unique_ptr<Iterable> Parser::parse_iterable()
{

    
    if (check_if_list_comp())
    {
        return parse_list_comprehension();
    }
    else if (check_if_generator())
    {
        return parse_generator();
    } else {
        return parse_literal_iterable();
    }
    
};

std::unique_ptr<Identifier> Parser::parse_identifier()
{
    std::unique_ptr<Identifier> id = std::make_unique<Identifier>(this->current_token.value, current_span());
    check_and_consume(TokenType::ID);
    return id;
};

// Slice Parser::parse_iterable_slice()
// {
//     return Slice()
// };

std::unique_ptr<Subscription> Parser::parse_iterable_subscription()
{
    LineColumn span_start = this->current_token.pointer;
    std::unique_ptr<Identifier> id = std::make_unique<Identifier>(this->current_token.value, current_span());
    consume(2);

    std::unique_ptr<Expression> index = parse_expression();
    check_and_consume(TokenType::RSQUARE);

    LineColumn span_end = this->current_token.pointer;
    return std::make_unique<Subscription>(std::move(id),std::move(index), (Span){span_start, span_end});
};

std::unique_ptr<Value> Parser::parse_attribute_reference()
{
    LineColumn span_start = this->current_token.pointer;
    std::unique_ptr<Identifier> object = std::make_unique<Identifier>(this->current_token.value, current_span());
    consume();
    if (lookahead({TokenType::PERIOD}))
    {
        consume();
        std::unique_ptr<Value> reference = parse_attribute_reference();
        LineColumn span_end = this->current_token.pointer;
        return std::make_unique<AttributeReference>(std::move(object), std::move(reference), (Span){span_start, span_end}); // span here is broken
    }
    return object;
};

std::unique_ptr<NumericLiteral> Parser::parse_numeric_literal()
{
    return std::make_unique<NumericLiteral>(std::stod(consume()), current_span());
};

std::unique_ptr<BooleanLiteral> Parser::parse_boolean_literal()
{
    return std::make_unique<BooleanLiteral>(consume()=="true" ? true : false, current_span());
};

std::unique_ptr<StringLiteral> Parser::parse_string_literal()
{
    return std::make_unique<StringLiteral>(consume(), current_span());
};

// List Parser::parse_list() {};



