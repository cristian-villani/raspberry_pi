CC=gcc
CFLAGS=-Wall -O2

SRC=src/main.c
OUT=virtual_sensors

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

clean:
	rm -f $(OUT)
