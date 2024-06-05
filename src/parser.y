%require "3.2"
%language "c++"

%code requires {
    #include "lexer.hpp"

    #include <string>
    #include "ast/program.hpp"
}

%locations
%define api.location.file "location.hpp"

%parse-param {ProgramAST* &ast_root}

%parse-param {FooLexer &lexer}

%header

%code {
    #define yylex lexer.yylex
}

%token <ident_val> IDENT
%token <num_val> NUMBER

%token _VAR_ "var"
%token _FUNC_ "func"
%token _ENTRY_ "entry"
%token _RETURN_ "return"

%union {
    std::string* ident_val;
    int num_val;

    vector<DeclAST*>* DeclAST_list;
    DeclAST* DeclAST;
    vector<OperatorAST*>* OperatorAST_list;
    OperatorAST* OperatorAST;

    ExprAST* ExprAST;

    vector<string>* strings;
}

%type <DeclAST_list> var_decl var_list decl decls

%type <DeclAST> var entry_decl func_decl
%type <OperatorAST_list> body
%type <OperatorAST_list> operator_list

%type <strings> params_list

%type <OperatorAST> operator assignment return_statement 
%type <ExprAST> expr t f p

%%

program: 
    decls ';' { ast_root = new ProgramAST(*$1); }

decls: 
      decls ';' decl    { $1->insert($1->end(), $3->begin(), $3->end()); $$ = $1; } 
    | decl              { $$ = $1; }
    | %empty            { $$ = new vector<DeclAST*>(); }

decl: 
      var_decl
    | func_decl     { $$ = new vector<DeclAST*>({$1}); } 
    | entry_decl    { $$ = new vector<DeclAST*>({$1}); }

var_decl: 
    "var" var_list { $$ = $2; }

var_list: 
    var_list ',' var 
    {
        $1->push_back($3);
        $$ = $1;
    } 
    | var 
    { 
        $$ = new vector<DeclAST*>({$1});
    }

var: 
    IDENT '=' expr 
    { 
        $$ = new VarAST(*$1, $3);
        delete $1;
    }

entry_decl: 
    "entry" '=' IDENT 
    { 
        $$ = new EntryAST(*$3); 
        delete $3;
    }

func_decl:
    "func" IDENT params_list body 
    {
        $$ = new FuncAST(*$2, *$3, *$4);
        delete $2;
    }

params_list: 
    '(' ')'
    {
        $$ = new vector<string>();
    }

body:
    '{' operator_list ';' '}' 
    { 
        $$ = $2; 
    }

operator_list: 
        operator_list ';' operator 
        { 
            $1->push_back($3); 
            $$ = $1; 
        }
        | operator 
        { 
            $$ = new vector<OperatorAST*>({$1});
        }
        | %empty 
        { 
            $$ = new vector<OperatorAST*>(); 
        }

operator: 
          assignment 
        | return_statement 
        | expr { $$ = new ExprOperatorAST($1); }

assignment: 
    IDENT '=' expr 
    {
        $$ = new AssignAST(*$1,$3);
        delete $1;
    }

return_statement: 
    "return" expr 
    {
        $$ = new ReturnAST($2);
    }

expr:   
       t '+' expr  
    {
        $$ = new BinaryOpExprAST('+', $1, $3); 
    }
    |  t '-' expr  
    {
        $$ = new BinaryOpExprAST('-', $1, $3); 
    }
    |  t

t:   
    f '*' t  
    { 
        $$ = new BinaryOpExprAST('*', $1, $3);
    }
    |  f '/' t 
    { 
        $$ = new BinaryOpExprAST('/', $1, $3);
    }
    |  f

f: 
    '-' p  
    { 
        $$ = new UnaryOpExprAST('-', $2);
    }
    |  p

p:   
     IDENT 
    { 
        $$ = new VariableAST(*$1);
        delete $1;
    } 
    | NUMBER 
    { 
        $$ = new NumberAST($1); 
    }
    | IDENT '(' ')' 
    { 
        $$ = new FCallAST(*$1); 
        delete $1;
    }
    | '(' expr ')' 
    { 
        $$ = $2; 
    }

%%

void yy::parser::error(const yy::location& loc, const std::string &message)
{
    std::cerr << "Error: " << message << " on " << loc << std::endl;
}
