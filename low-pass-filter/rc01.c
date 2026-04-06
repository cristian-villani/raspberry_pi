#include <bcm2835.h>
#include <stdio.h>
#include <pthread.h>

#define PIN 26  // BCM GPIO26 for PWM (GPIO37 on your setup)
#define SPI_CHANNEL 0       // MCP3008 SPI channel
#define CS_PIN RPI_GPIO_P1_16  // SPI Chip Select pin

// Function to generate a square wave on GPIO pin
void* generate_square_wave(void* arg) {
  int frequency = 100;  // Set frequency (0.1kHz)
  int period = 1000000 / frequency;  // Calculate period in microseconds

  bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_OUTP);

  while(1){
    bcm2835_gpio_write(PIN, HIGH);  // Set GPIO pin high
    bcm2835_delayMicroseconds(period / 2);  // Delay for half period
    bcm2835_gpio_write(PIN, LOW);   // Set GPIO pin low
    bcm2835_delayMicroseconds(period / 2);  // Delay for half period
  }
}

// Function to read ADC data from MCP3008 (SPI)
int read_adc(int channel){
  char buf[3];
  buf[0] = 1;  // Start bit
  buf[1] = (8 + channel) << 4;  // Single-ended mode, channel selection
  buf[2] = 0;

  bcm2835_spi_transfern(buf, 3);  // Send data to MCP3008

  // Combine the 2 bytes (ADC result)
  int value = ((buf[1] & 3) << 8) | buf[2];
  return value;
}

int main(){
  if(!bcm2835_init()){
    printf("Failed to initialize bcm2835 library\n");
    return 1;
  }

  if(!bcm2835_spi_begin()){
    printf("Failed to initialize SPI\n");
    return 1;
  }

  bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
  bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
  bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_256);

  // Create thread for square wave generation
  pthread_t wave_thread;
  if(pthread_create(&wave_thread, NULL, generate_square_wave, NULL) != 0){
    printf("Error creating wave generation thread\n");
    return 1;
  }

  // Continuously read the ADC and print values (for the capacitor voltage)
  while(1){
    int adc_value = read_adc(2);  // Read channel 0 (your sensor)
    printf("ADC Value (Channel 2): %d\n", adc_value);

    // Wait a little to avoid flooding the output
    bcm2835_delay(5);  // Delay for 5 ms
  }

  bcm2835_spi_end();
  bcm2835_close();
  return 0;
}
