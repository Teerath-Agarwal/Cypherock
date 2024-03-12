#ifndef CR_H
#define CR_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// #include "alice.h"
// #include "bob.h"
#include "./crypto/bignum.h"
#include "./crypto/ecdsa.h"
#include "./crypto/secp256k1.h"

extern curve_point alice_public_key;
extern curve_point bob_public_key;

void bn_set_rand(bignum256 *x, const bignum256 *nf);
uint8_t hex_to_bin(const char x);
void str_to_bn(const char *input_str, bignum256 *out_num);

#endif