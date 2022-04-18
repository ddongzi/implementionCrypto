#include <stdio.h>
#include "aes.h"

int main() {
    printf("Hello, World! %x\n",'\x00'+224);
    unsigned char *s="HelloHelloHello.";
    unsigned char *k="HelloHelloHello.";
    int i;

    s=aes_enc(s,k);
    for (i=0;i<16;i++){
        printf("%x",s[i]);
    }
    return 0;
}

