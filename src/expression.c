#include "expr_utils.h"
#include "fwd_declarations.h"
#include "lex.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * RValue precedence:
 *
 */

int sp = 0;

// IDENT, NUMBER, FNCALL, PAREN
void RValueExpression7(char **stream, symbol_table_t **local_symtab,
                       symbol_table_t **global_symtab) {
  int push_bp = sp;

  int tk = peek_token(stream);
  if (tk == '(') {
    next_token(stream);
    sp++;
    RValueExpression(stream, local_symtab, global_symtab, 0);
    int rg_paren = sp;

    if (peek_token(stream) == ')') {
      next_token(stream);
      printf("$%d = $%d\n", push_bp, rg_paren);
      sp = push_bp;
      return;
    } else {
      printf("No closing brackets found.\n");
      exit(1);
    }
  } else if (tk == TOKEN_IDENTIFIER) {
    next_token(stream);
    int sname_len = get_token_length();
    char *sname = str_dup(get_token());
    // fncall
    if (peek_token(stream) == '(') {
      next_token(stream);

      symbol_table_t *sym =
          symbols_find_callable(*global_symtab, sname, sname_len);

      int param_stack[32];
      int p_stack_len = 0;

      int save_param_reg = sp;

      if (peek_token(stream) == ')') {
        next_token(stream);
      } else {
        while (peek_token(stream) != ')') {
          sp++;
          RValueExpression(stream, local_symtab, global_symtab, 0);
          int rg_param = sp;

          param_stack[p_stack_len] = rg_param;
          p_stack_len++;

          int pk_tok = peek_token(stream);
          switch (pk_tok) {
          case ',':
            next_token(stream);
            continue;
          case ')':
            next_token(stream);
            goto out; /** THE HORROR! */
          case TOKEN_END:
            printf("No closing brackets found.\n");
            exit(1);
          default:
            printf("Invalid separator.\n");
            exit(1);
          }
        }
      out:

        for (int i = 0; i < p_stack_len; i++) {
          printf("param $%d\n", param_stack[i]);
        }
      }

      printf("call %s\n", sname);
      printf("$%d = call_result\n", push_bp);
      sp = push_bp;
    } else if (peek_token(stream) == '[') {
      // array index
      next_token(stream);

      sp++;
      RValueExpression(stream, local_symtab, global_symtab, 0);
      int rg_index = sp;

      if (peek_token(stream) == ']') {
        next_token(stream);
        printf("$%d = %s[$%d]\n", push_bp, sname, rg_index);
        sp = push_bp;
        return;
      } else {
        /** ERROR */
        printf("Unclosed array index operator.\n");
        exit(1);
      }
    } else {
      // normal variable
      symbol_table_t *sym = symbols_find(*local_symtab, sname, sname_len, 0);
      if (!sym)
        sym = symbols_find(*global_symtab, sname, sname_len, 1);

      printf("$%d = %s\n", push_bp, sname);
      sp = push_bp;
    }

    free(sname);
    sp = push_bp;
  } else if (tk == TOKEN_NUMBER) {
    // number

    next_token(stream);
    char *snum = str_dup(get_token());
    int snum_len = get_token_length();

    printf("$%d = %s\n", push_bp, snum);
    sp = push_bp;
    free(snum);
  }
}

// DEREF, MINUS, AMPERSAND
void RValueExpression6(char **stream, symbol_table_t **local_symtab,
                       symbol_table_t **global_symtab) {
  int push_bp = sp;

  char un_op = 0;
  if (peek_token(stream) != '*' && peek_token(stream) != '-' &&
      peek_token(stream) != '&') {

    RValueExpression7(stream, local_symtab, global_symtab);
    sp = push_bp;
    return;
  }

  un_op = next_token(stream);

  sp++;
  RValueExpression6(stream, local_symtab, global_symtab);
  int rg_a = sp;

  printf("$%d = %c$%d\n", push_bp, un_op, rg_a);
  sp = push_bp;
}

