#include <bcm2835.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define PIN1 26  // BCM GPIO26 for PWM (GPIO37)
#define PIN2 19  // BCM GPIO19 for PWM (GPIO35)

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

  bcm2835_gpio_fsel(PIN1, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(PIN2, BCM2835_GPIO_FSEL_OUTP);

  bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
  bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
  bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_256);

  struct timespec start_time, end_time;

  // Continuously read the ADC and print values (for the capacitor voltage)
  int p_adc_value = 100000;
  int detected1 = 0;
  while(detected1 == 0){
    bcm2835_gpio_write(PIN1, HIGH);
    int adc_value = read_adc(2);  // Read channel 2 (LDR1)
    printf("ADC Value (Channel 2): %d\n", adc_value);
    if((adc_value > p_adc_value) && (adc_value - p_adc_value > 10)){
      clock_gettime(CLOCK_MONOTONIC, &start_time);
      printf("Object detectet!\n");
      detected1 = 1;
      int p_adc_value2 = 100000;
      int detected2 = 0;
      while(detected2 == 0){
        bcm2835_gpio_write(PIN2, HIGH);
        int adc_value2 = read_adc(3);  // Read channel 3 (LDR2)
        printf("ADC Value (Channel 3): %d\n", adc_value2);
        if((adc_value2 > p_adc_value2) && (adc_value2 - p_adc_value2 > 10)){
          clock_gettime(CLOCK_MONOTONIC, &end_time);
          double time_elapsed = (end_time.tv_sec - start_time.tv_sec) +
                                (end_time.tv_nsec - start_time.tv_nsec) /1e9;
          printf("Object 2 detectet!\n");
          printf("Time elapsed: %lf\n", time_elapsed);
          printf("Speed(assuming 7.0 cm distance): %lf m/s\n",
                 0.07/time_elapsed);
          detected2 = 1;
        }
        p_adc_value2 = adc_value2;
        bcm2835_delay(50);  // Delay for 0.05 s
      }
      bcm2835_gpio_write(PIN2, HIGH);
    }
    p_adc_value = adc_value;

    // Wait a little to avoid flooding the output
    bcm2835_gpio_write(PIN1, LOW);
    bcm2835_delay(50);  // Delay for 0.05 s
  }

  bcm2835_spi_end();
  bcm2835_close();
  return 0;
}

