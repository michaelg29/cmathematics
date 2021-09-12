#include "exp_util.h"
#include <string.h>
#include <stdlib.h>

/*
    bits 5 downto 0 hold the precedence
    bit  6          is 1 if the function is left associative
    bit  7          is 1 if the function is a unary function
*/
char exp_getPrecedence(exp_token *f)
{
    if (!(f->type == FUNCTION || f->type == OPERATOR))
    {
        return -1;
    }
    return f->val.funcVal.state & 0b00011111;
}

bool exp_isFunction(exp_token *f)
{
    if (!(f->type == FUNCTION || f->type == OPERATOR))
    {
        return false;
    }
    return f->val.funcVal.state & 0b00100000;
}

bool exp_isLeftAssociative(exp_token *f)
{
    if (!(f->type == FUNCTION || f->type == OPERATOR))
    {
        return false;
    }
    return f->val.funcVal.state & 0b01000000;
}

bool exp_isUnary(exp_token *f)
{
    if (!(f->type == FUNCTION || f->type == OPERATOR))
    {
        return false;
    }
    return f->val.funcVal.state & 0b10000000;
}

bool exp_charIsNumber(char c)
{
    return (c >= '0' && c <= '9') ||
           c == '.' ||
           c == '-';
}

exp_token *exp_findElement(unsigned int i, strstream *s, avl *list, int *length)
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
        return exp_findElement(i, s, list->right, length);
    }
    else if (cmp < 0 && list->left)
    {
        // traverse left
        return exp_findElement(i, s, list->left, length);
    }

    *length = 0;
    return NULL;
}

exp_token *exp_createToken(tokentype type)
{
    exp_token *ret = malloc(sizeof(exp_token));
    ret->type = type;
    return ret;
}

exp_token *exp_createTokenConstant(double value)
{
    exp_token *ret = exp_createToken(CONSTANT);
    ret->val.constVal = value;
    return ret;
}

exp_token *exp_createTokenConstantString(char *name, double value, bool restricted)
{
    exp_token *ret = exp_createToken(CONSTANTSTR);
    ret->val.namedConstVal.name = name;
    ret->val.namedConstVal.value = value;
    ret->val.namedConstVal.restricted = restricted;
    return ret;
}

exp_token *exp_createTokenUnary(char *value)
{
    exp_token *ret = exp_createToken(FUNCTION);
    ret->val.strVal = value;
    return ret;
}

exp_token *exp_createTokenBinary(char *value)
{
    exp_token *ret = exp_createToken(FUNCTION);
    ret->val.strVal = value;
    return ret;
}

exp_token *exp_createTokenOperator(char *value)
{
    exp_token *ret = exp_createToken(OPERATOR);
    ret->val.strVal = value;
    return ret;
}

exp_token *exp_createTokenLParen()
{
    exp_token *ret = exp_createToken(LPAREN);
    ret->val.strVal = "(";
    return ret;
}

exp_token *exp_createTokenRParen()
{
    exp_token *ret = exp_createToken(RPAREN);
    ret->val.strVal = ")";
    return ret;
}

void exp_freeToken(exp_token *t)
{
    if (t->type != CONSTANT)
    {
        //free(t->val.strVal);
    }
    free(t);
}

void exp_freeTokenList(dynamicarray *list)
{
    dynarr_iterator it = dynarr_iterator_new(list);
    exp_token *cur = NULL;

    while ((cur = dynarr_iterator_next(&it)))
    {
        if (cur->type == CONSTANT)
        {
            free(cur);
        }
    }

    dynarr_free(list);
}

exp_token *exp_createDefaultFunction(char *name)
{
    exp_token *ret = exp_createTokenUnary(name);
    ret->type = FUNCTION;

    ret->val.funcVal.name = name;
    ret->val.funcVal.state = 0b11000000;
    ret->val.funcVal.eval.u_eval = NULL;

    return ret;
}

exp_token *exp_createDefaultUnaryFunction(char *name, UnaryFuncEval eval)
{
    return exp_createUnaryFunction(name, eval, FUNCTION, 0, true);
}

exp_token *exp_createUnaryFunction(char *name, UnaryFuncEval eval, tokentype type, char prec, bool left)
{
    exp_token *ret = exp_createTokenUnary(name);

    ret->type = type;
    ret->val.funcVal.name = name;

    char leftState = left ? (1 << 6) : 0;
    char typeState = type == FUNCTION ? (1 << 5) : 0;
    prec &= 0b00011111;
    ret->val.funcVal.state = 0b10000000 | leftState | typeState | prec;
    ret->val.funcVal.eval.u_eval = eval;

    return ret;
}

exp_token *exp_createDefaultBinaryFunction(char *name, BinaryFuncEval eval)
{
    return exp_createBinaryFunction(name, eval, FUNCTION, 0, true);
}

exp_token *exp_createBinaryFunction(char *name, BinaryFuncEval eval, tokentype type, char prec, bool left)
{
    exp_token *ret = exp_createTokenUnary(name);

    ret->type = type;
    ret->val.funcVal.name = name;

    char leftState = left ? (1 << 6) : 0;
    char typeState = type == FUNCTION ? (1 << 5) : 0;
    prec &= 0b00011111;
    ret->val.funcVal.state = 0b00000000 | leftState | typeState | prec;
    ret->val.funcVal.eval.b_eval = eval;

    return ret;
}

exp_tokenNode *exp_createNode(exp_token *t)
{
    exp_tokenNode *ret = malloc(sizeof(exp_tokenNode));
    ret->t = t;
    ret->left = NULL;
    ret->right = NULL;

    return ret;
}

void exp_freeNode(exp_tokenNode *node)
{
    if (node->left)
    {
        exp_freeNode(node->left);
    }
    if (node->right)
    {
        exp_freeNode(node->right);
    }
    free(node);
}

void exp_freeNodeDeep(exp_tokenNode *node)
{
    if (node->left)
    {
        exp_freeNodeDeep(node->left);
    }
    if (node->right)
    {
        exp_freeNodeDeep(node->right);
    }
    exp_freeToken(node->t);
    free(node);
}

double exp_evalUnary(exp_token *t, double x)
{
    return t->val.funcVal.eval.u_eval(x);
}

double exp_evalBinary(exp_token *t, double x, double y)
{
    return t->val.funcVal.eval.b_eval(x, y);
}
