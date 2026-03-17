CC=g++
LDFLAGS= -lbcm2835

all:	co2 co2b

co2:	co2.o
	$(CC) co2.o -o co2

co2.o: co2.c
	$(CC) -c co2.c -o co2.o

co2b: co2b.o
	$(CC) co2b.o -o co2b $(LDFLAGS)

co2b.o:
	$(CC) -c co2b.c -o co2b.o

clean:
	rm -f *.o co2 co2b
