#include <stdio.h>

#include "func.h"
int main() {
    printf("Hello, World! %x\n",'\x00'+224);
    unsigned long key=188;
    unsigned long pt=233;
    char ct[8];
    des_encrypt(key,pt,ct);
    return 0;
}

