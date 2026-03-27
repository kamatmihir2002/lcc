#pragma once
#include "symtab.h"

char* getExpressionResult();

int isType(int token);
void PtrLevel(char** stream, int* ptrlevel);
void SymArray(char** stream, char** symname, int* symlen, int* arr_len);

void Program(char** stream, symbol_table_t** global_symtab);
void Declaration(char** stream, symbol_table_t** symtab);
void FunctionDeclaration(char** stream, symbol_table_t** local_symtab, symbol_table_t** global_symtab);
void FunctionDefinition(char** stream, symbol_table_t** local_symtab, symbol_table_t** global_symtab);
void GlobalStatement(char** stream, symbol_table_t** global_symtab) ;
void Statement(char** stream, symbol_table_t** local_symtab, symbol_table_t** global_symtab) ;
void StatementBlock(char** stream, symbol_table_t** local_symtab, symbol_table_t** global_symtab) ;
void IfStatement(char** stream, symbol_table_t** local_symtab, symbol_table_t** global_symtab) ;
void WhileStatement(char** stream, symbol_table_t** local_symtab, symbol_table_t** global_symtab) ;
void Expression(char** stream, symbol_table_t** local_symtab, symbol_table_t** global_symtab);
void LValueExpression(char** stream, symbol_table_t** local_symtab, symbol_table_t** global_symtab);
void RValueExpression(char** stream, symbol_table_t** local_symtab, symbol_table_t** global_symtab, int min_bp);
