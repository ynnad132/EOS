CC=gcc

all:hw3

hw2: hw3.o
	$(CC) -o hw3 hw3.o 
	
hw2.o: hw3.c
	$(CC) -c -o hw3.o hw3.c -lpthread

clean:
	rm -f  hw3 hw3.*o
