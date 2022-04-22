#include <stdio.h>
#include "sha.h"
#include <string.h>

int main() {
    printf("Hello, World! %x\n", '\x00' + 224);

    char s[BUFSIZ];
    uint32_t *r;
    printf("In : ");
    scanf("%s", s);
    r=sha(s, strlen(s));
    for (int i = 0; i < 5; ++i) {
        printf("%x ",r[i]);
    }

    return 0;
}

