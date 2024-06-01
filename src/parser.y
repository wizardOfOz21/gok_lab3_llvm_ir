%require "3.2"
%language "c++"

%code requires {
    #include <string>
    #include "lexer.hpp"
}

%locations

%define api.value.type {LexVal}
%parse-param {const bool debug}

%parse-param {FooLexer &lexer}

%header

%code {
    #define yylex lexer.yylex
}

%token IDENT
%token NUMBER

%token VAR
%token FUNC

%%

program: IDENT {std::cout << $1.ident_val << std::endl;} | NUMBER | VAR | FUNC

%%

void yy::parser::error(const yy::location& loc, const std::string &message)
{
    std::cerr << "Error: " << message << " on " << loc << std::endl;
}
