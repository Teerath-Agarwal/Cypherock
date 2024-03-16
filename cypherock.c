#include "cypherock.h"

curve_point alice_public_key;
curve_point bob_public_key;
int mode;

#define DLEN 78
char dec_2powi[LEN][DLEN];

void pre(){
    dec_2powi[0][0] = 1;
    for (int i=1; i<DLEN; i++)
        dec_2powi[0][i] = 0;
    int carry = 0;
    for (int i=1; i<LEN; i++)
        for (int j=0; j<DLEN; j++){
            dec_2powi[i][j] = 2*dec_2powi[i-1][j] + carry;
            carry = dec_2powi[i][j]/10;
            dec_2powi[i][j] %= 10;
        }
}

static void add_dec(char *a, int idx){
    int carry = 0;
    for (int i=0; i<DLEN; i++){
        a[i] += dec_2powi[idx][i] + carry;
        carry = a[i]/10;
        a[i] %= 10;
    }
}

void bn_set_rand(bignum256 *x, const bignum256 *nf){
    int t;
    uint8_t r[LEN/8];
    do{
        for (int i = 0; i < LEN/8; i++) {
            if (!(i % 4))
                t = rand();
            r[i] = (t >> ((i % 4) * 8)) & 0xFF;
        }
        bn_read_be(r, x);
        bn_mod(x, nf);
    } while (bn_is_zero(x));
}

void bn_print(const bignum256 *x){
    uint8_t hex[LEN/8];
    char dec[DLEN];
    bn_write_le(x, hex);
    int flag = 0;
    printf("\tHexadecimal: ");
    for (int i=LEN/8-1; i>=0; i--)
        if ( (flag |= !!hex[i]) || !mode)
            printf("%02x", hex[i]);
    printf("\n");

    for (int i=0; i<DLEN; i++)
        dec[i] = 0;
    for (int i=0; i<LEN; i++)
        if ((hex[i/8] >> (i%8)) & 1) add_dec(dec, i);
    flag = 0;
    printf("\tDecimal:     ");
    for (int i=DLEN-1; i>=0; i--)
        if ( (flag |= !!dec[i]) || !mode)
            printf("%c", dec[i]+'0');
    printf("\n");
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
    uint8_t in[LEN/8];
    uint8_t out[LEN/8];
    bn_write_le(x,in);
    sha3_256(in, LEN/8, out);
    bn_read_le(out, res);
}

void calc_additive_share(const bignum256 *x, bignum256 *res){
    // x is an array of length 256
    // res = sigma (0 to LEN) 2^i * x[i], in the modulo domain of prime.
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