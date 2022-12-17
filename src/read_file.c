#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "./../include/read_file.h"

void *readNbit(FILE *f, size_t bit){
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

uint64_t *read_all_file(FILE *file, size_t bit){ 
    size_t file_len;
    int i, j;
    void *tmp = malloc(bit / 8);
    uint64_t *res;

    fseek(file, 0, SEEK_END);
    file_len = ftell(file);
    rewind(file);

    res = (uint64_t *) malloc(sizeof(uint64_t) * ((file_len / 8) + 2));

    for(j = 0; j < (file_len / 8); j++){
        *((uint64_t *) tmp) = 0;
        
        for(i = 1; i <= (bit / 8); i++){
            fread(&(res[j]), 1, 1, file);
            *((uint64_t *) tmp) = (uint64_t) (*((uint64_t *) tmp) | (res[j] << (bit - (i * 8))));
        }

        res[j] = (uint64_t) *((uint64_t *) tmp);
    }

    if(file_len % 8 != 0){
        *((uint64_t *) tmp) = 0;
        
        for(i = 1; i <= file_len % 8; i++){
            fread(&(res[j]), 1, 1, file);
            *((uint64_t *) tmp) = (uint64_t) (*((uint64_t *) tmp) | (res[j] << (bit - (i * 8))));
        }

        res[j] = (uint64_t) *((uint64_t *) tmp);
    }

    res[j + 1] = 0;

    return res;
}