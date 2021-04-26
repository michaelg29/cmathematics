#include "lib/dynamicarray.h"
#include "lib/strstream.h"
#include "lib/avl.h"

#include "sy_util.h"

#ifndef SHUNTINGYARD_H
#define SHUNTINGYARD_H

void sy_init();
void sy_cleanup();

dynamicarray parseTokens(char *str);
dynamicarray RPN(char *str);
char *RPNstring(char *str);
SY_tokenNode *getEquationTree(dynamicarray RPN);
double evalTree(SY_tokenNode *tree);

double SY_eval(SY_token *t, double x, double y);

bool SY_registerVariable(char *name, double value);
bool SY_clearVariable(char *name);

#endif