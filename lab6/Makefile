CC=gcc

all:server client

server: server.o
	$(CC) -o server server.o
	
server.o: server.c
	$(CC) -c -o server.o server.c -lpthread


client: client.o
	$(CC) -o client client.o
	
client.o: client.c
	$(CC) -c -o client.o client.c 

clean:
	rm -f  server server.*o
	rm -f  client client.*o
