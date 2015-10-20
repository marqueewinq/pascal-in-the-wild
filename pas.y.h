
#ifndef __PAS_Y_H_
#define __PAS_Y_H_

#include "heading.h"
#include "tok.h"

#define who for(map<string, Variable*>::iterator it = vs.begin(); it != vs.end(); it++) cerr << it->first << " " << it->second->ty << " " << it->second->decl << "\n";
#define pasin stdin
#define pasout stdout


/* variables */

vector <string> sym;
map <string, Variable*> vs; /* symbol table */


/* prototypes */
int yyerror(string s);
int yyerror(char* s);
int yylex();

nodeType *opr(int oper, int nops, ...);
nodeType *id(char* s);
nodeType *con(Variable value);
nodeType *sa(slist *s);
void freeNode(nodeType *p);

Variable ex(nodeType *p);
Variable mkVariable(MyType = VOID, int _i = 0, double _d = 0, char _s[] = "");

Variable mkVariable(MyType _type, int _i, double _d, char _s[])
{
	Variable v;
	v.s = new char[256];
	if (_s != NULL && strcmp(_s, "") != 0)
		strcpy(v.s, _s);
	else
		v.s = "";
	v.i = _i, v.d = _d, v.ty = _type, v.decl = false;
	return v;
}

int operator == (Variable a, Variable b)
	{
		if (a.ty == INT)
			return a.i == b.i;
		else if (a.ty == DOUBLE)
			return a.d == b.d;
		else if (a.ty == STRING)
			return strcmp(a.s, b.s);
		yyerror("something strange");
	}
	
int operator != (Variable a, Variable b)
	{
		return !(a == b);
	}


#define SIZEOF_NODETYPE ((char *)&p->con - (char *)p)
nodeType *con(Variable value) 
{
	nodeType *p;
	if ((p = new nodeType) == NULL)
		yyerror("memory lack");

	/* copy information */
	p->type = typeCon;
//	cerr << value.i << " " << value.d << " " << value.type << "\n";
	p->con.value = value;
	return p;
}

nodeType *sa(slist *s)
{
	nodeType *p;
	if ((p = new nodeType) == NULL)
		yyerror("memory lack");
		
	/* cp information */
	if (!s)
		yyerror("slist is NULL");
	p->type = typeSlist;
	p->sl.s = s;
	return p;
}

nodeType *id(char* s) 
{
	nodeType *p;
	if ((p = new nodeType) == NULL)
		yyerror("memory lack");

	/* copy information */

	p->type = typeId;
	sym.push_back(s);
	p->id.i = sym.size() - 1;

	return p;
}

string print_oper(int oper)
{
	switch(oper)
	{
		case _R_INTEGER:
			return "integer";
		case _R_DOUBLE:
			return "double";
		case _BUILTIN_READ:
			return "read";
		case _BUILTIN_WRITE:
			return "write";
		case _R_WHILE: 
			return "while";
		case _R_IF:
			return "if";
		case SEPARATOR_LIST:
			return ",";
		case SEPARATOR_LINE:
			return ";";
		case ASSIGN:
			return ":=";
		case o_plus:
			return "+";

/*				case o_minus: 
					return ex(p->opr.op[0]) - ex(p->opr.op[1]);
				case o_mul: 
					return ex(p->opr.op[0]) * ex(p->opr.op[1]);
				case o_div: 
					return ex(p->opr.op[0]) / ex(p->opr.op[1]);
				case o_mod:
					return ex(p->opr.op[0]) % ex(p->opr.op[1]);
				case o_land:
					return ex(p->opr.op[0]) & ex(p->opr.op[1]);
				case o_lor:
					return ex(p->opr.op[0]) | ex(p->opr.op[1]);
				case cmp_l:
					return ex(p->opr.op[0]) < ex(p->opr.op[1]);
				case cmp_g: 
					return ex(p->opr.op[0]) > ex(p->opr.op[1]);
				case cmp_geq: 
					return ex(p->opr.op[0]) >= ex(p->opr.op[1]);
				case cmp_leq: 
					return ex(p->opr.op[0]) <= ex(p->opr.op[1]);
				case cmp_neq: 
					return ex(p->opr.op[0]) != ex(p->opr.op[1]);
				case cmp_eq: 
					return ex(p->opr.op[0]) == ex(p->opr.op[1]);
*/
	}
	
	return "fuck shit idk";
}

nodeType *opr(int oper, int nops, ...) 
{
//	cerr << print_oper(oper) << "\n";
	
	va_list ap;
	nodeType *p;
	if ((p = new nodeType) == NULL)
		yyerror("memory lack");
			
	/* copy information */
	p->type = typeOpr;
	p->opr.oper = oper;
	p->opr.nops = nops;
	p->opr.op = new struct nodeTypeTag*[nops];
	
	va_start(ap, nops);
	for (int i = 0; i < nops; i++)
		p->opr.op[i] = va_arg(ap, nodeType*);
	va_end(ap);
	
	return p;
}

