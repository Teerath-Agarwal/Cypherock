#include "cypherock.h"
#include "alice.h"
#include "bob.h"

int32_t main(){
    srand(time(NULL));
    assertions();
    int mode;
    scanf("%d", &mode);
    if (mode){
        /*
        char stra[65], strb[65];
        getchar();
        fgets(stra, sizeof(stra), stdin);
        fgets(strb, sizeof(strb), stdin);
        str_to_bn(stra, &a);
        str_to_bn(strb, &b);
        */
       input_a();
       input_b();
    }
    else {
        gen_a();
        gen_b();
    }
    alice_gen_priv_key();
    return 0;
}