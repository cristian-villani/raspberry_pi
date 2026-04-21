CC=g++
CFLAGS=-c
LDFLAGS=-lbcm2835

MOTOR_DIR=../motor
SENSOR_DIR=../sensor

all: car01 ultrasonic

car01: test_cli.o
	$(CC) test_cli.o -L$(MOTOR_DIR) -lmotor -o car01 $(LDFLAGS)

ultrasonic: test_ultrasonic.o
	$(CC) test_ultrasonic.o -L$(SENSOR_DIR) -lsensor -o ultrasonic $(LDFLAGS)

test_cli.o: test_cli.c $(MOTOR_DIR)/motor.h
	$(CC) $(CFLAGS) test_cli.c -o test_cli.o

test_ultrasonic.o: test_ultrasonic.c $(SENSOR_DIR)/ultrasonic.h
	$(CC) $(CFLAGS) test_ultrasonic.c -o test_ultrasonic.o

clean:
	rm -f *.o car01 ultrasonic
