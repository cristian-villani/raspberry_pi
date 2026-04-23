CC=g++
CFLAGS=-c
LDFLAGS=-lbcm2835 -lbluetooth

MOTOR_DIR=../motor
SENSOR_DIR=../sensor

all: bcar01

bcar01: car_server.o
	$(CC) car_server.o -L$(MOTOR_DIR) -L$(SENSOR_DIR) -lmotor -lsensor -o bcar01 $(LDFLAGS)

car_server.o: car_server.c $(MOTOR_DIR)/motor.h $(SENSOR_DIR)/ultrasonic.h
	$(CC) $(CFLAGS) car_server.c -o car_server.o

clean:
	rm -f *.o bcar01
