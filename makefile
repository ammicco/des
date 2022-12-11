OBJS = manipulate_string.o base64.o bit_permutation.o read_file.o des.o main.o
EXEC = main

$(EXEC): $(OBJS)
	gcc -o $(EXEC) $(OBJS)
	
$(EXEC).o: main.c
	gcc -c main.c
	
des.o: des.c
	gcc -c des.c
	
read_file.o: read_file.c
	gcc -c read_file.c

bit_permutation.o: bit_permutation.c
	gcc -c bit_permutation.c
	
base64.o: base64.c
	gcc -c base64.c

manipulate_string.o: manipulate_string.c
	gcc -c manipulate_string.c

clean:
	rm *.o main
