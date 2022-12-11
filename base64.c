#include "base64.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *encode(const unsigned char *s, size_t byte){
    int l, i, diff;
    char *base64;
    long start;

    l = (byte  * 8) % 6 == 0 ? (byte  * 8) / 6 : ((byte  * 8) / 6) + 1;
    base64 = (char *) malloc(sizeof(char) * (l + 4));

    if(!base64){
        fputs("Error: malloc() error in function encode().\nexit.", stderr);
        return NULL;
    }

    start = (long) base64;

    for(i = 0; i <= byte; i += 3){
        *base64++ = (char) alphabet[(s[i] >> 2) & 0x3f];
        *base64++ = (char) alphabet[((s[i] << 4) | (s[i + 1] >> 4)) & 0x3f];
        *base64++ = (char) alphabet[((s[i + 1] << 2) | (s[i + 2] >> 6)) & 0x3f];
        *base64++ = (char) alphabet[s[i + 2] & 0x3f];
    }

    diff = ((start - (long) base64) - l) < 0 ? -(start - (long) base64) - l : (start - (long) base64) - l;

    if(diff > 0){
        base64--;

        if(diff == 2){
            base64--;
            *base64++ = '=';
        }

        *base64++ = '=';
    }

    *base64++ = '\0';

    return (char *) start;
}

static int get_index(char c){
    int i = 0; 

    while(c != alphabet[i]){
        i++;
    }

    return i;
}

char *decode(const unsigned char *s, size_t byte){
    int l, i;
    int *indexes;
    char *plain;
    long start;

    indexes = (int *) malloc(sizeof(int) * (int) byte);
    
    if(!indexes){
        fputs("Error: malloc() error in function decode().\nexit.", stderr);
        return NULL;
    }

    if(s[byte - 2] == '='){
        byte--;
    }
    
    if(s[byte - 1] == '='){
        byte--;
    }

    l = byte * 6 / 8;
    plain = (char *) malloc(sizeof(char) * (l * 12));

    if(!indexes){
        fputs("Error: malloc() error in function decode().\nexit.", stderr);
        return NULL;
    }

    start = (long) plain;

    for(i = 0; i < (int) byte; i++){
        indexes[i] = get_index(s[i]);
    }

    for(i = 0; i < (int) byte; i += 4){
        *plain++ = (char) ((indexes[i] << 2) | indexes[i + 1] >> 4) & 0xff;
        *plain++ = (char) ((indexes[i + 1] << 4) | indexes[i + 2] >> 2) & 0xff;
        *plain++ = (char) ((indexes[i + 2] << 6) | indexes[i + 3]) & 0xff;
    }

    return (char *) start;
}
