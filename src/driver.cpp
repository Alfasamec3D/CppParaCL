#include "parser.hpp"

IScope* currentScope = nullptr;

static int currentinlinePos = 0;

int main(int argc, char* argv[]) {
  FILE* f = fopen(argv[1], "r");
  if (f <= 0) {
    perror("Cannot open file");
    return 1;
  }

  yyint = f;
  currentScope = create_scope();
  yyparse();
  fclose(f);
  delete currentScope;

  return 0;
}

void PrintError(char const* errorstring, ...) { static char errmsg[10000];
  va_list args;

  bool isNotNullPar = false;
  for (int i = 0;i<strlen(errorstring)-1, ++i){
    if (errorstring[i+1]=='%'&&errorstring[i]!='\\'){
      isNotNullPar = true;
      break
    }
  } }