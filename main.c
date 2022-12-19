#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "./include/read_file.h"
#include "./include/des.h"
#include "./include/base64.h"

static void usage(){                                                                   /* display help function */
   fputs("Usage: ./main [-e (encode) | -d (decode)] <data file> <key file> \n\
              Options:\n\
              [-v (verbose)] - verbose output \n\
              [-g (generate)] [new key path] - generate new random key \n\
              [-o (output file) <output filename>] - save the output in a file\n\
              [-h (help)] - display this help\n", 
   stderr);
}

int main(int argc, char **argv){
   int i = 0, j = 0, x = 0;                                                               /* variables for loops */
   int c, type = -1, out = 0, v = 0;                                                      /* variables for cli args */

   char *tmp_str = (char *) malloc(sizeof(char) * (8 + 1));                         /* string for print single des block output */
   char *output_str;                                                                      /* string for print all the blocks (ECB) */
   char *output_f_name = NULL;                                                            /* file name for output in a file */
   
   FILE *file, *k, *output_file;                                                          /* files for: encode/decode file, key file and output file */
   
   size_t len;                                                        
   
   uint8_t tmp;                                                                           /* for read single byte from des output */
   uint64_t *input, *key, d;                                                              /* for des things */

   if(!tmp_str){                                                                          /* check if malloc return correctly */
      fputs("tmp_str malloc() error in function main()!\nExit.\n", stderr);

      return 2;
   }

   while((c = getopt(argc, argv, "vgedo:h")) != -1){                            /* parse cli args */
      switch(c){
         case 'v': v = 1; break;                                                          /* verbose output */
         case 'g': if(!argv[2]){                                                          /* generate des 64 (56) bit key in a file */
               fputs("No filename specified, deafult is ./key\n", stderr);
               generate_random_key("key");
            }else{
               generate_random_key(argv[2]); 
            }
            
            return 1;
         case 'e': type = DES_ENC; break;                                                 /* encode */
         case 'd': type = DES_DEC; break;                                                 /* decode */
         case 'o': out = 1; output_f_name = optarg; break;                                /* output in a file*/
         case 'h': usage(); return 1;                                                     /* display help */
         case '?': printf("Exit.\n"); return 4;                                    /* handle unknow options */
      }
   }

   if(optind == argc || argc == 1){                                                       /* if optrions number == argc          */
      usage();                                                                            /* there's no file and key files       */
                                                                                          /* specified, so display help and exit */
      return 1;
   }

   if(type == -1){                                                                        /* if user doesn't specify encode or   */
      fputs("No type specified, default is encode.\n", stderr);                 /* decode mode, default is encode      */

      type = DES_ENC;
   }

   file  = fopen(argv[optind], "rb");                                               /* open files for data and key */
   k     = fopen(argv[optind + 1], "rb");

   if(!file || !k){                                                                                /* check id fopen returns correctly */
      fputs("file or key malloc() error in function main()!\nExit.\n", stderr);

      return 2;
   }

   input = read_all_file(file, 64);                            /* read all the data file and save in 64 bit chunk array (last cell is 0) */
   key   = readNbit(k, 64);                              /* read 64 bit from the key's file                          */

   if(out == 1){                                                          /* if user output in file, calculate the file length */
      fseek(file, 0, SEEK_END);
      len = ftell(file);           

      output_str = (char *) malloc(8 * len);                         /* and allocate space for the output string to save in file */                       
   
      if(!output_f_name){                                                                             /* if user not specify output file name, exit */
         fputs("No filename specified for output, default!\nUse \"-o <output file name>\"\nExit.\n", stderr);

         return 3;
      }

      output_file = fopen(output_f_name, "wb");                                       /* open output file */                        

      if(!output_file){                                                                               /* check if fopen returns correctly */
         fputs("output_file malloc() error in function main()!\nExit.\n", stderr);

         return 2;
      }
   }

   while(i < len / 8){                                                   /* execute des ECB, encode/decode every block and concatenate all */
      x = 0;

      d = des(input[i], *(key), v, type);    /* execute des on 64 bit block */

      for(j = 7; j >= 0; j--){                                             /* transofrm des output in a string */
         tmp = d >> (8 * j);

         tmp_str[x] = (char) tmp;
         x++;
      }

      printf("Output:\t\t0x%lx (text: %s -- base64: %s)\n", 
                     d, 
                     tmp_str, 
                     encode((const unsigned char *) tmp_str, 8)
      );

      if(out == 1){                                                        /* if have to save output in file, concatenate the single block string */
         fwrite(tmp_str, 8, 1, output_file);
      }

      i++;
   }

   free(tmp_str);
   fclose(output_file);
   fclose(file);
   fclose(k);

   return 0;                                                                                          /* finally return */
}