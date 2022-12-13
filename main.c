#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

#include "./include/read_file.h"
#include "./include/des.h"
#include "./include/base64.h"

static void usage(){
   fputs("Usage: ./main [-e (encode) | -d (decode)] <data file> <key file> \n\
              [-v (verbose)] - verbose output \n\
              [-g (generate)] [new key path] - generate new random key \n", 
   stderr);
}

int main(int argc, char **argv){
   int c, v = 0, i = 0, j = 0, type = -1;
   char *s = malloc(sizeof(char) * (8 + 1));
   uint8_t tmp;
   FILE *f, *k;
   uint64_t *input, *key, res, d;

   while((c = getopt(argc, argv, "hevgedh:")) != -1){
      switch(c){
         case 'v': v = 1; break; 
         case 'g': if(!argv[2]){
               fputs("No filename specified, deafult is ./key\n", stderr);
               generate_random_key("key");
            }else{
               generate_random_key(argv[2]); 
            }
            
            return 1;
         case 'e': type = DES_ENC; break;
         case 'd': type = DES_DEC; break;
         case 'h': usage(); return 1;
      }
   }

   if(optind == argc){
      usage();

      return 1;
   }

   if(type == -1){
      fputs("No type specified, default is encode.\n", stderr);

      type = DES_ENC;
   }

   f = fopen(argv[optind], "rb");
   k = fopen(argv[optind + 1], "rb");

   input = readbit(f, 64);
   key = readbit(k, 64);

   d = des(*(input), *(key), v, type);

   for(i = 7; i >= 0; i--){
      tmp = d >> (8 * i);

      s[j] = (char) tmp;
      j++;
   }

   printf("output:\t\t0x%lx (text: %s -- base64: %s)\n", d, s, encode((const unsigned char *) s, 8));

   return 0;
}