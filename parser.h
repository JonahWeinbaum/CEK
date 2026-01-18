#ifndef PARSER_H
#define PARSER_H

#define MAX_VAR_LENGTH 10

typedef enum {
  VAR,
  ABS,
  APP,
  NUM,
  OPR
} AstTag;

typedef struct AstNode AstNode; 

struct AstNode {
  AstTag type;
  union {
    struct { char * name; } var;
    struct { int value; } num;
    struct { AstNode *param; AstNode* body; } abs;
    struct { AstNode* fun; AstNode *arg; } app;
    struct { char op; AstNode *lhs; AstNode *rhs; } opr;
  };
};

AstNode* parse(char*);
AstNode* parse_M(void);
AstNode* parse_V(void);
AstNode* make_num(int); 
void print_node(AstNode*, int);

#endif
