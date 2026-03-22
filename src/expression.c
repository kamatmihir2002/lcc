#include "fwd_declarations.h"
#include "lex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * RValue precedence:
 *
 */

char reg_stack_vars[32][32]; 
int reg_stack = 0;

//IDENT, NUMBER, FNCALL, PAREN
void RValueExpression7(char** stream, symbol_table_t** local_symtab, symbol_table_t** global_symtab) {
    int save_reg = reg_stack;

    int tk = peek_token(stream);
    if (tk == '(') {
        next_token(stream);
        reg_stack++;
        RValueExpression(stream, local_symtab, global_symtab);
        int rg_paren = reg_stack;

        if (peek_token(stream) == ')') {
            next_token(stream); 
            printf("$%d = $%d\n", save_reg, rg_paren);
            reg_stack = save_reg;
            return;
        }
        else {
            printf("No closing brackets found.\n");
            exit(1);
        }
    }
    else if (tk == TOKEN_IDENTIFIER) {
        next_token(stream);
        char* sname = malloc(sizeof(char) * get_token_length());
        int sname_len = get_token_length();
        memcpy(sname, get_token(), sizeof(char) * (get_token_length() + 1));
        sname[get_token_length()] = 0;

        // fncall
        if (peek_token(stream) == '(') {
            next_token(stream);

            symbol_table_t* sym = symtab_find_sym(*global_symtab, sname, sname_len);
            
            if (!sym) {
                printf("Function not found.\n");
                exit(1);
            }
            else {
                if (!sym->sym_is_func) {
                    printf("Calling a non callable symbol.\n");
                    exit(1);
                }
            }

            int param_stack[32];
            int p_stack_len = 0;

            int save_param_reg = reg_stack;

            if (peek_token(stream) == ')') {
                next_token(stream);
                
            }
            else {
                while (peek_token(stream)!= ')') {
                    
                    reg_stack++;
                    RValueExpression(stream, local_symtab, global_symtab);
                    int rg_param = reg_stack;
                    
                    param_stack[p_stack_len] = rg_param;
                    p_stack_len++;

                    if (peek_token(stream) == ',') {
                        next_token(stream);
                        continue;
                    } 
                    if (peek_token(stream) == TOKEN_END) {
                        printf("No closing brackets found.\n");
                        exit(1);
                    }
                    if (peek_token(stream) == ')') {
                        next_token(stream);
                        break;
                    }
                    printf("Invalid separator.\n");
                    exit(1);
                }
                
                for (int i = 0; i < p_stack_len; i++) {
                    printf("param $%d\n", param_stack[i]);
                }
            }

            printf("call %s\n", sname);
            printf("$%d = call_result\n", save_reg);
            reg_stack = save_reg;
        }
        else if (peek_token(stream) == '[') {
            // array index
            next_token(stream);
            reg_stack++;
            RValueExpression(stream, local_symtab, global_symtab);
            int rg_index = reg_stack;
            if (peek_token(stream) == ']') {
                next_token(stream);
                printf("$%d = %s[$%d]\n", save_reg, sname, rg_index);
                reg_stack = save_reg;
                return;
            }
            else {
                /** ERROR */
                printf("Unclosed array index operator.\n");
                exit(1);
            }
        }
        else {
            // normal variable
            symbol_table_t* sym = symtab_find_sym(*local_symtab, sname, sname_len);
            if (!sym){
                sym = symtab_find_sym(*global_symtab, sname, sname_len);
            }

            if (!sym) {
                printf("Symbol not found.\n");
                exit(1);
            }
            printf("$%d = %s\n", save_reg, sname);
            reg_stack = save_reg;
        }

        free(sname);
        reg_stack = save_reg;
    }
    else if (tk == TOKEN_NUMBER) {
        // number

        next_token(stream);
        char* snum = malloc(sizeof(char) * (get_token_length() + 1));
        int snum_len = get_token_length();
        memcpy(snum, get_token(), sizeof(char) * (get_token_length() + 1));
        snum[snum_len] = 0;

        printf("$%d = %s\n", save_reg, snum);
        reg_stack = save_reg;
        free(snum);
    }
    
}

