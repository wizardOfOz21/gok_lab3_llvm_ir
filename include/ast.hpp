#pragma once

#include <vector>
#include <string>

using std::string;
using std::vector;

template <class T>
vector<T> make_vec()
{
    vector<T> v;
    return v;
}

template <class T>
vector<T> *make_vec(const T &element)
{
    auto v = new vector<T>();
    v->push_back(element);
    return v;
}

template <class T>
vector<T> *all(vector<T> *a, vector<T> *b)
{
    a->insert(a->end(), b->begin(), b->end());
    return a;
}
class PAST
{
public:
    virtual ~PAST(){};
};

class FCallAST : PAST
{
public:
    string f_name;

    FCallAST(string name) : f_name(name) {}
};

class IdentAST : PAST
{
public:
    string val;

    IdentAST(string name) : val(name) {}
};

class NumberAST : PAST
{
public:
    int val;

    NumberAST(int val) : val(val){};
};

class FactorAST
{
public:
    char sign;
    PAST *p;

    ~FactorAST()
    {
        delete p;
    }

    FactorAST(char sign) : sign(sign) {}
    FactorAST(char sign, PAST *p) : sign(sign), p(p) {}
};

class TermAST
{
public:
    vector<FactorAST *> factors;
    vector<char> signs; // знак каждого множителя, * или /, у первого – умножить

    ~TermAST()
    {
        for (auto f : factors)
        {
            delete f;
        }
    }
};

class ExprAST
{
public:
    vector<TermAST *> terms;
    vector<char> signs; // знак каждого слагаемого, + или -, у первого – плюс

    ~ExprAST()
    {
        for (auto t : terms)
        {
            delete t;
        }
    }
};

class InnerExprAST : PAST
{
public:
    ExprAST *expr;
    ~InnerExprAST()
    {
        delete expr;
    }

    InnerExprAST(ExprAST *expr) : expr(expr) {}
};

class OperatorAST
{
public:
    virtual ~OperatorAST(){};
};

class AssignStatementAST : OperatorAST
{
public:
    string var;
    ExprAST *val;

    ~AssignStatementAST()
    {
        delete val;
    }

    AssignStatementAST(string var, ExprAST *val) : var(var), val(val) {}
};

class ReturnStatementAST : OperatorAST
{
public:
    ExprAST *val;

    ~ReturnStatementAST()
    {
        delete val;
    }

    ReturnStatementAST(ExprAST *val) : val(val) {}
};

class ExprStatementAST : OperatorAST
{
public:
    ExprAST *val;

    ExprStatementAST(ExprAST *val): val(val) {} 
    ~ExprStatementAST()
    {
        delete val;
    }
};

class DeclAST
{
public:
    virtual ~DeclAST(){};
};

class VarAST : DeclAST
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

class FuncAST : DeclAST
{
public:
    string name;
    vector<string> params;
    vector<OperatorAST *> body;

    FuncAST(string name, vector<string> &params,
            vector<OperatorAST *> &body) : name(name), params(params), body(body){};

    ~FuncAST()
    {
        for (auto op : body)
        {
            delete op;
        }
    }
};

class EntryAST : DeclAST
{
public:
    string name;
    EntryAST(string name) : name(name){};
};

class ProgramAST
{
public:
    vector<DeclAST *> decls;

    ProgramAST(vector<DeclAST *> &decls) : decls(decls) {};
    ~ProgramAST()
    {
        for (auto decl : decls)
        {
            delete decl;
        }
    };
};
