//
// Created by dong on 22-4-22.
//

#include <string.h>
#include <malloc.h>
#include "sha.h"

uint32_t A=0x67452301,B=0xEFCDAB89,C=0x98BADCFE,D=0x10325476,E=0xC3D2E1F0;


void wexpension(uint32_t Y[16],uint32_t W[80])
{
    int i;
    memcpy(W,Y,64);
    for (i=16;i<80;i++){
        W[i]= ROT_LEFT_SHIFT(W[i-16]^W[i-14]^W[i-8]^W[i-3],1);
    }
}
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
}

uint32_t *sha(char m[],uint64_t n)
{
    uint32_t Y[16];
    uint32_t W[80];
    int step=0,k,l;
    uint32_t tmp,*res;
    uint32_t a=A,b=B,c=C,d=D,e=E;
    uint32_t BUF[BUFSIZ];
    size_t L;
    pad(m,n,BUF,&L);

    res=malloc(16);

    for (l=0;l<L;l++){

        for (k=0;k<16;k++){
            Y[k]=BUF[l*16+k];
        }
        wexpension(Y,W);


        while (step<80){
            switch (step/20) {
                case 0:
                    e+= F(b,c,d);
                    e+=0x5A827999;
                    break;
                case 1:
                    e+= G(b,c,d);
                    e+=0x6ED9EBA1;
                    break;
                case 2:
                    e+= H(b,c,d);
                    e+=0x8F1BBCDC;
                    break;
                case 3:
                    e+= I(b,c,d);
                    e+=0xCA62C1D6;
                    break;
            }
            e+= ROT_LEFT_SHIFT(a,5);
            e+=W[step];
            b= ROT_LEFT_SHIFT(b,30);

            tmp=e;
            e=d;
            d=c;
            c=b;
            b=a;
            a=tmp;

            step++;
        }

    }


    A+=a;
    B+=b;
    C+=c;
    D+=d;
    E+=e;

    res[0]=A;
    res[1]=B;
    res[2]=C;
    res[3]=D;
    res[4]=E;

    return res;

}