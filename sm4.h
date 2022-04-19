//
// Created by dong on 22-4-19.
//

#ifndef MYCRYPTO_SM4_H
#define MYCRYPTO_SM4_H

#endif //MYCRYPTO_SM4_H

#include <stdint.h>

typedef uint32_t word_t;
typedef uint32_t word4_t[4];

#define ROUND   32
#define ROT_LEFT_SHIFT(x,n) (x<<n|x>>(32-n))
#define ROT_RIGHT_SHIFT(x,n) (x>>n|x<<(32-n))

uint8_t *sm4_enc(uint8_t *s,uint8_t *key);
word_t key_expension(word4_t ks,word_t cki);
void roundfunc(word4_t xs,word_t roundkey);
void reverse(word4_t xs);
word_t sbox(word_t w);


