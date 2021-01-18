#include "bigint.h"
#include <stdlib.h>
#include <string.h>

const bigint BIGINT_ZERO = {true, 0, 0, NULL};

/**
 * allocate memory for the integer
 * @param capacity the initial size of the array
 * @return the integer
 */
bigint allocateBigint(unsigned int capacity)
{
    bigint ret;
    ret.capacity = capacity;
    ret.noDigits = 0;
    ret.sign = true;
    // allocate array
    ret.digits = malloc(capacity * sizeof(int));
    return ret;
}

/**
 * free the memory of a big integer
 * @param i the integer
 */
void freeBigint(bigint *i)
{
    i->capacity = 0;
    i->noDigits = 0;
    i->sign = 1;

    free(i->digits);
}

/**
 * convert string to a big integer by parsing each digit
 * 
 * PROCESS
 * for each block of 9 base 10 digits
 * val = digit[8] * 10^8 + digit[7] * 10^7 + ... + digit[0] + 10^0
 *     = ((digit[8] * 10 + digit[7]) * 10 + ... ) * 10 + digit[0]
 * 
 * @param str the string
 * @return the integer
 */
bigint strToBigint(char *str)
{
    unsigned int len = strlen(str);  // length of raw string
    unsigned int effectiveLen = len; // length of string to parse after discarding negative sign and leading zeros
    unsigned int firstDigit = 0;     // index of first digit

    // sign placeholder
    bool sign = true;
    if (str[firstDigit] == '-')
    {
        // first character is negative sign
        sign = false;
        effectiveLen--;

        // advance cursor
        firstDigit++;
    }

    // skip leading zeros
    while (str[firstDigit] == '0')
    {
        effectiveLen--;

        // advance cursor
        firstDigit++;
    }

    if (effectiveLen == 0)
    {
        // no digits
        return BIGINT_ZERO;
    }

    // get number of digits
    unsigned int numDigits, extra;
    divMod(effectiveLen, NO_BASE_DIGITS, &numDigits, &extra);

    if (extra)
    {
        // remainder, so need extra digit
        numDigits++;
    }

    // allocate integer
    bigint ret = allocateBigint(numDigits);
    // set determined values
    ret.noDigits = numDigits;
    ret.sign = sign;

    unsigned int currentDigit = 0;            // index in bigint array
    unsigned int digitArrIdx = numDigits - 1; // index of the current bigint digit

    // begin counting down from number of digits in largest value integer
    char currentDigitCharIdx = extra ? extra : NO_BASE_DIGITS; // current string digit

    // iterate through each character
    for (unsigned int idx = firstDigit; idx < len; idx++)
    {
        char c = str[idx];

        // validate
        if (c >= '0' && c <= '9')
        {
            // get integer
            currentDigit *= 10;      // increase magnitude
            currentDigit += c - '0'; // get integer value of digit
            currentDigitCharIdx--;

            if (!currentDigitCharIdx)
            {
                // end of block, set in array
                ret.digits[digitArrIdx--] = currentDigit;

                // reset values
                currentDigit = 0;
                currentDigitCharIdx = NO_BASE_DIGITS;
            }
        }
        else
        {
            // invalid character
            freeBigint(&ret);
            return BIGINT_ZERO;
        }
    }

    return ret;
}

/**
 * creates new positive big integer from an unsigned integer
 * 
 * PROCESS - conversion of bases
 * i_10 / BASE = i' R digits[0] = LSD (least significant digit)
 * i' / BASE = i'' R digits[1]
 * ...
 * i''' / BASE = 0 R digits[N - 1] = MSD (most significant digit)
 * 
 * @param i the integer
 * @return the integer
 */
bigint newPositiveBigint(unsigned int i)
{
    // get number of digits
    unsigned int noDigits = numDigits(i, BASE);

    bigint ret = allocateBigint(noDigits);
    ret.noDigits = noDigits;
    for (unsigned int idx = 0; idx < noDigits; idx++)
    {
        // get next digit
        divMod(i, BASE, &i, &ret.digits[idx]);
    }

    return ret;
}

