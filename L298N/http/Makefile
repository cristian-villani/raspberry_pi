CC=g++
CFLAGS=-c
LDFLAGS=-lbcm2835

MOTOR_DIR=../motor
SENSOR_DIR=../sensor

all: hcar01

hcar01: car_server.o
	$(CC) car_server.o -L$(MOTOR_DIR) -L$(SENSOR_DIR) -lmotor -lsensor -o hcar01 $(LDFLAGS)

car_server.o: car_server.c $(MOTOR_DIR)/motor.h $(SENSOR_DIR)/ultrasonic.h
	$(CC) $(CFLAGS) car_server.c -o car_server.o

clean:
	rm -f *.o hcar01
