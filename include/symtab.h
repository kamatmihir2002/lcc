#pragma once

typedef struct symbol_table {
    struct symbol_table* next;
    int symtype;
    char sym_ptrlevel;
    char sym_lenarray;
    int symlen;
    char symname[0];
} symbol_table_t;

symbol_table_t* symtab_push_single_sym(symbol_table_t* symtab, int symtype, char* symname, int symlen);

symbol_table_t* symtab_push_single_ptr(symbol_table_t* symtab, int symtype, char* symname, int symlen, char ptrlevel);

symbol_table_t* symtab_push_array_sym(symbol_table_t* symtab, int symtype, char* symname, int symlen, int arr_len);

symbol_table_t* symtab_push_array_ptr(symbol_table_t* symtab, int symtype, char* symname, int symlen, char ptrlevel, int arr_len);

symbol_table_t* symtab_find_sym(symbol_table_t* symtab, char* symname, int symlen);

symbol_table_t* symtab_tail(symbol_table_t* symtab);