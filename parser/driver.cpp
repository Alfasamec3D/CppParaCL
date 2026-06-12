#include "parser.hpp"

IScope* currentScope = nullptr;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::fprintf(stderr, "Usage: %s <source-file>\n", argv[0]);
        return 1;
    }

    FILE* f = std::fopen(argv[1], "r");
    if (!f) {
        std::perror("Cannot open file");
        return 1;
    }

    yyin = f;
    currentScope = create_scope();
    const int rc = yyparse();
    std::fclose(f);
    delete currentScope;

    return rc;
}

void PrintError(char const* fmt, ...) {
    char buf[10000];
    va_list args;
    va_start(args, fmt);
    std::vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    std::fprintf(stderr, "Error: %s at line %d\n", buf, yylineno);
}
