//
// Created by dong on 22-4-24.
//

#ifndef MYCRYPTO_SM2_H
#define MYCRYPTO_SM2_H

#endif //MYCRYPTO_SM2_H
#include <gmp.h>
#include "sm3.h"

#define SM2_P     "8542D69E 4C044F18 E8B92435 BF6FF7DE 45728391 5C45517D 722EDB8B 08F1DFC3"
#define SM2_A     "787968B4 FA32C3FD 2417842E 73BBFEFF 2F3C848B 6831D7E0 EC65228B 3937E498"
#define SM2_B     "63E4C6D3 B23B0C84 9CF84241 484BFE48 F61D59A5 B16BA06E 6E12D1DA 27C5249A"
#define SM2_N     "8542D69E 4C044F18 E8B92435 BF6FF7DD 29772063 0485628D 5AE74EE7 C32E79B7"
#define SM2_G_X   "421DEBD6 1B62EAB6 746434EB C3CC315E 32220B3B ADD50BDC 4C4E6C14 7FEDD43D"
#define SM2_G_Y   "0680512B CBB42C07 D47349D2 153B70C4 E5D7FDFC BFA36EA1 A85841B9 E46E09A2"

typedef struct {
    mpz_t p;
    mpz_t a;
    mpz_t b;
    mpz_t n;
}ecc_t;

typedef struct {
    mpz_t x;
    mpz_t y;
    mpz_t n;
}ecpoint_t;
void receiver_b();
void sender_A();
void global();

void ecc_g_order(ecpoint_t *generator,mpz_t n);
void set_ecc_order();
void ecc_kadd(ecpoint_t *res,ecpoint_t *point,mpz_t k);
void ecc_add(ecpoint_t *res,ecpoint_t *point1,ecpoint_t *point2);