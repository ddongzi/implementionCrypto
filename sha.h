//
// Created by dong on 22-4-22.
//

#ifndef MYCRYPTO_SHA_H
#define MYCRYPTO_SHA_H

#endif //MYCRYPTO_SHA_H
#include <stdint.h>

#define ROT_LEFT_SHIFT(x,n) ((x)<<(n)|(x)>>(32-(n)))

#define F(b,c,d)    (((b)&(c))|((~(b))&(d)))
#define G(b,c,d)    ((b)^(c)^(d))
#define H(b,c,d)    (((b)&(c))|((b)&(d))|((c)&(d)))
#define I(b,c,d)    ((b)^(c)^(d))
void init();
void update(char m[]);
uint32_t *hexdigest();
char *digest();
