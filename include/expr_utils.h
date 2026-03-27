
#pragma once

#include "symtab.h"

#define OP_doubleEqual -1
#define OP_nEqual -2
#define OP_OR -3
#define OP_AND -4
#define OP_GTEQ -6
#define OP_LTEQ -8
#define NUMOPS

char *str_dup(char *s);

char *GetOpString(int op);

symbol_table_t *symbols_find_callable(symbol_table_t *symtab, char *symname,
                                      int symlen);

symbol_table_t *symbols_find(symbol_table_t *symtab, char *symname, int symlen,
                             char exit_if_not_found);

int getOp(char **stream, int *len);
short bpow(char op);