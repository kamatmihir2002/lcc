
#include "lex.h"

#include <string.h>

#define _token(tk, kw)                                                         \
  if (strcmp((tk), #kw) == 0) {                                                \
    return t(kw);                                                              \
  }

char token[128];
int tokensize;

char peek(char **stream) { return **stream; }

char advance(char **stream) {
  (*stream)++;
  return *((*stream) - 1);
}

char is_space(char ch) {
  return (ch == ' ') || (ch == '\n') || (ch == '\r') || (ch == '\t');
}

void skip_whitespace(char **stream) {
  while (is_space(peek(stream))) {
    advance(stream);
  }
}

char alpha(char ch) {
  return ((ch >= 'A') && (ch <= 'Z')) || ((ch >= 'a') && (ch <= 'z'));
}

char numeric(char ch) { return ((ch >= '0') && (ch <= '9')); }

char alnum(char ch) { return alpha(ch) || numeric(ch); }

// Returns either:
// - TOKEN_* (>=256)
// - or single ASCII character (e.g. '+', '=', '(')
int next_token(char **stream) {
  skip_whitespace(stream);

  char ch = peek(stream);

  if (ch == '\0') {
    return TOKEN_END;
  }

  tokensize = 0;

  if (alpha(ch) || ch == '_') {
    while (alnum(peek(stream)) || peek(stream) == '_') {
      token[tokensize++] = advance(stream);
    }
    token[tokensize] = 0;

    _token(token, int);
    _token(token, char);
    _token(token, if);
    _token(token, while);
    _token(token, else);
    _token(token, return );
    return TOKEN_IDENTIFIER;
  }

  // Number
  if (numeric(ch)) {
    while (numeric(peek(stream))) {
      token[tokensize++] = advance(stream);
    }
    token[tokensize] = 0;

    return TOKEN_NUMBER;
  }

  // String literal
  if (ch == '"') {
    token[tokensize++] = advance(stream); // skip opening quote

    while (peek(stream) != '"' && peek(stream) != '\0') {
      token[tokensize++] = advance(stream);
    }

    if (peek(stream) == '"') {
      token[tokensize++] = advance(stream); // closing quote
    }

    token[tokensize] = 0;

    return TOKEN_STRING;
  }

  // Anything else return character itself
  return advance(stream);
}

int peek_token(char **stream) {
  char *pre = *stream;
  int token = next_token(stream);
  *stream = pre;
  return token;
}

char *get_token() { return token; }

int get_token_length() { return tokensize; }