//
// Created by dong on 22-4-21.
//
#include "md5.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>

uint32_t A=0x67452301,B=0xEFCDAB89,C=0x98BADCFE,D=0x10325476;

const uint8_t SHIFT[4][16] = {
        {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22},
        {5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20},
        {4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23},
        {6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21}
};

const uint32_t T[64] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
        0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
        0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
        0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
        0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
        0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

uint32_t F(uint32_t b, uint32_t c, uint32_t d) {
    return (b&c)|((~b)&d);
}

uint32_t G(uint32_t b, uint32_t c, uint32_t d) {
    return (b&d)|(c&(~d));
}

uint32_t H(uint32_t b, uint32_t c, uint32_t d) {
    return b^c^d;
}

uint32_t I(uint32_t b, uint32_t c, uint32_t d) {
    return c^(b|(~d));
}

uint32_t P0(uint32_t k) {
    return k;
}

uint32_t P1(uint32_t k) {
    return (1+5*k)%16;
}

uint32_t P2(uint32_t k) {
    return (5+3*k)%16;
}

uint32_t P3(uint32_t k) {
    return (7*k)%16;
}

/**
 * Important.
 * =>   0x11|12|13|14|15|16|17|18. ...n=10byte
 * <=   0x14 13 12 11 |18 17 16 15....|0x50 00 00 00| 00 00 00 00
 * @param m
 * @param n
 * @param buf
 * @param Lp
 */
void pad(char m[],uint64_t n, uint32_t buf[], size_t *Lp)
{
    int i;
    size_t n2,L=n/64+1;
    uint8_t *tmp=malloc(L*64);
    if (n%64>=56)
        L++;

    memcpy(tmp,m,n);
    n2=n;
    tmp[n2]=0x80;
    while (n2%64!=56)
        tmp[++n2]=0x00;

    /* little-endian: 4* 8bit ->32 bit */
    for (i=0;i<n2;i=i+4){
        buf[i/4]=tmp[i+3]<<24|tmp[i+2]<<16|tmp[i+1]<<8|tmp[i];
    }
    n=n*8;
    /* little-endian: 64bit ->2* 32bit */
    /* a-32|b-32 => b-32|a-32 */
    buf[L*16-2]=(uint32_t )n;
    buf[L*16-1]=(uint32_t )(n>>32);
    *Lp=L;
}
uint32_t u32_to_little(uint32_t w)
{
    return (w&0xff)<<24|(w>>8&0xff)<<16|(w>>16&0xff)<<8|(w>>24&0xff);
}

uint32_t *md5(char m[],int n) {
    uint32_t t;
    uint32_t BUF[BUFSIZ];
    uint32_t Y[16];
    uint32_t(*gs[4])(uint32_t, uint32_t, uint32_t) = {F, G, H, I};
    uint32_t(*ps[4])(uint32_t) = {P0, P1, P2, P3};
    uint32_t (*g)(uint32_t, uint32_t, uint32_t);
    uint32_t (*p)(uint32_t);
    uint32_t a,b,c,d;
    uint32_t *res= malloc(16);
    int i, k,l;
    int L;
    pad(m,n,BUF,&L);

    for (l=0;l<L;l++){

        for (k=0;k<16;k++){
            Y[k]=BUF[l*16+k];
        }

        a=A;
        b=B;
        c=C;
        d=D;
        for (i = 0; i < 4; i++) {
            g = gs[i];
            p = ps[i];

            for (k = 0; k < 16; k++) {
                a = a + g(b, c, d);
                a += Y[p(k)];
                a += T[i * 16 + k];
                a=ROT_LEFT_SHIFT(a,SHIFT[i][k]);
                a+=b;
                t=d;
                d=c;
                c=b;
                b=a;
                a=t;

            }
        }
        A+=a;
        B+=b;
        C+=c;
        D+=d;

    }

    res[0]= u32_to_little(A);
    res[1]= u32_to_little(B);
    res[2]= u32_to_little(C);
    res[3]= u32_to_little(D);

    return res;
}



