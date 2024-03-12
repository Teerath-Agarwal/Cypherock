#include "bob.h"

bignum256 b = {0};
bignum256 private_key = {0};
bignum256 d[256];

void input_b(){
    uint64_t b_;
    scanf("%lld", &b_);
    bn_read_uint64(b_, &b);
}

void gen_b(){
    bn_set_rand(&b, &secp256k1.prime);
}

void bob_gen_priv_key(){
    bn_set_rand(&private_key, &secp256k1.order);
}

void bob_gen_public_key(uint16_t idx){
    scalar_multiply(&secp256k1, &private_key, &bob_public_key);
    if (bn_testbit(&b, idx)) {
        point_add(&secp256k1, &alice_public_key, &bob_public_key);
    }
}