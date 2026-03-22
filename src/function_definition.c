#include "lex.h"
#include "fwd_declarations.h"

#include <stdio.h>

int getSize(int tk_type) {
    switch(tk_type) {
        case TOKEN_int: return 4;
        case TOKEN_char: return 1;
        default:
            return 1;
    }
    return 1;
}

long long getType(int tk_type) {
    char* type_int = "int\0";
    char* type_char = "char\0";

    switch(tk_type) {
        case TOKEN_int: return *(long long*)(type_int);
        case TOKEN_char: return *(long long*)(type_char);
        default:
            return 1;
    }
    return 1;
}

void FunctionDefinition(char** stream, symbol_table_t** local_symtab, symbol_table_t** global_symtab) {
    char* save = *stream;
    FunctionDeclaration(stream, local_symtab, global_symtab);
    
    if (*stream == save) {
        *stream = save;
        return;
    }

    symbol_table_t* fun_symbol = symtab_tail(*global_symtab);
    long long retType = getType(fun_symbol->symtype);

    printf("function_name %s return %s ptr_level %d\n", fun_symbol->symname, (char*)(&retType), fun_symbol->sym_ptrlevel);
    symbol_table_t* params = *local_symtab;
    if (params) {
        while (params->next) {
            printf("param %s size %d pointer_level %d array_length %d\n", params->symname, getSize(params->symtype), params->sym_ptrlevel, params->sym_lenarray);
            params = params->next;
        }
    }
    
    printf("param %s size %d pointer_level %d array_length %d\n", params->symname, getSize(params->symtype), params->sym_ptrlevel, params->sym_lenarray);
    if (peek_token(stream) != ';') {
        StatementBlock(stream, local_symtab, global_symtab);
    }
    else {
        next_token(stream);
    }
}
