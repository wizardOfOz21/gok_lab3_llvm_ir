#pragma once

#include <vector>
#include <string>
#include "operator.hpp"

using std::string;
using std::vector;

class ExprAST : public OperatorAST
{
public:
    virtual ~ExprAST() {}
};

class FCallAST : public ExprAST
{
public:
    string name;

    FCallAST(const string &name)
        : name(name) {}
};

class VariableAST : public ExprAST
{
public:
    string name;

    VariableAST(const string &name)
        : name(name) {}
};

class NumberAST : public ExprAST
{
public:
    int val;

    NumberAST(int val)
        : val(val){};
};

class UnaryOpExprAST : public ExprAST
{
public:
    char op;
    ExprAST *arg;

    UnaryOpExprAST(char op, ExprAST *arg)
        : op(op), arg(arg) {}
    ~UnaryOpExprAST()
    {
        delete arg;
    }
};

class BinaryOpExprAST : public ExprAST
{
public:
    char op;
    ExprAST *lhs;
    ExprAST *rhs;

    BinaryOpExprAST(char op, ExprAST *lhs, ExprAST *rhs)
        : op(op), lhs(lhs), rhs(rhs) {}
    ~BinaryOpExprAST()
    {
        delete lhs;
        delete rhs;
    }
};
