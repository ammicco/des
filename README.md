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
      gcc -o ./obj/manipulate_string.o -c ./src/manipulate_string.c
      gcc -o ./obj/base64.o -c ./src/base64.c
      gcc -o ./obj/bit_permutation.o -c ./src/bit_permutation.c
      gcc -o ./obj/read_file.o -c ./src/read_file.c
      gcc -o ./obj/des.o -c ./src/des.c
      gcc -o ./obj/main.o -c main.c
      gcc -o main ./obj/manipulate_string.o ./obj/base64.o ./obj/bit_permutation.o ./obj/read_file.o ./obj/des.o ./obj/main.o
    $ ./main [-v] "data file" "key file"
```
To clean: 
``` bash
    $ make clean
      rm ./obj/*.o main
``` 

### Example
 - Create a file called "file" and another one called "key": 
```  bash
    $ touch file key
```
 - Write text in these files: 
``` bash
    $ echo "testdata" > file # 64 bit
    $ echo "test key" > key  # 64 bit 
    # you can also write more than 64 bit but it reads only the first 64
```
 - Compile the program:
``` bash
    $ make
```
 - Run the test: 
  - no verbose: 
``` bash
    $ ./main file key
      Plain text:     0x7465737464617461 (testdata)
      des:            0x1f411241ddcb52b4 (text: AA��R� -- base64: H0ESQd3LUrQ=)
``` 
  - verbose: 
``` bash 
    $ ./main -v file key
      Plain text:     0x7465737464617461 (testdata)
      Key:            0x74657374206b6579 (test key)
      Round 1:  left - 0xff0004, right - 0x339716c4, roundkey - 0xd0be66572870
      Round 2:  left - 0x339716c4, right - 0xa2b8045c, roundkey - 0xe0ae76c0c217
      Round 3:  left - 0xa2b8045c, right - 0xfbcd7be3, roundkey - 0xe4f636d70688
      Round 4:  left - 0xfbcd7be3, right - 0xf0951c1f, roundkey - 0xe69772981349
      Round 5:  left - 0xf0951c1f, right - 0x5ebb84c, roundkey - 0xeed27312f224
      Round 6:  left - 0x5ebb84c, right - 0xbd0315a8, roundkey - 0xafd35a702da0
      Round 7:  left - 0xbd0315a8, right - 0x3bbcc0b8, roundkey - 0x2e53dba8281b
      Round 8:  left - 0x3bbcc0b8, right - 0xa0745a68, roundkey - 0x3f5959677212
      Round 9:  left - 0xa0745a68, right - 0xd3ad2ee4, roundkey - 0x1f59d98a8b03
      Round 10: left - 0xd3ad2ee4, right - 0xd4840f2, roundkey - 0x1f61dd564714
      Round 11: left - 0xd4840f2, right - 0xfecf7a8e, roundkey - 0x1f4d8d5901c8
      Round 12: left - 0xfecf7a8e, right - 0x5dd7ff6b, roundkey - 0x5b29adc0f009
      Round 13: left - 0x5dd7ff6b, right - 0x5e608258, roundkey - 0x99acad62362c
      Round 14: left - 0x5e608258, right - 0x5c523aa0, roundkey - 0xd12eaeb819aa
      Round 15: left - 0x5c523aa0, right - 0xb0803165, roundkey - 0xf0bea4045a33
      Round 16: left - 0x7ad5913b, right - 0xb0803165, roundkey - 0xf0bc26a849c0
      des:            0x1f411241ddcb52b4 (text: AA��R� -- base64: H0ESQd3LUrQ=)
```