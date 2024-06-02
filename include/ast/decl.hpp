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
    virtual bool signup() {};
};

class VarAST : public DeclAST
{
public:
    string name;
    ExprAST *val;

    VarAST(const string &name, ExprAST *val) : name(name), val(val) {}

    // выделяет стек под переменную и регистрирует переменную в списке имен
    bool signup()
    {
        if (val->has_calls())
        {
            return LogErrorV("Нельзя использовать вызовы функций при инициализации переменной");
        }

        // TODO: сделать возможным объявлять переменные через другие переменные, которые встречаются позже
        // TODO: сделать возможным объявление без инициализации (с инициализацией по умолчанию нулем)

        Value *init_val = val->codegen();

        if (!init_val)
        {
            return false;
        }

        AllocaInst *alloca = CreateEntryBlockAlloca(MainFunc, name);
        Builder->CreateStore(init_val, alloca);

        NamedValues[name] = alloca;
        return true;
    };

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

    // регистрирует прототип функции в модуль
    bool signup()
    {
        std::vector<Type *> Ints(0, Type::getInt32Ty(*TheContext));
        FunctionType *FT =
            FunctionType::get(Type::getInt32Ty(*TheContext), Ints, false);
        
        if (!FT) {
            LogErrorV("Неверный прототип функции");
            return false;
        }

        Function *F =
            Function::Create(FT, Function::ExternalLinkage, name, TheModule.get());

        if (!F) {
            LogErrorV("Не получилось создать функцию");
            return false;
        }

        return true;
    };

    ~FuncAST()
    {
        for (auto op : body)
        {
            delete op;
        }
    }
};

class EntryAST : public DeclAST
{
public:
    string name;

    EntryAST(const string &name) : name(name){};
};
