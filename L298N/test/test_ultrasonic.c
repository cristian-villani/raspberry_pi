#include <bcm2835.h>
#include "../sensor/ultrasonic.h"

int main() {
  bcm2835_init();
  ultrasonic_init();

  for(int i=0; i<100; i++){
    ultrasonic_get_distance();
    bcm2835_delay(100);
  }

  bcm2835_close();
  return 0;
}
