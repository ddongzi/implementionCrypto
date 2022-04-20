#include <stdio.h>
#include "zuc.h"

int main() {
    printf("Hello, World! %x\n",'\x00'+224);
    uint8_t k[16]={'H'};
    uint8_t iv[16]={'O'};
    printf("In : 16 key: ");
    scanf("%s",k);
    zuc(k,iv);

    return 0;
}

