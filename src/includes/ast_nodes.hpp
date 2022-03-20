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

// Statements

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
    Expression condition;
    StatementBlock block;
    public:
        WhileLoop(Expression condition, StatementBlock block, Span span) : condition(condition), block(block), Statement(span) {};
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
    Expression conditional;
    StatementBlock ifBlock;
    StatementBlock elseBlock;
    public:
        IfStatement(Expression conditional, StatementBlock ifBlock, StatementBlock elseBlock, Span span) : conditional(conditional), ifBlock(ifBlock), elseBlock(elseBlock), Statement(span) {};
};

class Expression : public Statement
{
    public:
        Expression(Span span) : Statement(span) {};
        virtual ~Expression() {}
        virtual llvm::Value *codegen();

};

// Values

class Value : public Expression
{
    public: 
        Value(Span span) : Expression(span) {};
        virtual ~Value() {}
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

class StringLiteral: public Iterable
{
    std::string value;
    public:
        StringLiteral(std::string value, Span span) : value(value), Iterable(span) {};
        llvm::Value *codegen();
};

class Identifier : public Value
{
    std::string value;
    public:
        Identifier(std::string value, Span span) : value(value), Value(span) {};
        llvm::Value *codegen();
};

class Subscription : public Value
{
    Identifier id;
    Expression index;
    
    public:
        Subscription(Identifier id, Expression index, Span span) : id(id), index(index), Value(span) {};
        llvm::Value *codegen();
};

class AttributeReference : public Value
{
    Identifier object;
    Value attribute;

    public:
        AttributeReference(Identifier object, Value attribute, Span span) : object(object), attribute(attribute), Value(span) {};
        llvm::Value *codegen();
};

// iterables

class Iterable : public Value
{
    public:
        Iterable(Span span) : Value(span) {};
};

class Slice : public Iterable
{
    Identifier id;
    Expression start;
    Expression stop;
    public:
        Slice(Identifier id, Expression start, Expression stop, Span span) : id(id), start(start), stop(stop), Iterable(span) {};
        llvm::Value *codegen();
};

class List : public Iterable
{
   std::vector<std::string> elements;
   public:
        List(std::vector<std::string> elements, Span span) : elements(elements), Iterable(span) {};
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
    Expression start;
    Expression step;
    Expression stop;
    public:
         Generator(Expression start, Expression step, Expression stop, Span span) : start(start), stop(stop), step(step), Iterable(span) {};
         llvm::Value *codegen();
};



// Expressions


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
    Expression body;
    std::vector<Identifier> ids;
    Value iterable;
    Expression filter;
    public:
        ListComprehension(Expression body, std::vector<Identifier> ids, Value iterable, Expression filter, Span span) : body(body), ids(ids), iterable(iterable), filter(filter), Expression(span) {};
        llvm::Value *codegen();
};

class FunctionCall: public Expression
{
     std::string id;
     std::vector<Expression> args;

     public:
        FunctionCall(const std::string id, std::vector<Expression> args, Span span) : id(id), args(args), Expression(span) {};
        llvm::Value *codegen();
};





