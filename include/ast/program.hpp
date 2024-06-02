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
        return true;
    }

    void codegen()
    {
    }
};
