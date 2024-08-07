#include "cypherock.h"
#include "alice.h"
#include "bob.h"
#include "checker.h"

int32_t main(){
    srand(time(NULL));
    // assertions();
    mode = 0;
    scanf("%d", &mode);
    if (mode){
       input_a();
       input_b();
    }
    else {
        gen_a();
        gen_b();
    }
    for (int i=0; i<LEN; i++){
        alice_gen_priv_key();
        bob_gen_priv_key();
        alice_gen_public_key();
        bob_gen_public_key(i);
        bignum256 encrypted_messages[2];
        get_encrypted_messages(i, encrypted_messages);
        decrypt_message(i, encrypted_messages);
    }
    calculate_c();
    calculate_d();
    validate_result();
    return 0;
}