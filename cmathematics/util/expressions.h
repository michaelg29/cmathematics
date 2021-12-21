#include "../lib/dynamicarray.h"
#include "../lib/strstream.h"
#include "../lib/avl.h"

#include "exp_util.h"

#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

void exp_init();
void exp_cleanup();

dynamicarray exp_parseTokens(char *str);
dynamicarray exp_RPN(char *str);
char *exp_RPNstring(char *str);
exp_tokenNode *exp_getEquationTree(dynamicarray RPN);
double exp_evalTree(exp_tokenNode *tree);

double exp_eval(exp_token *t, double x, double y);

bool exp_registerVariable(char *name, double value);
bool exp_clearVariable(char *name);

#endif // EXPRESSIONS_H