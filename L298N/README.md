# Using the L298N motor driver

This project demonstrates how to control two DC motors using an L298N motor
driver and a Raspberry Pi with hardware PWM.

![Car](media/car.jpg)

Circuit detail:

![Car detail](media/detail.jpg)

[Watch the video](media/car.mp4)

## Wiring

Make sure the L298N driver is connected to the Raspberry Pi GPIO pins as defined in the source code:

- ENA : GPIO18 (PWM0)
- ENB : GPIO13 (PWM1)
- IN1 : GPIO17
- IN2 : GPIO27
- IN3 : GPIO22
- IN4 : GPIO23

Both the Raspberry Pi and motor power supply must share a common ground.

## Compile and run the test program

Install the bcm2835 library first (required for GPIO access).

From the project root directory, build all components with "make".
Alternatively, you can build only the test program: 

```
make -C test
```

Run the test program as root:

```
sudo ./test/car01
```

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

The setup shown in the photos is for demonstration purposes only.

Four 1.5V batteries are enough to make the wheels move, but they are
definitely not sufficient for proper driving performance. Use a stronger
power supply for real use.

### Http version

For the video, I used a battery box with 8 1.2 V rechargeable batteries
and a 3000 mAh power bank to power the Raspberry Pi Zero.

In this setup, the left and right wheels were connected to the corresponding
left and right motor driver output terminals.

The car server program (run as sudo ./hcar01), which listens for
connections on port 5000, can be found in the http/ directory.

In the video, my son sent HTTP commands directly to the Raspberry Pi
Zero's IP address using the HTTP Shortcut app.

### Bluetooth version

If the Raspberry Pi and a mobile phone are paired via Bluetooth,
the Bluetooth control interface can be used.

To run it:

1. Navigate to the bluetooth directory
2. Compile the Bluetooth server (bcar01) using the provided Makefile
3. Ensure the Bluetooth systemd services are installed and enabled
4. Start or reboot the Raspberry Pi to automatically activate the Bluetooth listener

Once running (as sudo ./bcar01), the system exposes a single-client RFCOMM
channel. Only one mobile application can control the car at a time.

The included Android app connects to the Raspberry Pi using its Bluetooth MAC address
and sends control commands over this channel.
