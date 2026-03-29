#include <bcm2835.h>
#include <stdio.h>

uint16_t read_mcp3008(uint8_t channel) {
    char buf[3];

    buf[0] = 0x01; // Start bit
    buf[1] = (0x08 | channel) << 4;
    buf[2] = 0x00;

    bcm2835_spi_transfern(buf, 3);

    uint16_t value = ((buf[1] & 0x03) << 8) | buf[2];
    return value;
}

int main(int argc, char **argv) {
    if (!bcm2835_init()) return 1;
    if (!bcm2835_spi_begin()) return 1;

    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_65536);
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);

    while (1) {
        uint16_t value = read_mcp3008(0); // CH0
        printf("ADC Value: %d\n", value);
        bcm2835_delay(1000);
    }

    bcm2835_spi_end();
    bcm2835_close();
    return 0;
}
