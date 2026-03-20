# Using the HCSR04 sensor for distance measurement

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

You can adjust the sensor timing or sensitivity in the program if needed.
