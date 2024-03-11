#include "cypherock.h"

void bn_set_rand(bignum256 *x){
    int t;
    uint8_t r[32];
    for (int i = 0; i < 32; i++) {
        if (!(i % 4))
            t = rand();
        r[i] = (t >> ((i % 4) * 8)) & 0xFF;
    }
    bn_read_be(r, x);
    bn_mod(x, &secp256k1.prime);
}

uint8_t hex_to_bin(const char x){
    if (x<='9' && x>='0') return (uint8_t)(x - '0');
    else if (x=='\0') return (uint8_t)(1<<4);
    return (uint8_t)(x - 'a' + 10);
}

// Doesn't work, confusion regarding positioning of bit and le or be
void str_to_bn(const char *input_str, bignum256 *out_num){
    uint8_t num_be[32];
    for (int i = 0; i<32; i++)
        num_be[i] = 0;
    for (int i = 0; i<32; i++){
        uint8_t t1 = hex_to_bin(input_str[2*i]);
        uint8_t t2 = hex_to_bin(input_str[2*i + 1]);
        if (t1>=(1<<4)) break;
        num_be[i] = t1 << 4;
        if (t2>=(1<<4)) break;
        num_be[i] |= t2;
        printf("%d ", num_be[i]);
    }
    bn_read_le(num_be, out_num);
}