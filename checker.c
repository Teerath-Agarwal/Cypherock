#include "checker.h"

extern bignum256 a,b,c_,d_;
static bignum256 res;

void print_res(int x){
    printf(x ? "Success!\n" : "Failure!\n");
    printf("a:\n");
    bn_print(&a);
    printf("b:\n");
    bn_print(&b);
    printf("res = a x b :\n");
    bn_print(&res);
    printf("c:\n");
    bn_print(&c_);
    printf("d:\n");
    bn_print(&d_);
}

void validate_result(){
    bignum256 r_rhs;
    bn_copy(&b, &res);
    bn_multiply(&a, &res, &secp256k1.prime);
    bn_mod(&res, &secp256k1.prime);

    bn_copy(&d_, &r_rhs);
    bn_addmod(&r_rhs, &c_, &secp256k1.prime);
    bn_mod(&r_rhs, &secp256k1.prime);

    print_res(bn_is_equal(&res, &r_rhs));
}