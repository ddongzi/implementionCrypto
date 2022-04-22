//
// Created by dong on 22-4-21.
//

#ifndef MYCRYPTO_MD5_H
#define MYCRYPTO_MD5_H

#endif //MYCRYPTO_MD5_H
#include <stdint.h>
#include <string.h>
#define ROT_LEFT_SHIFT(x,n) (x<<n|x>>(32-n))
uint32_t *md5(char m[],int n) ;