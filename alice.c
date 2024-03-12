#include "alice.h"

bignum256 a = {0};
bignum256 private_key = {0};
bignum256 c[256];

void input_a(){
    uint64_t a_;
    scanf("%lld", &a_);
    bn_read_uint64(a_, &a);
}

void gen_a(){
    bn_set_rand(&a, &secp256k1.prime);
}

void alice_gen_priv_key(){
    bn_set_rand(&private_key, &secp256k1.order);
}

void alice_gen_public_key(){
    scalar_multiply(&secp256k1, &private_key, &alice_public_key);
}