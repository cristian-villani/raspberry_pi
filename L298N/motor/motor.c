#include <stdio.h>
#include <bcm2835.h>
#include "motor.h"
#include "../sensor/ultrasonic.h"

// Motor A
#define IN1 17          // GPIO17
#define IN2 27          // GPIO27
#define ENA 18          // GPIO18 (PWM)
#define ENA_CHANNEL 0   // PWM0

// Motor B
#define IN3 22          // GPIO22
#define IN4 23          // GPIO23
#define ENB 13          // GPIO13 (PWM)
#define ENB_CHANNEL 1   // PWM1

#define PWM_RANGE 1024

#define MAX_DISTANCE 30

volatile MotorState current_state = STOP;
volatile int current_speed = 0;

static void set_speed(int channel, int speed){
  if (speed < 0) speed = 0;
  if (speed > PWM_RANGE) speed = PWM_RANGE;
  bcm2835_pwm_set_data(channel, speed);
}

void motor_init(){
  // bcm2835_init();
  current_state = STOP;
  current_speed = 0;

  // Direction pins
  bcm2835_gpio_fsel(IN1, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(IN2, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(IN3, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(IN4, BCM2835_GPIO_FSEL_OUTP);

  // Initialize direction pins LOW
  bcm2835_gpio_write(IN1, LOW);
  bcm2835_gpio_write(IN2, LOW);
  bcm2835_gpio_write(IN3, LOW);
  bcm2835_gpio_write(IN4, LOW);

  // PWM pins
  bcm2835_gpio_fsel(ENA, BCM2835_GPIO_FSEL_ALT5);
  bcm2835_gpio_fsel(ENB, BCM2835_GPIO_FSEL_ALT0);

  bcm2835_pwm_set_clock(BCM2835_PWM_CLOCK_DIVIDER_64);
  bcm2835_delay(10);

  bcm2835_pwm_set_mode(0, 0, 0);
  bcm2835_pwm_set_mode(1, 0, 0);

  bcm2835_pwm_set_mode(0, 1, 1);
  bcm2835_pwm_set_mode(1, 1, 1);

  bcm2835_pwm_set_range(0, PWM_RANGE);
  bcm2835_pwm_set_data(0, 0);

  bcm2835_pwm_set_mode(1, 1, 1);
  bcm2835_pwm_set_range(1, PWM_RANGE);
  bcm2835_pwm_set_data(1, 0);

  bcm2835_delay(10);

  bcm2835_pwm_set_data(ENA_CHANNEL, 0);
  bcm2835_pwm_set_data(ENB_CHANNEL, 0);
}

void motor_test(){

  // Motor A test
  bcm2835_gpio_write(IN1, HIGH);
  bcm2835_gpio_write(IN2, LOW);
  set_speed(0, 600);
  bcm2835_delay(3000);

  // Motor B test
  bcm2835_gpio_write(IN3, HIGH);
  bcm2835_gpio_write(IN4, LOW);
  set_speed(1, 600); // ENB PWM
  bcm2835_delay(3000);

  // stop safely
  bcm2835_gpio_write(IN1, LOW);
  bcm2835_gpio_write(IN2, LOW);
  bcm2835_gpio_write(IN3, LOW);
  bcm2835_gpio_write(IN4, LOW);
}

void motor_update(){
  switch (current_state) {
    case FORWARD:
      motor_forward(current_speed);
      break;
    case BACKWARD:
      motor_backward(current_speed);
      break;
    case LEFT:
      motor_left(current_speed);
      break;
    case RIGHT:
      motor_right(current_speed);
      break;
    case STOP:
      motor_stop();
      break;
  }
}

void motor_forward(int speed){
  printf("Motor forward called...\n");

  current_state = FORWARD;
  current_speed = speed;

  bcm2835_gpio_write(IN1, HIGH);
  bcm2835_gpio_write(IN2, LOW);
  bcm2835_gpio_write(IN3, HIGH);
  bcm2835_gpio_write(IN4, LOW);

  set_speed(0, speed);
  set_speed(1, speed);
}

void motor_backward(int speed){
  printf("Motor backward called...\n");

  current_state = BACKWARD;
  current_speed = speed;

  bcm2835_gpio_write(IN1, LOW);
  bcm2835_gpio_write(IN2, HIGH);
  bcm2835_gpio_write(IN3, LOW);
  bcm2835_gpio_write(IN4, HIGH);

  set_speed(0, speed);
  set_speed(1, speed);
}

void motor_left(int speed){
  printf("Motor left called...\n");

  current_state = LEFT;
  current_speed = speed;

  bcm2835_gpio_write(IN1, HIGH);
  bcm2835_gpio_write(IN2, LOW);
  bcm2835_gpio_write(IN3, LOW);
  bcm2835_gpio_write(IN4, HIGH);

  set_speed(0, speed);
  set_speed(1, speed);
}

void motor_right(int speed){
  printf("Motor right called...\n");

  current_state = RIGHT;
  current_speed = speed;

  bcm2835_gpio_write(IN1, LOW);
  bcm2835_gpio_write(IN2, HIGH);
  bcm2835_gpio_write(IN3, HIGH);
  bcm2835_gpio_write(IN4, LOW);

  set_speed(0, speed);
  set_speed(1, speed);
}

void motor_stop(){
  printf("Motor stop called...\n");

  current_state = STOP;
  current_speed = 0;

  bcm2835_gpio_write(IN1, LOW);
  bcm2835_gpio_write(IN2, LOW);
  bcm2835_gpio_write(IN3, LOW);
  bcm2835_gpio_write(IN4, LOW);

  set_speed(0, 0);
  set_speed(1, 0);
}

//
// Test functions - DO NOT USE IN CAR LOGIC
//

void motor_accelerate(int initial_speed, int target_speed){
  printf("Motor accelerate called...\n");
  for(int s = initial_speed; s <= target_speed; s += 20){
    current_speed = s;
    motor_forward(s);
    bcm2835_delay(50);
  }
}

void motor_decelerate(int initial_speed){
  printf("Motor decelerate called...\n");
  for(int s = initial_speed; s >= 0; s -= 20){
    current_speed = s;
    motor_forward(s);
    bcm2835_delay(50);
  }
  motor_stop();
}
