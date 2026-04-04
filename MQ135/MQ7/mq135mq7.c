#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// reading Channel of MCP3008
unsigned int readADC(unsigned char channel) {
  // SPI Start-Command (for MCP3008)
  char buffer[3];
  buffer[0] = 0x01;  // Startbit
  buffer[1] = (0x08 + channel) << 4;  // Channeladdress (Shift of 4 Bit)
  buffer[2] = 0x00;  // Dummy-Byte for SPI-Communication

  // Send command und receive answer
  bcm2835_spi_transfern(buffer, 3);  // send 3 Bytes buffer

  // calculate ADC value (12 Bit)
  unsigned int value = ((buffer[1] & 0x03) << 8) + buffer[2];
  return value;
}

int main(int argc, char **argv) {
  // Initialize the BCM2835-Bibliothek and SPI
  if(!bcm2835_init()){
    printf("BCM2835-Bibliothek konnte nicht initialisiert werden!\n");
    return 1;
  }

  // Initialize SPI
  bcm2835_spi_begin();
  bcm2835_spi_set_speed_hz(1000000);  // SPI speed set to 1 MHz
  bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
  bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);  // Mode0 (CPOL=0, CPHA=0)

  // Read sensor values
  while(1){
    unsigned int mq135_value = readADC(0);  // Read MQ135 (CH0)
    unsigned int mq7_value = readADC(1);    // Read MQ7 (CH1)

    // Print values
    printf("MQ135 Wert: %u\n", mq135_value);
    printf("MQ7 Wert: %u\n", mq7_value);

    // Wait 5 seconds
    sleep(5);
  }

  // close SPI and BCM2835
  bcm2835_spi_end();
  bcm2835_close();
  return 0;
}
