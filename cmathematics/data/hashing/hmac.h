#include "../../cmathematics.h"

#ifndef HMAC_H
#define HMAC_H

int hmac_sha(unsigned char *key, int keyLen,
             unsigned char *txt, int txtLen,
             char *sha_mode,
             unsigned char **out);

#endif // HMAC_H