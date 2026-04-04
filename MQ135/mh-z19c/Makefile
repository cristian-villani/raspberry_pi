CC=g++
LDFLAGS= -lbcm2835

all:	co2

co2:	co2.o
	$(CC) co2.o -o co2 $(LDFLAGS)

mq135.o:
	$(CC) -c co2.c -o co2.o

clean:
	rm -f *.o co2
