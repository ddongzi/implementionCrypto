//
// Created by dong on 22-4-23.
//
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "sm3.h"

uint32_t H[8]={    0x7380166f, 0x4914b2b9, 0x172442d7, 0xda8a0600,
                   0xa96f30bc, 0x163138aa, 0xe38dee4d, 0xb0fb0e4e};


void pad(char m[],uint64_t n,uint32_t buf[],size_t *Lp)
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

    /* big-endian: 4* 8bit ->32 bit */
    for (i=0;i<n2;i=i+4){
        buf[i/4]=tmp[i]<<24|tmp[i+1]<<16|tmp[i+2]<<8|tmp[i+3];
    }
    n=n*8;
    /* big-endian: 64bit ->2* 32bit */
    /* a-32|b-32 => b-32|a-32 */
    buf[L*16-2]=(uint32_t )(n>>32);
    buf[L*16-1]=(uint32_t )n;
    *Lp=L;

    printf("pad:");
    for (i = 0; i <L*16 ; ++i) {
        printf("%x",buf[i]);
    }

}
void wexpension(uint32_t Y[16],uint32_t W[68])
{
    int i;
    memcpy(W,Y,64);
    for (i=16;i<68;i++){
        W[i]= P1(W[i-16]^W[i-9]^ ROT_LEFT_SHIFT(W[i-3],15))^ ROT_LEFT_SHIFT(W[i-13],7)^W[i-6];
    }
}
uint32_t FF1(uint32_t a,uint32_t b,uint32_t c)
{
    return a^b^c;
}
uint32_t FF2(uint32_t a,uint32_t b,uint32_t c)
{
    return (a&b)|(a&c)|(b&c);
}
uint32_t GG1(uint32_t a,uint32_t b,uint32_t c)
{
    return a^b^c;
}
uint32_t GG2(uint32_t a,uint32_t b,uint32_t c)
{
    return (a&b)|((~a)&c);
}

void update(char m[])
{
    uint32_t Y[16];
    uint32_t W[68];
    uint64_t n= strlen(m);
    int step,k,l;
    uint32_t tmp;
    uint32_t BUF[BUFSIZ];
    size_t L;
    uint32_t T;
    uint32_t (*FF)(uint32_t,uint32_t,uint32_t);
    uint32_t (*GG)(uint32_t,uint32_t,uint32_t);

    uint32_t SS1,SS2;
    uint32_t a,b,c,d,e,f,g,h;

    pad(m,n,BUF,&L);


    for (l=0;l<L;l++){
        for (k=0;k<16;k++){
            Y[k]=BUF[l*16+k];
        }
        wexpension(Y,W);

        a=H[0],b=H[1],c=H[2],d=H[3];
        e=H[4],f=H[5],g=H[6],h=H[7];

        for (step=0;step<64;step++){
            if (step<16){
                T=0x79cc4519;
                FF=FF1;
                GG=GG1;
            } else{
                T=0x7a879d8a;
                FF=FF2;
                GG=GG2;
            }

            {
                SS2= ROT_LEFT_SHIFT(ROT_LEFT_SHIFT(a,12)+e+ ROT_LEFT_SHIFT(T,step),7);
                SS1=SS2^ ROT_LEFT_SHIFT(a,12);
            }
            {
                d+=FF(a,b,c)+SS1+(W[step]^W[step+4]);
                b= ROT_LEFT_SHIFT(b,9);
                tmp=d;
                d=c;
                c=b;
                b=a;
                a=tmp;
            }
            {
                h+=GG(e,f,g)+SS2+W[step];
                h= P0(h);
                f= ROT_LEFT_SHIFT(f,19);
                tmp=h;
                h=g;
                g=f;
                f=e;
                e=tmp;
            }

            printf("Step %d: %08x %08x %08x %08x %08x %08x %08x %08x\n ",step,a,b,c,d,e,f,g,h);

        }
        H[0]^=a;
        H[1]^=b;
        H[2]^=c;
        H[3]^=d;
        H[4]^=e;
        H[5]^=f;
        H[6]^=g;
        H[7]^=h;

    }

}
uint32_t *hexdigest()
{
    int k;
    uint32_t *res;
    res=malloc(20);
    for (k=0;k<8;k++){
        res[k]=H[k];
        printf("%0x ",H[k]);
    }
    putchar('\n');
    return res;
}