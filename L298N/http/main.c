#include <bcm2835.h>
#include "motor.h"

int main() {
  motor_init();

  // motor_test();
  int testSpeed = 280;

  motor_forward(testSpeed);
  bcm2835_delay(5000);

  motor_left(testSpeed);
  bcm2835_delay(5000);

  motor_right(testSpeed);
  bcm2835_delay(2000);

  motor_forward(testSpeed);
  bcm2835_delay(5000);

  motor_accelerate(testSpeed, testSpeed+100);
  motor_decelerate(testSpeed+100);

  motor_backward(testSpeed);
  bcm2835_delay(5000);

  motor_stop();

  bcm2835_close();
  return 0;
}
