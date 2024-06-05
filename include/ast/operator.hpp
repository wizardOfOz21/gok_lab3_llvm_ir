#pragma once

#include <string>
#include "builder.hpp"
#include "expr.hpp"

using std::string;
class OperatorAST
{
public:
    virtual ~OperatorAST(){};
    virtual void codegen() {};
    virtual bool is_return() { return false; };
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

    void codegen()
    {
        Value *var_alloca = NamedValues[var_name];

        if (!var_alloca)
        {
            in_func_name_print();
            std::cout << "Такой переменной ранее не было объявлено: " << var_name;
            return;
        }

        Value* var_val = val->codegen();

        if (!var_val) {
            in_func_name_print();
            std::cout << "Не удалось вычислить значение переменной: " << var_name;
            return;
        }

        Builder->CreateStore(var_val, var_alloca);
    };

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

    void codegen()
    {
        Value *ret_val = val->codegen();

        if (!ret_val)
        {
            in_func_name_print();
            std::cout << "Возвращаемое значение функции не удалось вычислить" << std::endl;
            return;
        }

        Builder->CreateRet(ret_val);
    };

    bool is_return() { return true; };

    ReturnAST(ExprAST *val)
        : val(val) {}
};

// Если унаследовать ExprAST от OperatorAST, будет циклическая зависимость между файлами, поэтому так
class ExprOperatorAST : public OperatorAST
{

public:
    ExprAST *val;

    ExprOperatorAST(ExprAST *val) : val(val) {}


    void codegen() {
        Value* Val = val->codegen();
        if (!Val) {
            in_func_name_print();
            std::cout << "Не удалось вычислить значение свободного выражения" << std::endl;
            return;
        }
        Builder->Insert(Val, "tmp");
    }


    ~ExprOperatorAST()
    {
        delete val;
    }
};
