#ifndef CEK_H
#define CEK_H

#include <stdlib.h>
#include <string.h>
#include "parser.h"

typedef struct Sub Sub;
typedef struct Env Env;
typedef struct Closure Closure;
typedef struct Control Control;
typedef struct Kont Kont;
typedef struct State State;

struct Sub {
  AstNode* v;
  Closure* e; 
};

Sub* make_sub(AstNode* v, Closure* e) {
  Sub* sub = malloc(sizeof(Sub));
  sub->v = v;
  sub->e = e;
  return sub;
}

struct Env {
  Sub* sub;
  Env *next;
};

Env* add_sub(Env* env, Sub* sub) {
  Env* ep = malloc(sizeof(Env));
  ep->next = env;
  ep->sub = sub;
  return ep;
}

struct Closure {
  AstNode* x;
  Env* e; 
};

Closure* make_closure(AstNode* x, Env* e) {
  Closure* cp = malloc(sizeof(Closure));
  cp->x = x;
  cp->e = e;
  return cp;
}

typedef enum {
  MT,
  FN,
  AR, 
  OP
} KontTag; 

struct Kont {
  KontTag type; 
  union {
    struct { Closure* v; Kont* k; } fn;
    struct { Closure* c; Kont* k; } ar;
    struct { char op; Closure* lhs; Closure* rhs; Kont* k; } op;
  };
};

Kont* make_mt() {
  Kont* kp = malloc(sizeof(Kont));
  kp->type = MT;
  return kp;
}

Kont* make_fn(Closure* v, Kont* k) {
  Kont* kp = malloc(sizeof(Kont));
  kp->type = FN;
  kp->fn.v = v;
  kp->fn.k = k;
  return kp;
}

Kont* make_ar(Closure* c, Kont* k) {
  Kont* kp = malloc(sizeof(Kont));
  kp->type = AR;
  kp->ar.c = c;
  kp->ar.k = k;
  return kp;
}

Kont* make_op(char op, Closure* lhs, Closure* rhs, Kont* k) {
  Kont* kp = malloc(sizeof(Kont));
  kp->type = OP;
  kp->op.op = op;
  kp->op.lhs = lhs;
  kp->op.rhs = rhs;  
  kp->op.k = k;
  return kp;
}

struct State {
  Closure *c;
  Kont *k; 
};

Closure* sub(AstNode* v, Env* e) {
  while(e != NULL) {
    if (strcmp(e->sub->v->var.name, v->var.name)) {
      return e->sub->e;
    }
    
    e = e->next; 
  }

  return NULL;
}

State* step(State* s) {
  State* sp = malloc(sizeof(State));
  AstNode* x = s->c->x;
  Env* e = s->c->e;
  Kont* k = s->k;

  if (x->type == APP) {
    Closure* cfun = make_closure(x->app.fun, e);
    Closure* carg = make_closure(x->app.arg, e);
    Kont* kp = make_ar(carg, k);
    sp->c = cfun;
    sp->k = kp;
    return sp;
  }

  if (x->type == OPR) {
       char op = x->opr.op;
       Closure* empty = NULL;
       Closure* lhs = make_closure(x->opr.lhs, e);
       Closure* rhs = make_closure(x->opr.rhs, e);
       Kont* kp = make_op(op, empty, rhs, k);
       sp->c = lhs;
       sp->k = kp;
       return sp;
  }

  if (x->type == VAR) {
    Closure* cp = sub(x, e);
    sp->c = cp;
    sp->k = k;
    return sp;
  }

  if (k->type == OP) {
    char op = k->op.op;
    Closure* empty = NULL;
    Closure* lhs = k->op.lhs;
    Closure* rhs = k->op.rhs;
    
    // Empty lhs
    if (lhs == NULL) {
      sp->c = rhs;            
      sp->k = make_op(op, s->c, empty, k);
    // Otherwise we may apply delta
    } else {
      sp->c = make_closure(make_num(lhs->x->num.value + s->c->x->num.value), e);
      sp->k = k->op.k;
    }
    return sp;
  }
  // ((\f.\x.(f x) \y.(+ y y)) (+ 1 20))
  
  if (k->type == AR) {
    sp->c = k->ar.c;
    sp->k = make_fn(s->c, k->ar.k);
    return sp;
  }

  if (k->type == FN) {
    Sub *sub = make_sub(k->fn.v->x->abs.param, s->c);
    Env* ep = add_sub(e, sub);
    sp->c = make_closure(k->fn.v->x->abs.body, ep);
    sp->k = k->fn.k;
    return sp;
  }

  return s;

    /*    char op = x->opr.op; */
    /*    Closure* empty = make_closure(NULL, e);        */
    /*    Closure* lhs = make_closure(x->opr.lhs, e); */
    /*    Closure* rhs = make_closure(x->opr.rhs, e); */
    /*    Kont* kp = make_op(op, lhs, empty, k); */
    /*    sp->c = rhs; */
    /*    sp->k = kp; */
    /*    return sp; */
    /* } */
    /* char op = x->opr.op; */
    /* int v1 = x->opr.lhs->num.value; */
    /* int v2 = x->opr.rhs->num.value; */
    /* AstNode* deltav1v2 = make_num(v1+v2); */
    /* Closure* res = make_closure(deltav1v2, NULL); */
    /* sp->c = res; */
    /* sp->k = k; */
    /* return sp; */  
}

#endif
