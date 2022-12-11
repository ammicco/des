#include "read_file.h"
#include <stdint.h>
#include <stdlib.h>

void *readbit(FILE *f, size_t bit){
    int i, len;
    void *tmp = malloc(bit / 8); 
    uint64_t r = 0;

    *((uint64_t *) tmp) = 0;

    for(i = 1; i <= bit / 8; i++){
        fread(&r, 1, 1, f);
        *((uint64_t *) tmp) = (uint64_t) (*((uint64_t *) tmp) | (r << (bit - (i * 8))));
    }

    return tmp;
}