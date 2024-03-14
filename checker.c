#include "checker.h"

extern bignum256 a,b,c_,d_;
static bignum256 r_lhs, r_rhs;

void print_res(int x){
    printf(x ? "Success!\n" : "Failure!\n");
    printf("a:\n");
    bn_print(&a);
    printf("b:\n");
    bn_print(&b);
    printf("LHS = a x b :\n");
    bn_print(&r_lhs);
    printf("c:\n");
    bn_print(&c_);
    printf("d:\n");
    bn_print(&d_);
    printf("RHS = c + d :\n");
    bn_print(&r_rhs);
}

void validate_result(){
    bn_copy(&b, &r_lhs);
    bn_multiply(&a, &r_lhs, &secp256k1.prime);
    bn_mod(&r_lhs, &secp256k1.prime);

    bn_copy(&d_, &r_rhs);
    bn_addmod(&r_rhs, &c_, &secp256k1.prime);
    bn_mod(&r_rhs, &secp256k1.prime);

    pre();
    print_res(bn_is_equal(&r_lhs, &r_rhs));
}