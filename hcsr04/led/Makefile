CC=cc
LDFLAGS= -lbcm2835

all:	lhcsr04

lhcsr04: lhcsr04.o
	$(CC) lhcsr04.o -o lhcsr04 $(LDFLAGS)

lhcsr04.o: lhcsr04.c
	$(CC) $(CFLAGS) -c lhcsr04.c -o lhcsr04.o

clean:
	rm -f *.o lhcsr04

