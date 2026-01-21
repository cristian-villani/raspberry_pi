#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // for sleep()

// Function prototypes
int sensor0_read();
int sensor1_read();
float sensor2_read();
void print_readings(int s0, int s1, float s2);

int main() {
    // Initialize random seed
    srand(time(NULL));

    printf("Starting Virtual Sensors Demo (Ctrl+C to stop)\n");

    while(1) {
        // Read sensors
        int s0 = sensor0_read();
        int s1 = sensor1_read();
        float s2 = sensor2_read();

        // Print readings
        print_readings(s0, s1, s2);

        // Check simple alerts
        if(s1 > 85) {
            printf("Alert: Sensor1 > 85!\n");
        }
        if(s0 < 20) {
            printf("Alert: Sensor0 < 20!\n");
        }

        sleep(1); // Wait 1 second before next reading
    }

    return 0;
}

// Sensor functions
int sensor0_read() {
    return rand() % 100; // 0-99
}

int sensor1_read() {
    return (rand() % 50) + 50; // 50-99
}

float sensor2_read() {
    return ((float)rand() / RAND_MAX) * 10.0; // 0.0-10.0
}

// Print readings with timestamp
void print_readings(int s0, int s1, float s2) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    printf("[%02d:%02d:%02d] Sensor0: %d, Sensor1: %d, Sensor2: %.2f\n",
           t->tm_hour, t->tm_min, t->tm_sec, s0, s1, s2);
}
