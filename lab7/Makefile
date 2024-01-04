CC=gcc

all:game guess

game: game.o
	$(CC) -o game game.o
	
game.o: game.c
	$(CC) -c -o game.o game.c 


guess: guess.o
	$(CC) -o guess guess.o
	
guess.o: guess.c
	$(CC) -c -o guess.o guess.c 

clean:
	rm -f  game game.*o
	rm -f  guess guess.*o
