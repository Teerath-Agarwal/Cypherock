#ifndef CR_H
#define CR_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "./crypto/bignum.h"
#include "./crypto/ecdsa.h"
#include "./crypto/secp256k1.h"
#include "./crypto/sha3.h"

#define LEN 256
extern curve_point alice_public_key;
extern curve_point bob_public_key;

// Assumes x, prime are normalised and x <= prime
static inline void bn_negate(bignum256 *x){
    bn_subtract(&secp256k1.prime, x, x);
}

void bn_set_rand(bignum256 *x, const bignum256 *nf);

void bn_print(const bignum256 *x);

// cp1 = cp1 - cp2
void point_subt(const ecdsa_curve *curve, curve_point *cp1,
               const curve_point *cp2);

void get_hash(const bignum256 *x, bignum256 *res);

void calc_additive_share(const bignum256 *x, bignum256 *res);

uint8_t hex_to_bin(const char x);
void str_to_bn(const char *input_str, bignum256 *out_num);

void assertions();

#endif