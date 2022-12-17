OPT = -Wall -ansi -pedantic 
OBJS = ./obj/manipulate_string.o ./obj/base64.o ./obj/bit_permutation.o ./obj/read_file.o ./obj/des.o ./obj/main.o
EXEC = main

$(EXEC): $(OBJS)
	gcc -o $(EXEC) $(OBJS)
	
./obj/$(EXEC).o: main.c
	gcc -o ./obj/main.o -c main.c
	
./obj/des.o: ./src/des.c
	gcc -o ./obj/des.o -c ./src/des.c
	
./obj/read_file.o: ./src/read_file.c
	gcc -o ./obj/read_file.o -c ./src/read_file.c

./obj/bit_permutation.o: ./src/bit_permutation.c
	gcc -o ./obj/bit_permutation.o -c ./src/bit_permutation.c
	
./obj/base64.o: ./src/base64.c
	gcc -o ./obj/base64.o -c ./src/base64.c

./obj/manipulate_string.o: ./src/manipulate_string.c
	mkdir ./obj
	gcc -o ./obj/manipulate_string.o -c ./src/manipulate_string.c

clean:
	rm ./obj/*.o main
	rmdir obj
