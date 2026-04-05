CC=gcc
LDFLAGS= -lbcm2835

all:	bmeco2mq7

bmeco2mq7:	bmeco2mq7.o
	$(CC) bmeco2mq7.o -o bmeco2mq7 $(LDFLAGS)

mq135.o:
	$(CC) -c bmeco2mq7.c -o bmeco2mq7.o

clean:
	rm -f *.o bmeco2mq7
