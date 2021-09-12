#include "expressions.h"

#include "exp_util.h"
#include "../lib/functions.h"
#include "../lib/strstream.h"

#include <string.h>
#include <stdlib.h>
#include <math.h>

avl *functions;
avl *constants;
exp_token *subtractionToken;
exp_token *multiplicationToken;
exp_token *lparenToken;
exp_token *rparenToken;

void exp_init()
{
    // create functions tree
    functions = avl_createEmptyRoot(strkeycmp);
    functions = avl_insert(functions, "sin", exp_createDefaultUnaryFunction("sin", sin));
    functions = avl_insert(functions, "cos", exp_createDefaultUnaryFunction("cos", cos));
    functions = avl_insert(functions, "tan", exp_createDefaultUnaryFunction("tan", tan));
    functions = avl_insert(functions, "asin", exp_createDefaultUnaryFunction("asin", asin));
    functions = avl_insert(functions, "acos", exp_createDefaultUnaryFunction("acos", acos));
    functions = avl_insert(functions, "atan", exp_createDefaultUnaryFunction("atan", atan));
    functions = avl_insert(functions, "csc", exp_createDefaultUnaryFunction("csc", csc));
    functions = avl_insert(functions, "sec", exp_createDefaultUnaryFunction("sec", sec));
    functions = avl_insert(functions, "cot", exp_createDefaultUnaryFunction("cot", cot));
    functions = avl_insert(functions, "acsc", exp_createDefaultUnaryFunction("acsc", acsc));
    functions = avl_insert(functions, "asec", exp_createDefaultUnaryFunction("asec", asec));
    functions = avl_insert(functions, "acot", exp_createDefaultUnaryFunction("acot", acot));
    functions = avl_insert(functions, "abs", exp_createDefaultUnaryFunction("abs", absf));
    functions = avl_insert(functions, "ln", exp_createDefaultUnaryFunction("ln", log));
    functions = avl_insert(functions, "log2", exp_createDefaultUnaryFunction("log2", log2));
    functions = avl_insert(functions, "log10", exp_createDefaultUnaryFunction("log10", log10));
    functions = avl_insert(functions, "sqrt", exp_createDefaultUnaryFunction("sqrt", sqrt));
    functions = avl_insert(functions, "exp", exp_createDefaultUnaryFunction("exp", exp));

    subtractionToken = exp_createBinaryFunction("-", subtract, OPERATOR, 2, true);
    multiplicationToken = exp_createBinaryFunction("*", multiply, OPERATOR, 3, true);
    functions = avl_insert(functions, "+", exp_createBinaryFunction("+", add, OPERATOR, 2, true));
    functions = avl_insert(functions, "-", subtractionToken);
    functions = avl_insert(functions, "*", multiplicationToken);
    functions = avl_insert(functions, "/", exp_createBinaryFunction("/", divide, OPERATOR, 3, true));
    functions = avl_insert(functions, "^", exp_createBinaryFunction("^", pow, OPERATOR, 4, false));

    // create constants tree
    constants = avl_createEmptyRoot(strkeycmp);
    constants = avl_insert(constants, "pi", exp_createTokenConstantString("pi", 4.0 * atan(1.0), true));
    constants = avl_insert(constants, "e", exp_createTokenConstantString("e", exp(1.0), true));

    // initialize other tokens
    lparenToken = exp_createTokenLParen();
    rparenToken = exp_createTokenRParen();
}

void exp_cleanup()
{
    avl_freeDeep(functions);
    avl_freeDeep(constants);

    exp_freeToken(lparenToken);
    exp_freeToken(rparenToken);
}

dynamicarray exp_parseTokens(char *str)
{
    unsigned int n = strlen(str);
    strstream eqn = strstream_alloc(n);
    strstream_concat(&eqn, "%s", str);

    dynamicarray ret = dynarr_defaultAllocate();

    tokentype prevType = ELSE;
    tokentype curType = ELSE;
    unsigned int objLength = 0;
    for (unsigned int i = 0; i < n; i += objLength)
    {
        char c = eqn.str[i];

        exp_token *obj = NULL;
        objLength = 1;

        if (c == ' ' || c == '\t' || c == '\n' || c == ',')
        {
            continue;
        }
        else if (c == '(')
        {
            curType = LPAREN;
            obj = lparenToken;
        }
        else if (c == ')')
        {
            curType = RPAREN;
            obj = rparenToken;
        }
        else
        {
            if (c == '-')
            {
                if (prevType == CONSTANT || prevType == RPAREN)
                {
                    // subtraction sign
                    curType = OPERATOR;
                    obj = subtractionToken;
                }
            }
            if (exp_charIsNumber(c) && curType != OPERATOR)
            {
                // is number
                bool acceptDecimal = c != '.';
                curType = CONSTANT;

                unsigned int nextI = i + 1;
                while (nextI < n && exp_charIsNumber(eqn.str[nextI]))
                {
                    if (eqn.str[nextI] == '-')
                    {
                        break;
                    }
                    else if (eqn.str[nextI] == '.')
                    {
                        if (!acceptDecimal)
                        {
                            break;
                        }
                        acceptDecimal = true;
                    }
                    nextI++;
                }
                objLength = nextI - i;

                if (objLength == 1 && c == '-')
                {
                    // implicitly insert a -1
                    obj = exp_createTokenConstant(-1.0);
                }
                else
                {
                    // parse from substring
                    obj = exp_createTokenConstant(
                        atof(strstream_substrLength(&eqn, i, objLength)));
                }
            }
            else
            {
                // not a numerical character

                // search through function tree
                obj = exp_findElement(i, &eqn, functions, &objLength);
                if (objLength)
                {
                    // found a function
                    curType = obj->type;
                }
                else
                {
                    // search through constant tree
                    obj = exp_findElement(i, &eqn, constants, &objLength);
                    curType = CONSTANT;
                    if (!objLength)
                    {
                        obj = exp_createTokenConstant(0.0);
                        objLength = 1;
                    }
                }

                if ((curType == FUNCTION || curType == CONSTANT) &&
                    !(prevType == ELSE || prevType == OPERATOR || prevType == LPAREN))
                {
                    // implicitly add multiplication sign
                    dynarr_addLast(&ret, multiplicationToken);
                }
            }
        }

        prevType = curType;
        curType = ELSE;

        dynarr_addLast(&ret, obj);
    }

    return ret;
}

