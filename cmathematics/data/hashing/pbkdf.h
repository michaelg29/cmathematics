#include "../../cmathematics.h"

#ifndef PBKDF_H
#define PBKDF_H

void pbkdf2_hmac_sha(unsigned char *pwd, int pwdLen,
                     unsigned char *salt, int saltLen,
                     int c, char *sha_mode,
                     int dkLen, unsigned char **out);

#endif // PBKDF_H