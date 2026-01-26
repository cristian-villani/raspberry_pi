# Virtual Sensors

**Author:** Cristian Villani  
**Project Type:** Embedded Linux / C Learning Project  

---

## Purpose

This project is a **learning exercise** in embedded Linux programming, structured C code, and simple sensor simulation.  

The goal is **not to replicate a real embedded system**, but to practice:

- Writing modular C programs for Linux.
- Creating virtual sensors that simulate readings.
- Handling loops, timing, and simple alerts.
- Structuring a project with a `Makefile` for easy compilation.
- Preparing a framework that could later be expanded for **real hardware** like a Raspberry Pi.

---

## Features

- Three virtual sensors generating random readings:
  - Sensor0 → integer 0–99
  - Sensor1 → integer 50–99
  - Sensor2 → float 0.0–10.0
- Prints sensor readings with timestamps every second.
- Alerts when Sensor1 exceeds 85.
- Clean, modular C code that is easy to expand.

---

## Getting Started

1. Clone the repository:

```bash
git clone https://github.com/cristian-villani/raspberry_pi.git
cd raspberry_pi/virtual_sensors
```

2. Compile the program:

```
make
```

3. Run the program:

```
./virtual_sensors
```

4. Stop the program with Ctrl+C.


## Example Output

```
Starting Virtual Sensors Demo (Ctrl+C to stop)
[12:47:04] Sensor0: 31, Sensor1: 70, Sensor2: 6.28
[12:47:05] Sensor0: 27, Sensor1: 91, Sensor2: 7.48
Alert: Sensor1 > 85!
[12:47:06] Sensor0: 69, Sensor1: 97, Sensor2: 1.87
Alert: Sensor1 > 85!
[12:47:07] Sensor0: 11, Sensor1: 52, Sensor2: 0.19
Alert: Sensor0 < 20!
```

## Suggested improvements

These ideas are meant as **learning exercises** for anyone exploring embedded Linux programming:

- Split sensors into separate modules (`sensors.c/h`) to practice modular C programming.
- Add configuration for polling intervals and thresholds.
- Log sensor readings to a file for analysis.
- Replace virtual sensors with **real Raspberry Pi GPIO inputs**.
- Add more sensors or alert conditions to simulate a more complete system.

