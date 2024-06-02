%require "3.2"
%language "c++"

%code requires {
    #include "lexer.hpp"

    #include <string>
    #include "ast.hpp"
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

    vector<DeclAST*>* decl_list;

    vector<VarAST*>* var_decl_list;
    VarAST* var_decl;

    FuncAST* func_decl;

    EntryAST* entry_decl;

    vector<OperatorAST*>* op_list;

    OperatorAST* op;

    vector<string>* strings;

    AssignStatementAST* assign;
    ReturnStatementAST* return_;
    ExprStatementAST* expr_stmt;
    ExprAST* expr;

    TermAST* term;
    FactorAST* factor;
    PAST* p;
}

%type <decl_list> decls
%type <decl_list> decl

%type <var_decl_list> var_decl
%type <var_decl_list> var_list

%type <var_decl> var
%type <entry_decl> entry_decl
%type <func_decl> func_decl

%type <op_list> body
%type <op_list> operator_list

%type <op> operator

%type <strings> params_list

%type <assign> assignment
%type <return_> return_statement
%type <expr_stmt> expr_statement
%type <expr> expr

%type <term> t
%type <factor> f
%type <p> p

%%

program: 
    decls ';' { ast_root = new ProgramAST(*$1); }

decls: 
      decls ';' decl    { all($1,$3); $$ = $1; } 
    | decl              { $$ = $1; }
    | %empty            { $$ = new vector<DeclAST*>(); }

decl: 
      var_decl      { $$ = (vector<DeclAST*>*)$1;  } 
    | func_decl     { $$ = make_vec((DeclAST*)$1); } 
    | entry_decl    { $$ = make_vec((DeclAST*)$1); }

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
        $$ = make_vec($1);
    }

var: 
    IDENT '=' expr 
    { 
        $$ = new VarAST(*$1, $3);
    }

entry_decl: 
    "entry" '=' IDENT 
    { 
        $$ = new EntryAST(*$3); 
    }

func_decl: 
    "func" IDENT params_list body 
    {
        $$ = new FuncAST(*$2, *$3, *$4);
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
            $$ = make_vec($1); 
        }
        | %empty 
        { 
            $$ = new vector<OperatorAST*>(); 
        }

operator: 
        assignment 
        { 
            $$ = (OperatorAST*)$1;
        } 
        | return_statement 
        { 
            $$ = (OperatorAST*)$1;
        }
        | expr_statement
        { 
            $$ = (OperatorAST*)$1;
        }

params_list: 
    '(' ')'
    {
        $$ = new vector<string>();
    }

assignment: 
    IDENT '=' expr 
    {
        $$ = new AssignStatementAST(*$1,$3);
    }

return_statement: 
    "return" expr 
    {
        $$ = new ReturnStatementAST($2);
    }

expr_statement: 
    expr
    {
        $$ = new ExprStatementAST($1);
    }

expr:   
    t '+' expr  
    {
        $3->terms.push_back($1);
        $3->signs.push_back('+'); 
        $$ = $3; 
    }
    |  t '-' expr  
    {
        $3->terms.push_back($1);
        $3->signs.push_back('-'); 
        $$ = $3;
    }
    |  t 
    {
        auto e = new ExprAST();
        e->terms.push_back($1);
        $$ = e;
    }

t:   
    f '*' t  
    { 
        $3->factors.push_back($1); 
        $3->signs.push_back('*'); 
        $$ = $3; 
    }
    |  f '/' t 
    { 
        $3->factors.push_back($1); 
        $3->signs.push_back('/'); 
        $$ = $3; 
    }
    |  f 
    { 
        auto t = new TermAST();
        t->factors.push_back($1);
        $$ = t;
    }

f: 
    '-' p  
    { 
        $$ = new FactorAST('-', $2); 
    }
    |  p   
    { 
        $$ = new FactorAST('+', $1);
    }

p:   
      IDENT 
    { 
        $$ = (PAST*) new IdentAST(*$1); 
    } 
    | NUMBER 
    { 
        $$ = (PAST*) new NumberAST($1); 
    }
    | IDENT '(' ')' 
    { 
        $$ = (PAST*) new FCallAST(*$1); 
    }
    | '(' expr ')' 
    { 
        $$ = (PAST*) new InnerExprAST($2); 
    }

%%

void yy::parser::error(const yy::location& loc, const std::string &message)
{
    std::cerr << "Error: " << message << " on " << loc << std::endl;
}
