CC=cc
LDFLAGS= -lbcm2835

all:	hcsr04

hcsr04: hcsr04.o
	$(CC) hcsr04.o -o hcsr04 $(LDFLAGS)

hcsr04.o: hcsr04.c
	$(CC) $(CFLAGS) -c hcsr04.c -o hcsr04.o

clean:
	rm -f *.o hcsr04

