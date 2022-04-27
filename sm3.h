//
// Created by dong on 22-4-23.
//

#ifndef MYCRYPTO_SM3_H
#define MYCRYPTO_SM3_H

#endif //MYCRYPTO_SM3_H
#define ROT_LEFT_SHIFT(x,n) ((x)<<(n)|(x)>>(32-(n)))
#define P1(x)   ((x)^(ROT_LEFT_SHIFT((x),15))^(ROT_LEFT_SHIFT((x),23)))
#define P0(x)   ((x)^(ROT_LEFT_SHIFT((x),9))^(ROT_LEFT_SHIFT((x),17)))
#include <stdint.h>
void init();
void update(char m[]);
uint32_t *hexdigest();

