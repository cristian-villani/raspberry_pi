CC = gcc
CFLAGS = -Wall -I./src

SRC = src/main.c
OBJ = $(SRC:.c=.o)
TARGET = sensor_sim

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

