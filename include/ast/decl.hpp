#pragma once

#include <vector>
#include <string>

#include "operator.hpp"

using std::string;
using std::vector;

class DeclAST
{
public:
    virtual ~DeclAST(){};
    virtual bool signup() { return false; };
    virtual void codegen() {};
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
            std::cout << "Нельзя использовать вызовы функций при инициализации переменной: "
                      << name << std::endl;
            return false;
        }

        // TODO: сделать возможным объявлять переменные через другие переменные, которые встречаются позже
        // TODO: сделать возможным объявление без инициализации (с инициализацией по умолчанию нулем)

        Value *init_val = val->codegen();

        if (!init_val)
        {
            std::cout << "Переменная, по-видимому, ссылается на не объявленную переменную, но это не точно: "
                      << name << std::endl;
            return false;
        }

        Value *old_alloca = NamedValues[name];

        if (old_alloca)
        {
            std::cout << "Переменная \"" << name
                      << "\" уже объявлена, новое значение заменит старое, будьте осторожны" << std::endl;
            Builder->CreateStore(init_val, old_alloca);
            return true;
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

        if (!FT)
        {
            std::cout << "Неверный прототип функции: " << name << std::endl;
            return false;
        }

        Function *old_F = TheModule->getFunction(name);

        if (old_F)
        {
            std::cout << "Такая функция уже объявлена, либо это системная функция \"" << main_func_name << "\": " << name << std::endl;
            return false;
        }

        Function *F =
            Function::Create(FT, Function::ExternalLinkage, name, TheModule.get());

        if (!F)
        {
            std::cout << "Не получилось создать функцию: " << name << std::endl;
            return false;
        }

        return true;
    };

    void codegen()
    {
        Function *F = TheModule->getFunction(name);

        if (!F)
        {
            std::cout << "Функция не найдена при проходе кодогенератора: " << name << std::endl;
            return;
        }

        if (!F->empty())
        {
            std::cout << "Найденная при проходе кодогенератора функция не пуста: " << name << std::endl;
            return;
        }

        BasicBlock *BB = BasicBlock::Create(*TheContext, "entry", F);
        Builder->SetInsertPoint(BB);

        bool has_return = false;
        for (auto op : body)
        {
            op->codegen();
            if (op->is_return())
            {
                has_return = true;
                break;
            }
        }

        if (!has_return) {
            auto ret_val = new NumberAST(0);
            ReturnAST(ret_val).codegen();
        }

        verifyFunction(*F);

        TheFPM->run(*F, *TheFAM);
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

    bool signup()
    {
        entry_function_name = name;
        return true;
    }
};
