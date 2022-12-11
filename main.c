#include <unistd.h>

#include "read_file.h"
#include "des.h"
#include "base64.h"

static void usage(){
   fputs("Usage: ./des [-v] \"data file\" \"key file\"\n", stderr);
}

int main(int argc, char **argv){
   int c, v = 0, i, j = 0;
   char *s = malloc(sizeof(char) * (8 + 1));
   uint8_t tmp;
   FILE *f, *k;
   uint64_t *input, *key, res, d;

   if(argc < 3){
      usage();

      return -1;
   }

   while((c = getopt(argc, argv, "hv:")) != -1){
      switch(c){
         case 'h': usage(); return -2;
         case 'v': v = 1; break; 
      }
   }

   if(argc < 4){
      f = fopen(argv[1], "rb");
      k = fopen(argv[2], "rb");
   }else{
      f = fopen(argv[2], "rb");
      k = fopen(argv[3], "rb");
   }

   input = readbit(f, 64);
   key = readbit(k, 64);

   d = des(*(input), *(key), v);

   for(i = 7; i >= 0; i--){
      tmp = d >> (8 * i);

      s[j] = (char) tmp;
      j++;
   }

   printf("des:\t\t0x%lx (text: %s -- base64: %s)\n", d, s, encode((const unsigned char *) s, 8));

   return 0;
}