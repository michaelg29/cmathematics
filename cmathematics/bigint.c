#include "bigint.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

int arrOne[1] = {1};

const bigint BIGINT_ZERO = {true, 0, 0, NULL};
const bigint BIGINT_ONE = {true, 1, 1, arrOne};
const bigint BIGINT_NEG_ONE = {false, 1, 1, arrOne};

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
 * copies an array of integers into a bigint structure
 * @param arr the array
 * @param n the number of digits
 * @param sign the sign of the bigint
 * @return the bigint structure
 */
bigint copyIntArr(int *arr, unsigned int n, bool sign)
{
    bigint ret = allocateBigint(n);
    ret.noDigits = n;
    ret.sign = sign;

    memcpy(ret.digits, arr, n * sizeof(int));

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
 * creates new positive big integer from an unsigned long long
 * same process as newPositiveBigInt
 * 
 * @param l the num
 * @return the big integer
 */
bigint newLLPositiveBigInt(unsigned long long l)
{
    // get number of digits
    unsigned long long noDigits = numDigitsLL(l, BASE);

    bigint ret = allocateBigint(noDigits);
    ret.noDigits = noDigits;
    for (unsigned int idx = 0; idx < noDigits; idx++)
    {
        unsigned long long digit;

        // get next digit
        divModLL(l, BASE, &l, (unsigned long long *)&digit);
        ret.digits[idx] = digit;
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
 * creates new big integer from a long long
 * calls newLLPositiveBigInt then applies sign after
 * @param i the long long
 * @return the big integer
 */
bigint newLLBigInt(long long l)
{
    bool sign = true;
    if (l < 0LL)
    {
        sign = false;
        l *= -1LL;
    }

    // pass in positive component
    bigint ret = newLLPositiveBigInt(l);
    // set sign
    ret.sign = sign;

    return ret;
}

/**
 * multiplies two integers together and handles possible overflow
 * @param i1 the first integer
 * @param i2 the second integer
 * @return the big integer containing the product
 */
bigint multiplyIntInt(unsigned int i1, unsigned int i2)
{
    bigint ret = newLLPositiveBigInt((long long)i1 * (long long)i2);

    // get product (account for overflow)
    return ret;
}

/**
 * trims leading zeros in the array of digits
 * @param b the pointer to the integer to trim
 */
void trimBigint(bigint *b)
{
    // determine how many digits to remove
    while (!b->digits[b->noDigits - 1])
    {
        b->noDigits--;
    }

    // copy over elements
    int *newDigits = malloc(b->noDigits * sizeof(int));

    for (unsigned int i = 0; i < b->noDigits; i++)
    {
        newDigits[i] = b->digits[i];
    }

    // temporary variable for swap
    int *tmp = b->digits;
    b->digits = newDigits;
    newDigits = tmp;

    // free old
    free(newDigits);
    free(tmp);
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
    if (!i || !i->noDigits)
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
    trimBigint(&ret);

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
    trimBigint(&ret);

    return ret;
}

/**
 * multiplies two bigints
 * @param i1 the first integer
 * @param i2 the second integer
 * @return the product
 */
bigint multiplyBigint(bigint i1, bigint i2)
{
    /*
        simplification checks
    */

    // if either is zero, return zero
    if (!(compareBigint(i1, BIGINT_ZERO) && compareBigint(i2, BIGINT_ZERO)))
    {
        return BIGINT_ZERO;
    }

    // if either number is 1, copy the other digits and keep the sign
    // if either number is -1, copy the other digits and reverse the sign
    char i1Comparison_p1 = compareBigint(i1, BIGINT_ONE);
    char i1Comparison_n1 = compareBigint(i1, BIGINT_NEG_ONE);
    if (!(i1Comparison_p1 && i1Comparison_n1))
    {
        return copyIntArr(
            i2.digits,
            i2.noDigits,
            !i1Comparison_p1 ? i2.sign : !i2.sign);
    }

    char i2Comparison_p1 = compareBigint(i2, BIGINT_ONE);
    char i2Comparison_n1 = compareBigint(i2, BIGINT_NEG_ONE);
    if (!(i2Comparison_p1 && i2Comparison_n1))
    {
        return copyIntArr(
            i1.digits,
            i1.noDigits,
            !i2Comparison_p1 ? i1.sign : !i1.sign);
    }

    /*
        determination steps
    */

    // get min and max number of digits
    unsigned int minNoDigits, maxNoDigits;
    if (i1.noDigits > i2.noDigits)
    {
        minNoDigits = i2.noDigits;
        maxNoDigits = i1.noDigits;
    }
    else
    {
        minNoDigits = i1.noDigits;
        maxNoDigits = i2.noDigits;
    }

    /*
        determine which algorithm to use

        long multiplication: O(n * m), n >= m
        karatsuba multiplication: O(max(n, m)^(lg 3)) ~= O(max^1.585) ~= O(n^1.5)

        Use long multiplication when
            n * m <= n^1.5
                m <= n^0.5 = sqrt(n)

            or under the threshold
    */
    if (maxNoDigits < KARATSUBA_THRESHOLD ||
        minNoDigits <= sqrt(maxNoDigits))
    {
        // perform long multiplication
        return longMultiplyBigint(i1, i2);
    }
    else
    {
        // perform karatsuba multiplication
        return karatsubaMultiplyBigint(i1, i2);
    }
}

/**
 * multiplies two bigints using long (elementary) multiplication
 * @param i1 the first integer
 * @param i2 the second integer
 * @return the product
 */
bigint longMultiplyBigint(bigint i1, bigint i2)
{
    // compute the sign using XNOR
    bool sign = !(i1.sign ^ i2.sign);

    // get result and insert into bigint structure
    unsigned int noDigits;
    int *retArr = longMultiplyIntArr(i1.digits, 0, i1.noDigits,
                                     i2.digits, 0, i2.noDigits,
                                     &noDigits);
    bigint ret = copyIntArr(retArr, noDigits, sign);

    // remove leading zeros
    trimBigint(&ret);

    // free tempoary array
    free(retArr);

    return ret;
}

/**
 * multiplies two specified ranges of integers using long (elementary) multiplication
 * @param i1 the first integer
 * @param i1i the index of the first digit in the first integer
 * @param i1f the index of the last digit (non-inclusive) in the first integer
 * @param i2 the second integer
 * @param i2i the index of the first digit in the second integer
 * @param i2f the index of the last digit (non-inclusive) in the second integer
 * @param outSize the variable to output the product size to
 * @return the integer array containing the product
 */
int *longMultiplyIntArr(int *i1, unsigned int i1i, unsigned int i1f,
                        int *i2, unsigned int i2i, unsigned int i2f,
                        unsigned int *outSize)
{
    unsigned int i1range = i1f - i1i;
    unsigned int i2range = i2f - i2i;
    unsigned int noDigits = i1range + i2range;

    // allocate
    int *ret = malloc(noDigits * sizeof(int));
    if (!ret)
    {
        *outSize = 0;
        return NULL;
    }

    // set all to 0
    for (unsigned int i = 0; i < noDigits; i++)
    {
        ret[i] = 0;
    }

    // for each digit of i1
    for (unsigned int i = 0; i < i1range; i++)
    {
        int carry = 0;

        // for each digit of i2
        for (unsigned int j = 0; j < i2range; j++)
        {
            // get product (account for overflow)
            // prod[0] = addition to digit[i + j]
            // prod[1] = addition to carry
            bigint prod = multiplyIntInt(i1[i1i + i], i2[i2i + j]);

            // assign to current digit
            if (prod.noDigits > 0)
            {
                ret[i + j] += prod.digits[0];
            }
            if (carry)
            {
                ret[i + j] += carry;
            }

            // determine carry for next digit
            if (ret[i + j] >= BASE)
            {
                carry = 1;
                ret[i + j] -= BASE;
            }
            else
            {
                carry = 0;
            }

            // add carry from the product
            unsigned int factor = 1;
            for (int k = 1; k < prod.noDigits; k++)
            {
                carry += prod.digits[k] * factor;
                factor *= BASE;
            }
        }

        // deal with leftover carry
        if (carry)
        {
            ret[i + i2range] += carry;
        }
    }

    *outSize = noDigits;
    return ret;
}

/**
 * multiplies two bigints using karatsuba multiplication
 * @param i1 the first integer
 * @param i2 the second integer
 * @return the product
 */
bigint karatsubaMultiplyBigint(bigint i1, bigint i2)
{
    // compute the sign using XNOR
    bool sign = !(i1.sign ^ i2.sign);

    // calculate implicit padding (does not actually exist)
    // determine next power of 2
    unsigned int maxNoDigits = MAX(i1.noDigits, i2.noDigits);
    unsigned int nextPow2 = 1;
    while (nextPow2 < maxNoDigits)
    {
        // multiply by 2
        nextPow2 <<= 1;
    }

    // get result and insert into bigint structure
    unsigned int noDigits;
    int *retArr = karatsubaMultiplyIntArr(i1.digits, i1.noDigits,
                                          i2.digits, i2.noDigits,
                                          0, nextPow2,
                                          &noDigits);
    bigint ret = copyIntArr(retArr, noDigits, sign);

    // remove leading zeros
    trimBigint(&ret);

    // free temporary array
    free(retArr);

    return ret;
}

/**
 * multiplies two specified ranges of integers using long (elementary) multiplication
 * @param i1 the first integer
 * @param i1size one more than the index of the last digit in the first integer
 * @param i2 the second integer
 * @param i2size one more than the index of the last digit in the second integer
 * @param idxi the index of the first digit in both integers, when padded to a power of 2 with zeros
 * @param idxf the index of the last digit in both integers, when padded to a power of 2 with zeros
 * @param outSize the variable to output the product size to
 * @return the integer array containing the product
 */
int *karatsubaMultiplyIntArr(int *i1, unsigned int i1size,
                             int *i2, unsigned int i2size,
                             unsigned int idxi, unsigned int idxf,
                             unsigned int *outSize)
{
    // if either index out of bounds, return all zeros
    if (idxi >= i1size || idxi >= i2size)
    {
        *outSize = 0;
        return NULL;
    }

    unsigned int range = idxf - idxi;
    unsigned int noDigits = range << 1;
    unsigned int sumTermSize = range >> 1;

    // Base case: do long multiplication if under threshold
    if (range <= KARATSUBA_THRESHOLD)
    {
        // if padded final index goes beyond the inputs, output the maximum index of the array
        // ie. take the minimum of the size and the padded index
        return longMultiplyIntArr(i1, idxi, MIN(i1size, idxf),
                                  i2, idxi, MIN(i2size, idxf),
                                  outSize);
    }

    // allocate return array
    int *ret = malloc(noDigits * sizeof(int));

    /*
        calculate sum terms
    */
    int *i1sum = malloc(sumTermSize * sizeof(int));
    int *i2sum = malloc(sumTermSize * sizeof(int));

    // use parallel pointers to iterate through left half and right half of the digit array
    unsigned int leftIdx = idxi;
    unsigned int rightIdx = idxi + sumTermSize;
    bool c1 = false;
    bool c2 = false;

    // insert numbers for sum terms
    // only add value if the digit exists (ie. index not beyond the size)
    for (int i = 0; i < sumTermSize; i++, leftIdx++, rightIdx++)
    {
        i1sum[i] = c1;
        c1 = false;
        if (leftIdx < i1size)
            i1sum[i] += i1[leftIdx];
        if (rightIdx < i1size)
            i1sum[i] += i1[rightIdx];

        i2sum[i] = c2;
        c2 = false;
        if (leftIdx < i2size)
            i2sum[i] += i2[leftIdx];
        if (rightIdx < i2size)
            i2sum[i] += i2[rightIdx];

        // consider carry if not last digit
        if (i < sumTermSize - 1)
        {
            if (i1sum[i] >= BASE)
            {
                c1 = true;
                i1sum[i] -= BASE;
            }

            if (i2sum[i] >= BASE)
            {
                c2 = true;
                i2sum[i] -= BASE;
            }
        }
    }

    /*
        do recursive calls
    */
    /*
        hi = x1y1
        lo = x0y0
        md = x0y1 + x1y0 > 0

        md' = (x0 + x1) * (y0 + y1) = x0y0 + x0y1 + x1y0 + x1y1
        md  = x0y1 + x1y0 = (x0y0 + x0y1 + x1y0 + x1y1) - x0y0 - x1y1 = md' - hi - lo > 0
    */
    unsigned int hiSize, loSize, mdSize;
    int *hi = karatsubaMultiplyIntArr(i1, i1size, i2, i2size, idxi + sumTermSize, idxf, &hiSize);
    int *lo = karatsubaMultiplyIntArr(i1, i1size, i2, i2size, idxi, idxi + sumTermSize, &loSize);
    int *md = karatsubaMultiplyIntArr(i1sum, sumTermSize, i2sum, sumTermSize, 0, sumTermSize, &mdSize);

    // attain middle term through subtractions
    // net result is positive, so do not have to worry about last carry
    int carry = 0;
    for (unsigned int i = 0; i < mdSize; i++)
    {
        md[i] -= ((i < hiSize) ? hi[i] : 0) +
                 ((i < loSize) ? lo[i] : 0);
        if (carry)
        {
            md[i] += carry;
        }

        carry = 0;
        while (md[i] < 0)
        {
            carry--;
            md[i] += BASE;
        }
    }

    /*
        combine results of sub-problems
    */
    for (int i = 0; i < range; i++) // low term
    {
        ret[i] = (i < loSize) ? lo[i] : 0;
    }
    for (int i = 0; i < range; i++) // hi term
    {
        ret[i + range] = (i < hiSize) ? hi[i] : 0;
    }

    carry = false;
    for (int i = 0; i < range; i++) // middle term (increment)
    {
        ret[i + sumTermSize] += (i < mdSize) ? md[i] : 0;
        if (carry)
        {
            ret[i + sumTermSize]++;
        }

        carry = false;
        if (ret[i + sumTermSize] >= BASE)
        {
            carry = true;
            ret[i + sumTermSize] -= BASE;
        }
    }
    // consider last carry
    if (carry)
    {
        ret[noDigits - sumTermSize]++;
    }

    /*
        cleanup
    */
    free(lo);
    free(md);
    free(hi);
    free(i1sum);
    free(i2sum);

    *outSize = noDigits;
    return ret;
}