#include "token.hpp"
#include "span.hpp"
#include <vector>
#include "llvm/IR/BasicBlock.h"
// base classes
#include "types.hpp"

class Node
{
    public:
        Span span;
    protected:
        Node(Span span) : span(span) {};
        virtual ~Node(){};
};

class Program: public Node
{
    std::vector<Statement> program;
    Span x;
    public:
        Program(std::vector<Statement> program, Span span): program(program), Node(span) {};
};

class Statement : public Node
{
    public:
        Statement(Span span) : Node(span) {};
};

class StatementBlock : public Statement
{
    std::vector<Statement> statements;
    public:
        StatementBlock(std::vector<Statement> statements, Span span) : statements(statements), Statement(span) {};
};

class Parameter : public Node
{
    std::string id;
    std::string type;
    public: 
        Parameter(std::string id, std::string type, Span span) : id(id), type(type), Node(span) {};
};

class FunctionPrototype : public Statement
{
    std::string name;
    std::vector<Parameter> params;
    std::string return_type;
    public:
        FunctionPrototype(std::string name, std::vector<Parameter> params, std::string return_type, Span span) : name(name), params(params), return_type(return_type), Statement(span) {};
};

class FunctionDefenition : public Statement
{
    FunctionPrototype prototype;
    Statement body; // only ever an Expression or a StatementBlock
    public:
        FunctionDefenition(FunctionPrototype prototype, Statement body, Span span) : prototype(prototype), body(body), Statement(span) {};
};

class ForLoop : public Statement
{
    StatementBlock body;
    Value iterable;
    std::vector<Identifier> ids;
    public:
        ForLoop(std::vector<Identifier> ids, Value iterable, StatementBlock body, Span span) : body(body), iterable(iterable), ids(ids), Statement(span) {};

};

class WhileLoop : public Statement
{
    BooleanExpression condition;
    StatementBlock block;
    public:
        WhileLoop(BooleanExpression condition, StatementBlock block, Span span) : condition(condition), block(block), Statement(span) {};
};

class Assignment : public Statement
{
    std::string id;
    std::string type;
    Expression rhs;
    public:
        Assignment(std::string id, std::string type, Expression rhs, Span span) : id(id), type(type), rhs(rhs), Statement(span) {};

};

class IfStatement : public Statement
{
    BooleanExpression conditional;
    StatementBlock ifBlock;
    StatementBlock elseBlock;
    public:
        IfStatement(BooleanExpression conditional, StatementBlock ifBlock, StatementBlock elseBlock, Span span) : conditional(conditional), ifBlock(ifBlock), elseBlock(elseBlock), Statement(span) {};
};

class Expression : public Statement
{
    public:
        Expression(Span span) : Statement(span) {};
        virtual ~Expression() {}
        virtual llvm::Value *codegen();

};

class Value : public Expression
{
    public: 
        Value(Span span) : Expression(span) {};
        virtual ~Value() {}
};

class Iterable : public Value
{
    public:
        Iterable(Span span) : Value(span) {};
};

class Set : public Iterable
{
    std::vector<double> Elem;
    TypeDef Type;
    public: 
        Set(std::vector<double> elem, TypeDef type, Span span) : Elem(elem), Type(type), Iterable(span) {};
        llvm::Value *codegen();
};   

class Generator : public Iterable
{
    Value start;
    Value step;
    Value stop;
    public:
         Generator(Value start, Value step, Value stop, Span span) : start(start), stop(stop), step(step), Iterable(span) {};
         llvm::Value *codegen();
};




class NumericLiteral : public Value
{
    double value;
    public:
        NumericLiteral(double value, Span span) : value(value), Value(span) {};
        llvm::Value *codegen();
};

class BooleanLiteral: public Value
{
    bool value;
    public:
        BooleanLiteral(bool value, Span span) : value(value), Value(span) {};  
        llvm::Value *codegen();
};

class StringLiteral: public Value
{
    std::string value;
    public:
        StringLiteral(std::string value, Span span) : value(value), Value(span) {};
        llvm::Value *codegen();
};

class Identifier: public Value
{
    std::string value;
    public:
        Identifier(std::string value, Span span) : value(value), Value(span) {};
        llvm::Value *codegen();
};

class BinaryExpression: public Expression
{
    Expression lhs;
    Expression rhs;
    TokenType op;
    public:
        BinaryExpression(Expression lhs, Expression rhs, TokenType op, Span span) : lhs(lhs), rhs(rhs), op(op), Expression(span) {};
        llvm::Value *codegen();
};

class BooleanExpression: public BinaryExpression
{
    public:
        BooleanExpression(Expression lhs, Expression rhs, TokenType op, Span span) : BinaryExpression(lhs, rhs, op, span) {};
};

class ListComprehension: public Expression
{

};

class FunctionCall: public Expression
{
     std::string id;
     std::vector<Expression> args;

     public:
        FunctionCall(const std::string id, std::vector<Expression> args, Span span) : id(id), args(args), Expression(span) {};
        llvm::Value *codegen();
};





