CC=cc
LDFLAGS= -lbcm2835

all:	ldr_test light_gate

ldr_test: ldr_test.o
	$(CC) ldr_test.o -o ldr_test $(LDFLAGS)

light_gate: light_gate.o
	$(CC) light_gate.o -o light_gate $(LDFLAGS)

ldr_test.o: ldr_test.c
	$(CC) $(CFLAGS) -c ldr_test.c -o ldr_test.o

light_gate.o: light_gate.c
	$(CC) $(CFLAGS) -c light_gate.c -o light_gate.o

clean:
	rm -f *.o ldr_test light_gate
