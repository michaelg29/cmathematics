#include "shuntingyard.h"

#include "sy_util.h"
#include "lib/sy_functions.h"
#include "lib/strstream.h"

#include <string.h>
#include <stdlib.h>
#include <math.h>

avl *functions;
avl *constants;
SY_token *subtractionToken;
SY_token *multiplicationToken;
SY_token *lparenToken;
SY_token *rparenToken;

void sy_init()
{
    // create functions tree
    functions = avl_createEmptyRoot(strkeycmp);
    functions = avl_insert(functions, "sin", SY_createDefaultUnaryFunction("sin", sin));
    functions = avl_insert(functions, "cos", SY_createDefaultUnaryFunction("cos", cos));
    functions = avl_insert(functions, "tan", SY_createDefaultUnaryFunction("tan", tan));
    functions = avl_insert(functions, "asin", SY_createDefaultUnaryFunction("asin", asin));
    functions = avl_insert(functions, "acos", SY_createDefaultUnaryFunction("acos", acos));
    functions = avl_insert(functions, "atan", SY_createDefaultUnaryFunction("atan", atan));
    functions = avl_insert(functions, "csc", SY_createDefaultUnaryFunction("csc", csc));
    functions = avl_insert(functions, "sec", SY_createDefaultUnaryFunction("sec", sec));
    functions = avl_insert(functions, "cot", SY_createDefaultUnaryFunction("cot", cot));
    functions = avl_insert(functions, "acsc", SY_createDefaultUnaryFunction("acsc", acsc));
    functions = avl_insert(functions, "asec", SY_createDefaultUnaryFunction("asec", asec));
    functions = avl_insert(functions, "acot", SY_createDefaultUnaryFunction("acot", acot));
    functions = avl_insert(functions, "abs", SY_createDefaultUnaryFunction("abs", absf));
    functions = avl_insert(functions, "ln", SY_createDefaultUnaryFunction("ln", log));
    functions = avl_insert(functions, "log2", SY_createDefaultUnaryFunction("log2", log2));
    functions = avl_insert(functions, "log10", SY_createDefaultUnaryFunction("log10", log10));
    functions = avl_insert(functions, "sqrt", SY_createDefaultUnaryFunction("sqrt", sqrt));
    functions = avl_insert(functions, "exp", SY_createDefaultUnaryFunction("exp", exp));

    subtractionToken = SY_createBinaryFunction("-", subtract, OPERATOR, 2, true);
    multiplicationToken = SY_createBinaryFunction("*", multiply, OPERATOR, 3, true);
    functions = avl_insert(functions, "+", SY_createBinaryFunction("+", add, OPERATOR, 2, true));
    functions = avl_insert(functions, "-", subtractionToken);
    functions = avl_insert(functions, "*", multiplicationToken);
    functions = avl_insert(functions, "/", SY_createBinaryFunction("/", divide, OPERATOR, 3, true));
    functions = avl_insert(functions, "^", SY_createBinaryFunction("^", pow, OPERATOR, 4, false));

    // create constants tree
    constants = avl_createEmptyRoot(strkeycmp);
    constants = avl_insert(constants, "pi", SY_createTokenConstantString("pi", 4.0 * atan(1.0), true));
    constants = avl_insert(constants, "e", SY_createTokenConstantString("e", exp(1.0), true));

    // initialize other tokens
    lparenToken = SY_createTokenLParen();
    rparenToken = SY_createTokenRParen();
}

void sy_cleanup()
{
    avl_freeDeep(functions);
    avl_freeDeep(constants);

    SY_freeToken(lparenToken);
    SY_freeToken(rparenToken);
}

dynamicarray parseTokens(char *str)
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

        SY_token *obj = NULL;
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
            if (SY_charIsNumber(c) && curType != OPERATOR)
            {
                // is number
                bool acceptDecimal = c != '.';
                curType = CONSTANT;

                unsigned int nextI = i + 1;
                while (nextI < n && SY_charIsNumber(eqn.str[nextI]))
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
                    obj = SY_createTokenConstant(-1.0);
                }
                else
                {
                    // parse from substring
                    obj = SY_createTokenConstant(
                        atof(strstream_substrLength(&eqn, i, objLength)));
                }
            }
            else
            {
                // not a numerical character

                // search through function tree
                obj = SY_findElement(i, &eqn, functions, &objLength);
                if (objLength)
                {
                    // found a function
                    curType = obj->type;
                }
                else
                {
                    // search through constant tree
                    obj = SY_findElement(i, &eqn, constants, &objLength);
                    curType = CONSTANT;
                    if (!objLength)
                    {
                        obj = SY_createTokenConstant(0.0);
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

dynamicarray RPN(char *str)
{
    dynamicarray tokens = parseTokens(str);

    unsigned int n = tokens.size;
    dynamicarray queue = dynarr_defaultAllocate();
    dynamicarray stack = dynarr_defaultAllocate();

    for (unsigned int i = 0; i < n; i++)
    {
        SY_token *t = tokens.list[i];
        tokentype type = t->type;
        SY_token *lastStack = stack.size ? stack.list[stack.size - 1] : NULL;

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
                    (SY_isFunction(lastStack) ||
                     SY_getPrecedence(lastStack) > SY_getPrecedence(t) ||
                     (SY_getPrecedence(lastStack) == SY_getPrecedence(t) &&
                      SY_isLeftAssociative(lastStack))))
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

// char *RPNstring(char *str);

SY_tokenNode *getEquationTree(dynamicarray RPN)
{
    dynamicarray stack = dynarr_defaultAllocate();

    dynarr_iterator it = dynarr_iterator_new(&RPN);
    SY_token *cur = NULL;

    while ((cur = dynarr_iterator_next(&it)))
    {
        // while another token is to be processed
        if (cur->type == CONSTANT || cur->type == CONSTANTSTR)
        {
            // create a new number node
            dynarr_addLast(&stack, SY_createNode(cur));
        }
        else
        {
            // create a function node
            SY_tokenNode *func = SY_createNode(cur);
            if (SY_isUnary(cur))
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

    SY_tokenNode *ret = dynarr_removeLast(&stack);
    dynarr_freeDeep(&stack);

    return ret;
}

double evalTree(SY_tokenNode *tree)
{
    return tree ? SY_eval(tree->t, evalTree(tree->left), evalTree(tree->right)) : 0.0;
}

double SY_eval(SY_token *t, double x, double y)
{
    if (!t)
    {
        return 0.0;
    }

    if (t->type == FUNCTION || t->type == OPERATOR)
    {
        return SY_isUnary(t) ? SY_evalUnary(t, x) : SY_evalBinary(t, x, y);
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

bool SY_registerVariable(char *name, double value)
{
    SY_token *t = avl_get(constants, name);
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
        t = SY_createTokenConstantString(name, value, false);
        constants = avl_insert(constants, name, t);
    }

    return true;
}

bool SY_clearVariable(char *name)
{
    SY_token *t = avl_get(constants, name);
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