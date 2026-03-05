CC=cc
LDFLAGS= -lbcm2835

all:	bme280_01

bme280_01: bme280_01.o
	$(CC) bme280_01.o -o bme280_01 $(LDFLAGS)

bme280_01.o: bme280_01.c
	$(CC) $(CFLAGS) -c bme280_01.c -o bme280_01.o

clean:
	rm -f *.o bme280_01
