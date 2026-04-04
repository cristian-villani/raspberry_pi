CC=g++
LDFLAGS= -lbcm2835

all:	mq135mq7

mq135mq7:	mq135mq7.o
	$(CC) mq135mq7.o -o mq135mq7 $(LDFLAGS)

mq135.o:
	$(CC) -c mq135mq7.c -o mq135mq7.o

clean:
	rm -f *.o mq135mq7
