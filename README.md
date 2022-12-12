# DES

Here's a simple implementation of Data Encryption Standard in <img src = "https://raw.githubusercontent.com/rahulbanerjee26/githubAboutMeGenerator/main/icons/c.svg" width = "20px"> language.

It works reading a 64 bit block from a file and a 64 bit key from another file, after that encode with DES algorithm.
The DES encoded string is displayed in text ascii format and also base 64 format. 
You also can run with "-v" option for verbose output that displays:
 - the key
 - every round left and right half of data 
 - every round subkey

## Usage: 
``` bash 
    $ make
      gcc -c manipulate_string.c
      gcc -c base64.c
      gcc -c bit_permutation.c
      gcc -c read_file.c
      gcc -c des.c
      gcc -c main.c
      gcc -o main manipulate_string.o base64.o bit_permutation.o read_file.o des.o main.o
    $ ./main [-v] "data file" "key file"
```
To clean: 
``` bash
    $ make clean
      rm *.o main
``` 