/* heading.h */

#ifndef __HEADING_H_
#define __HEADING_H_

#define YY_NO_UNPUT
#define YYDEBUG_LEXER_TEXT yytext

using namespace std;

#include <iostream>
#include <stdarg.h>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <string>
#include <map>
#include <sstream>
#include <algorithm>
#include <vector>
#include <sstream>
#include <stdarg.h>

typedef enum { typeCon, typeId, typeSlist, typeOpr } nodeEnum;
typedef enum { VOID, INT, DOUBLE, STRING } MyType;

struct slist
{
	char s[256];
	slist *next;	
};

struct Variable
{
	int i;
	double d;
	char *s;
	MyType ty;
	bool decl;
};

/* constants */
typedef struct {
	Variable value; /* value of constant */
} conNodeType;

/* identifiers */
typedef struct {
	int i; /* subscript to sym array */
} idNodeType;

/* lists for identifier_list */

typedef struct {
	slist *s;
} slNodeType;

/* operators */
typedef struct {
	int oper; /* operator */
	int nops; /* number of operands */
	struct nodeTypeTag **op; /* operands (expandable) */
} oprNodeType;

typedef struct nodeTypeTag {
	nodeEnum type; /* type of node */
	/* union must be last entry in nodeType */
	/* because operNodeType may dynamically increase */
	union {
		conNodeType con; /* constants */
		idNodeType id; /* identifiers */
		slNodeType sl; /* sym list */
		oprNodeType opr; /* operators */
	};
} nodeType; 

#endif // __HEADING_H_
