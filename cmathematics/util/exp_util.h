#include "../lib/strstream.h"
#include "../lib/dynamicarray.h"
#include "../lib/avl.h"
#include "../cmathematics.h"

#ifndef EXP_UTIL_H
#define EXP_UTIL_H

typedef char tokentype;
#define OPERATOR (tokentype)0
#define CONSTANT (tokentype)1
#define CONSTANTSTR (tokentype)2
#define FUNCTION (tokentype)3
#define LPAREN (tokentype)4
#define RPAREN (tokentype)5
#define ELSE (tokentype)6

// callback to unary function (1 argument)
typedef double (*UnaryFuncEval)(double x);

// callback to binary function (2 arguments)
typedef double (*BinaryFuncEval)(double x, double y);

typedef struct
{
    char *name; // name

    /*
        bits 4 downto 0 hold the precedence
        bit  5          is 1 if the token is a function, 0 if an operator
        bit  6          is 1 if the function is left associative
        bit  7          is 1 if the function is a unary function, 0 if binary
    */
    unsigned char state;

    // function evaluation callbacks
    union
    {
        UnaryFuncEval u_eval;
        BinaryFuncEval b_eval;
    } eval;
} exp_function;

typedef struct
{
    char *name;
    double value;
    bool restricted;
} exp_constant;

typedef struct
{
    tokentype type;
    union
    {
        double constVal;
        char *strVal;
        exp_function funcVal;
        exp_constant namedConstVal;
    } val;
} exp_token;

typedef struct exp_tokenNode
{
    exp_token *t;
    struct exp_tokenNode *left;
    struct exp_tokenNode *right;
} exp_tokenNode;

bool exp_charIsNumber(char c);
exp_token *exp_findElement(unsigned int i, strstream *s, avl *list, int *length);

exp_token *exp_createToken(tokentype type);
exp_token *exp_createTokenConstant(double value);
exp_token *exp_createTokenConstantString(char *name, double value, bool restricted);
exp_token *exp_createTokenUnary(char *value);
exp_token *exp_createTokenBinary(char *value);
exp_token *exp_createTokenOperator(char *value);
exp_token *exp_createTokenLParen();
exp_token *exp_createTokenRParen();

void exp_freeToken(exp_token *t);
void exp_freeTokenList(dynamicarray *list);

char exp_getPrecedence(exp_token *f);
bool exp_isFunction(exp_token *f);
bool exp_isLeftAssociative(exp_token *f);
bool exp_isUnary(exp_token *f);

exp_token *exp_createDefaultFunction(char *name);
exp_token *exp_createDefaultUnaryFunction(char *name, UnaryFuncEval eval);
exp_token *exp_createUnaryFunction(char *name, UnaryFuncEval eval, tokentype type, char prec, bool left);
exp_token *exp_createDefaultBinaryFunction(char *name, BinaryFuncEval eval);
exp_token *exp_createBinaryFunction(char *name, BinaryFuncEval eval, tokentype type, char prec, bool left);

exp_tokenNode *exp_createNode(exp_token *t);
void exp_freeNode(exp_tokenNode *node);
void exp_freeNodeDeep(exp_tokenNode *node);

double exp_evalUnary(exp_token *t, double x);
double exp_evalBinary(exp_token *t, double x, double y);
double exp_eval(exp_token *t, double x, double y);

#endif // EXP_UTIL_H