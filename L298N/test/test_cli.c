#include <bcm2835.h>
#include "../motor/motor.h"

#define SPEED 280

int main() {
  motor_init();

  // motor_test();

  motor_forward(SPEED);
  bcm2835_delay(5000);

  motor_left(SPEED);
  bcm2835_delay(5000);

  motor_right(SPEED);
  bcm2835_delay(2000);

  motor_forward(SPEED);
  bcm2835_delay(5000);

  motor_accelerate(SPEED, SPEED+300);
  motor_decelerate(900);

  motor_backward(SPEED);
  bcm2835_delay(5000);

  motor_stop();

  bcm2835_close();
  return 0;
}
