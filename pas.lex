%{
#include "heading.h"
#include <stdio.h>
#include "tok.h"
#include "inttostr.h"

int yyerror(char *s);
extern vector <string> sym;
extern map <string, Variable*> vs; /* symbol table */
extern vector <string> collect;

%}

%%

[0-9]+         { yylval.iValue = atoi(yytext);             return NUM; }
[0-9]+\.[0-9]* { yylval.dValue = strtoflt(string(yytext)); return EXP; }
\"[^"\n]*["\n] { strcpy(yylval.sValue, string(yytext).substr(1, strlen(yytext) - 2).c_str());            return STR; }

"program"   {return _R_PROGRAM;}
"begin"     {return _R_BEGIN;}
"end"       {return _R_END;}
"var"       {return _R_VAR;}
"array"     {return _R_ARRAY;}
".."        {return _R_DOTS;}
"of"        {return _R_OF;}
"integer"   {return _R_INTEGER;}
"double"    {return _R_DOUBLE;}
"string"    {return _R_STRING;}
"function"  {return _R_FUNCTION;}
"procedure" {return _R_PROCEDURE;}
"if"        {return _R_IF;}
"then"      {return _R_THEN;}
"else"      {return _R_ELSE;}
"while"     {return _R_WHILE;}
"do"        {return _R_DO;}
"not"       {return _R_NOT;}
":="        {return ASSIGN;}
","         {return SEPARATOR_LIST;}
";"         {return SEPARATOR_LINE;}
"."         {return SEPARATOR_DEAD;}
":"         {return TYPIFIER;}
"("         {return BRACKET_O;}
")"         {return BRACKET_C;}
"["         {return SBRACKET_O;}
"]"         {return SBRACKET_C;}

"read"      {return _BUILTIN_READ;}
"write"     {return _BUILTIN_WRITE;}


"+"  return o_plus;
"-"  return o_minus;
"or" return o_lor;
"*"  return o_mul;
"/"|"div"  return o_div;
"%"|"mod"  return o_mod;
"and" return o_land;

"<" return cmp_l;
"<=" return cmp_leq;
"=" return cmp_eq;
"<>" return cmp_neq;
">" return cmp_g;
">=" return cmp_geq;

"//".*[\n] { }

[a-zA-Z][a-zA-Z0-9_]* {strcpy(yylval.sIndex, yytext); vs[yytext] = new Variable; return ID;}


[ \t]       {}
[\n]		{yylineno++;}

.           {yyerror("unknown character");}

%%

