#ifndef ALICE
#define ALICE

#include "cypherock.h"

void input_a();
void gen_a();
void alice_gen_priv_key();
void alice_gen_public_key();
void get_encrypted_messages(int idx, bignum256 *enc_m);
void calculate_c();

#endif