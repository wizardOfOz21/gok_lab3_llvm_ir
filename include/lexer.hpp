#pragma once

#include <string>
#include <iostream>
#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

class FooLexer : public yyFlexLexer
{

private:
    int env;

public:
    FooLexer(std::istream &in, int env) : yyFlexLexer(&in), env(env){};
    int yylex(std::string *const yylval);
};