/**
 * creates new big integer from an integer
 * calls newPositiveBigInt then applies sign after
 * @param i the integer
 * @return the big integer
 */
bigint newBigint(int i)
{
    bool sign = true;
    if (i < 0)
    {
        sign = false;
        i *= -1;
    }

    // pass in positive component
    bigint ret = newPositiveBigint(i);
    // set sign
    ret.sign = sign;

    return ret;
}

/**
 * get string representation of number
 * 
 * EXAMPLE
 * [456789780, 123] => "123456789780"
 * 
 * digitIdx strIdx  char    offset
 * 1        0       '3'     2
 *                  '2'     1
 *                  '1'     0 => "123"
 * 2        3       '0'     8
 *                  '8'     7
 *                  ...
 *                  '5'     1
 *                  '4'     0 => "456789780"
 *                               -----------
 *                               "123456789780"
 * 
 * @param i the pointer to the big integer
 * @return the string
 */
char *bigintPtrToString(bigint *i)
{
    if (!i->noDigits)
    {
        // no digits
        return "0";
    }

    unsigned int noChars = (i->noDigits - 1) * NO_BASE_DIGITS;
    // may not need all characters for the last
    unsigned int numDigitsLast = numDigits(i->digits[i->noDigits - 1], 10);
    noChars += numDigitsLast;

    if (!i->sign)
    {
        // extra character for negative sign
        noChars++;
    }

    // allocate string (extra character for terminator)
    char *ret = malloc((noChars + 1) * sizeof(char));

    if (!ret)
    {
        // could not allocate memory
        return NULL;
    }

    unsigned int strIdx = 0;
    if (!i->sign)
    {
        // set first char to negative sign
        ret[strIdx++] = '-';
    }

    // highest magnitude digits are last in the array but first characters in string
    char offset = numDigitsLast;
    int strDigit;
    for (int digitIdx = i->noDigits;
         digitIdx; // digitIdx != 0
         digitIdx--)
    {
        // keep track of original offset
        char increment = offset;

        // get integer value
        int digit = i->digits[digitIdx - 1];
        while (offset--)
        {
            // get next lowest magnitude digit in base 10
            divMod(digit, 10, &digit, &strDigit);
            ret[strIdx + offset] = '0' + strDigit;
        }

        offset = NO_BASE_DIGITS;

        // advance cursor
        strIdx += increment;
    }

    // string termination
    ret[noChars] = '\0';

    return ret;
}

/**
 * get string representation of number
 * @param i the big integer
 * @return the string
 */
char *bigintToString(bigint i)
{
    return bigintPtrToString(&i);
}

/**
 * comparison of two integers
 * @param i1 the first integer
 * @param i2 the second integer
 * @return -1 if i1 < i2, 0 if i1 = i2, 1 if i1 > i2
 */
char compareBigint(bigint i1, bigint i2)
{
    if (i1.sign && !i2.sign) // i1 >= 0, i2 < 0 -> i1 > i2
    {
        return 1;
    }
    else if (!i1.sign && i2.sign) // i1 < 0, i2 >= 0 -> i1 < i2
    {
        return -1;
    }
    else if (!i1.sign && !i2.sign) // i1 < 0, i2 < 0
    {
        // both negative, compare the positives, return opposite
        i1.sign = true;
        i2.sign = true;
        char comparison = compareBigint(i1, i2);
        return -comparison;
    }

    // both positive
    // if one has more digits, it is larger because has a higher magnitude term
    if (i1.noDigits > i2.noDigits)
    {
        return 1;
    }
    else if (i1.noDigits < i2.noDigits)
    {
        return -1;
    }

    // same number of digits, compare individual digits
    // start from highest magnitude terms
    unsigned int idx = i1.noDigits;
    while (idx--)
    {
        // highest magnitude terms matter
        if (i1.digits[idx] > i2.digits[idx])
        {
            return 1;
        }
        else if (i1.digits[idx] < i2.digits[idx])
        {
            return -1;
        }
    }

    // all digits equal
    return 0;
}

