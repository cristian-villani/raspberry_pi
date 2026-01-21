CC = gcc
CFLAGS = -Wall -I./src
LIBS = -lbcm2835

SRC = src/main.c src/sensor.c
OBJ = $(SRC:.c=.o)
TARGET = bcm_mock

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

