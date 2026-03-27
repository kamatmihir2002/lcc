#include "fwd_declarations.h"
#include "lex.h"

#include <stdio.h>
#include <stdlib.h>

void FunctionDeclaration(char **stream, symbol_table_t **local_symtab,
                         symbol_table_t **global_symtab) {
  char *save_stream = *stream;
  if (!isType(peek_token(stream))) {
    *stream = save_stream;
    return;
  }

  int type = next_token(stream);
  int ptr_level = 0;
  PtrLevel(stream, &ptr_level);
  char *symname = NULL;
  int symlen = 0, arrlen = 0;
  SymArray(stream, &symname, &symlen, &arrlen);

  symbol_table_t *gtab = *global_symtab;
  symbol_table_t *fname = symtab_find_sym(*global_symtab, symname, symlen);

  if (!fname) {
    (*global_symtab) = symtab_push_array_ptr((*global_symtab), type, symname,
                                             symlen, ptr_level, arrlen);
    fname = symtab_tail(*(global_symtab));
    fname->sym_is_func = 1;
    fname->sym_func_has_def = 0;
    *global_symtab = fname;
  } else {

    /** make sure to preserve global_symtab outside this func */
    *global_symtab = fname;
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

  } else {
    // not a fn declaration.
    if (gtab)
      *global_symtab = gtab;
    *stream = save_stream;
    return;
  }
}