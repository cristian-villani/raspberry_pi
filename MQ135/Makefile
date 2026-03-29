CC=g++
LDFLAGS= -lbcm2835

all:	mq135

mq135:	mq135.o
	$(CC) mq135.o -o mq135 $(LDFLAGS)

mq135.o:
	$(CC) -c mq135.c -o mq135.o

clean:
	rm -f *.o mq135
