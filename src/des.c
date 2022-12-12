#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "./../include/des.h"

/* return a 64 bit permutation of the initial value */
uint64_t initial_permutation(uint64_t input){
    return permutation(input, (int *) initial_permutation_table, 64, "msb");
}  

/* return a 56 bit reduced key */
uint64_t key_reduce(uint64_t key){
    uint64_t s56k = permutation(key, (int *) key_permutation_table, 64, "msb");

    s56k >>= 8;
    s56k &= 0xffffffffffffff; /* 56 right bit mask */

    return s56k;
}

/* return a 48 bit subkeys */
uint64_t *subkeys_generate(uint64_t k56b){
    int i;
    uint64_t sk = 0;
    uint64_t *keys = (uint64_t *) malloc(sizeof(uint64_t) * 16);
    uint32_t r = (uint32_t) k56b & 0xfffffff; /* 28 right bit mask*/
    uint32_t l = (uint32_t) (k56b >> 28) & 0xfffffff; /* 28 right bit mask*/
    uint32_t tmp;

    if(!keys){
        fputs("malloc() error in function subkey_generate()!\nExit.\n", stderr);
    }

    for(i = 0; i < 16; i++){
        tmp = l;

        l <<= key_shift_table[i];
        l &= 0xfffffff; /* 28 right bit mask*/
        l |= tmp >> (28 - key_shift_table[i]);

        tmp = r;

        r <<= key_shift_table[i];
        r &= 0xfffffff; /* 28 right bit mask*/
        r |= tmp >> (28 - key_shift_table[i]);

        sk = 0;

        sk |= (uint64_t) ((uint64_t) l << 28);
        sk |= r;
        sk &= 0xffffffffffffff; /* 48 right bit mask*/

        sk <<= 8;
        sk = key_compression(sk);

        keys[i] = sk;
    }

    return keys;
}

/* return a compressed 48 bit key */
uint64_t key_compression(uint64_t key){
    uint64_t s48b = permutation(key, (int *) compression_key_table, 64, "msb");

    s48b >>= 16;
    s48b &= 0xffffffffffff; /* 48 right bit mask*/

    return s48b;
}

/* return an expansion from 32 bit of data to 48 bit of data */
uint64_t expansion(uint32_t input){
    uint64_t e = permutation((uint64_t) ((uint64_t) input << 32), (int *) expansion_permutation, 64, "msb");

    e >>= 16;
    e &= 0xffffffffffff; /* 48 right bit mask*/

    return e;
}  

/* return a 32 bit data passed in sbox from 48 bit input */
uint32_t sbox(uint64_t data){
    int i, j = 0;
    uint8_t row, column;
    uint8_t *sbi = (uint8_t *) malloc(sizeof(uint8_t) * 8);
    uint8_t *sbo = (uint8_t *) malloc(sizeof(uint8_t) * 8);
    uint32_t out = 0;

    if(!sbi || !sbo){
        fputs("malloc() error in function sbox()!\nExit.\n", stderr);
    }

    for(i = 7; i >= 0; i--){
        sbi[i] = (data >> (j * 6)) & 0x3f;
        j++;
    }

    for(i = 0; i < 8; i++){
        row = ((sbi[i] & 0x1) | ((sbi[i] >> 4) & 0x2)) & 0x3;
        column = (sbi[i] & 0x1e) >> 1;

        switch(i){
            case 0: sbo[i] = sbox1[row][column]; break;
            case 1: sbo[i] = sbox2[row][column]; break;
            case 2: sbo[i] = sbox3[row][column]; break;
            case 3: sbo[i] = sbox4[row][column]; break;
            case 4: sbo[i] = sbox5[row][column]; break;
            case 5: sbo[i] = sbox6[row][column]; break;
            case 6: sbo[i] = sbox7[row][column]; break;
            case 7: sbo[i] = sbox8[row][column]; break;
            default: fputs("sbox() function error!\nExit.\n", stderr);
        }

        sbo[i] &= 0xf;
    }

    j = 0;

    for(i = 7; i >= 0; i--){
        out |= sbo[i] << (4 * j);
        j++;
    }

    free(sbi);
    free(sbo);

    return out;
}

