CC=g++
CFLAGS=-std=c++17
LDFLAGS= -lSDL2

all:	hitTheTarget

hitTheTarget:	main.o
	$(CC) main.o -o hitTheTarget $(LDFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

clean:
	rm -f *.o hitTheTarget
