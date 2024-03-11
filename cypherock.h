#ifndef CR_H
#define CR_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// #include "alice.h"
// #include "bob.h"
#include "./crypto/bignum.h"
#include "./crypto/secp256k1.h"

void bn_set_rand(bignum256 *x);
uint8_t hex_to_bin(const char x);
void str_to_bn(const char *input_str, bignum256 *out_num);

#endif