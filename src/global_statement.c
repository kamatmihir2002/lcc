#include "lex.h"
#include "fwd_declarations.h"

#include <stdio.h>
#include <stdlib.h>

void GlobalStatement(char** stream, symbol_table_t** global_symtab) {
    char* save = *stream;
    symbol_table_t* local_symtab = NULL;
    FunctionDefinition(stream, &local_symtab, global_symtab);

    if (*stream != save) {
        return;
    }

    Declaration(stream, global_symtab);
    if (next_token(stream) == ';')
        return;
    else {
        printf("Missing semicolon.\n");
        exit(1);
    }
}
