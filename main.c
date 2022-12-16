#include <unistd.h>

#include "./include/read_file.h"
#include "./include/des.h"
#include "./include/base64.h"

static void usage(){
   fputs("Usage: ./main [-e (encode) | -d (decode)] <data file> <key file> \n\
              [-v (verbose)] - verbose output \n\
              [-g (generate)] [new key path] - generate new random key \n\
              [-o (output file)] [output filename] - save the output in a file\n\
              [-h (help)] - display this help\n", 
   stderr);
}

int main(int argc, char **argv){
   int c, v = 0, i = 0, j = 0, type = -1, out = 0;
   char *s = (char *) malloc(sizeof(char) * (8 + 1));
   char *ofn = NULL;
   uint8_t tmp;
   FILE *f, *k, *o;
   uint64_t *input, *key, res, d;

   while((c = getopt(argc, argv, "vgedo::h")) != -1){
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
         case 'o': out = 1; ofn = optarg; break;
         case 'h': usage(); return 1;
         case '?': printf("Exit.\n"); return 4;
      }
   }

   if(optind == argc || argc == 1){
      usage();

      return 1;
   }

   if(type == -1){
      fputs("No type specified, default is encode.\n", stderr);

      type = DES_ENC;
   }

   f = fopen(argv[optind], "rb");
   k = fopen(argv[optind + 1], "rb");

   if(!f || !k){
         fputs("malloc() error in function main()!\nExit.\n", stderr);

         return 2;
      }

   input = readbit(f, 64);
   key = readbit(k, 64);

   d = des(*(input), *(key), v, type);

   for(i = 7; i >= 0; i--){
      tmp = d >> (8 * i);

      s[j] = (char) tmp;
      j++;
   }

   printf("Output:\t\t0x%lx (text: %s -- base64: %s)\n", d, s, encode((const unsigned char *) s, 8));

   if(out == 1){
      if(!ofn){
         if(type == DES_DEC){
            fputs("No filename specified for output, default is \"out.dec\"", stderr);
         }else{
            fputs("No filename specified for output, default is \"out.enc\"", stderr);
         }
      }

      o = fopen(ofn ? ofn : "out", "wb");

      if(!o){
         fputs("malloc() error in function main()!\nExit.\n", stderr);

         return 2;
      }

      fputs(s, o);
   }

   free(s);

   return 0;
}