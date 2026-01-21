#include "sensor.h"
#include <stdlib.h>
#include <time.h>

void init_sensors() {
    srand(time(NULL)); // Seed random generator
}

SensorData read_sensors() {
    SensorData data;
    data.temperature = rand() % 100;   // 0-99 °C
    data.light = rand() % 1000;        // 0-999 lx
    data.pressure = rand() % 200;      // 0-199 hPa
    return data;
}

