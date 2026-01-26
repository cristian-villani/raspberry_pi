# Raspberry Pi BCM2835 Mock Sensor Project

**Author:** Cristian Villani
**Project Type:** Embedded Linux / C Learning Project

---

## Purpose

This project is a **learning exercise** in embedded Linux programming, structured C code, and simple sensor simulation.


## Overview

This project is a learning exercise in embedded Linux programming on the Raspberry Pi. It uses the bcm2835 C library
 to simulate GPIO interaction with virtual sensors.

The goal is to practice the setup, compilation, and structure of a real Raspberry Pi project before integrating actual hardware sensors.


## Requirements

Raspberry Pi (any model with GPIO pins)

Raspberry Pi OS or compatible Linux distribution

bcm2835 C library installed

GCC compiler (gcc)

Make (make)

⚠️ This project must be compiled on a Raspberry Pi because the bcm2835 library interacts with the Pi’s hardware registers.


## Features

- Simulates multiple virtual sensors generating random data

- Uses bcm2835 library to mimic GPIO initialization and communication

- Prints sensor data in real-time to the console

- Modular C code structure for easy extension


## Learning Purpose

This project is not intended for production use. It is meant to:

- Practice embedded Linux programming on Raspberry Pi

- Learn to integrate C libraries (bcm2835)

- Experiment with GPIO concepts without hardware

- Prepare for future real sensor integration


## Build

Clone this repository and run:

```bash
cd bcm_mock
make
```

## Run

```
sudo ./bcm_mock
```

Note: sudo may be required for GPIO access.

## Example Output

```
Starting virtual sensors with GPIO output...
Temp: 40°C  |  Light: 921 lx  |  Pressure: 191 hPa
Temp: 23°C  |  Light: 90 lx  |  Pressure: 40 hPa
Temp: 4°C  |  Light: 306 lx  |  Pressure: 36 hPa
Temp: 46°C  |  Light: 723 lx  |  Pressure: 167 hPa
Temp: 1°C  |  Light: 624 lx  |  Pressure: 142 hPa
Temp: 51°C  |  Light: 332 lx  |  Pressure: 170 hPa
Temp: 42°C  |  Light: 155 lx  |  Pressure: 152 hPa
Temp: 6°C  |  Light: 557 lx  |  Pressure: 21 hPa
```


## Suggested Improvements

- Connect and read from real sensors via GPIO

- Add logging to a file

- Implement more complex system monitoring logic

- Expand to multiple types of virtual or real sensors

