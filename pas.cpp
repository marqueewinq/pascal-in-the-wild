#include "heading.h"
#include "tok.h"
// prototype of bison-generated parser function
int yyparse();
extern FILE* yyin;
int yydebug;
extern int yy_flex_debug;

int main(int argc, char **argv)
{
  yydebug = 5;
  yy_flex_debug = 1;

  if ((argc > 1) && ((yyin = fopen(argv[1], "r")) == NULL))
  {
    cerr << argv[0] << ": File " << argv[1] << " cannot be opened.\n";
    exit( 1 );
  }
  
  return yyparse();
}

