#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include "parser.h"
#include "cek.h"

#define BUFF_SIZE 100

int main(void) {
  char buff[BUFF_SIZE] = {0}; 
  while (true) { 
    printf("-> ");
    fflush(stdout); 
    read(0, buff, BUFF_SIZE);

    AstNode* root = parse(buff);
    print_node(root, 0);
    printf("=====================\n");
    State* s = malloc(sizeof(State));
    s->c = make_closure(root, NULL);
    s->k = make_mt();
    State* s_next = step(s);
    int i = 0; 
    while (i < 10) {
       print_node(s_next->c->x, 0);
       printf("=====================\n");
       s_next = step(s_next);
       i++;
    }
  }
  return 0;
}
