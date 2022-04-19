#include <stdio.h>
#include "sm4.h"

int main() {
    printf("Hello, World! %x\n",'\x00'+224);
    unsigned char *s="HelloHelloHello.";
    unsigned char *k="HelloHelloHello.";
    int i;
    unsigned char *t;
    for (i=0;i<16;i++){
        printf("%x",s[i]);
    }
    putchar('\n');
    t=sm4_enc(s,k);
    for (i=0;i<16;i++){
        printf("%x ",t[i]);
    }
    return 0;
}

