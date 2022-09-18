#pragma once

#ifndef AST_HPP
#define AST_HPP

#include "token.hpp"
#include "span.hpp"
#include <vector>
// base classes
#include "types.hpp"

// #include "llvm/ADT/APFloat.h"
// #include "llvm/ADT/STLExtras.h"
// #include "llvm/IR/BasicBlock.h"
// #include "llvm/IR/Constants.h"
// #include "llvm/IR/DerivedTypes.h"
// #include "llvm/IR/Function.h"
// #include "llvm/IR/IRBuilder.h"
// #include "llvm/IR/LLVMContext.h"
// #include "llvm/IR/Module.h"
// #include "llvm/IR/Type.h"
// #include "llvm/IR/Verifier.h"


class JSONSerializable
{
    public:
        virtual std::string toJSON(int lvl) = 0;
};

class Node : public JSONSerializable
{
    public:
        Span span;
        Node(Span span) : span(span) {};
        std::string toJSON(int lvl) override { return "Should not happen";};
        virtual ~Node() = default;
        // virtual llvm::Value *codegen() =0;
};

class Statement : public Node
{
    public:
        Statement(Span span) : Node(span) {};
        std::string toJSON(int lvl) override { return "Should not happen";};
        // llvm::Value *codegen() override;
};

class Program: public Node
{
    std::vector<std::unique_ptr<Statement>> program;
    Span x;
    public:
        Program(std::vector<std::unique_ptr<Statement>> program, Span span): program(std::move(program)), Node(span) {};
        std::string toJSON(int lvl) override;
        // llvm::Value *codegen() override;
};

class Expression : public Statement
{
    public:
        Expression(Span span) : Statement(span) {};
        std::string toJSON(int lvl) override { return "Should not happen";};
        // llvm::Value *codegen() override;
};

class Value : public Expression
{
    public: 
        Value(Span span) : Expression(span) {};
        std::string toJSON(int lvl) override { return "Should not happen";};
        // llvm::Value *codegen() override;
};

class Identifier : public Value
{
    std::string value;
    public:
        Identifier(std::string value, Span span) : value(value), Value(span) {};
        std::string toJSON(int lvl) override;
        // llvm::Value *codegen() override;
};

class Iterable : public Value
{
    public:
        Iterable(Span span) : Value(span) {};
        std::string toJSON(int lvl) override { return "Should not happen";};
        // llvm::Value *codegen() override;
};

// Statements

class StatementBlock : public Statement
{
    std::vector<std::unique_ptr<Statement>> statements;
    public:
        StatementBlock(std::vector<std::unique_ptr<Statement>> statements, Span span) : statements(std::move(statements)), Statement(span) {};
        std::string toJSON(int lvl) override;
        // llvm::Value *codegen() override;
};

class Parameter : public Node
{
    std::string id;
    std::string type;
    public: 
        Parameter(std::string id, std::string type, Span span) : id(id), type(type), Node(span) {};
        std::string toJSON(int lvl) override;
        // llvm::Value *codegen() override;
};

class FunctionPrototype : public Statement
{
    std::string name;
    std::vector<std::unique_ptr<Parameter>> params;
    std::string return_type;
    public:
        FunctionPrototype(std::string name, std::vector<std::unique_ptr<Parameter>> params, std::string return_type, Span span) : name(name), params(std::move(params)), return_type(return_type), Statement(span) {};
        std::string toJSON(int lvl) override;
        // llvm::Function *codegen();
};

class FunctionDefenition : public Statement
{
    std::unique_ptr<FunctionPrototype> prototype;
    std::unique_ptr<Statement> body; // only ever an Expression or a StatementBlock
    public:
        FunctionDefenition(std::unique_ptr<FunctionPrototype> prototype, std::unique_ptr<Statement> body, Span span) : prototype(std::move(prototype)), body(std::move(body)), Statement(span) {};
        std::string toJSON(int lvl) override;
        // llvm::Function *codegen();
};

class ReturnStatement : public Statement
{
    std::unique_ptr<Expression> statement;
    public:
        ReturnStatement(std::unique_ptr<Expression> statement, Span span) : statement(std::move(statement)), Statement(span) {};
        std::string toJSON(int lvl) override;
        // llvm::Value *codegen() override;
};

class ForLoop : public Statement
{
    std::unique_ptr<StatementBlock> body;
    std::unique_ptr<Value> iterable;
    std::vector<std::unique_ptr<Identifier>> ids;
    public:
        ForLoop(std::vector<std::unique_ptr<Identifier>> ids, std::unique_ptr<Value> iterable, std::unique_ptr<StatementBlock> body, Span span) : body(std::move(body)), iterable(std::move(iterable)), ids(std::move(ids)), Statement(span) {};
        std::string toJSON(int lvl) override;
        // llvm::Value *codegen() override;

};

class WhileLoop : public Statement
{
    std::unique_ptr<Expression> condition;
    std::unique_ptr<StatementBlock> body;
    public:
        WhileLoop(std::unique_ptr<Expression> condition, std::unique_ptr<StatementBlock> body, Span span) : condition(std::move(condition)), body(std::move(body)), Statement(span) {};
        std::string toJSON(int lvl) override;
        // llvm::Value *codegen() override;
};

class Assignment : public Statement
{
    std::string id;
    std::string type;
    std::unique_ptr<Expression> rhs;
    public:
        Assignment(std::string id, std::string type, std::unique_ptr<Expression> rhs, Span span) : id(id), type(type), rhs(std::move(rhs)), Statement(span) {};
        std::string toJSON(int lvl) override;
        // llvm::Value *codegen() override;
};

