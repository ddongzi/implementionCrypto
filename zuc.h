//
// Created by dong on 22-4-20.
//

#ifndef MYCRYPTO_ZUC_H
#define MYCRYPTO_ZUC_H

#endif //MYCRYPTO_ZUC_H

#include <stdint.h>

#define ROT_LEFT_SHIFT(x,n) (x<<n|x>>(32-n))
/* a+b mod (2^31-1) */
#define ADD_MOD(a,b)    (((a+b)&0x7fffffff)+((a+b)>>31))
/* a*2^k mod (2^31-1) */
#define MUL_POW2_k(a,k)    ((a<<k|a>>(31-k))&0x7fffffff)
void zuc(uint8_t *key,uint8_t *iv);