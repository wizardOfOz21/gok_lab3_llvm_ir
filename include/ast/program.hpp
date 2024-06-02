#pragma once

#include <vector>
#include "decl.hpp"

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
};
