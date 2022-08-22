#include "includes/ast.hpp"
#include <iostream>
#include <map>

std::string newJsonNode(int lvl, std::string name, std::map<std::string, std::unique_ptr<Node>> children) 
{

    std::string tab(lvl,'\t');
    std::string out = tab +"{\n";
    out += tab + "\"" + name + "\": {\n";
    for (auto const& [key, value] : children) {
        out += tab + "\t\"" + key + "\": " + value->toJSON(lvl+1) + ",\n";
    }
    out += tab + "}\n";
    return out;
}

std::string Program::toJSON(int lvl)
{

    std::string out = "{\n";
    std::string tab(lvl,'\t');
    out += "\t\"program\" : [";
    for (auto& s : program)
    {
        out += "\n" + s->toJSON((lvl+2)) + ",";
    }
    out += "\n" + tab + "]";
    out += "\n}";
    return out;
}

std::string Identifier::toJSON(int lvl)
{
    std::string out = "";
    std::string tab(lvl, '\t');
    out = out + "\"identifier\" : " + "\"" + this->value + "\"";
    return out;
}

std::string Parameter::toJSON(int lvl)
{
    std::string out = "{";
    std::string tab(lvl, '\t');
    out += "\n" + tab + "\"parameter\" : {";
    out += "\n\t" + tab + "\"id\" : \"" + this->id + "\"";
    out += "\n\t" + tab + "\"type\" : \"" + this->type + "\"";
    out += "\n\t" + tab + "}";
    return out;
}

std::string StatementBlock::toJSON(int lvl)
{
    std::string out = "{ ";
    for (auto& s : statements)
    {
        std::string tab(lvl, '\t');
        out = out + "\n" + tab + "\"StatementBlock\" : {" + s->toJSON((lvl+1)) + "\n" + tab + "}";
    }
    return out;
}

std::string FunctionPrototype::toJSON(int lvl)
{
    std::string out = "{";
    std::string tab(lvl, '\t');
    out += "\n" + tab + "\"FunctionPrototype\" : {";
    out += "\n\t" + tab +"\"name \" :" + name + ",";
    out += "\n\t" + tab +"\"params\": ";
    for (auto& p : params)
    {
        out += p->toJSON(lvl+1);
    }
    out += "\n\t" + tab + "],";
    out += "\n\t" + tab +"\"return_type\" :" + return_type + ",";
    out += "\n" + tab + "}";
    return out;
}

std::string FunctionDefenition::toJSON(int lvl)
{
    std::string tab(lvl, '\t');
    std::string out = tab + "{";

    out += "\n\t" + tab + "\"FunctionDefenition\" : {";
    out += "\n\t\t" + tab + "\"proto\" :" + prototype->toJSON(lvl+2) + ",";
    out += "\n\t\t" + tab +"\"body\" :" + body->toJSON(lvl+2) + ",";
    out += "\n\t" + tab + "}";
    return out;
}

std::string ForLoop::toJSON(int lvl)
{
    std::string out = "{ ";
    std::string tab(lvl, '\t');
    out += "\n" + tab + "\"ForLoop\" : {";
    out += "\n\t" + tab + "\"iterable: \"" + iterable->toJSON(lvl+1) + ",";
    out += "\n\t" + tab + "\"body: \"" + body->toJSON(lvl+1) + ",";
    for (auto& i : ids)
    {
        out += "\n\t" + tab + "\"id: \"" + i->toJSON(lvl+1) + ",";
    }
    out += "\n" + tab + "}";
    return out;
}

std::string WhileLoop::toJSON(int lvl)
{
    std::string out = "{ ";
    std::string tab(lvl, '\t');
    out += "\n" + tab + "\"WhileLoop\" : {";
    out += "\n\t" + tab + "\"condition: \"" + condition->toJSON(lvl+1) + ",";
    out += "\n\t" + tab + "\"body: \"" + body->toJSON(lvl+1) + ",";
    out += "\n" + tab + "}";
    return out;
}

std::string Assignment::toJSON(int lvl)
{
    std::string out = "{ ";
    std::string tab(lvl, '\t');
    out += "\n" + tab + "\"Assignment\" : {";
    out += "\n\t" + tab + "\"id\" : \"" + id + "\",";
    out += "\n\t" + tab + "\"type\" : \"" + type + "\",";
    out += "\n\t" + tab + "\"expr\" : " + rhs->toJSON(lvl+1) + ",";
    out += "\n" + tab + "}";
    return out;
}

std::string IfStatement::toJSON(int lvl)
{
    std::string out = "{ ";
    std::string tab(lvl, '\t');
    out += "\n" + tab + "\"IfStatement\" : {";
    out += "\n\t" + tab + "\"condition\" : \"" + conditional->toJSON(lvl+1) + ",";
    out += "\n\t" + tab + "\"ifblock\" : \"" + ifBlock->toJSON(lvl+1) + ",";
    out += "\n\t" + tab + "\"elseblock\" : \"" + elseBlock->toJSON(lvl+1) + ",";
    out += "\n" + tab + "}";
    return out;
}

