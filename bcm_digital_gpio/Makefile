CC = gcc
CFLAGS = -Wall -I./src
LIBS = -lbcm2835

SRC = src/
BUILD = build/

all: mult_sensor read_sensor

mult_sensor: $(BUILD)mock_sensors.o
	$(CC) -o mult_sensor $(BUILD)mock_sensors.o $(LIBS)

$(BUILD)mock_sensors.o: $(SRC)mock_sensors.c
	$(CC) $(CFLAGS) -c $(SRC)mock_sensors.c -o $(BUILD)mock_sensors.o

read_sensor: $(BUILD)read_sensors.o
	$(CC) -o read_sensor $(BUILD)read_sensors.o $(LIBS)

$(BUILD)read_sensors.o: $(SRC)read_sensors.c
	$(CC) $(CFLAGS) -c $(SRC)read_sensors.c -o $(BUILD)read_sensors.o

clean:
	rm -f $(BUILD)*.o mult_sensor read_sensor