void freeNode(nodeType *p) 
{
	int i;
	if (!p) 
		return;
	if (p->type == typeOpr) 
	{
		for (i = 0; i < p->opr.nops; i++)
			freeNode(p->opr.op[i]);
		delete[] p->opr.op;
	}
	delete (p);
}


Variable ex(nodeType *p) 
{
	if (p == NULL)
		return mkVariable();

	int r = 0;
	double rd = 0;
	nodeType *cur = NULL;
	slist *s;
	char* sy;
	Variable a, b, res;
	switch (p->type) 
	{
		case typeSlist:
			yyerror("something strange happened");
		case typeCon:
			return p->con.value;
		case typeId:
			if (vs.find(sym[p->id.i]) == vs.end() || vs[sym[p->id.i]]->ty == VOID)
				yyerror("undeclared variable invoke");
			return *vs[sym[p->id.i]];
		case typeOpr: 
			switch(p->opr.oper) 
			{
				case _R_INTEGER:
					s = p->opr.op[0]->sl.s;
					while (s != NULL)
					{
						vs[string(s->s)] = new Variable;
						*vs[string(s->s)] = mkVariable();
						vs[string(s->s)]->decl = true;
						vs[string(s->s)]->ty = INT;
						s = s->next;
					}
					return mkVariable();
				case _R_DOUBLE:
					s = p->opr.op[0]->sl.s;
					while (s != NULL)
					{
						vs[string(s->s)] = new Variable;
						*vs[string(s->s)] = mkVariable();
						vs[string(s->s)]->decl = true;
						vs[string(s->s)]->ty = DOUBLE;
						s = s->next;
					}
					return mkVariable();					
				case _BUILTIN_READ:
					s = p->opr.op[0]->sl.s;
					while (s != NULL)
					{
						if (vs.find(string(s->s)) == vs.end())
							yyerror("undeclared symbol");
						if (vs[string(s->s)] == NULL)
							yyerror("something strange happened");
												
						fprintf(pasout, "#   ");
						if (vs[string(s->s)]->ty == INT)			
						{
							fscanf(pasin, "%d", &r);
							vs[string(s->s)]->i = r;
						}
						else if (vs[string(s->s)]->ty == DOUBLE)
						{
							fscanf(pasin, "%lf", &rd);
							vs[string(s->s)]->d = rd;
						}
						else
						{
							yyerror("undeclared variable usage");
						}
						
						s = s->next;
					}
					return mkVariable();
				case _BUILTIN_WRITE:
					a = ex(p->opr.op[0]);
					switch (a.ty)
					{
						case INT:
							fprintf(pasout, "#   %d\n", a.i);
							break;
						case DOUBLE:
							fprintf(pasout, "#   %lf\n", a.d);
							break;
						case STRING:
							fprintf(pasout, "#   %s\n", a.s);
							break;
						case VOID:
							yyerror("undeclared variable usage");
							break;
					}
					return mkVariable();
				case _R_WHILE: 
					while (ex(p->opr.op[0]) != mkVariable())
						ex(p->opr.op[1]); 
					return mkVariable();
				case _R_IF:
					if (ex(p->opr.op[0]) != mkVariable())
						ex(p->opr.op[1]);
					else if (p->opr.nops > 2)
						ex(p->opr.op[2]);
					return mkVariable(); 
				case SEPARATOR_LIST:
					// TODO
				case SEPARATOR_LINE:
					ex(p->opr.op[0]);
					return ex(p->opr.op[1]);
				case ASSIGN:
					if (p->opr.op[0]->id.i >= sym.size() || vs.find(sym[p->opr.op[0]->id.i]) == vs.end() || vs[sym[p->opr.op[0]->id.i]] == NULL )//|| !vs[sym[p->opr.op[0]->id.i]]->decl)
						yyerror("undeclared variable usage");
					a = ex(p->opr.op[1]);
					if (vs[sym[p->opr.op[0]->id.i]]->ty != a.ty)
						yyerror("type clash");

					vs[sym[p->opr.op[0]->id.i]]->i = a.i;
					vs[sym[p->opr.op[0]->id.i]]->d = a.d;
					return a;
				case o_plus:
					a = ex(p->opr.op[0]);
					b = ex(p->opr.op[1]);
					if (a.ty != b.ty)
						yyerror("type clash");
					if (a.ty == STRING)
						yyerror(string("unimplemented operation over strings"));
					return mkVariable(a.ty, a.i + b.i, a.d + b.d);
				case o_minus: 
					a = ex(p->opr.op[0]);
					b = ex(p->opr.op[1]);
					if (a.ty != b.ty)
						yyerror("type clash");
					if (a.ty == STRING)
						yyerror(string("unimplemented operation over strings"));
					return mkVariable(a.ty, a.i - b.i, a.d - b.d);
				case o_mul:
					a = ex(p->opr.op[0]);
					b = ex(p->opr.op[1]);
					if (a.ty != b.ty)
						yyerror("type clash");
					if (a.ty == STRING)
						yyerror(string("unimplemented operation over strings"));
					return mkVariable(a.ty, a.i * b.i, a.d * b.d);
				case o_div: 
					a = ex(p->opr.op[0]);
					b = ex(p->opr.op[1]);
					if (a.ty != b.ty)
						yyerror("type clash");
					if (a.ty == STRING)
						yyerror(string("unimplemented operation over strings"));
					if (a.ty == INT)
						return mkVariable(INT, a.i / b.i, 0);
					else if (a.ty == DOUBLE)
						return mkVariable(DOUBLE, 0, a.d / b.d);
				case o_mod:
					a = ex(p->opr.op[0]);
					b = ex(p->opr.op[1]);
					if (a.ty != INT || b.ty != INT)
						yyerror("type clash");
					if (a.ty == STRING)
						yyerror(string("unimplemented operation over strings"));
					return mkVariable(a.ty, a.i + b.i, 0);
				case o_land:
					a = ex(p->opr.op[0]);
					b = ex(p->opr.op[1]);
					if (a.ty != INT || b.ty != INT)
						yyerror("type clash");
					if (a.ty == STRING)
						yyerror(string("unimplemented operation over strings"));
					return mkVariable(a.ty, a.i & b.i, 0);
				case o_lor:
					a = ex(p->opr.op[0]);
					b = ex(p->opr.op[1]);
					if (a.ty != INT || b.ty != INT)
						yyerror("type clash");
					if (a.ty == STRING)
						yyerror(string("unimplemented operation over strings"));
					return mkVariable(a.ty, a.i | b.i, 0);
				case cmp_l:
					a = ex(p->opr.op[0]);
					b = ex(p->opr.op[1]);
					if (a.ty != b.ty)
						yyerror("type clash");
					if (a.ty == STRING)
						yyerror(string("unimplemented operation over strings"));
					if (a.ty == INT)
						return mkVariable(INT, a.i < b.i, 0);
					else if (a.ty == DOUBLE)
						return mkVariable(INT, a.d < b.d, 0);
				case cmp_g: 
					a = ex(p->opr.op[0]);
					b = ex(p->opr.op[1]);
					if (a.ty != b.ty)
						yyerror("type clash");
					if (a.ty == STRING)
						yyerror(string("unimplemented operation over strings"));
					if (a.ty == INT)
						return mkVariable(INT, a.i > b.i, 0);
					else if (a.ty == DOUBLE)
						return mkVariable(INT, a.d > b.d, 0);
				case cmp_geq: 
					a = ex(p->opr.op[0]);
					b = ex(p->opr.op[1]);
					if (a.ty != b.ty)
						yyerror("type clash");
					if (a.ty == STRING)
						yyerror(string("unimplemented operation over strings"));
					if (a.ty == INT)
						return mkVariable(INT, a.i >= b.i, 0);
					else if (a.ty == DOUBLE)
						return mkVariable(INT, a.d >= b.d, 0);
				case cmp_leq: 
					a = ex(p->opr.op[0]);
					b = ex(p->opr.op[1]);
					if (a.ty != b.ty)
						yyerror("type clash");
					if (a.ty == STRING)
						yyerror(string("unimplemented operation over strings"));
					if (a.ty == INT)
						return mkVariable(INT, a.i <= b.i, 0);
					else if (a.ty == DOUBLE)
						return mkVariable(INT, a.d <= b.d, 0);
				case cmp_neq: 
					a = ex(p->opr.op[0]);
					b = ex(p->opr.op[1]);
					if (a.ty != b.ty)
						yyerror("type clash");
					if (a.ty == STRING)
						yyerror(string("unimplemented operation over strings"));
					if (a.ty == INT)
						return mkVariable(INT, a.i != b.i, 0);
					else if (a.ty == DOUBLE)
						return mkVariable(INT, a.d != b.d, 0);
				case cmp_eq: 
					a = ex(p->opr.op[0]);
					b = ex(p->opr.op[1]);
					if (a.ty != b.ty)
						yyerror("type clash");
					if (a.ty == STRING)
						yyerror(string("unimplemented operation over strings"));
					if (a.ty == INT)
						return mkVariable(INT, a.i == b.i, 0);
					else if (a.ty == DOUBLE)
						return mkVariable(INT, (a.d - b.d) < 1e-9, 0);
			}
	}
	
	return mkVariable();
} 

int yyerror(string s)
{
	return yyerror(s.c_str());
}

int yyerror(char* s)
{
  extern int yylineno;	// defined and maintained in lex.c
  extern char *yytext;	// defined and maintained in lex.c
  
  fprintf(stderr, "\n#   program rejected by interpreter due to %s NEAR symbol \"%s\" on line %d\n", s, yytext, yylineno);
  exit(1);
}


#endif // __PAS_Y_H_