/**
 * adds two integers
 * @param i1 the first integer
 * @param i2 the second integer
 * @return the sum
 */
bigint addBigint(bigint i1, bigint i2)
{
    if (i1.sign && !i2.sign) // i1 + -i2 = i1 - i2
    {
        i2.sign = true;
        return subtractBigint(i1, i2);
    }
    else if (!i1.sign && i2.sign) // -i1 + i2 = i2 - i1
    {
        i1.sign = true;
        return subtractBigint(i2, i1);
    }

    // extra digit in case of extra carry
    unsigned int noDigits = MAX(i1.noDigits, i2.noDigits) + 1;

    bigint ret = allocateBigint(noDigits);
    // set determined values
    ret.noDigits = noDigits;

    // placeholder
    bool carry = false;
    for (unsigned int idx = 0; idx < ret.noDigits; idx++)
    {
        // determine carryover from operation on previous digit
        int res = carry ? 1 : 0;
        // add corresponding digits
        if (idx < i1.noDigits)
        {
            res += i1.digits[idx];
        }
        if (idx < i2.noDigits)
        {
            res += i2.digits[idx];
        }

        // determine carryover for operation on next digit
        if (res >= BASE)
        {
            carry = true;
            res -= BASE;
        }
        else
        {
            carry = false;
        }

        // set digit in result
        ret.digits[idx] = res;
    }

    // remove leading zeros
    while (!ret.digits[ret.noDigits - 1])
    {
        ret.noDigits--;
    }

    if (!i1.sign && !i2.sign) // -i1 + -i2 = -(i1 + i2)
    {
        ret.sign = false;
    }

    return ret;
}

/**
 * subtracts two integers
 * @param i1 the integer to subtract from
 * @param i2 the integer to subtract
 * @return the difference
 */
bigint subtractBigint(bigint i1, bigint i2)
{
    char comparison = compareBigint(i1, i2);
    if (!comparison)
    {
        // the numbers are the same
        return BIGINT_ZERO;
    }

    if (i1.sign && !i2.sign) // i1 - -i2 = i1 + i2
    {
        i2.sign = true;
        return addBigint(i1, i2);
    }
    else if (!i1.sign && i2.sign) // -i1 - i2 = -i1 + -i2
    {
        i2.sign = false;
        return addBigint(i1, i2);
    }
    else if (!i1.sign && !i2.sign) // -i1 - -i2 = -i1 + i2 = i2 - i1
    {
        i1.sign = true;
        i2.sign = true;
        return subtractBigint(i2, i1);
    }
    else // both positive, find max
    {
        if (comparison < 0) // i2 > i1; i1 - i2 = -(i2 - i1)
        {
            bigint res = subtractBigint(i2, i1);
            res.sign = !res.sign;
            return res;
        }
    }

    // know both are positive and i1 is larger
    unsigned int noDigits = i1.noDigits;

    bigint ret = allocateBigint(noDigits);
    // set determined values
    ret.noDigits = noDigits;

    // placeholder
    bool carry = false;
    for (unsigned int idx = 0; idx < ret.noDigits; idx++)
    {
        // determine carryover from operation on previous digit
        int res = carry ? -1 : 0;
        // add corresponding digit from first term
        // subtract corresponding digit from second term
        if (idx < i1.noDigits)
        {
            res += i1.digits[idx];
        }
        if (idx < i2.noDigits)
        {
            res -= i2.digits[idx];
        }

        // determine carryover for operation on next digit
        if (res < 0)
        {
            carry = true;
            res += BASE;
        }
        else
        {
            carry = false;
        }

        // set digit in result
        ret.digits[idx] = res;
    }

    // remove leading zeros
    while (!ret.digits[ret.noDigits - 1])
    {
        ret.noDigits--;
    }

    return ret;
}