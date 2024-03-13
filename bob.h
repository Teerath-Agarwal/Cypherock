#ifndef BOB
#define BOB

#include "cypherock.h"

void input_b();
void gen_b();
void bob_gen_priv_key();
void bob_gen_public_key(uint16_t idx);
void decrypt_message(int idx, const bignum256 *enc_m);
void calculate_d();

#endif