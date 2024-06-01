#pragma once 

#include <vector>
#include <string>

using std::string;
using std::vector;

class PAST {
public: 
    virtual ~PAST(){};
};

class IdentAST: PAST {
public:
    string val;
};

class NumberAST: PAST {
public:
    int val;
};

class ExprAST;
class InnerExprAST: PAST {
public:
    ExprAST* expr;
    ~InnerExprAST() {
        delete expr;
    }
};

class FactorAST
{
public:
    char sign;
    PAST* p;

    ~FactorAST() {
        delete p;
    }
};

class TermAST
{
public:
    vector<FactorAST*> factors;
    vector<char> signs; // знак каждого множителя, * или /, у первого – умножить

    ~TermAST() {
        for (auto f : factors) {
            delete f;
        }
    }
};

class ExprAST
{
public:
    vector<TermAST*> terms;
    vector<char> signs; // знак каждого слагаемого, + или -, у первого – плюс

    ~ExprAST() {
        for (auto t : terms) {
            delete t;
        }
    }
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
};

class ReturnStatementAST : OperatorAST
{
public:
    ExprAST *val;

    ~ReturnStatementAST()
    {
        delete val;
    }
};

class ExprStatementAST : OperatorAST
{
public:
    ExprAST *val;

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
};

class ProgramAST
{
public:
    vector<DeclAST *> decls;

    ~ProgramAST()
    {
        for (auto decl : decls)
        {
            delete decl;
        }
    };
};
