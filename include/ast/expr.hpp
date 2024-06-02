#pragma once

#include <vector>
#include <string>
#include "operator.hpp"
#include "builder.hpp"

using std::string;
using std::vector;

class ExprAST : public OperatorAST
{
public:
    virtual ~ExprAST() {}
    virtual Value *codegen() {};
    virtual bool has_refs() {};
    virtual bool has_calls() {};
};

class FCallAST : public ExprAST
{
public:
    string name;

    FCallAST(const string &name)
        : name(name) {}

    bool has_refs() {return false;}
    bool has_calls() {return true;}
};

class VariableAST : public ExprAST
{
public:
    string name;

    VariableAST(const string &name)
        : name(name) {}

    bool has_refs() {return true;}
    bool has_calls() {return false;}

    Value *codegen()
    {
        AllocaInst *val = NamedValues[name];
        if (!val)
        {
            return LogErrorV("Unknown variable name");
        }

        return Builder->CreateLoad(val->getAllocatedType(), val, name.c_str());
    }
};

class NumberAST : public ExprAST
{
public:
    int val;

    NumberAST(int val)
        : val(val){};

    bool has_refs() {return false;}
    bool has_calls() {return false;}

    Value *codegen()
    {
        return ConstantInt::get(*TheContext, APInt(32, val, true));
    }
};

class UnaryOpExprAST : public ExprAST
{
public:
    char op;
    ExprAST *arg;

    UnaryOpExprAST(char op, ExprAST *arg)
        : op(op), arg(arg) {}

    bool has_refs() {return arg->has_refs();}
    bool has_calls() {return arg->has_calls();}

    ~UnaryOpExprAST()
    {
        delete arg;
    }

    Value *codegen()
    {
        Value *expr_value = arg->codegen();
        if (!expr_value)
        {
            return nullptr;
        }
        switch (op)
        {
        case '-':
            return Builder->CreateNeg(expr_value, "negtmp");
        default:
            return nullptr;
        }
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

    bool has_refs() {return lhs->has_refs() && rhs->has_refs();}
    bool has_calls() {return lhs->has_calls() && rhs->has_calls();}

    ~BinaryOpExprAST()
    {
        delete lhs;
        delete rhs;
    }

    Value *codegen()
    {
        Value *lhs_val = lhs->codegen();
        Value *rhs_val = rhs->codegen();

        if (!lhs_val || !rhs_val)
        {
            return nullptr;
        }

        switch (op)
        {
        case '+':
            return Builder->CreateAdd(lhs_val, rhs_val, "addtmp");
        case '-':
            return Builder->CreateSub(lhs_val, rhs_val, "subtmp");
        case '*':
            return Builder->CreateMul(lhs_val, rhs_val, "mulcmp");
        case '/':
            return Builder->CreateSDiv(lhs_val, rhs_val, "divtmp");

        default:
            nullptr;
        }
    }
};
