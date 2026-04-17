CC=g++
CFLAGS=-c

all: libmotor.a

motor.o: motor.c motor.h
	$(CC) $(CFLAGS) motor.c -o motor.o

libmotor.a: motor.o
	ar rcs libmotor.a motor.o

clean:
	rm -f *.o *.a
