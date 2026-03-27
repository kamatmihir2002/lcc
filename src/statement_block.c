#include "fwd_declarations.h"
#include "lex.h"
#include "symtab.h"

#include <stdio.h>
#include <stdlib.h>

void StatementBlock(char **stream, symbol_table_t **local_symtab,
                    symbol_table_t **global_symtab) {
  if (peek_token(stream) != '{') {
    return;
  }
  printf("block_start\n");
  next_token(stream);

  while (peek_token(stream) != TOKEN_END) {
    Statement(stream, local_symtab, global_symtab);
    if (peek_token(stream) == '}') {
      next_token(stream);
      printf("block_end\n");
      return;
    }
  }
  printf("Unclosed Statement Block.\n");
  exit(1);
}
