#include "includes/lexer.hpp"
#include "includes/ast_nodes.hpp"
#include <map>
class Parser
{

    static const std::map<std::string, int> precedence_map;

    public:

    Parser(Lexer lexer);

    Program parse();

    private:

    // utility

    bool lookahead(std::vector<Token> tokens);

    void consume(int repetition=1);

    void check_and_consume(TokenType expected_token_type);

    int check_precedence();

    // Statements

    StatementBlock parse_statement_block();

    Statement parse_statement();

    Assignment parse_assignment();

    Statement parse_return();

    FunctionDefenition parse_function_defenition();

    FunctionPrototype parse_function_prototype();

    Parameter parse_parameter();

    IfStatement parse_if_statement();

    WhileLoop parse_while_loop();

    ForLoop parse_for_loop();

    // Expressions

    Expression parse_expression();

    Expression parse_primary();

    Expression parse_unary();

    Expression parse_parenthesis();

    FunctionCall parse_function_call();

    BinaryExpression parse_binary_expression();

    ListComprehension parse_list_comprehension();

    Generator parse_generator();

};