CC=g++
CFLAGS=-std=c++17
LDFLAGS= -lncurses

all:	grabThePoints

grabThePoints:	main.o levels.o filesrw.o
	$(CC) main.o levels.o filesrw.o -o grabThePoints $(LDFLAGS)

main.o: main.cpp levels.h
	$(CC) $(CFLAGS) -c main.cpp -o main.o

levels.o: levels.cpp levels.h
	$(CC) $(CFLAGS) -c levels.cpp -o levels.o

filesrw.o: filesrw.cpp
	$(CC) $(CFLAGS) -c filesrw.cpp -o filesrw.o

clean:
	rm -f *.o grabThePoints
