CC=cc
LDFLAGS= -lbcm2835

all:	bme280_01p

bme280_01p: bme280_01p.o
	$(CC) bme280_01p.o -o bme280_01p $(LDFLAGS)

bme280_01p.o: bme280_01p.c
	$(CC) $(CFLAGS) -c bme280_01p.c -o bme280_01p.o

clean:
	rm -f *.o bme280_01p
