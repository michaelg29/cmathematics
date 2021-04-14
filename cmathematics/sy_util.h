#include "lib/strstream.h"
#include "lib/avl.h"
#include "cmathematics.h"

#ifndef SY_UTIL_H
#define SY_UTIL_H

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
} SY_function;

typedef struct
{
    char *name;
    double value;
} SY_constant;

typedef struct
{
    tokentype type;
    union
    {
        double constVal;
        char *strVal;
        SY_function funcVal;
        SY_constant namedConstVal;
    } val;
} SY_token;

typedef struct SY_tokenNode
{
    SY_token *t;
    struct SY_tokenNode *left;
    struct SY_tokenNode *right;
} SY_tokenNode;

bool SY_charIsNumber(char c);
SY_token *SY_findElement(int i, strstream *s, avl *list, int *length);

SY_token *SY_createToken(tokentype type);
SY_token *SY_createTokenConstant(double value);
SY_token *SY_createTokenConstantString(char *name, double value);
SY_token *SY_createTokenUnary(char *value);
SY_token *SY_createTokenBinary(char *value);
SY_token *SY_createTokenOperator(char *value);
SY_token *SY_createTokenLParen();
SY_token *SY_createTokenRParen();

void SY_freeToken(SY_token *t);

char SY_getPrecedence(SY_token *f);
bool SY_isFunction(SY_token *f);
bool SY_isLeftAssociative(SY_token *f);
bool SY_isUnary(SY_token *f);

SY_token *SY_createDefaultFunction(char *name);
SY_token *SY_createDefaultUnaryFunction(char *name, UnaryFuncEval eval);
SY_token *SY_createUnaryFunction(char *name, UnaryFuncEval eval, tokentype type, char prec, bool left);
SY_token *SY_createDefaultBinaryFunction(char *name, BinaryFuncEval eval);
SY_token *SY_createBinaryFunction(char *name, BinaryFuncEval eval, tokentype type, char prec, bool left);

SY_tokenNode *SY_createNode(SY_token *t);
void SY_freeNode(SY_tokenNode *node);
void SY_freeNodeDeep(SY_tokenNode *node);

double SY_evalUnary(SY_token *t, double x);
double SY_evalBinary(SY_token *t, double x, double y);
double SY_eval(SY_token *t, double x, double y);

#endif