std::string NumericLiteral::toJSON(int lvl)
{
    std::string out = "";
    std::string tab(lvl, '\t');
    out = out + "\"numeric_literal\" : " + "\"" + std::to_string(this->value) + "\"";
    return out;
}

std::string BooleanLiteral::toJSON(int lvl)
{
    std::string out = "";
    std::string tab(lvl, '\t');
    out = out + "\"boolean_literal\" : " + "\"" + (this->value ? "true" : "false") + "\"";
    return out;
}

std::string StringLiteral::toJSON(int lvl)
{
    std::string out = "";
    std::string tab(lvl, '\t');
    out = out + "\"string_literal\" : " + "\"" + this->value + "\"";
    return out;
}

std::string Subscription::toJSON(int lvl)
{
    std::string out = "";
    std::string tab(lvl, '\t');
    out = out + "\"subscription\" : {" ;
    out += "\n" + tab + "\"id\" : " + id->toJSON(lvl+1);
    out += "\n" + tab + "\"index\" : " + index->toJSON(lvl+1);
    out += "\n" + tab + "}";
    return out;
}

std::string AttributeReference::toJSON(int lvl)
{
    std::string out = "";
    std::string tab(lvl, '\t');
    out = out + "\"attribute_reference\" : {" ;
    out += "\n" + tab + "\"id\" : " + object->toJSON(lvl+1);
    out += "\n" + tab + "\"attr\" : " + attribute->toJSON(lvl+1);
    out += "\n" + tab + "}";
    return out;
}

std::string Slice::toJSON(int lvl)
{
    std::string out = "";
    std::string tab(lvl, '\t');
    out = out + "\"slice\" : {" ;
    out += "\n" + tab + "\"id\" : " + id->toJSON(lvl+1);
    out += "\n" + tab + "\"start\" : " + start->toJSON(lvl+1);
    out += "\n" + tab + "\"stop\" : " + stop->toJSON(lvl+1);
    out += "\n" + tab + "}";
    return out;
}

std::string List::toJSON(int lvl)
{
    std::string out = "";
    std::string tab(lvl, '\t');
    out = out + "\"list\" : [" ;
    for (auto& i : elements)
    {
        out += i+ ",";
    }
    out += "\n" + tab + "]";
    return out;
}

std::string Set::toJSON(int lvl)
{
    std::string out = "";
    std::string tab(lvl, '\t');
    out = out + "\"set\" : [" ;
    for (auto& i : elements)
    {
        out += std::to_string(i) + ",";
    }
    out += "\n" + tab + "]";
    return out;
}

std::string Generator::toJSON(int lvl)
{
    std::string out = "";
    std::string tab(lvl, '\t');
    out = out + "\"generator\" : {" ;
    out += "\n\t" + tab + "\"start\" : " + start->toJSON(lvl+1);
    out += "\n\t" + tab + "\"step\" : " + step->toJSON(lvl+1);
    out += "\n\t" + tab + "\"stop\" : " + stop->toJSON(lvl+1);
    out += "\n\t" + tab + "}";
    return out;
}

std::string BinaryExpression::toJSON(int lvl)
{
    std::string out = "";
    std::string tab(lvl, '\t');
    out = out + "\"binary_expression\" : {" ;
    out += "\n\t" + tab + "\"op\" : " + op;
    out += "\n\t" + tab + "\"lhs\" : " + lhs->toJSON(lvl+1);
    out += "\n\t" + tab + "\"rhs\" : " + rhs->toJSON(lvl+1);
    out += "\n" + tab + "}";
    return out;
}

std::string ListComprehension::toJSON(int lvl)
{
    std::string out = "";
    std::string tab(lvl, '\t');
    out = out + "\"list_comprehension\" : {" ;
    out += "\n" + tab + "\"body\" : " + body->toJSON(lvl+1);
    out += "\n" + tab + "\"iterable\" : " + iterable->toJSON(lvl+1);
    for (auto& i : ids)
    {
        out += "\n" + tab + "\"id\" : " + i->toJSON(lvl+1);
    }
    out += "\n" + tab + "\"filter\" : " + filter->toJSON(lvl+1);
    out += "\n" + tab + "}";
    return out;
}

std::string FunctionCall::toJSON(int lvl)
{
    std::string out = "{";
    std::string tab(lvl, '\t');
    out += "\n" + tab + "\"function_call\" : {" ;
    out += "\n" + tab + "\"id\" : \"" + id + "\",";
    for (auto& i : args)
    {
        out += "\n" + tab + "\"arg\" : " + i->toJSON(lvl+1);
    }
    out += "\n" + tab + "}";
    return out;
}

std::string ReturnStatement::toJSON(int lvl)
{
    std::string out = "{ \"return_statement\" : " + statement->toJSON(lvl);
    return out;
}

std::string IterableLiteral::toJSON(int lvl)
{
    std::string tab(lvl, '\t');
    std::string out = "{ \"iterable_literal\" : ";
    for (auto& e : value)
    {
        out += "\t" + e->toJSON(lvl+1);
    }

    return out;
}