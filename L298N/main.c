#include <bcm2835.h>
#include "motor.h"

int main() {
  motor_init();

  // motor_test();

  motor_forward(600);
  bcm2835_delay(5000);

  motor_left(600);
  bcm2835_delay(5000);

  motor_right(600);
  bcm2835_delay(2000);

  motor_forward(600);
  bcm2835_delay(5000);

  motor_accelerate(600, 900);
  motor_decelerate(900);

  motor_backward(600);
  bcm2835_delay(5000);

  motor_stop();

  bcm2835_close();
  return 0;
}
