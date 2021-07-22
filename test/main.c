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

    aes_encrypt(txt, 32, key, 16, &cipher);

    printf("Plaintext: ");
    printCharArr(txt, 32, false);
    printf("Key: ");
    printCharArr(key, 16, false);
    printf("Cipher: ");
    printCharArr(cipher, 32, false);

    return 0;
}