// DEREF, MINUS, AMPERSAND
void RValueExpression6(char** stream, symbol_table_t** local_symtab, symbol_table_t** global_symtab) {

    int save_reg = reg_stack;
    char un_op = 0;
    if (peek_token(stream) != '*' && peek_token(stream) != '-' && peek_token(stream) != '&') {
        
        RValueExpression7(stream, local_symtab, global_symtab);
        reg_stack = save_reg;
        return;
    }

    un_op = next_token(stream);

    reg_stack++;
    RValueExpression6(stream, local_symtab, global_symtab);
    int rg_a = reg_stack;
    printf("$%d = %c$%d\n", save_reg, un_op, rg_a);
    reg_stack = save_reg;
}

// MULDIV
void RValueExpression5(char** stream, symbol_table_t** local_symtab, symbol_table_t** global_symtab) {

    int save_reg = reg_stack;
    RValueExpression6(stream, local_symtab, global_symtab);
    int rg_a = reg_stack;

    int op_mul = 0;
    if (peek_token(stream) != '*' && peek_token(stream) != '/') {
        reg_stack = save_reg;
        return;
    }

    op_mul = next_token(stream);


    reg_stack++;
    RValueExpression5(stream, local_symtab, global_symtab);
    int rg_b = reg_stack;
    printf("$%d = $%d %c $%d\n", save_reg, rg_a, op_mul, rg_b);
    reg_stack = save_reg;
}

// ADDSUB
void RValueExpression4(char** stream, symbol_table_t** local_symtab, symbol_table_t** global_symtab) {

    int save_reg = reg_stack;
    RValueExpression5(stream, local_symtab, global_symtab);
    int rg_a = reg_stack;

    int op_add = 0;
    if (peek_token(stream) != '+' && peek_token(stream) != '-') {
        reg_stack = save_reg;
        return;
    }

    op_add = next_token(stream);


    reg_stack++;
    RValueExpression4(stream, local_symtab, global_symtab);
    int rg_b = reg_stack;

    printf("$%d = $%d %c $%d\n", save_reg, rg_a, op_add, rg_b);
    reg_stack = save_reg;
}

// GT/LT (EQ)
void RValueExpression3(char** stream, symbol_table_t** local_symtab, symbol_table_t** global_symtab) {

    int save_reg = reg_stack;
    RValueExpression4(stream, local_symtab, global_symtab);
    int rg_a = reg_stack;
    
    int gt_lt;
    int eq;
    if (peek_token(stream) != '<' && peek_token(stream) != '>') {
        reg_stack = save_reg;
        return;
    }

    gt_lt = (next_token(stream) == '>');
    
    if (peek_token(stream) != '=') {
        eq = 0;
    }
    else {
        eq = 1;
        next_token(stream);
    }
    

    reg_stack++;
    RValueExpression3(stream, local_symtab, global_symtab);
    int rg_b = reg_stack;

    char n[3];
    n[0] = '<';

    if (gt_lt) {
        n[0] = '>';
    }
    n[1] = 0;
    if (eq) {
        n[1] = '=';
        n[2] = 0;
    }
    

    printf("$%d = $%d %s $%d\n", save_reg, rg_a, n, rg_b);
    reg_stack = save_reg;
}

// Eq | NEQ
void RValueExpression2(char** stream, symbol_table_t** local_symtab, symbol_table_t** global_symtab) {

    int save_reg = reg_stack;

    RValueExpression3(stream, local_symtab, global_symtab);
    int rg_a = reg_stack;

    if (peek_token(stream) != '=' && peek_token(stream) != '!') {
        reg_stack = save_reg;
        return;
    }

    // save this stream position if assignment
    char* save_if_assignment = *stream;
    char neq = (next_token(stream));

    if (peek_token(stream) != '=') {
        reg_stack = save_reg;
        if (neq == '=') {
            *stream = save_if_assignment;
        }
        return;
    }
    next_token(stream);
    
    reg_stack++;
    RValueExpression2(stream, local_symtab, global_symtab);
    int rg_b = reg_stack;

    printf("$%d = $%d %c= $%d\n", save_reg, rg_a, neq, rg_b);
    reg_stack = save_reg;
}

// AND
void RValueExpression1(char** stream, symbol_table_t** local_symtab, symbol_table_t** global_symtab) {

    int save_reg = reg_stack;
    
    
    RValueExpression2(stream, local_symtab, global_symtab);
    int rg_a = reg_stack;

    if (peek_token(stream) != '&') {
        reg_stack = save_reg;

        return;
    }
    next_token(stream);
    if (peek_token(stream) != '&') {
        reg_stack = save_reg;
        return;
    }
    next_token(stream);
 
    reg_stack++;
    RValueExpression1(stream, local_symtab, global_symtab);
    int rg_b = reg_stack;
    printf("$%d = $%d && $%d\n", save_reg, rg_a, rg_b);
    reg_stack = save_reg;
}

