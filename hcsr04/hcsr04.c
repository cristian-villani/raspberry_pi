#include <bcm2835.h>
#include <stdio.h>
#include <unistd.h>

#define TRIG RPI_V2_GPIO_P1_40  // GPIO21
#define ECHO RPI_V2_GPIO_P1_38  // GPIO20

int main(int argc, char **argv)
{
  if(!bcm2835_init()){
    fprintf(stderr, "Failing to initialize bcm2835. Not running as root?\n");
    return 1;
  }

  if(getuid() != 0){
    fprintf(stderr, "Warning: You are not runnung as root\n");
    fprintf(stderr, "GPIO reads may always return 0.\n");
  }

  bcm2835_gpio_fsel(TRIG, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(ECHO, BCM2835_GPIO_FSEL_INPT);

  while(1){
    // Trigger pulse
    bcm2835_gpio_write(TRIG, LOW);
    bcm2835_delayMicroseconds(2);

    bcm2835_gpio_write(TRIG, HIGH);
    bcm2835_delayMicroseconds(10);
    bcm2835_gpio_write(TRIG, LOW);

    // Wait for echo start with timeout
    uint64_t timeout = bcm2835_st_read() + 30000; // 30 ms
    while(bcm2835_gpio_lev(ECHO) == LOW){
      if(bcm2835_st_read() > timeout) break;
    }

    uint64_t start = bcm2835_st_read();

    // Wait for echo end with timeout
    timeout = bcm2835_st_read() + 30000;
    while(bcm2835_gpio_lev(ECHO) == HIGH){
      if (bcm2835_st_read() > timeout) break;
    }

    uint64_t end = bcm2835_st_read();

    uint64_t travel_time = end - start;

    double distance = travel_time / 58.0; // in cm

    printf("Distance: %.2f cm\n", distance);

    bcm2835_delay(2000);
  }

  bcm2835_close();
  return 0;
}
