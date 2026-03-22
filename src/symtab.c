
#include <stdlib.h>
#include <string.h>

#include "symtab.h"

symbol_table_t* push_sym(symbol_table_t* symtab, int symtype, char* symname, int symlen, char sym_ptrlevel, char sym_lenarray) {
    if (!symtab) {
        symbol_table_t* s = malloc(sizeof(symbol_table_t) + sizeof(char) * (symlen + 1));
        s->next = NULL;
        memcpy(&(s->symname), symname, (symlen) * sizeof(char));
        s->symname[symlen] = 0;
        s->sym_ptrlevel = sym_ptrlevel;
        s->sym_lenarray = sym_lenarray;
        s->symlen = symlen;
        s->symtype = symtype;
        return s;
    }
    else {
        symbol_table_t* st = symtab;
        while (st->next) {
            if (strcmp(st->symname, symname) == 0) {
                return NULL;
            }
            st = st->next;
        }
        st->next = push_sym(NULL, symtype, symname, symlen, sym_ptrlevel, sym_lenarray);
        return symtab;
    }
}

symbol_table_t* symtab_push_single_sym(symbol_table_t* symtab, int symtype, char* symname, int symlen) {
    return push_sym(symtab, symtype, symname, symlen, 0, 1);
} 

symbol_table_t* symtab_push_single_ptr(symbol_table_t* symtab, int symtype, char* symname, int symlen, char ptrlevel) {
    return push_sym(symtab, symtype, symname, symlen, ptrlevel, 1);
}


symbol_table_t* symtab_push_array_sym(symbol_table_t* symtab, int symtype, char* symname, int symlen, int arr_len) {
    return push_sym(symtab, symtype, symname, symlen, 0, arr_len);
} 

symbol_table_t* symtab_push_array_ptr(symbol_table_t* symtab, int symtype, char* symname, int symlen, char ptrlevel, int arr_len) {
    return push_sym(symtab, symtype, symname, symlen, ptrlevel, arr_len);
}


symbol_table_t* symtab_find_sym(symbol_table_t* symtab, char* symname, int symlen) {
    symbol_table_t* st = symtab;
    while (st->next) {
        if (strcmp(st->symname, symname) == 0)
            return st;
        st = st->next;
    }
    if (strcmp(st->symname, symname) == 0)
        return st;
    return NULL;
}

symbol_table_t* symtab_tail(symbol_table_t* symtab) {
    if (!symtab) {
        return NULL;
    }

    symbol_table_t* st = symtab;
    while (st->next) {
        st = st->next;
    }
    return st;
}