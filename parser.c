 #include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "lexer.h"
#include "parser.h"

/* struct AstNode { */
/*   Tag type; */
/*   union { */
/*     struct { char * name; } var; */
/*     struct { int value; } num; */
/*     struct { char *param; AstNode* body; } abs; */
/*     struct { AstNode* fun; AstNode *arg; } app; */
/*     struct { char *op; AstNode *lhs; AstNode *rhs; } opr; */
/*   }; */
/* }; */

AstNode* make_var(char *name) {
  AstNode* node = malloc(sizeof(AstNode));
  node->type = VAR;
  node->var.name = malloc(strlen(name));
  strcpy(node->var.name, name);
  return node;
}

AstNode* make_num(int num) {
  AstNode* node = malloc(sizeof(AstNode));
  node->type = NUM;
  node->num.value = num;
  return node;
}

AstNode* make_abs(char* param, AstNode* body) {
  AstNode* node = malloc(sizeof(AstNode));
  node->type = ABS;
  node->abs.param = make_var(param);
  node->abs.body = body;
  return node;
}

AstNode* make_app(AstNode* fun, AstNode* arg) {
  AstNode* node = malloc(sizeof(AstNode));
  node->type = APP;
  node->app.fun = fun;
  node->app.arg = arg;
  return node;
}

AstNode* make_opr(char op, AstNode* lhs, AstNode* rhs) {
  AstNode* node = malloc(sizeof(AstNode));
  node->type = OPR;
  node->opr.op = op;
  node->opr.lhs = lhs;
  node->opr.rhs = rhs;
  return node;
}

int is_value(AstNode* node) {
  switch (node->type) {
  case NUM:
  case ABS:
    return 1;

  default:
      return 0;
  }
}

AstNode* parse(char *in) {
  // Reset global state 
  input = in;
  pos = 0;
  
  AstNode* node = parse_M();
  
  return node;
}

AstNode* parse_V(void) {
  skip_ws();

  AstNode* ret = NULL;

  // Abstraction
  if (match('\\')) {
    char param[MAX_VAR_LENGTH];

    int i = 0;
    for (; isalpha(peek()); i++) {
      param[i] = next();
    }
    param[i] = 0; // NULL terminate

    match('.');
    
    AstNode* body = parse_M();

    match('(');
    
    return make_abs(param, body);
  }

  // Variable 
  if (isalpha(peek())) {
    char name[MAX_VAR_LENGTH];

    int i = 0;
    for (; isalpha(peek()); i++) {
      name[i] = next();
    }
    name[i] = 0; // NULL terminate

    return make_var(name);
  }

  // Number
  if (isdigit(peek())) {
    char number[MAX_VAR_LENGTH];

    int i = 0;
    for (; isdigit(peek()); i++) {
            number[i] = next();
    }
    number[i] = 0; // NULL terminate
    int value = atoi(number);
    return make_num(value);
  }
}

AstNode* parse_M(void) {
  skip_ws();

  if (match('(')) {

    // Operator
    if (peek() == '+') {
      char op = next();
      AstNode* lhs = parse_M();
      AstNode* rhs = parse_M();
      match(')');

      return make_opr(op, lhs, rhs);

    // Application      
    } else {
      AstNode* fun = parse_M();
      AstNode* arg = parse_M();
      match(')');

      return make_app(fun, arg);
    }    
  }

  return parse_V();
}

void print_node(AstNode* node) {
  switch (node->type) {
  case VAR:
    printf("%s", node->var.name); 
    break;
  case NUM:
    printf("%d", node->num.value); 
    break;
  case ABS:
    printf("\\");
    print_node(node->abs.param);
    printf(".");
    print_node(node->abs.body);
    break;
  case OPR:
    printf("(");    	   
    print_node(node->opr.lhs);	   
    printf(" %c ", node->opr.op);
    print_node(node->opr.rhs);
    printf(")");    
    break;
  case APP:
    printf("(");
    print_node(node->app.fun);
    printf(" ");
    print_node(node->app.arg);
    printf(")"); 
    break;
  default:
    printf("Not yet implemented\n");
  }
}
