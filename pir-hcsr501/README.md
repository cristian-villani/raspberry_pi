# Using the RPI HC-SR501 sensor for Motion detection

![Circuit](images/circuit.jpg)

## Compile and run

Install the bcm2835 library first and ensure that your system is handling
i2c bus communication correctly. Compile the source file with 'make' and
run the sensor reading program by typing './sr501'.

## Try it out

The program will print to standard output if a motion is detected.
You can stop the program with Ctrl-C.

```
PIR Sensor Test Running
No Motion
No Motion
No Motion
Motion Detected!
No Motion
No Motion
No Motion
No Motion
Motion Detected!
No Motion
No Motion
No Motion
Motion Detected!
^C
Stopping program...
GPIO cleaned up
```

You can adjust the sensor time delay or the sensor sensitivity but you
can also adjust the time delays in the program if needed.