class IfStatement : public Statement
{
    std::unique_ptr<Expression> conditional;
    std::unique_ptr<StatementBlock> ifBlock;
    std::unique_ptr<StatementBlock> elseBlock;
    public:
        IfStatement(std::unique_ptr<Expression> conditional, std::unique_ptr<StatementBlock> ifBlock, std::unique_ptr<StatementBlock> elseBlock, Span span) : conditional(std::move(conditional)), ifBlock(std::move(ifBlock)), elseBlock(std::move(elseBlock)), Statement(span) {};
        std::string toJSON(int lvl) override;
        // llvm::Value *codegen() override;
};

// Values

class NumericLiteral : public Value
{
    double value;
    public:
        NumericLiteral(double value, Span span) : value(value), Value(span) {};
        std::string toJSON(int lvl) override;
        // llvm::Value *codegen() override;
};

class BooleanLiteral: public Value
{
    bool value;
    public:
        BooleanLiteral(bool value, Span span) : value(value), Value(span) {};
        std::string toJSON(int lvl) override;  
        // llvm::Value *codegen() override;
};

class StringLiteral: public Iterable
{
    std::string value;
    public:
        StringLiteral(std::string value, Span span) : value(value), Iterable(span) {};
        std::string toJSON(int lvl) override;
        // llvm::Value *codegen() override;

};

class IterableLiteral: public Iterable
{
    std::vector<std::unique_ptr<Expression>> value;
    public:
        IterableLiteral(std::vector<std::unique_ptr<Expression>> value, Span span) : value(std::move(value)), Iterable(span) {};
        std::string toJSON(int lvl) override;
        // llvm::Value *codegen() override;
};



class Subscription : public Value
{
    std::unique_ptr<Identifier> id;
    std::unique_ptr<Expression> index;
    
    public:
        Subscription(std::unique_ptr<Identifier> id, std::unique_ptr<Expression> index, Span span) : id(std::move(id)), index(std::move(index)), Value(span) {};
        std::string toJSON(int lvl) override;
        // llvm::Value *codegen() override;
};

class AttributeReference : public Value
{
    std::unique_ptr<Identifier> object;
    std::unique_ptr<Value> attribute;

    public:
        AttributeReference(std::unique_ptr<Identifier> object, std::unique_ptr<Value> attribute, Span span) : object(std::move(object)), attribute(std::move(attribute)), Value(span) {};
        std::string toJSON(int lvl) override;
        // llvm::Value *codegen() override;
};

// iterables

class Slice : public Iterable
{
    std::unique_ptr<Identifier> id;
    std::unique_ptr<Expression> start;
    std::unique_ptr<Expression> stop;
    public:
        Slice(std::unique_ptr<Identifier> id, std::unique_ptr<Expression> start, std::unique_ptr<Expression> stop, Span span) : id(std::move(id)), start(std::move(start)), stop(std::move(stop)), Iterable(span) {};
        std::string toJSON(int lvl) override;
        // llvm::Value *codegen() override;
};

class List : public Iterable
{
   std::vector<std::string> elements;
   public:
        List(std::vector<std::string> elements, Span span) : elements(elements), Iterable(span) {};
        std::string toJSON(int lvl) override;
        // llvm::Value *codegen() override;
};      

class Set : public Iterable
{
    std::vector<double> elements;
    TypeDef Type;
    public: 
        Set(std::vector<double> elements, TypeDef type, Span span) : elements(elements), Type(type), Iterable(span) {};
        std::string toJSON(int lvl) override;
        // llvm::Value *codegen() override;
};  

class Generator : public Iterable
{
    std::unique_ptr<Expression> start;
    std::unique_ptr<Expression> step;
    std::unique_ptr<Expression> stop;
    public:
         Generator(std::unique_ptr<Expression> start, std::unique_ptr<Expression> step, std::unique_ptr<Expression> stop, Span span) : start(std::move(start)), stop(std::move(stop)), step(std::move(step)), Iterable(span) {};
         std::string toJSON(int lvl) override;
         // llvm::Value *codegen() override;
};



// Expressions


class BinaryExpression: public Expression
{
    std::unique_ptr<Expression> lhs;
    std::unique_ptr<Expression> rhs;
    std::string op;
    public:
        BinaryExpression(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs, std::string op, Span span) : lhs(std::move(lhs)), rhs(std::move(rhs)), op(op), Expression(span) {};
        std::string toJSON(int lvl) override;
        // llvm::Value *codegen() override;
};

class BooleanExpression: public BinaryExpression
{
    public:
        BooleanExpression(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs, std::string op, Span span) : BinaryExpression(std::move(lhs), std::move(rhs), op, span) {};
        std::string toJSON(int lvl) override { return "should not happen";};
};

class ListComprehension: public Iterable
{
    std::unique_ptr<Expression> body;
    std::vector<std::unique_ptr<Identifier>> ids;
    std::unique_ptr<Value> iterable;
    std::unique_ptr<Expression> filter;
    public:
        ListComprehension(std::unique_ptr<Expression> body, std::vector<std::unique_ptr<Identifier>> ids, std::unique_ptr<Value> iterable, std::unique_ptr<Expression> filter, Span span) : body(std::move(body)), ids(std::move(ids)), iterable(std::move(iterable)), filter(std::move(filter)), Iterable(span) {};
        std::string toJSON(int lvl) override;
        // llvm::Value *codegen() override;
};

class FunctionCall: public Expression
{
     std::string id;
     std::vector<std::unique_ptr<Expression>> args;

     public:
        FunctionCall(const std::string id, std::vector<std::unique_ptr<Expression>> args, Span span) : id(id), args(std::move(args)), Expression(span) {};
        std::string toJSON(int lvl) override;
        // llvm::Value *codegen() override;
};


#endif


