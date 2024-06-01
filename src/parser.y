%require "3.2"
%language "c++"

%code requires {
    #include <string>
    #include "lexer.hpp"
    #include "ast.hpp"
}

%locations

%define api.value.type {LexVal}
%parse-param {ProgramAST* &ast_root}

%parse-param {FooLexer &lexer}

%header

%code {
    #define yylex lexer.yylex
}

%token IDENT
%token NUMBER

%token _VAR_ "var"
%token _FUNC_ "func"
%token _ENTRY_ "entry"
%token _RETURN_ "return"

%%

program: decls ';' { ast_root = (ProgramAST*)11; }
decls: decls ';' decl | decl | %empty

decl: var_decl | func_decl | entry_decl

var_decl: "var" var_list
var_list: var_list ',' var | var 
var: IDENT '=' expr

func_decl: "func" IDENT params_list body
body: '{' operator_list ';' '}'
params_list: '(' ')'

entry_decl: "entry" '=' IDENT

operator_list: operator_list ';' operator  | operator | %empty
operator: assignment | return_statement | expr

assignment: IDENT '=' expr
return_statement: "return" expr

expr:   t '+' expr   |  t '-' expr  |  t
t:   f '*' t  |  f '/' t |  f
f:  '-' p  |  p
p:   IDENT | NUMBER | f_call | '(' expr ')'
f_call: IDENT '(' ')'

%%

void yy::parser::error(const yy::location& loc, const std::string &message)
{
    std::cerr << "Error: " << message << " on " << loc << std::endl;
}
