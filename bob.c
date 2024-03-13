#include "bob.h"

bignum256 b = {0};
bignum256 d_ = {0};
static bignum256 private_key = {0};
static bignum256 d[LEN];

void input_b(){
    uint64_t b_;
    scanf("%lu", &b_);
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

void decrypt_message(int idx, const bignum256 *enc_m){
    curve_point key;
    bignum256 kr = {0};
    point_multiply(&secp256k1, &private_key, &alice_public_key, &key);
    
    int i = bn_testbit(&b, idx);
    bn_multiply(&key.x, &key.y, &secp256k1.prime);
    bn_mod(&key.y, &secp256k1.prime);
    get_hash(&key.y, &kr);
    bn_xor(&d[idx], &kr, &enc_m[i]);
}

void calculate_d(){
    calc_additive_share(d, &d_);
}