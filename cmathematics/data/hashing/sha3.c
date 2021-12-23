#include "sha.h"
#include "sha3.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../../lib/arrays.h"

unsigned int sha3_rotConst[5][5] = {
    {0, 1, 190, 28, 91},
    {36, 300, 6, 55, 276},
    {3, 10, 171, 153, 231},
    {105, 45, 15, 21, 136},
    {210, 66, 253, 120, 78}};

unsigned long long sha3_roundConsts[SHA3_NR] = {
    0x0000000000000001,
    0x0000000000008082,
    0x800000000000808A,
    0x8000000080008000,
    0x000000000000808B,
    0x0000000080000001,
    0x8000000080008081,
    0x8000000000008009,
    0x000000000000008A,
    0x0000000000000088,
    0x0000000080008009,
    0x000000008000000A,
    0x000000008000808B,
    0x800000000000008B,
    0x8000000000008089,
    0x8000000000008003,
    0x8000000000008002,
    0x8000000000000080,
    0x000000000000800A,
    0x800000008000000A,
    0x8000000080008081,
    0x8000000000008080,
    0x0000000080000001,
    0x8000000080008008};

void sha3_keccak_f(unsigned long long A[5][5])
{
    for (int i = 0; i < SHA3_NR; i++)
    {
        // THETA ===============================
        unsigned long long C[5];
        unsigned long long D[5];

        // calculate C
        for (int x = 0; x < 5; x++)
        {
            C[x] = 0L;
            for (int y = 0; y < 5; y++)
            {
                C[x] ^= A[y][x];
            }
        }

        // calculate D
        for (int x = 0; x < 5; x++)
        {
            D[x] = C[(x + 4) % 5] ^ leftRotateLL(C[(x + 1) % 5], 1);
        }

        // put result into A
        for (int x = 0; x < 5; x++)
        {
            for (int y = 0; y < 5; y++)
            {
                A[y][x] ^= D[x];
            }
        }

        // RHO, PI ===============================
        unsigned long long B[5][5];
        memset(B, 0, 5 * 5 * sizeof(unsigned long long));

        for (int x = 0; x < 5; x++)
        {
            for (int y = 0; y < 5; y++)
            {
                B[(2 * x + 3 * y) % 5][y] = leftRotateLL(A[y][x], sha3_rotConst[y][x]);
            }
        }

        // CHI ===============================
        for (int x = 0; x < 5; x++)
        {
            for (int y = 0; y < 5; y++)
            {
                A[y][x] = B[y][x] ^
                          ((~B[y][(x + 1) % 5]) & B[y][(x + 2) % 5]);
            }
        }

        // IOTA ===============================
        A[0][0] ^= sha3_roundConsts[i];
    }
}

void sha3_initContext(sha3_context *ctx, int mode) {
    ctx->ret_len = sha_getRetLenIdx(mode);
    ctx->r = sha_getBlockLenIdx(mode);

    ctx->stateCursor = 0;
    memset(ctx->A, 0, 5 * 5 * sizeof(unsigned long long));
}

void sha3_update(sha3_context *ctx, unsigned char *in, int n)
{
    // absorb new bytes

    int cursor = 0; // cursor in message

    int resLen = ctx->stateCursor + n;
    int noBlocks = resLen / ctx->r; // determine number of complete blocks
    int extra = (resLen % ctx->r);
    if (!noBlocks) {
        // extra is only in the first block with occupied bytes
        extra -= ctx->stateCursor;
    }
    if (extra) {
        noBlocks++;
    }

    for (int i = 0; i < noBlocks; i++) {
        // block variables
        int blockCursor = ctx->stateCursor; // cursor in the block

        // know we will complete the block
        int noBytesInBlock = ctx->r - ctx->stateCursor;
        if (i == noBlocks - 1 && extra) {
            // writing incomplete block
            noBytesInBlock = extra;
        }
        int yInit = ctx->stateCursor / (5 * 8); // initial y coordinate

        int rowPos = ctx->stateCursor % (5 * 8); // position along the row in bytes
        int xInit = rowPos / 8; // initial x coordinate
        int bInit = rowPos % 8; // initial byte in the word

        // XOR block into the state
        for (int y = yInit; y < 5; y++)
        {
            for (int x = xInit; x < 5; x++)
            {
                // write into tmp
                if (blockCursor < ctx->r &&
                    blockCursor < noBytesInBlock + ctx->stateCursor) // have not written the complete block yet
                {
                    unsigned long long tmp = 0L;

                    int noBytesInWord = MIN(8, (noBytesInBlock + ctx->stateCursor) - blockCursor) - bInit;
                    // write bytes from message
                    memcpy(&tmp, in + cursor + blockCursor - ctx->stateCursor, noBytesInWord);
                    if (bInit) {
                        tmp <<= (bInit << 3); // left shift by bits to account for initial cursor position
                        // will start at beginning of next byte
                        bInit = 0;
                    }

                    // xor tmp into the state
                    ctx->A[y][x] ^= tmp;

                    // advance block cursor
                    blockCursor += noBytesInWord;
                }
                else {
                    break;
                }
            }
        }

        if (blockCursor == ctx->r) {
            // KECCAK-f on complete block
            sha3_keccak_f(ctx->A);

            // return state cursor to beginning
            ctx->stateCursor = 0;
        }
        else {
            // advance state cursor
            ctx->stateCursor = blockCursor;
        }

        // advance message cursor
        cursor += noBytesInBlock;
    }
}

void sha3_digest(sha3_context *ctx, unsigned char **out) {
    // PADDING
    int x, y, b, rowPos;
    unsigned long long tmp;

    // pad initial byte (0b011 = 6)
    y = ctx->stateCursor / (5 * 8); // initial y coordinate
    rowPos = ctx->stateCursor % (5 * 8); // position along the row in bytes
    x = rowPos / 8; // initial x coordinate
    b = rowPos % 8; // initial byte in the word
    tmp = (0x06ULL) << (b << 3); // byte position => bit position
    ctx->A[y][x] ^= tmp; // XOR into state

    // pad final byte (0b10*0)
    y = (ctx->r - 1) / (5 * 8); // final y coordinate
    rowPos = (ctx->r - 1) % (5 * 8); // position along the row in bytes
    x = rowPos / 8; // final x coordinate
    tmp = (0x01ULL) << 0x3f;
    ctx->A[y][x] ^= tmp; // XOR into state
    
    // SQUEEZING PHASE
    int cursor = 0; // cursor for the output string
    // allocate output
    *out = malloc(ctx->ret_len * sizeof(unsigned char));
    if (!(*out)) {
        return;
    }

    // squeezing rounds
    while (cursor < ctx->ret_len)
    {
        // KECCAK-f
        sha3_keccak_f(ctx->A);

        // block variables
        int noBytesInBlock = MIN(ctx->r, MAX(ctx->ret_len - cursor, 0)); // number of bytes to copy to output

        // copy
        memcpy(*out + cursor, ctx->A, noBytesInBlock);

        // advance cursor
        cursor += noBytesInBlock;
    }
}