dynamicarray exp_RPN(char *str)
{
    dynamicarray tokens = exp_parseTokens(str);

    unsigned int n = tokens.size;
    dynamicarray queue = dynarr_defaultAllocate();
    dynamicarray stack = dynarr_defaultAllocate();

    for (unsigned int i = 0; i < n; i++)
    {
        exp_token *t = tokens.list[i];
        tokentype type = t->type;
        exp_token *lastStack = stack.size ? stack.list[stack.size - 1] : NULL;

        switch (type)
        {
        case CONSTANT:
        case CONSTANTSTR:
            dynarr_addLast(&queue, t);
            break;
        case FUNCTION:
            dynarr_addLast(&stack, t);
            break;
        case OPERATOR:
            if (stack.size)
            {
                while (
                    lastStack->type != LPAREN &&
                    (exp_isFunction(lastStack) ||
                     exp_getPrecedence(lastStack) > exp_getPrecedence(t) ||
                     (exp_getPrecedence(lastStack) == exp_getPrecedence(t) &&
                      exp_isLeftAssociative(lastStack))))
                {
                    // pop from the stack to the queue
                    dynarr_addLast(&queue, dynarr_removeLast(&stack));
                    if (!stack.size)
                    {
                        break;
                    }
                    lastStack = stack.list[stack.size - 1];
                }
            }
            dynarr_addLast(&stack, t);
            break;
        case LPAREN:
            dynarr_addLast(&stack, t);
            break;
        case RPAREN:
            if (stack.size)
            {
                while (lastStack->type != LPAREN)
                {
                    // pop from the stack to the queue
                    dynarr_addLast(&queue, dynarr_removeLast(&stack));
                    if (!stack.size)
                    {
                        break;
                    }
                    lastStack = stack.list[stack.size - 1];
                }
                dynarr_removeLast(&stack);
            }
            break;
        default:
            dynarr_free(&stack);
            dynarr_free(&tokens);
            return queue;
        };
    }

    while (stack.size)
    {
        // pop from the stack to the queue
        dynarr_addLast(&queue, dynarr_removeLast(&stack));
    }

    dynarr_free(&stack);
    dynarr_free(&tokens);
    return queue;
}

// char *exp_RPNstring(char *str);

exp_tokenNode *exp_getEquationTree(dynamicarray RPN)
{
    dynamicarray stack = dynarr_defaultAllocate();

    dynarr_iterator it = dynarr_iterator_new(&RPN);
    exp_token *cur = NULL;

    while ((cur = dynarr_iterator_next(&it)))
    {
        // while another token is to be processed
        if (cur->type == CONSTANT || cur->type == CONSTANTSTR)
        {
            // create a new number node
            dynarr_addLast(&stack, exp_createNode(cur));
        }
        else
        {
            // create a function node
            exp_tokenNode *func = exp_createNode(cur);
            if (exp_isUnary(cur))
            {
                // unary function, insert one child
                if (stack.size)
                {
                    func->left = dynarr_removeLast(&stack);
                }
            }
            else
            {
                // binary function, insert two children
                // right child is second argument
                if (stack.size)
                {
                    func->right = dynarr_removeLast(&stack);
                }

                // left child is first argument
                if (stack.size)
                {
                    func->left = dynarr_removeLast(&stack);
                }
            }
            dynarr_addLast(&stack, func);
        }
    }

    exp_tokenNode *ret = dynarr_removeLast(&stack);
    dynarr_freeDeep(&stack);

    return ret;
}

double exp_evalTree(exp_tokenNode *tree)
{
    return tree ? exp_eval(tree->t, exp_evalTree(tree->left), exp_evalTree(tree->right)) : 0.0;
}

double exp_eval(exp_token *t, double x, double y)
{
    if (!t)
    {
        return 0.0;
    }

    if (t->type == FUNCTION || t->type == OPERATOR)
    {
        return exp_isUnary(t) ? exp_evalUnary(t, x) : exp_evalBinary(t, x, y);
    }
    else if (t->type == CONSTANT)
    {
        return t->val.constVal;
    }
    else if (t->type == CONSTANTSTR)
    {
        return t->val.namedConstVal.value;
    }

    return 0.0;
}

bool exp_registerVariable(char *name, double value)
{
    exp_token *t = avl_get(constants, name);
    if (t)
    {
        if (t->val.namedConstVal.restricted)
        {
            // cannot modify restricted value
            return false;
        }

        // replace existing value
        t->val.namedConstVal.value = value;
    }
    else
    {
        // create a token
        t = exp_createTokenConstantString(name, value, false);
        constants = avl_insert(constants, name, t);
    }

    return true;
}

bool exp_clearVariable(char *name)
{
    exp_token *t = avl_get(constants, name);
    if (t)
    {
        if (t->val.namedConstVal.restricted)
        {
            // cannot remove restricted variable
            return false;
        }

        avl_remove(constants, name);

        return true;
    }
    else
    {
        return false;
    }
}