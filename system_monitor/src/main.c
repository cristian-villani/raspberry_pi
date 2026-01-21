#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

// Simulated GPIO states
bool gpio0 = false;
bool gpio1 = false;

// Function to simulate reading sensors
void read_sensors() {
    // Toggle states for demonstration
    gpio0 = !gpio0;
    gpio1 = !gpio1;
}

// Function to log system state
void log_state() {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    printf("[%02d:%02d:%02d] GPIO0: %s | GPIO1: %s\n",
        t->tm_hour, t->tm_min, t->tm_sec,
        gpio0 ? "HIGH" : "LOW",
        gpio1 ? "HIGH" : "LOW"
    );
}

int main() {
    printf("Starting System Monitor...\n");

    while (1) {
        read_sensors();
        log_state();
        sleep(1);  // 1-second loop
    }

    return 0;
}
