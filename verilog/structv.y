%{
#include <stdio.h>
#include <string.h>
#include <vector>
#include <assert.h>
#include "symbol.h"
#include "ast.h"

extern int yylex(void);
extern int yylineno;

void yyerror(const char *str)
{
    fprintf(stderr,"%d: %s\n", yylineno, str);
}
 
int yywrap()
{
    return 1;
} 
  
%}

%union {
    int int_val;
    char* str_val;
    constant_t const_val;

    wire_list_t* idlist_val;

    module_list_t* module_list_val;
    module_t* module_val;
    module_head_t* module_head_val;

    stm_list_t* statement_list_val;
    stm_t* statement_val;

    module_inst_t* module_inst_val;
    bindinglist_t* bindlist_val;

    assgn_t* assgn_val;
    exp_t* exp_val;
    exp_list_t* exp_list_val;

    wirename_t* wirename_val;
    wire_list_t* wire_val;
    wire_list_t* input_val; 
    wire_list_t* output_val;
}

/*
 * These are the terminal symbols.
 */

%token MODULE INPUT OUTPUT WIRE ENDMODULE LPAREN ASSIGN EQUALS
%token COMMA RPAREN LBRACKET RBRACKET DOT SEMICOLON COLON
%token LBRACE RBRACE
%token <int_val> NUMBER 
%token <str_val> ID 
%token <const_val> BINCONST

/*
 * Non-terminals. 
 */
%type <idlist_val>          id_list
%type <idlist_val>          id_list_empty
%type <idlist_val>          id_list_some
%type <module_list_val>     file
%type <module_val>          module
%type <module_head_val>     module_head
%type <input_val>           input_decl
%type <output_val>          output_decl
%type <wire_val>            wire_decl
%type <statement_val>       statement
%type <statement_list_val>  statements
%type <bindlist_val>        module_inputs
%type <wirename_val>        obj_name
%type <exp_val>             expression
%type <exp_list_val>        exp_list
%type <module_inst_val>     module_inst
%type <assgn_val>           assign_stm

/* Start symbol.  */
%start file

%%

file: 
    /* only one module. */
    module
    {
        module_list_t* ml = new module_list_t();
        ml->push_back($1);
        modules = $$ = ml;
    }
    /* more than one module. */
    | file module
    {
        module_list_t* ml = $1;
        ml->push_back($2);
        modules = $$ = ml;
    }
;
module: 
    module_head statements module_tail
    {
        $$ = new module_t($1, $2);
        delete $1;
    }
;
module_head: MODULE ID LPAREN id_list RPAREN
    {
        $$ = new module_head_t($2, $4); 
        free($2);
    }
;

id_list: 
    /* nothing */
    id_list_empty 
    { 
        $$ = $1;
    } 
    /* or something. */
    | id_list_some 
    { 
        $$ = $1;
    };

id_list_empty: 
    /* empty list. */
    {
        $$ = new wire_list_t();
    }
;
id_list_some:
    /* only one element. */
    obj_name
    {
        $$ = new wire_list_t();
        $$->push_back(*$1);
        delete $1;
    }
    /* multiple elements. */
    | id_list_some COMMA obj_name
    {
        $$ = $1;
        $$->push_back(*$3);
        delete $3;
    }
;

module_tail: ENDMODULE
    {
    }
;

statements: 
    /* a bunch of statements */
    statements SEMICOLON statement
    {
        stm_list_t* l = $1;
        $$ = l;
        if($3) {
            l->push_back($3);
        }
    }
    /* only one statement. */
    | statement
    {
        stm_list_t* l = new stm_list_t();
        $$ = l;
        if($1) {
            l->push_back($1);
        }
    }
;

statement:
    /* input declaration */
    input_decl
    {
        $$ = new stm_t(stm_t::INPUT, $1);
    }

    /* output declaration */
    | output_decl
    {
        $$ = new stm_t(stm_t::OUTPUT, $1);
    }

    /* wire declaration */
    | wire_decl
    {
        $$ = new stm_t(stm_t::WIRE, $1);
    }
    /* module instantiation */
    | module_inst
    {
        $$ = new stm_t($1);
    }
    /* assign statement  */
    | assign_stm
    {
        $$ = new stm_t($1);
    }
    /* nothing */
    |
    {
        $$ = NULL;
    }
