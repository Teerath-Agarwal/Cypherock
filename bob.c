#include "bob.h"

bignum256 b = {0};
bignum256 private_key = {0};

void input_b(){
    uint64_t b_;
    scanf("%lld", &b_);
    bn_read_uint64(b_, &b);
}

void gen_b(){
    bn_set_rand(&b);
}

void bob_gen_priv_key(){
    bn_set_rand(&private_key);
}