CC=g++
LDFLAGS= -lbcm2835

all:	mq7co2

mq7co2:	mq7co2.o
	$(CC) mq7co2.o -o mq7co2 $(LDFLAGS)

mq135.o:
	$(CC) -c mq7co2.c -o mq7co2.o

clean:
	rm -f *.o mq7co2
