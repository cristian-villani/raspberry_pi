#ifndef MOTOR_H
#define MOTOR_H

void motor_init();
void motor_test();
void motor_forward(int speed);
void motor_backward(int speed);
void motor_left(int speed);
void motor_right(int speed);
void motor_accelerate(int current_speed, int speed);
void motor_decelerate(int current_speed);
void motor_stop();
void motor_update();

typedef enum {
    STOP,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
} MotorState;

extern volatile MotorState current_state;
extern volatile int current_speed;

#endif
