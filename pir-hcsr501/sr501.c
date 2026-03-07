#include <stdio.h>
#include <bcm2835.h>  // Include the BCM2835 library
#include <signal.h>   // For interrupt behavior

// Define the GPIO pin to which the PIR sensor is connected
#define PIR_PIN RPI_V2_GPIO_P1_40  // GPIO 21, Pin 40 on the Raspberry Pi header
// Define the GPIO pin to which the LED light is connected
#define LED_PIN RPI_V2_GPIO_P1_38  // GPIO 20, Pin 38 on the Raspberry Pi header

void cleanup(int sig)
{
    printf("\nStopping program...\n");

    bcm2835_gpio_write(LED_PIN, LOW);  // LED ausschalten
    bcm2835_close();

    printf("GPIO cleaned up\n");
    exit(0);
}

int main(int argc, char **argv)
{
    // Initialize the BCM2835 library
    if (!bcm2835_init()) {
        printf("BCM2835 initialization failed!\n");
        return 1;
    }

    signal(SIGINT, cleanup);  // Ctrl-C Handler

    // Set up the PIR_PIN as an input
    bcm2835_gpio_fsel(PIR_PIN, BCM2835_GPIO_FSEL_INPT); // Set GPIO 21 as input
    bcm2835_gpio_fsel(LED_PIN, BCM2835_GPIO_FSEL_OUTP); // Set GPIO 20 as output

    // Enable internal pull-up resistor (optional, depending on sensor wiring)
    bcm2835_gpio_set_pud(PIR_PIN, BCM2835_GPIO_PUD_DOWN);

    // Print message indicating that the program is running
    printf("PIR Sensor Test Running\n");

    // Main loop
    while (1) {
        // Read the state of the PIR sensor (GPIO 17)
        if (bcm2835_gpio_lev(PIR_PIN)) {
            printf("Motion Detected!\n");
            fflush(stdout);
            bcm2835_gpio_write(LED_PIN, HIGH); // LED ON

            // Wait for the PIR sensor to reset after detecting motion
            // Wait for 0.01 seconds before reading again
            // (increase delay if necessary)
            bcm2835_delay(1000);
        } else {
            printf("No Motion\n");
            fflush(stdout);
            bcm2835_gpio_write(LED_PIN, LOW); // LED OFF
        }
        // Wait a bit before checking again (adjust this delay as needed)
        bcm2835_delay(500);  // Delay of 0.5 second before next check
    }

    // Close the BCM2835 library when done
    bcm2835_close();
    return 0;
}

