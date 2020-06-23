%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "ast.h"
#include "structv.tab.hh"

extern "C" int yylex(void);

%}

%s IN_C_COMMENT IN_CPP_COMMENT

%%
<INITIAL>{
"/*"                                    BEGIN(IN_C_COMMENT);
\/\/                                    BEGIN(IN_CPP_COMMENT);
assign                                  return ASSIGN;
module                                  return MODULE;
input                                   return INPUT;
output                                  return OUTPUT;
wire                                    return WIRE;
tri                                     return WIRE;
endmodule                               return ENDMODULE;
[0-9]+                                  yylval.int_val = atoi(yytext); return NUMBER;
[_A-Za-z\/\\\$][A-Za-z0-9_\/\\\$]*      yylval.str_val = (strdup(yytext)); return ID;
[0-9]+'b[01]+                           yylval.const_val = get_const(yytext); return BINCONST;
\(                                      return LPAREN;
,                                       return COMMA;
\)                                      return RPAREN;
\{                                      return LBRACE;
\}                                      return RBRACE;
\[                                      return LBRACKET;
\]                                      return RBRACKET;
\.                                      return DOT;
;                                       return SEMICOLON;
:                                       return COLON;
=                                       return EQUALS;
[ \t+\r]+                               /* ignore whitespace. */
\n                                      yylineno++;
}

<IN_CPP_COMMENT>{
\n                                      yylineno++; BEGIN(INITIAL);
[^\n]+                                  // eat up other stuff.
}

<IN_C_COMMENT>{
"*/"                                    BEGIN(INITIAL);
[^*\n]+                                 // eat comment in chunks
"*"                                     // eat the lone star
\n                                      yylineno++;
}

%%
