#include <bcm2835.h>
#include <stdio.h>
#include <unistd.h>
#include "sensor.h"

#define LED_PIN RPI_GPIO_P1_11

int main() {
    if (!bcm2835_init()) {
        printf("bcm2835 init failed. Are you running as root?\n");
        return 1;
    }

    // Setup GPIO pin as output
    bcm2835_gpio_fsel(LED_PIN, BCM2835_GPIO_FSEL_OUT);

    // Initialize virtual sensors
    init_sensors();

    printf("Starting virtual sensors with GPIO output...\n");

    for (int i = 0; i < 20; i++) {
        SensorData data = read_sensors();

        // Print sensor data
        printf("Temp: %d°C  |  Light: %d lx  |  Pressure: %d hPa\n",
               data.temperature, data.light, data.pressure);

        // GPIO control: light LED if temperature > 50
        if (data.temperature > 50) {
            bcm2835_gpio_write(LED_PIN, HIGH);
        } else {
            bcm2835_gpio_write(LED_PIN, LOW);
        }

        sleep(1);
    }

    // Reset GPIO and close library
    bcm2835_gpio_write(LED_PIN, LOW);
    bcm2835_close();

    return 0;
}
