CC=cc
LDFLAGS= -lbcm2835

all:	sr501

sr501: sr501.o
	$(CC) sr501.o -o sr501 $(LDFLAGS)

sr501.o: sr501.c
	$(CC) $(CFLAGS) -c sr501.c -o sr501.o

clean:
	rm -f *.o sr501