/* return 32 bit value after pbox permutation */
uint32_t pbox(uint32_t data){
    return permutation((uint64_t) data << 32, (int *) pbox1, 64, "msb") >> 32;
}

/* return 64 bit value after final permutation */
uint64_t final_permutation(uint64_t data){
    return permutation(data, (int *) final_permutation_table, 64, "msb");
}

/* return a 64 bit value represent a random key */
uint64_t generate_random_key(char *file){
    int i, j = 0;
    char *s = (char *) malloc(sizeof(char) * (8 + 1));
    uint8_t tmp;
    uint32_t tk;
    uint64_t key;
    FILE *f = fopen(file, "w");

    if(!s || !f){
        fputs("malloc() or fopen() error in function generate_random_key()!\nExit.\n", stderr);
    }

    srand(time(NULL));
    key = rand() | (uint64_t) rand() << 32;

    for(i = 7; i >= 0; i--){
        tmp = key >> (8 * i);

        s[j] = (char) tmp;
        j++;
    }

    fputs(s, f);
    printf("Generated key: 0x%lx (%s)\n", key, s);
    free(s);

    return key;
}

/* use all the previus function to encode with des */
uint64_t des(uint64_t data, uint64_t key, int verbose, int type){
    int i, j = 0, ik; 
    char *s = (char *) malloc(sizeof(char) * (8 + 1));
    char *k = (char *) malloc(sizeof(char) * (8 + 1));
    uint8_t tmps, tmpk;
    uint64_t *roundk, f = 0;
    uint32_t r, l, tmpr;

    if(!s || !k){
        fputs("malloc() error in function des()!\nExit.\n", stderr);
    }

    for(i = 7; i >= 0; i--){
        tmps = data >> (8 * i);
        tmpk = key >> (8 * i);

        s[j] = (char) tmps;
        k[j] = (char) tmpk;
        j++;
    }
    
    printf("input:\t\t0x%lx (%s)\n", data, s);

    if(verbose){
        fprintf(stderr, "Key:\t\t0x%lx (%s)\n", key, k);
    }

    free(s);
    free(k);

    data = initial_permutation(data);
    roundk = subkeys_generate(key_reduce(key));

    if(type){
        ik = 0;
    }else{
        ik = 15;
    }

    for(i = 0; i < 15; i++){
        r = (uint32_t) (data & 0xffffffff);
        l = (uint32_t) ((data >> 32) & 0xffffffff);
        tmpr = r;

        r = (pbox(sbox((roundk[ik] ^ expansion(r)))) ^ l);
        l = tmpr;

        data = 0;

        if(verbose){
            if(i < 9){
                fprintf(stderr, "Round %d:  left - 0x%x, right - 0x%x, roundkey - 0x%lx\n", i + 1, l, r, roundk[i]);
            }else{
                fprintf(stderr, "Round %d: left - 0x%x, right - 0x%x, roundkey - 0x%lx\n", i + 1, l, r, roundk[i]);
            }
        }

        data |= r;
        data |= (uint64_t) ((uint64_t) l << 32);

        if(type){
            ik++;
        }else{
            ik--;
        }
    }

    r = (uint32_t) (data & 0xffffffff);
    l = (uint32_t) ((data >> 32) & 0xffffffff);
    tmpr = r;
    
    l = (pbox(sbox(roundk[ik] ^ expansion(r))) ^ l); 
    r = tmpr;

    f |= r;
    f |= (uint64_t) ((uint64_t) l << 32);

    if(verbose){
        fprintf(stderr, "Round %d: left - 0x%x, right - 0x%x, roundkey - 0x%lx\n", i + 1, l, r, roundk[i]);
    }

    return final_permutation(f);
}