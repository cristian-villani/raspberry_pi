CC=g++
CFLAGS=-c
LDFLAGS=-lbcm2835

MOTOR_DIR=../motor

all: car01

car01: test_cli.o
	$(CC) test_cli.o -L$(MOTOR_DIR) -lmotor -o car01 $(LDFLAGS)

test_cli.o: test_cli.c $(MOTOR_DIR)/motor.h
	$(CC) $(CFLAGS) test_cli.c -o test_cli.o

clean:
	rm -f *.o car01
