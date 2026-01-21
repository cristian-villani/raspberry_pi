#ifndef SENSOR_H
#define SENSOR_H

// Virtual sensor structure
typedef struct {
    int temperature;
    int light;
    int pressure;
} SensorData;

// Initialize sensors (nothing special for virtual sensors)
void init_sensors();

// Read sensor data (random values)
SensorData read_sensors();

#endif

