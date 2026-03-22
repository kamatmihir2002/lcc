#pragma once

#define t(kw) TOKEN_ ## kw

typedef enum {
    TOKEN_IDENTIFIER = 256, // start above ASCII range
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_END,
    t(char),
    t(int),
    t(if),
    t(while),
    t(else),
    t(return)
} TokenType;

int next_token(char** stream);

int peek_token(char** stream);

char* get_token();

int get_token_length();