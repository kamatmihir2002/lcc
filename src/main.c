#include "ucc.h"

#include <stdio.h>
#include <stdlib.h>

char *read_stdin(int *len) {
  fseek(stdin, 0, SEEK_END);
  *len = ftell(stdin);
  fseek(stdin, 0, SEEK_SET);
  *len -= ftell(stdin);
  char *buf = malloc(sizeof(char) * (*len + 1));
  fread(buf, sizeof(char), *len, stdin);
  buf[*len] = 0;
  return buf;
}

int main(int argc, char **argv) {
  symbol_table_t *gt = NULL;
  symbol_table_t *lt = NULL;
  char *s;
  int len = 0;
  if (argc == 1) {

    s = read_stdin(&len);
  } else if (argc == 2) {
    s = argv[1];
  }

  Program(&s, &gt);
}
