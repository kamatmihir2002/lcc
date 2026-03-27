#include "fwd_declarations.h"
#include "lex.h"
#include "symtab.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SYM_REFER -1
#define SYM_CREATE 1

int isType(int token) { return (token == TOKEN_int) || (token == TOKEN_char); }

void PtrLevel(char **stream, int *ptrlevel) {
  while (peek_token(stream) == '*') {
    (*ptrlevel)++;
    next_token(stream);
  }
}

void SymArray(char **stream, char **symname, int *symlen, int *arr_len) {
  if (peek_token(stream) != TOKEN_IDENTIFIER) {
    return;
  }

  int tok = next_token(stream);
  *symlen = get_token_length();
  (*symname) = malloc(sizeof(char) * (1 + get_token_length()));
  memcpy((*symname), get_token(), get_token_length());
  (*symname)[get_token_length()] = 0;
  if (peek_token(stream) == '[') {
    next_token(stream);
    if (peek_token(stream) == TOKEN_NUMBER) {
      next_token(stream);
      (*arr_len) = atoi(get_token());
      if (peek_token(stream) == ']') {
        next_token(stream);
      } else
        return;
    } else
      return;
  }

  return;
}

void Declaration(char **stream, symbol_table_t **symtab) {
  if (!isType(peek_token(stream))) {
    return;
  }

  int type = next_token(stream);
  int ptr_level = 0;
  PtrLevel(stream, &ptr_level);
  char *symname = NULL;
  int symlen = 0, arrlen = 0;
  SymArray(stream, &symname, &symlen, &arrlen);

  symbol_table_t *sym = symtab_find_sym((*symtab), symname, symlen);
  if (sym) {
    printf("Symbol exists.\n");
    exit(1);
  }
  (*symtab) = symtab_push_array_ptr((*symtab), type, symname, symlen, ptr_level,
                                    arrlen);
}