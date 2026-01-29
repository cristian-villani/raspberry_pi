CC = gcc
CFLAGS = -Wall -Wextra -g

all: server client

server: server.o network_utils.o
	$(CC) $(CFLAGS) server.c -o server network_utils.o

client: client.o network_utils.o
	$(CC) $(CFLAGS) client.c -o client network_utils.o

server.o: server.c network_utils.h
	$(CC) $(CFLAGS) -c server.c

client.o: client.c network_utils.h
	$(CC) $(CFLAGS) -c client.c

clean:
	rm -f server client

