#include <stdio.h>
#include "mac.h"
#include "sha.h"
#include <string.h>
#include <stdint.h>

int main() {
    printf("Hello, World! %x\n", '\x00' + 224);

    char key[BUFSIZ];
    char m[BUFSIZ];
    uint32_t *r;
    int i;
    printf("key :");
    scanf("%s", key);
    printf("msg :");
    scanf("%s", m);

    r=hmac_sha1(key,m);

    for (i=0;i<5;i++){
        printf("%0x ",r[i]);
    }

    return 0;
}

