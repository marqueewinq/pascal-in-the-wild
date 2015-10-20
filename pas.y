%{
#include "heading.h"
#include "pas.y.h"
%}

%union 
{
	int iValue;
	double dValue;
	char sValue[256];
	char sIndex[256]; /* symbol table index */
	slist *sl;
	nodeType *nPtr; /* node pointer */
};

%start file

%token <iValue> NUM
%token <dValue> EXP
%token <sValue> STR
%token <sIndex> ID
%token <ty> _R_INTEGER _R_DOUBLE _R_STRING

%token _R_PROGRAM _R_BEGIN _R_END _R_VAR _R_ARRAY _R_OF _R_DOTS _R_FUNCTION _R_PROCEDURE _R_IF _R_THEN _R_ELSE _R_WHILE _R_DO _R_NOT SEPARATOR_LINE SEPARATOR_LIST SEPARATOR_DEAD TYPIFIER BRACKET_O BRACKET_C SBRACKET_O SBRACKET_C _BUILTIN_READ _BUILTIN_WRITE ASSIGN

%left o_plus o_minus o_lor 
%left o_mul o_div o_mod o_land
%left cmp_leq cmp_geq cmp_l cmp_g cmp_eq cmp_neq

%type <sl> identifier_list
%type <nPtr> file program declaration declarationz_list compound_statement optional_statement statement_list statement expr_list expr simple_expr term factor variable

%%

file:
	/*empty*/ 
	{ 
		fprintf(stderr, "#   warning: empty file\n"); 
		exit(0);
	}
	| program 
	{ 
		ex($1); 
		freeNode($1); 
		fprintf(stderr, "#   program accepted by interpreter\n");
	}
	;
	
program:
	_R_PROGRAM ID SEPARATOR_LINE declarationz_list compound_statement SEPARATOR_DEAD 
	{ 
		$$ = opr(SEPARATOR_LINE, 2, $4, $5); 
	}
	;
	
identifier_list: 
	ID
	{
		$$ = new slist;
		strcpy($$->s, $1);
		$$->next = NULL;
	}
	| ID SEPARATOR_LIST identifier_list
	{
		$$ = new slist;
		strcpy($$->s, $1);
		$$->next = $3;
	}
	;

declarationz_list:
	/* empty */
	{
		$$ = NULL;
	}
	| declarationz_list _R_VAR declaration
	{
		$$ = opr(SEPARATOR_LINE, 2, $1, $3);
	}
	;
	
declaration:
	identifier_list TYPIFIER _R_INTEGER SEPARATOR_LINE
	{
		$$ = opr(_R_INTEGER, 1, sa($1));
	}
	| identifier_list TYPIFIER _R_DOUBLE SEPARATOR_LINE
	{
		$$ = opr(_R_DOUBLE, 1, sa($1));
	}
	;
		
compound_statement:
	_R_BEGIN optional_statement _R_END
	{
		$$ = $2;
	}
	;
	
optional_statement:
	/*empty*/
	{
		$$ = NULL;
	}
	| statement_list
	{
		$$ = $1;
	}
	;
	
statement_list:
	statement 
	{
		$$ = $1;
	}
	| statement_list SEPARATOR_LINE statement 
	{
		$$ = opr(SEPARATOR_LINE, 2, $1, $3); 
	}
	;
	
statement:
	variable ASSIGN expr 
	{
		$$ = opr(ASSIGN, 2, $1, $3);
	}
	| compound_statement 
	{
		$$ = $1;
	}
	| _R_WHILE expr _R_DO statement 
	{ 
		$$ = opr(_R_WHILE, 2, $2, $4);
	} 
	| _R_IF expr _R_THEN statement _R_ELSE statement 
	{ 
		$$ = opr(_R_IF, 3, $2, $4, $6);
	}
	| _BUILTIN_READ BRACKET_O identifier_list BRACKET_C
	{
		$$ = opr(_BUILTIN_READ, 1, sa($3));
	}
	| _BUILTIN_WRITE BRACKET_O expr_list BRACKET_C
	{
		$$ = opr(_BUILTIN_WRITE, 1, $3);
	}
	;
	
variable:
	ID
	{
		$$ = id ($1);
	}
	;
		
expr_list:
	expr 
	{
		$$ = $1;
	}
	| expr_list SEPARATOR_LIST expr 
	{
//		$$ = opr(SEPARATOR_LIST, 2, $1, $3); // we dont know
	}
	;
	
expr:
	simple_expr 
	{
		$$ = $1;
	}
	| simple_expr cmp_leq simple_expr 
	{
		$$ = opr(cmp_leq, 2, $1, $3);
	}
	| simple_expr cmp_geq simple_expr
	{
		$$ = opr(cmp_geq, 2, $1, $3);
	}
	| simple_expr cmp_eq  simple_expr
	{
		$$ = opr(cmp_eq, 2, $1, $3);
	}
	| simple_expr cmp_neq simple_expr
	{
		$$ = opr(cmp_neq, 2, $1, $3);
	}
	| simple_expr cmp_l   simple_expr
	{
		$$ = opr(cmp_l, 2, $1, $3);
	}
	| simple_expr cmp_g   simple_expr
	{
		$$ = opr(cmp_g, 2, $1, $3);
	}
	;

simple_expr:
	term 
	{ 
		$$ = $1;
	}
	| simple_expr o_plus  term 
	{
		$$ = opr(o_plus, 2, $1, $3);
	}
	| simple_expr o_minus term
	{
		$$ = opr(o_minus, 2, $1, $3);
	}
	| simple_expr o_lor   term
	{
		$$ = opr(o_lor, 2, $1, $3);
	}
	;

term:
	factor
	{
		$$ = $1;
	}
	| term o_mul  factor 
	{
		$$ = opr(o_mul, 2, $1, $3);
	}
	| term o_land factor
	{
		$$ = opr(o_land, 2, $1, $3);
	}
	| term o_div  factor
	{
		$$ = opr(o_div, 2, $1, $3);
	}
	| term o_mod  factor
	{
		$$ = opr(o_mod, 2, $1, $3);
	}
	;

factor:
	NUM 
	{
		$$ = con(mkVariable(INT, $1, 0, ""));
	}
	| EXP
	{
		$$ = con(mkVariable(DOUBLE, 0, $1, ""));
	}
	| STR
	{
		$$ = con(mkVariable(STRING, 0, 0, $1));
	}
	| ID 
	{
		$$ = id ($1);
	}
	| BRACKET_O expr BRACKET_C
	{
		$$ = $2;
	}
	;

%%

