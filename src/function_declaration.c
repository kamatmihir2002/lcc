#include "lex.h"
#include "fwd_declarations.h"

#include <stdlib.h>
#include <stdio.h>

void FunctionDeclaration(char** stream, symbol_table_t** local_symtab, symbol_table_t** global_symtab) {
    char* save_stream = *stream;
    if (!isType(peek_token(stream))) {
        *stream = save_stream;
        return;
    }

    int type = next_token(stream);
    int ptr_level = 0;
    PtrLevel(stream, &ptr_level);
    char* symname = NULL;
    int symlen = 0, arrlen = 0;
    SymArray(stream, &symname, &symlen, &arrlen);
    
    (*global_symtab) = symtab_push_array_ptr((*global_symtab), type, symname, symlen, ptr_level, arrlen);
    
    if (!(*global_symtab)) {
        /** error, already exists */
    }

    if (peek_token(stream) == '(') {
        next_token(stream);

        while (peek_token(stream) != TOKEN_END) {

            if (peek_token(stream) == ')') {
                next_token(stream);
                break;
            }

            Declaration(stream, local_symtab);

            if (peek_token(stream) == ')') {
                next_token(stream);
                break;
            }
            if (peek_token(stream) == ',') {
                next_token(stream);
                continue;
            }
            printf("Invalid separator.\n");
            exit(1);
        }
        // next_token(stream);
    }
    else {
        // not a fn declaration.
        *stream = save_stream;
        return;
    }


}