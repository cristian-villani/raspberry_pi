CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = system_monitor
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)

.PHONY: all clean

