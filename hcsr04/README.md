# Using the HCSR04 sensor for distance measurement

![Circuit](circuit.jpg)

Circuit detail:

![Circuit detail](detail.jpg)

## Important Note on Wiring

Please note that the wiring in the original setup shown in the pictures
was incorrect. Instead of connecting the resistors in series as intended
for creating the correct voltage divider, I mistakenly placed them in
parallel. This created an unintended voltage drop, resulting in a lower
voltage (~1.6V) being sent to the GPIO pin, instead of the intended 3.3V.

While this wiring error did not cause any damage and the system still
produced usable results, it could lead to unreliable signal detection
in different setups. The weaker voltage signal might not be detected properly
by the GPIO in other circumstances.

To avoid this mistake, please make sure to wire the resistors in series,
as shown in the corrected picture below:

```
      5V
       |
    [10kΩ]
       |
   To GPIO (max 3.3V)
       |
    [10kΩ]
       |
    [10kΩ]
       |
      GND
```

1. Correct Circuit (Series Resistors, picture above):
When resistors are connected in series, they form a proper voltage divider,
ensuring that the voltage is reduced to the correct level (3.3V) for the
GPIO pin.

2. Incorrect Circuit (Parallel Resistors, photo above):
In the parallel configuration, the voltage supplied to the GPIO is weaker
(~1.6V), which may result in unreliable signal detection or other issues.

## Compile and run

Install the bcm2835 library first and compile the source file with 'make'.
Run the sensor reading program as root by typing 'sudo ./hcsr04'.

## Try it out

The program will print the distance of an object (up to ca. 5 m) with
millimeter-level accuracy. Example output:

```
Distance: 69.22 cm
Distance: 68.81 cm
Distance: 68.53 cm
Distance: 100.00 cm
Distance: 100.28 cm
Distance: 100.28 cm
Distance: 100.26 cm
Distance: 100.26 cm
^C
```

You can adjust the timing delays in the program if needed.

[Results](./results/README.md)
