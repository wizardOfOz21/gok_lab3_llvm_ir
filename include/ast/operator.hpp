#pragma once

#include <string>

using std::string;

class ExprAST;
class OperatorAST
{
public:
    virtual ~OperatorAST(){};
};

class AssignAST : public OperatorAST
{
public:
    string var_name;
    ExprAST *val;

    ~AssignAST()
    {
        delete val;
    }

    AssignAST(const string var_name, ExprAST *val)
        : var_name(var_name), val(val) {}
};

class ReturnAST : public OperatorAST
{
public:
    ExprAST *val;

    ~ReturnAST()
    {
        delete val;
    }

    ReturnAST(ExprAST *val)
        : val(val) {}
};
