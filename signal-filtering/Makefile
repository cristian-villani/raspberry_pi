CC=g++
LDFLAGS= -lbcm2835

all:	rc01

rc01:	rc01.o
	$(CC) rc01.o -o rc01 $(LDFLAGS)

rc01.o: rc01.c
	$(CC) -c rc01.c -o rc01.o

clean:
	rm -f *.o rc01
