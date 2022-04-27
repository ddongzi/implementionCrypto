#include <stdio.h>
#include "sm2.h"
#include <string.h>
#include <stdint.h>

int main() {
    printf("Hello, World! %x\n",     '\x00' + 224);

    global();
    receiver_b();

    return 0;
}

