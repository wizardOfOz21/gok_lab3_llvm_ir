#pragma once

#include <string>
#include <iostream>
#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif
#include <location.hh>

struct LexVal
{
    std::string ident_val;
    int num_val;
};

class FooLexer : public yyFlexLexer
{

private:
    int env;

    int cur_line = 1;
    int cur_column = 1;

public:
    FooLexer(std::istream &in, int env) : yyFlexLexer(&in), env(env) {};
    int yylex(LexVal *const yylval, yy::location *const yylloc);
};
