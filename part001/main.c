#include <stdio.h>
#include <stdlib.h>

#include <editline/history.h>
#include <editline/readline.h>

#include "../mpc/mpc.h"

#define PARSERS \
  " \
  number   : /-?[0-9]+/ ; \
  operator : '+' | '-' | '*' |'/' ; \
  expr     : <number> | '(' <operator> <expr>+ ')' ; \
  mylisp   : /^/ '(' <operator> <expr>+ ')' /$/ ; \
"

long eval_op(long x, char *op, long y) {
  if (strcmp(op, "+") == 0) return x + y;
  if (strcmp(op, "-") == 0) return x - y;
  if (strcmp(op, "*") == 0) return x * y;
  if (strcmp(op, "/") == 0) return x / y;
  return 0;
}

long eval(mpc_ast_t *t) {
  if (strstr(t->tag, "number")) return atoi(t->contents);
  int n = 1;
  if (strstr(t->children[n]->contents, "(")) n = 2;

  char *op = t->children[n]->contents;
  long x   = eval(t->children[n + 1]);

  for (int i = n + 2; strstr(t->children[i]->tag, "expr"); i++) {
    x = eval_op(x, op, eval(t->children[i]));
  }
  return x;
}

int main(int argc, char **argv) {
  mpc_parser_t *Number   = mpc_new("number");
  mpc_parser_t *Operator = mpc_new("operator");
  mpc_parser_t *Expr     = mpc_new("expr");
  mpc_parser_t *MyLisp   = mpc_new("mylisp");

  mpca_lang(MPCA_LANG_DEFAULT, PARSERS, Number, Operator, Expr, MyLisp);
  puts("MyLisp Version 0.0.1");
  puts("Press Ctr+c to Exit\n");

  char *input;
  mpc_result_t r;

  while ((input = readline("MyLisp> ")) != NULL) {
    add_history(input);
    if (mpc_parse("<stdin>", input, MyLisp, &r)) {
      printf("%li\n", eval(r.output));
      mpc_ast_delete(r.output);
    } else {
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }
    free(input);
  }
  mpc_cleanup(4, Number, Operator, Expr, MyLisp);
  return 0;
}
