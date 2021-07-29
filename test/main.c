#include <stdio.h>
#include <stdlib.h>

#include "../cmathematics/cmathematics.h"
#include "../cmathematics/aes.h"

char hex[16] = "0123456789ABCDEF";
void printCharArr(unsigned char *arr, int len, bool asChar)
{
    printf("{ ");
    for (int i = 0; i < len; i++)
    {
        printf("%c%c ", hex[arr[i] >> 4], hex[arr[i] & 0x0f]);
    }
    printf("}\n");
}

int main()
{
    printf("Hello, world!\n");

    unsigned char *txt = "asidlhgfyiuyguaysdgbagasdcvetwee";
    unsigned char *key = "abcdefghijklmnop";
    unsigned char *cipher = NULL;
    unsigned char *dec = NULL;

    int noBlocks = aes_encrypt(txt, 32, key, AES_128, &cipher);
    int len = aes_decrypt(cipher, noBlocks, key, AES_128, &dec);

    printf("Plaintext: ");
    printCharArr(txt, 32, false);
    printf("Key:       ");
    printCharArr(key, 16, false);
    printf("Cipher:    ");
    printCharArr(cipher, noBlocks * BLOCK_LEN, false);
    printf("Decrypted: ");
    printCharArr(dec, len, false);

    // aes_mixCols(aes_inv_mixColMat); // gives identity matrix

    free(txt);
    free(key);
    free(cipher);
    free(dec);

    return 0;
}