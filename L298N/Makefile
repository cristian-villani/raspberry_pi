CC=g++
LDFLAGS= -lbcm2835

all:	car01

car01:	main.o motor.o
	$(CC) main.o motor.o -o car01 $(LDFLAGS)

main.o: main.c motor.h
	$(CC) -c main.c -o main.o

motor.o: motor.c motor.h
	$(CC) -c motor.c -o motor.o

clean:
	rm -f *.o car01
