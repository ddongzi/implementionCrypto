//
// Created by dong on 22-4-24.
//
#include <malloc.h>
#include "sm2.h"

#include <string.h>

#define DEBUG

ecc_t *ec;
ecpoint_t *g;
ecpoint_t *Pb;

void global()
{
    ec= (ecc_t *)malloc(sizeof(ecc_t));
    g= (ecpoint_t *)malloc(sizeof(ecpoint_t));

    // ecc: p,a,b,g
    mpz_inits(ec->p,ec->a,ec->b,ec->n,NULL);
    mpz_set_str(ec->a,SM2_A,16);
    mpz_set_str(ec->b,SM2_B,16);
    mpz_set_str(ec->p,SM2_P,16);

#ifdef DEBUG
    gmp_printf("ECC : bits:%d\np:%Zd\na:%Zx\nb:%Zx\nn:%Zx\n",mpz_sizeinbase(ec->p,2),ec->p,ec->a,ec->b,ec->n);
#endif

    // g,gx,gy,n:
    mpz_inits(g->x,g->y,g->n,NULL);
    mpz_set_str(g->x,SM2_G_X,16);
    mpz_set_str(g->y,SM2_G_Y,16);
    mpz_set_str(g->n,SM2_N,16);
#ifdef DEBUG
    gmp_printf("G : x:%Zx\ny:%Zx\nn:%Zx\n",g->x,g->y,g->n);
#endif

}
void ecc_g_order(ecpoint_t *generator,mpz_t n)
{

}
/**
 * hard
 */
void set_ecc_order()
{
    mpz_t x,tmp,tmp2;
    mpz_init_set_ui(x,1);
    mpz_inits(tmp,tmp2,NULL);

    mpz_set_ui(ec->n,0);
    while (mpz_cmp(ec->p,x)){

        mpz_pow_ui(tmp,x,3);
        mpz_mul(tmp2,x,ec->a);
        mpz_add(tmp,tmp,tmp2);
        mpz_add(tmp,tmp,ec->b);
        mpz_set_si(tmp,mpz_legendre(tmp,ec->p));
        mpz_add(ec->n,ec->n,tmp);
        mpz_add_ui(x,x,1);

#ifdef DEBUG
        //gmp_printf("order :%d   %Zd\n",mpz_legendre(tmp,ec->p),ec->n);
#endif
    }
    mpz_add(ec->n,ec->n,ec->p);
    mpz_add_ui(ec->n,ec->n,1);

    mpz_clears(x,tmp,tmp2,NULL);
}
void ecc_add(ecpoint_t *res,ecpoint_t *point1,ecpoint_t *point2)
{

}
void ecc_kadd(ecpoint_t *res,ecpoint_t *point,mpz_t k)
{

}
void ecpoint_print(char *s,ecpoint_t *ecpoint)
{
    printf("%s  ",s);
    gmp_printf("(%Zd, %Zd)\n",ecpoint->x,ecpoint->y);
}
int ecc_isfinite(ecpoint_t *ecpoint)
{
    return 0;
}
void receiver_b()
{
    mpz_t SKb;

    gmp_randstate_t randstate;
    gmp_randinit_default(randstate);


    //SKb:  1..n-1
    mpz_init_set_ui(SKb,1);
    while (mpz_cmp_ui(SKb,0)==0)
        mpz_urandomm(SKb,randstate,g->n);

    gmp_printf("B make a SK-B: %Zx\n",SKb);

    // Pb:
    Pb= malloc(sizeof(ecpoint_t));
    ecc_kadd(Pb,g,SKb);
    ecpoint_print("B make a PK-B: ",Pb);

}
void ecpoint_init(ecpoint_t *ecpoint)
{
    mpz_t x,y;
    mpz_inits(x,y,NULL);
    mpz_set(ecpoint->x,x);
    mpz_set(ecpoint->y,y);
}
/**
 *
 * @param sk
 * @param keylen
 */
void KDF(char *sk,int keylen,char *key)
{

}
/**
 * (x,y) -> 0x00 ff 11..|55 66.
 * @param ecpoint
 */
void ecpoint_to_char(ecpoint_t *ecpoint,char *res)
{

}
void xor(uint8_t *res,uint8_t *s1,uint8_t *s2)
{

}
void cat(uint8_t *res,uint8_t *a,uint8_t *b)
{

}
int is_0(uint8_t *s)
{

}
void hash(uint8_t *res,uint8_t *m)
{

}
void clear(uint8_t *buf)
{

}
void sender_A(){
    gmp_randstate_t randstate;
    mpz_t SKa;
    ecpoint_t *C1,*S,*Share;
    uint8_t buf[BUFSIZ];
    char M[BUFSIZ];
    size_t mlen;
    size_t klen;
    uint8_t key[klen];
    uint8_t C2[klen];
    uint8_t C3[64];

    gmp_randinit_default(randstate);
    mpz_inits(SKa,NULL);

    scanf("%s",M);
    mlen=strlen(M);
    klen=mlen;

    //SKa:  1..n-1
A:  mpz_set_ui(SKa,1);
    while (mpz_cmp_ui(SKa,0))
        mpz_urandomm(SKa,randstate,g->n);

    //C1:
    C1= (ecpoint_t *)malloc(sizeof(ecpoint_t));
    ecc_kadd(C1,g,SKa);

    //S:

    // sharek
    ecpoint_init(Share);
    ecc_kadd(Share,Pb,SKa);

    //sym k:
    ecpoint_to_char(Share,buf);
    KDF(buf,klen,key);

    if (is_0(key))
        goto A;


    // C2:
    xor(C2,M,key);

    // C3:    //todo: buf[]'\0'
    cat(buf,Share->x,M);
    cat(buf,buf,Share->y);
    hash(C3,buf);

    // C:=(C1,C2,C3)

}
