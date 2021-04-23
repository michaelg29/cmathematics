#include "sy_util.h"
#include <string.h>
#include <stdlib.h>

/*
    bits 5 downto 0 hold the precedence
    bit  6          is 1 if the function is left associative
    bit  7          is 1 if the function is a unary function
*/
char SY_getPrecedence(SY_token *f)
{
    if (!(f->type == FUNCTION || f->type == OPERATOR))
    {
        return -1;
    }
    return f->val.funcVal.state & 0b00011111;
}

bool SY_isFunction(SY_token *f)
{
    if (!(f->type == FUNCTION || f->type == OPERATOR))
    {
        return false;
    }
    return f->val.funcVal.state & 0b00100000;
}

bool SY_isLeftAssociative(SY_token *f)
{
    if (!(f->type == FUNCTION || f->type == OPERATOR))
    {
        return false;
    }
    return f->val.funcVal.state & 0b01000000;
}

bool SY_isUnary(SY_token *f)
{
    if (!(f->type == FUNCTION || f->type == OPERATOR))
    {
        return false;
    }
    return f->val.funcVal.state & 0b10000000;
}

bool SY_charIsNumber(char c)
{
    return (c >= '0' && c <= '9') ||
           c == '.' ||
           c == '-';
}

SY_token *SY_findElement(unsigned int i, strstream *s, avl *list, int *length)
{
    if (!list)
    {
        *length = 0;
        return NULL;
    }

    // compare to current key
    unsigned int n = strlen((char *)list->key);
    int cmp = 0;
    if (i + n < s->size)
    {
        // have room to find the token
        char c = s->str[i + n];              // store first excluded character
        s->str[i + n] = '\0';                // create a temporary terminator
        cmp = strcmp(s->str + i, list->key); // compare substring to the current key
        s->str[i + n] = c;                   // return character to spot
    }
    else
    {
        // still compare to end of the string
        cmp = strcmp(s->str + i, list->key);
    }

    if (!cmp)
    {
        // found match
        *length = n;
        return list->val;
    }
    else if (cmp > 0 && list->right)
    {
        // traverse right
        return SY_findElement(i, s, list->right, length);
    }
    else if (cmp < 0 && list->left)
    {
        // traverse left
        return SY_findElement(i, s, list->left, length);
    }

    *length = 0;
    return NULL;
}

SY_token *SY_createToken(tokentype type)
{
    SY_token *ret = malloc(sizeof(SY_token));
    ret->type = type;
    return ret;
}

SY_token *SY_createTokenConstant(double value)
{
    SY_token *ret = SY_createToken(CONSTANT);
    ret->val.constVal = value;
    return ret;
}

SY_token *SY_createTokenConstantString(char *name, double value)
{
    SY_token *ret = SY_createToken(CONSTANTSTR);
    ret->val.namedConstVal.name = name;
    ret->val.namedConstVal.value = value;
    return ret;
}

SY_token *SY_createTokenUnary(char *value)
{
    SY_token *ret = SY_createToken(FUNCTION);
    ret->val.strVal = value;
    return ret;
}

SY_token *SY_createTokenBinary(char *value)
{
    SY_token *ret = SY_createToken(FUNCTION);
    ret->val.strVal = value;
    return ret;
}

SY_token *SY_createTokenOperator(char *value)
{
    SY_token *ret = SY_createToken(OPERATOR);
    ret->val.strVal = value;
    return ret;
}

SY_token *SY_createTokenLParen()
{
    SY_token *ret = SY_createToken(LPAREN);
    ret->val.strVal = "(";
    return ret;
}

SY_token *SY_createTokenRParen()
{
    SY_token *ret = SY_createToken(RPAREN);
    ret->val.strVal = ")";
    return ret;
}

void SY_freeToken(SY_token *t)
{
    if (t->type != CONSTANT)
    {
        //free(t->val.strVal);
    }
    free(t);
}

SY_token *SY_createDefaultFunction(char *name)
{
    SY_token *ret = SY_createTokenUnary(name);
    ret->type = FUNCTION;

    ret->val.funcVal.name = name;
    ret->val.funcVal.state = 0b11000000;
    ret->val.funcVal.eval.u_eval = NULL;

    return ret;
}

SY_token *SY_createDefaultUnaryFunction(char *name, UnaryFuncEval eval)
{
    return SY_createUnaryFunction(name, eval, FUNCTION, 0, true);
}

SY_token *SY_createUnaryFunction(char *name, UnaryFuncEval eval, tokentype type, char prec, bool left)
{
    SY_token *ret = SY_createTokenUnary(name);

    ret->type = type;
    ret->val.funcVal.name = name;

    char leftState = left ? (1 << 6) : 0;
    char typeState = type == FUNCTION ? (1 << 5) : 0;
    prec &= 0b00011111;
    ret->val.funcVal.state = 0b10000000 | leftState | typeState | prec;
    ret->val.funcVal.eval.u_eval = eval;

    return ret;
}

SY_token *SY_createDefaultBinaryFunction(char *name, BinaryFuncEval eval)
{
    return SY_createBinaryFunction(name, eval, FUNCTION, 0, true);
}

SY_token *SY_createBinaryFunction(char *name, BinaryFuncEval eval, tokentype type, char prec, bool left)
{
    SY_token *ret = SY_createTokenUnary(name);

    ret->type = type;
    ret->val.funcVal.name = name;

    char leftState = left ? (1 << 6) : 0;
    char typeState = type == FUNCTION ? (1 << 5) : 0;
    prec &= 0b00011111;
    ret->val.funcVal.state = 0b00000000 | leftState | typeState | prec;
    ret->val.funcVal.eval.b_eval = eval;

    return ret;
}

SY_tokenNode *SY_createNode(SY_token *t)
{
    SY_tokenNode *ret = malloc(sizeof(SY_tokenNode));
    ret->t = t;
    ret->left = NULL;
    ret->right = NULL;

    return ret;
}

void SY_freeNode(SY_tokenNode *node)
{
    if (node->left)
    {
        SY_freeNode(node->left);
    }
    if (node->right)
    {
        SY_freeNode(node->right);
    }
    free(node);
}

void SY_freeNodeDeep(SY_tokenNode *node)
{
    if (node->left)
    {
        SY_freeNodeDeep(node->left);
    }
    if (node->right)
    {
        SY_freeNodeDeep(node->right);
    }
    SY_freeToken(node->t);
    free(node);
}

double SY_evalUnary(SY_token *t, double x)
{
    return t->val.funcVal.eval.u_eval(x);
}

double SY_evalBinary(SY_token *t, double x, double y)
{
    return t->val.funcVal.eval.b_eval(x, y);
}
