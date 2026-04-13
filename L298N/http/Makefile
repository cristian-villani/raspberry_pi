CC=g++
LDFLAGS= -lbcm2835

all:	scar01

scar01:	car_server.o motor.o
	$(CC) car_server.o motor.o -o scar01 $(LDFLAGS)

car_server.o: car_server.c motor.h
	$(CC) -c car_server.c -o car_server.o

motor.o: motor.c motor.h
	$(CC) -c motor.c -o motor.o

clean:
	rm -f *.o scar01
