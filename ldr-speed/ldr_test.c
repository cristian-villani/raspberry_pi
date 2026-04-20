#include <bcm2835.h>
#include <stdio.h>

#define PIN 26  // BCM GPIO26 for PWM (GPIO37)

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

  bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_OUTP);

  bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
  bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
  bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_256);

  // Continuously read the ADC and print values (for the capacitor voltage)
  while(1){
    bcm2835_gpio_write(PIN, HIGH);
    int adc_value = read_adc(2);  // Read channel 2 (LDR)
    printf("ADC Value (Channel 2): %d\n", adc_value);

    // Wait a little to avoid flooding the output
    bcm2835_gpio_write(PIN, LOW);
    bcm2835_delay(100);  // Delay for 0.1 s
  }

  bcm2835_spi_end();
  bcm2835_close();
  return 0;
}

