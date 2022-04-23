//
// Created by dong on 22-4-22.
//
#include "sha.h"
#include <string.h>


void cat(char *a,int na,char *b,int nb,char *c)
{

    int k;
    for (k=0;k<na;k++){
        c[k]=a[k];
    }
    for (k=0;k<nb;k++){
        c[k+na]=b[k];
    }
}

uint32_t *hmac_sha1(char *key,char *m)
{
    size_t keylen= strlen(key);
    size_t mlen= strlen(m);

    char kipad[64],kopad[64];
    char p1[64+mlen+1],*p2,p3[84+1];
    int k;
    if (keylen>64){
        init();
        update(key);
        key=digest(key);
        keylen=20;
    }

    //kpad:
    for (k=0;k<keylen;k++){
        kipad[k]=key[k]^0x36;
        kopad[k]=key[k]^0x5c;
    }
    for (;k<64;k++) {
        kipad[k]=0x36;
        kopad[k]=0x5c;
    }

    // p2=hash(k^ipad || m);
    init();
    cat(kipad,64,m,mlen,p1);
    p1[64+mlen]='\0';
    update(p1);
    p2=digest();

    // hash(k^opad || p2)
    init();
    cat(kopad,64,p2,20,p3);
    p3[84]='\0';
    update(p3);

    return hexdigest();

}