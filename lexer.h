#ifndef LEXER_H
#define LEXER_H

// Global position in string
int pos = 0;
char* input = NULL;

char peek(void) {
  return input[pos];
}

char next(void) {
  return input[pos++];
}

void skip_ws(void) {
  while(isspace(peek())) {
    pos++;
  }
}

int match(char c) {
  skip_ws();

  if (peek() == c) {
    next();
    return 1;
  }
  return 0;
}

#endif
