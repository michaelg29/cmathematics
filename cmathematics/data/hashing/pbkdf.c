#include "pbkdf.h"

#include "sha.h"
#include "hmac.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void pbkdf2_hmac_sha(unsigned char *pwd, int pwdLen,
                     unsigned char *salt, int saltLen,
                     int c, char *sha_mode,
                     int dkLen, unsigned char **out)
{
    *out = malloc(dkLen);
    memset(*out, 0, dkLen);

    // get sha parameters
    int mode = sha_getModeNum(sha_mode);
    int hLen = sha_getBlockLenIdx(mode);

    // calculate number of blocks
    int noBlocks = dkLen / hLen; // ceil(dkLen / hLen)
    if (dkLen % hLen)
    {
        noBlocks++;
    }

    // output cursor
    int cursor = 0;

    // key input to PRF
    unsigned char *concatSalt = malloc(saltLen + 4);
    memcpy(concatSalt, salt, saltLen);

    for (int i = 0; i < noBlocks; i++)
    {
        int blockLen = MIN(hLen, dkLen - cursor);
        unsigned char *block = NULL;

        // concatenate counter to salt
        unsigned int saltCounter = i + 1;
        for (int j = 0; j < 4; j++)
        {
            concatSalt[saltLen + 4 - j - 1] = (unsigned char)saltCounter; // get LSB
            saltCounter >>= 8;
        }

        // go through c iterations
        unsigned char *prevSalt = concatSalt;
        int prevSaltLen = saltLen + 4;
        for (int j = 0; j < c; j++)
        {
            // call the pseudo-random function (HMAC)
            int len = hmac_sha(pwd, pwdLen, prevSalt, prevSaltLen, sha_mode, &block);

            // XOR block into the output
            for (int k = 0; k < blockLen; k++)
            {
                (*out)[cursor + k] ^= block[k];
            }

            // update pointers
            free(prevSalt);
            prevSalt = block;
            prevSaltLen = len;
        }

        // advance output cursor
        cursor += hLen;
    }
}