CC = gcc
LIBS = -lbcm2835

all: bcm_pwm_gpio

bcm_pwm_gpio:
	$(CC) -o bcm_pwm_gpio sensor_simulation.c $(LIBS)

clean:
	rm -f bcm_pwm_gpio