// OR
void RValueExpression(char** stream, symbol_table_t** local_symtab, symbol_table_t** global_symtab) {
    int save_reg = reg_stack;
    
    
    RValueExpression1(stream, local_symtab, global_symtab);
    int rg_a = reg_stack;

    if (peek_token(stream) != '|') {
        reg_stack = save_reg;
        return;
    }
    next_token(stream);
    if (peek_token(stream) != '|') {
        reg_stack = save_reg;
        return;
    }
    next_token(stream);

    reg_stack++;
    RValueExpression(stream, local_symtab, global_symtab);
    int rg_b = reg_stack;
    printf("$%d = $%d || $%d\n", save_reg, rg_a, rg_b);
    reg_stack = save_reg;
}

/**
 * 
 * LValue precedence
 * 
 */

char lval_result[32];

void LValueExpression7(char** stream, symbol_table_t** local_symtab, symbol_table_t** global_symtab) {
    int save_reg = reg_stack;

    int tk = peek_token(stream);
    if (tk == '(') {
        next_token(stream);
        reg_stack++;
        RValueExpression(stream, local_symtab, global_symtab);
        int rg_paren = reg_stack;

        if (peek_token(stream) == ')') {
            next_token(stream); 
            
            //printf("$%d = $%d\n", save_reg, rg_paren);
            snprintf(lval_result, 32 * sizeof(char), "$%d", rg_paren);

            reg_stack = save_reg;
            return;
        }
        else {
            printf("No closing brackets found.\n");
            exit(1);
            
        }
    }
    else if (tk == TOKEN_IDENTIFIER) {
        next_token(stream);
        char* sname = malloc(sizeof(char) * get_token_length());
        int sname_len = get_token_length();
        memcpy(sname, get_token(), sizeof(char) * (get_token_length() + 1));
        sname[get_token_length()] = 0;
        
        if (peek_token(stream) == '[') {
            // array index
            next_token(stream);
            reg_stack++;
            RValueExpression(stream, local_symtab, global_symtab);
            int rg_index = reg_stack;
            if (peek_token(stream) == ']') {
                next_token(stream);
                snprintf(lval_result, 32 * sizeof(char), "%s[$%d]", sname, rg_index);
                reg_stack = save_reg;
                return;
            }
            else {
                /** ERROR */
                printf("No array closing brackets found.\n");
                exit(1);
            }
        }
        else {
            // normal variable
            // printf("$%d = %s\n", save_reg, sname);
            symbol_table_t* sym = symtab_find_sym(*local_symtab, sname, sname_len);
            if (!sym){
                sym = symtab_find_sym(*global_symtab, sname, sname_len);
            }

            if (!sym) {
                printf("Symbol not found.\n");
                exit(1);
            }
            
            snprintf(lval_result, sizeof(char) * 32, "%s", sname);
            reg_stack = save_reg;
        }

        free(sname);
        reg_stack = save_reg;
    }
    else if (tk == TOKEN_NUMBER) {
        printf("Can't assign values to a constant.\n");
        exit(1);
    }
}

void LValueExpression(char** stream, symbol_table_t** local_symtab, symbol_table_t** global_symtab) {

    int save_reg = reg_stack;
    char un_op = 0;
    if (peek_token(stream) != '*') {
        LValueExpression7(stream, local_symtab, global_symtab);
        reg_stack = save_reg;
        return;
    }

    un_op = next_token(stream);

    reg_stack++;
    RValueExpression6(stream, local_symtab, global_symtab);
    int rg_a = reg_stack;
    snprintf(lval_result, sizeof(lval_result), "*$%d", rg_a);
    reg_stack = save_reg;
}

void Expression(char** stream, symbol_table_t** local_symtab, symbol_table_t** global_symtab) {

    int save_reg = reg_stack;
    reg_stack++;
    LValueExpression(stream, local_symtab, global_symtab);

    if (peek_token(stream) != '=') {
        reg_stack = save_reg;
        return;
    }

    next_token(stream);

    reg_stack=-100;
    RValueExpression(stream, local_symtab, global_symtab);
    int rg_b = reg_stack;
    printf("%s = $%d\n", lval_result, rg_b);
    reg_stack = save_reg;
}

char* getExpressionResult() {
    return lval_result;
}
