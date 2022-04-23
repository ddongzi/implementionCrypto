#include <stdio.h>
#include "sm3.h"
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

    update(key);
    hexdigest();

    return 0;
}

