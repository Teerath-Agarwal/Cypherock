#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "alice.h"
#include "./crypto/bignum.h"
#include "bob.h"

void bn_set_rand(bignum256 *x){
    srand(time(NULL));
    int t;
    uint8_t r[32];
    for (int i = 0; i < 32; i++) {
        if (!(i % 4))
            t = rand();
        r[i] = (t >> ((i % 4) * 8)) & 0xFF;
    }
    bn_read_be(r, &x);
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

void exec__(){
    int mode;
    scanf("%d", &mode);
    bignum256 a = {0}, b = {0};
    if (mode){
        /*
        char stra[65], strb[65];
        getchar();
        fgets(stra, sizeof(stra), stdin);
        fgets(strb, sizeof(strb), stdin);
        str_to_bn(stra, &a);
        str_to_bn(strb, &b);
        */
       uint64_t a_,b_;
       scanf("%lld %lld", &a_, &b_);
       bn_read_uint64(a_, &a);
       bn_read_uint64(b_, &b);
    }
    else {
        bn_set_rand(&a);
        bn_set_rand(&b);
    }

}

int32_t main(){
    int T;
    scanf("%d", &T);
    while (T--)
    exec__();
    return 0;
}