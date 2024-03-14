#include "cypherock.h"

curve_point alice_public_key;
curve_point bob_public_key;

void bn_set_rand(bignum256 *x, const bignum256 *nf){
    int t;
    uint8_t r[LEN/8];
    for (int i = 0; i < LEN/8; i++) {
        if (!(i % 4))
            t = rand();
        r[i] = (t >> ((i % 4) * 8)) & 0xFF;
    }
    bn_read_be(r, x);
    bn_mod(x, nf);
}

void bn_print(const bignum256 *x){
    // Following code doesn't work. It prints the limbs in hexadecimal.
    // We need the number either in decimal or hexadecimal format (better will be both)
    // Give line breaks accordingly

    for (int i=BN_LIMBS-1; i>=0; i--)
        printf("%08x ", x->val[i]);
    printf("\n");
    // printf("Hexadecimal:    ");
}

void point_subt(const ecdsa_curve *curve, curve_point *cp1,
               const curve_point *cp2){
    curve_point temp;
    point_copy(cp2, &temp);
    bn_negate(&temp.y, &secp256k1.prime);
    point_add(curve, &temp, cp1);
}

int point_is_on_curve(const curve_point *point) {
    bignum256 y_square, x_cubed;

    bn_copy(&point->y, &y_square);
    bn_multiply(&point->y, &y_square, &secp256k1.prime);
    bn_mod(&y_square, &secp256k1.prime);

    bn_copy(&point->x, &x_cubed);
    bn_multiply(&point->x, &x_cubed, &secp256k1.prime);
    bn_multiply(&point->x, &x_cubed, &secp256k1.prime);

    bn_addi(&x_cubed, 7);
    bn_mod(&x_cubed, &secp256k1.prime);

    // Check if y^2 = (x^3 + 7) % prime
    return bn_is_equal(&y_square, &x_cubed);
}

void get_hash(const bignum256 *x, bignum256 *res){
    // res should contain the SHA3_256 hash of x. 
    // Convert to array of unsigned char, 
    // i.e., (unsigned char*) back and forth to use it
    bn_copy(x,res);
    bn_inverse(res, &secp256k1.prime);
    bn_fast_mod(res, &secp256k1.prime);
    bn_mod(res, &secp256k1.prime);
}

void calc_additive_share(const bignum256 *x, bignum256 *res){
    // x is an array of length 256
    // res = sigma (0 to 255) 2^i * x[i], in the modulo domain of prime.
    bn_zero(res);
    for (uint16_t i=0; i<256; i++){
        bignum256 t;
        bn_zero(&t);
        bn_setbit(&t, i);
        bn_multiply(&x[i], &t, &secp256k1.prime);
        bn_addmod(res, &t, &secp256k1.prime);
    }
    bn_mod(res, &secp256k1.prime);
}

uint8_t hex_to_bin(const char x){
    if (x<='9' && x>='0') return (uint8_t)(x - '0');
    else if (x=='\0') return (uint8_t)(1<<4);
    return (uint8_t)(x - 'a' + 10);
}

// Doesn't work, confusion regarding positioning of bit and le or be
// Not required anymore
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

void assertions(){
    bignum256 x = {0};
    bn_set_rand(&x, &secp256k1.order);
    curve_point r, nr, nr2;
    
    // Scalar multiplication = point multiplication with G
    scalar_multiply(&secp256k1, &x, &r);
    point_multiply(&secp256k1, &x, &secp256k1.G, &nr);
    assert(point_is_equal(&r,&nr));
    assert(point_is_on_curve(&r));

    // k*G = -((-k)*G) : works when k is negated wrt order, not prime.
    bn_negate(&x, &secp256k1.order);
    scalar_multiply(&secp256k1, &x, &nr);
    assert(point_is_negative_of(&r,&nr));
    
    // P(x,y) = -Q(x,-y)
    point_copy(&r, &nr2);
    bn_negate(&nr2.y, &secp256k1.prime);
    assert(point_is_on_curve(&nr2));
    assert(point_is_negative_of(&r,&nr2));
    
    // (P + Q) - P = Q
    bn_set_rand(&x, &secp256k1.order);
    scalar_multiply(&secp256k1, &x, &nr);
    point_copy(&nr, &nr2);
    point_add(&secp256k1, &r, &nr);
    point_subt(&secp256k1, &nr, &r);
    assert(point_is_equal(&nr2,&nr));
}