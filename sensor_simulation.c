#include <stdio.h>
#include <stdlib.h>
#include <bcm2835.h>
#include <unistd.h>

#define LED_PIN RPI_V2_GPIO_P1_12  // GPIO 18 (BCM)
#define TEMP_PIN RPI_V2_GPIO_P1_11 // GPIO 17 (BCM) - for simulation purpose

// Simulate temperature readings (0 to 100°C)
int readTemperatureSensor() {
    int temperature = rand() % 101;  // Generate random temperature between 0 and 100°C
    return temperature;
}

void controlLED(int temperature) {
    // Map the temperature to PWM duty cycle
    // 0°C -> 0% brightness, 100°C -> 100% brightness
    int dutyCycle = (temperature * 1024) / 100;  // Convert to range of 0 to 1024 for PWM
    
    // Set PWM data to control the LED brightness
    bcm2835_pwm_set_data(LED_PIN, dutyCycle);
    printf("Temperature: %d°C -> LED Brightness: %d (Duty Cycle)\n", temperature, dutyCycle);
}

int main() {
    if (!bcm2835_init()) {
        printf("BCM2835 initialization failed.\n");
        return -1;
    }

    // Set the LED pin for PWM output
    bcm2835_gpio_fsel(LED_PIN, BCM2835_GPIO_FSEL_ALT5);  // Set GPIO 18 to PWM (ALT5 function)
    bcm2835_pwm_set_range(LED_PIN, 1024);  // PWM range (0 to 1024)

    while (1) {
        // Simulate sensor reading
        int temperature = readTemperatureSensor();

        // Control the LED based on the temperature
        controlLED(temperature);

        // Wait 1 second before simulating the next sensor reading
        usleep(1000000);  // 1 second delay
    }

    bcm2835_close();
    return 0;
}

