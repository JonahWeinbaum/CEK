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

    State* s = malloc(sizeof(State));
    s->c = make_closure(root, NULL);
    s->k = make_mt();
    printf("=====================\n");        
    print_state(s);
    printf("\n");
    printf("=====================\n");    
    State* s_next = step(s);
    int is_stop = 0; 
    while (!is_stop) {
      s_next = step(s_next);
      print_state(s_next);
      printf("\n");
      printf("=====================\n");
      is_stop = (s_next->k->type == MT) && is_value(s_next->c->x);
    }
  }
  return 0;
}
