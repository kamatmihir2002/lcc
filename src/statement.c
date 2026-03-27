#include "fwd_declarations.h"
#include "lex.h"
#include <stdio.h>
#include <stdlib.h>

void Statement(char **stream, symbol_table_t **local_symtab,
               symbol_table_t **global_symtab) {
  if (isType(peek_token(stream))) {
    Declaration(stream, local_symtab);
    if (next_token(stream) != ';') {
      /** error */
      printf("Missing semicolon.\n");
      exit(1);
    } else
      return;
  } else if (peek_token(stream) == TOKEN_IDENTIFIER ||
             peek_token(stream) == '*') {
    Expression(stream, local_symtab, global_symtab);
    if (next_token(stream) != ';') {
      // char tok = next_token(stream);
      /** error */
      printf("Missing semicolon.\n");
      exit(1);
    } else
      return;

  } else if (peek_token(stream) == t(if)) {
    next_token(stream);
    Expression(stream, local_symtab, global_symtab);
    printf("if %s\n", getExpressionResult());
    StatementBlock(stream, local_symtab, global_symtab);
    if (peek_token(stream) == t(else)) {
      next_token(stream);
      printf("else\n");
      StatementBlock(stream, local_symtab, global_symtab);
    }
    printf("end\n");
  } else if (peek_token(stream) == t(while)) {
    next_token(stream);
    Expression(stream, local_symtab, global_symtab);
    printf("while %s\n", getExpressionResult());
    StatementBlock(stream, local_symtab, global_symtab);
    printf("end\n");
  } else if (peek_token(stream) == t(return )) {
    next_token(stream);

    if (peek_token(stream) == ';') {
      next_token(stream);
      printf("return\n");
      return;
    }

    Expression(stream, local_symtab, global_symtab);
    printf("return %s\n", getExpressionResult());
    if (next_token(stream) != ';') {
      /** error */
      printf("Missing semicolon.\n");
      exit(1);
    } else
      return;
  }
}