void RValueExpression(char **stream, symbol_table_t **local_symtab,
                      symbol_table_t **global_symtab, int min_bp) {
  int push_bp = sp;

  RValueExpression6(stream, local_symtab, global_symtab);
  int reg_a = sp;

  char op = 1;
  int len = 0;
  while (op = getOp(stream, &len)) {
    op = getOp(stream, &len);
    char *opString = GetOpString(op);

    short binding_pow = bpow(op);
    if (min_bp > ((char *)&binding_pow)[0]) {
      sp = push_bp;
      return;
    }

    (*stream) = (*stream) + len;

    sp++;
    RValueExpression(stream, local_symtab, global_symtab,
                     ((char *)&binding_pow)[1]);
    int reg_b = sp;
    printf("$%d = $%d %s $%d\n", push_bp, reg_a, opString, reg_b);
  }
  sp = push_bp;
}

/**
 *
 * LValue precedence
 *
 */

char lval_result[32];

void LValueExpression7(char **stream, symbol_table_t **local_symtab,
                       symbol_table_t **global_symtab) {
  int push_bp = sp;
  sp++;

  int tk = peek_token(stream);
  if (tk == '(') {
    next_token(stream);
    RValueExpression(stream, local_symtab, global_symtab, 0);
    int rg_paren = sp;

    if (peek_token(stream) == ')') {
      next_token(stream);

      // printf("$%d = $%d\n", save_reg, rg_paren);
      snprintf(lval_result, 32 * sizeof(char), "$%d\0", rg_paren);

      sp = push_bp;
      return;
    } else {
      printf("No closing brackets found.\n");
      exit(1);
    }
  } else if (tk == TOKEN_IDENTIFIER) {
    next_token(stream);
    char *sname = str_dup(get_token());
    int sname_len = get_token_length();

    if (peek_token(stream) == '[') {
      // array index
      next_token(stream);
      RValueExpression(stream, local_symtab, global_symtab, 0);
      int rg_index = sp;
      if (peek_token(stream) == ']') {
        next_token(stream);
        snprintf(lval_result, 32 * sizeof(char), "%s[$%d]\0", sname, rg_index);
        sp = push_bp;
        return;
      } else {
        /** ERROR */
        printf("No array closing brackets found.\n");
        exit(1);
      }
    } else {
      // normal variable
      // printf("$%d = %s\n", save_reg, sname);
      symbol_table_t *sym = symbols_find(*local_symtab, sname, sname_len, 0);
      if (!sym)
        sym = symbols_find(*global_symtab, sname, sname_len, 1);

      snprintf(lval_result, sizeof(char) * 32, "%s\0", sname);
      sp = push_bp;
    }

    free(sname);
    sp = push_bp;
  } else if (tk == TOKEN_NUMBER) {
    printf("Can't assign values to a constant.\n");
    exit(1);
  }
}

void LValueExpression(char **stream, symbol_table_t **local_symtab,
                      symbol_table_t **global_symtab) {
  int push_bp = sp;
  sp++;

  char un_op = 0;
  if (peek_token(stream) != '*') {
    LValueExpression7(stream, local_symtab, global_symtab);
    sp = push_bp;
    return;
  }

  un_op = next_token(stream);

  // sp++;
  RValueExpression(stream, local_symtab, global_symtab, 0);
  int rg_a = sp;
  snprintf(lval_result, sizeof(lval_result), "*$%d", rg_a);
  sp = push_bp;
}

void Expression(char **stream, symbol_table_t **local_symtab,
                symbol_table_t **global_symtab) {
  int push_bp = sp;
  sp++;

  LValueExpression(stream, local_symtab, global_symtab);

  if (peek_token(stream) != '=') {
    sp = push_bp;
    return;
  }

  next_token(stream);

  sp = -100;
  RValueExpression(stream, local_symtab, global_symtab, 0);
  int rg_b = sp;

  printf("%s = $%d\n", lval_result, rg_b);
  sp = push_bp;
}

char *getExpressionResult() { return lval_result; }
