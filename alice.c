#include "alice.h"

bignum256 a = {0};
bignum256 private_key = {0};

void input_a(){
    uint64_t a_;
    scanf("%lld", &a_);
    bn_read_uint64(a_, &a);
}

void gen_a(){
    bn_set_rand(&a);
}

void alice_gen_priv_key(){
    bn_set_rand(&private_key);
}