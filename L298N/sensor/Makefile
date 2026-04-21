CC=g++
CFLAGS=-c

all: libsensor.a

ultrasonic.o: ultrasonic.c ultrasonic.h
	$(CC) $(CFLAGS) ultrasonic.c -o ultrasonic.o

libsensor.a: ultrasonic.o
	ar rcs libsensor.a ultrasonic.o

clean:
	rm -f *.o *.a
