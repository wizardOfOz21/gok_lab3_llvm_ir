#pragma once

#include <vector>
#include "decl.hpp"
#include "builder.hpp"

using std::vector;
class ProgramAST
{
public:
    vector<DeclAST *> decls;

    ProgramAST(const vector<DeclAST *> &decls) : decls(decls){};
    ~ProgramAST()
    {
        for (auto decl : decls)
        {
            delete decl;
        }
    };

    // проходит по дереву и собирает прототипы функций
    // и инициализирует переменные
    bool declare()
    {
        Builder->SetInsertPoint(&MainFunc->getEntryBlock());

        for (auto decl : decls)
        {
            if (!decl->signup())
            {
                return false;
            }
        }

        if (entry_function_name.compare("") == 0) {
            std::cout << "Не была объявлена точка входа, используйте entry = <имя функции>" << std::endl;
            return false;
        }

        Function* entry_func = TheModule->getFunction(entry_function_name);

        if (!entry_func) {
            std::cout << "Точка входа не найдена" << std::endl;
            return false;
        }

        Value* ret_val = Builder->CreateCall(entry_func, {}, "mainrettmp");
        Builder->CreateRet(ret_val);
        return true;
    }

    void codegen()
    {
        for (auto decl : decls)
        {
            decl->codegen();
        }
    }
};