;

input_decl: 
    INPUT LBRACKET NUMBER COLON NUMBER RBRACKET id_list_some
    {
        int n1 = $3;
        int n2 = $5;
        wire_list_t* ids = $7;
        $$ = declare_objects(n1, n2, ids);
        delete (ids);
    }
    | INPUT id_list_some
    {
        $$ = $2;
    }
;

output_decl: 
    OUTPUT LBRACKET NUMBER COLON NUMBER RBRACKET id_list_some
    {
        int n1 = $3;
        int n2 = $5;
        wire_list_t* ids = $7;
        $$ = declare_objects(n1, n2, ids);
        delete (ids);
    }
    | OUTPUT id_list_some
    {
        $$ = $2;
    }
;

wire_decl:
    WIRE id_list_some
    {
        $$ = $2;
    }
    | WIRE LBRACKET NUMBER COLON NUMBER RBRACKET id_list_some
    {
        int n1 = $3;
        int n2 = $5;
        wire_list_t* ids = $7;
        $$ = declare_objects(n1, n2, ids);
        delete (ids);
    }

;

module_inst:
    ID obj_name LPAREN module_inputs RPAREN
    {
        wirename_t* module = new wirename_t($1);
        free($1);
        wirename_t* instance = $2;
        bindinglist_t* bindings = $4;
        $$ = new module_inst_t(module, instance, bindings);
    }
;
module_inputs:
    DOT ID LPAREN expression RPAREN
    {
        $$ = new bindinglist_t();
        $$->push_back(wirebinding_t(wirename_t($2), $4));
        free($2);
        delete $4;
    }
    | DOT ID LPAREN RPAREN
    {
        $$ = new bindinglist_t();
        free($2);
    }
    | module_inputs COMMA DOT ID LPAREN expression RPAREN
    {
        $$ = $1;
        $$->push_back(wirebinding_t(wirename_t($4), $6));
        free($4);
        delete $6;
    }
    | module_inputs COMMA DOT ID LPAREN RPAREN
    {
        $$ = $1;
        free($4);
    }
;
expression:
    obj_name 
    {
        $$ = new exp_t($1);
    }
    | ID LBRACKET NUMBER COLON NUMBER RBRACKET
    {
        int n1 = $3;
        int n2 = $5;
        exp_list_t* l = new exp_list_t();

        if(n1 <= n2) {
            for(int i=n1; i <= n2; i++) {
                wirename_t* wn = new wirename_t($1, i);
                l->push_back(new exp_t(wn));
            }
        } else {
            for(int i=n1; i >= n2; i--) {
                wirename_t* wn = new wirename_t($1, i);
                l->push_back(new exp_t(wn));
            }
        }
        free($1);
        $$ = new exp_t(new aggregate_t(l));
    }
    | BINCONST
    {
        $$ = new exp_t($1);
    }
    | LBRACE exp_list RBRACE
    {
        $$ = new exp_t(new aggregate_t($2));
    }
;
exp_list:
    expression
    {
        $$ = new exp_list_t();
        $$->push_back($1);
    }
    | exp_list COMMA expression
    {
        $1->push_back($3);
        $$ = $1;
    }
;
obj_name:
    ID
    {
        $$ = new wirename_t($1);
        free($1);
    }
    | ID LBRACKET NUMBER RBRACKET
    {
        $$ = new wirename_t($1, $3);
        free($1);
    }
    | ID LBRACKET NUMBER RBRACKET LBRACKET NUMBER RBRACKET
    {
        $$ = new wirename_t($1, $3, $6);
        free($1);
    }
;
assign_stm:
    ASSIGN obj_name EQUALS expression
    {
        $$ = new assgn_t($2, $4);
    }
;
%%

