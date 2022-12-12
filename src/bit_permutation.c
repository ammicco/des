#include "./../include/bit_permutation.h"

uint64_t permutation(uint64_t source, int *table, size_t bit_number, char *numbering_mode){
    const uint64_t *masks; 
    uint64_t result = 0;
    int i, j; 

    switch((int) bit_number){
        case  8: masks  = mask8;    break;
        case 16: masks  = masks16;  break;
        case 32: masks  = mask32;   break;
        case 64: masks  = mask64;   break;
        default: return -1;
    }

    if(strcmp(numbering_mode, "msb") == 0 || strcmp(numbering_mode, "MSB") == 0){
        for(i = 0; i < bit_number; i++){
            if(i <= table[i]){
                result |= ((source << (table[i] - i)) & masks[i]);
            }else{
                result |= ((source >> (i - table[i])) & masks[i]);
            }
        }
    }else if(strcmp(numbering_mode, "lsb") == 0 || strcmp(numbering_mode, "LSB") == 0){
        j = 0;

        for(i = bit_number - 1; i >= 0; i--){
            if(i <= table[j]){
                result |= ((source >> (table[j] - i)) & masks[j]);
            }else{
                result |= ((source << (i - table[j])) & masks[j]);
            }
            
            j++;
        }
    }else{
        return -1;
    }

    return result;
}
