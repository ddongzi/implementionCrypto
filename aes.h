//
// Created by dong on 22-4-18.
//

#ifndef MYCRYPTO_AES_H
#define MYCRYPTO_AES_H
#endif //MYCRYPTO_AES_H

#include <stdint.h>
#define NB 4
#define NK 4
#define NR 10

typedef uint8_t state_t[4][NB];
typedef uint8_t key_t[4][NK];
typedef uint8_t expanded_key_t[4][NB*(NR+1)];
typedef uint8_t round_key_t[4][NB];


void key_expension(key_t key,expanded_key_t expandedkey);

void addroundkey(state_t state,round_key_t roundkey);
void get_round_key(const expanded_key_t expandedKey,unsigned int j,round_key_t roundKey);
void roundfunc(state_t state,round_key_t roundKey);
void finalround(state_t state,round_key_t roundKey);

void bytesub(state_t state);
void shiftrow(state_t state);
void mixcolumn(state_t state);

void word_rotbyte(uint8_t a[4]);
void word_xor(uint8_t a[4],uint8_t b[4],uint8_t c[4]);
void word_bytesub(uint8_t a[4]);

uint8_t gadd(uint8_t a,uint8_t b);
uint8_t gmul(uint8_t a,uint8_t b);
unsigned char* aes_enc(unsigned char *s,unsigned char *keys);

