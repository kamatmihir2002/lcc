#include "fwd_declarations.h"
#include "lex.h"

#include <stdio.h>

void Program(char **stream, symbol_table_t **global_symtab) {
  int tok = 0;
  while (peek_token(stream) != TOKEN_END) {
    GlobalStatement(stream, global_symtab);
  }
}
