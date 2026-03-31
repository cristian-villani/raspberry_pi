# Using the L298N motor driver

This project demonstrates how to control two DC motors using an L298N motor
driver and a Raspberry Pi with hardware PWM.

![Car](car.jpg)

Circuit detail:

![Car detail](detail.jpg)


## Wiring

Make sure the L298N driver is connected to the Raspberry Pi GPIO pins as defined in the source code:

- ENA : GPIO18 (PWM0)
- ENB : GPIO13 (PWM1)
- IN1 : GPIO17
- IN2 : GPIO27
- IN3 : GPIO22
- IN4 : GPIO23

Both the Raspberry Pi and motor power supply must share a common ground.

## Compile and run

Install the bcm2835 library first and compile the source file with 'make'.
Run the test program as root by typing 'sudo ./car01'.

## Try it out

The program will test the forward and backward movements, the right and
left turning as well as the acceleration and deceleration. You can also
follow the behavior via the standard output:

```
Motor forward called...
Motor left called...
Motor right called...
Motor forward called...
Motor accelerate called...
Motor forward called...
Motor forward called...
Motor forward called...
Motor forward called...
...
```

## Note

The setup shown here is for demonstration purposes only.

Four 1.5V batteries are enough to make the wheels move, but they are
definitely not sufficient for proper driving performance. Use a stronger
power supply for real use.
