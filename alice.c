#include "alice.h"

bignum256 a = {0};
bignum256 c_ = {0};
static bignum256 private_key = {0};
static bignum256 c[LEN];

void input_a(){
    uint64_t a_;
    scanf("%lu", &a_);
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

void get_encrypted_messages(int idx, bignum256 *enc_m){
    bn_set_rand(&c[idx], &secp256k1.prime);
    bn_copy(&c[idx], &enc_m[0]);
    bn_copy(&c[idx], &enc_m[1]);
    bn_addmod(&enc_m[1], &a, &secp256k1.prime);
    bn_mod(&enc_m[1], &secp256k1.prime);
    curve_point key[2];
    point_multiply(&secp256k1, &private_key, &bob_public_key, &key[0]);

    curve_point bob_pubkey_B_A;
    point_copy(&bob_public_key, &bob_pubkey_B_A);
    point_subt(&secp256k1, &bob_pubkey_B_A, &alice_public_key);
    point_multiply(&secp256k1, &private_key, &bob_pubkey_B_A, &key[1]);

    bignum256 k[2];
    for (int i = 0; i < 2; i++)
    {
        bn_multiply(&key[i].x, &key[i].y, &secp256k1.prime);
        bn_mod(&key[i].y, &secp256k1.prime);
        get_hash(&key[i].y, &k[i]);
        bn_xor(&enc_m[i], &k[i], &enc_m[i]);
    }
}

void calculate_c(){
    calc_additive_share(c, &c_);
    bn_negate(&c_, &secp256k1.prime);
}