#include "expr_utils.h"
#include "lex.h"
#include "symtab.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *OpStrings = "+\0\0-\0\0*\0\0/\0\0>\0\0<\0\0==\0!=\0>=\0<=\0||\0&&";

char *str_dup(char *s) {
  char *st = s;
  while ((*st) != 0)
    st = st + 1;
  unsigned int len = (unsigned int)(st - s);
  char *sdup = malloc(sizeof(char) * len);
  memcpy(sdup, s, sizeof(char) * len);
  sdup[len] = 0;
  return sdup;
}

char *GetOpString(int op) {
  switch (op) {
  case '+':
    return OpStrings;
  case '-':
    return OpStrings + 3;
  case '*':
    return OpStrings + 6;
  case '/':
    return OpStrings + 9;
  case '>':
    return OpStrings + 12;
  case '<':
    return OpStrings + 15;
  case OP_doubleEqual:
    return OpStrings + 18;
  case OP_nEqual:
    return OpStrings + 21;
  case OP_GTEQ:
    return OpStrings + 24;
  case OP_LTEQ:
    return OpStrings + 27;
  case OP_OR:
    return OpStrings + 30;
  case OP_AND:
    return OpStrings + 33;
  default:
    return NULL;
  }
}

symbol_table_t *symbols_find_callable(symbol_table_t *symtab, char *symname,
                                      int symlen) {
  symbol_table_t *sym = symtab_find_sym(symtab, symname, symlen);

  if (!sym) {
    printf("Function not found.\n");
    exit(1);
  } else {
    if (!sym->sym_is_func) {
      printf("Calling a non callable symbol.\n");
      exit(1);
    }
  }
  return sym;
}

symbol_table_t *symbols_find(symbol_table_t *symtab, char *symname, int symlen,
                             char exit_if_not_found) {
  symbol_table_t *sym = symtab_find_sym(symtab, symname, symlen);
  if (!sym) {
    if (exit_if_not_found) {
      printf("Symbol not found.\n");
      exit(1);
    } else {
      return NULL;
    }
  }
  return sym;
}

int getOp(char **stream, int *len) {
  char return_op = 0;
  char *start = *stream;

  int pk_tok = peek_token(stream);
  int pk_tok_2;
  switch (pk_tok) {
  case '*':
  case '/':
  case '+':
  case '-':
    return_op = next_token(stream);
    break;
  case '=':
  case '|':
  case '&':
    next_token(stream);
    if (next_token(stream) == pk_tok) {
      switch (pk_tok) {
      case '=':
        return_op = OP_doubleEqual;
        break;
      case '|':
        return_op = OP_OR;
        break;
      case '&':
        return_op = OP_AND;
        break;
      }
    } else {
      printf("Operator error.");
      exit(1);
    }
    break;
  case '>':
  case '<':
    next_token(stream);
    pk_tok_2 = peek_token(stream);
    if (pk_tok_2 == '=') {
      next_token(stream);
      return_op = (pk_tok == '>') ? OP_GTEQ : OP_LTEQ;
    } else {
      return_op = pk_tok;
    }
    break;
  default:
    return_op = 0;
  }
  *len = *stream - start;
  *stream = start;
  return return_op;
}

short bpow(char op) {
  char bps[2] = "AB";
  short sb = *(short *)bps;
  short incrs = (10) << 8 | (10);

  switch (op) {
  case OP_OR:
    return sb - 5 * incrs;
  case OP_AND:
    return sb - 4 * incrs;
  case OP_doubleEqual:
  case OP_nEqual:
    return sb - 3 * incrs;
  case '>':
  case OP_GTEQ:
  case '<':
  case OP_LTEQ:
    return sb - 2 * incrs;
  case '+':
  case '-':
    return sb - incrs;
  case '*':
  case '/':
    return sb;
  default:
  }
}