#pragma once

#include <vector>
#include <string>

#include "expr.hpp"

using std::string;
using std::vector;

class DeclAST
{
public:
    virtual ~DeclAST(){};
};

class VarAST : public DeclAST
{
public:
    string name;
    ExprAST *val;

    VarAST(const string &name, ExprAST *val) : name(name), val(val) {}
    ~VarAST()
    {
        delete val;
    }
};

class FuncAST : public DeclAST
{
public:
    string name;
    vector<string> params;
    vector<OperatorAST *> body;

    FuncAST(const string &name, const vector<string> &params,
            const vector<OperatorAST *> &body) : name(name), params(params), body(body){};
    ~FuncAST()
    {
        for (auto op : body)
        {
            delete op;
        }
    }
};

class EntryAST : public  DeclAST
{
public:
    string name;

    EntryAST(const string &name) : name(name){};
};
