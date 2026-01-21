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

## Suggested improvements

These ideas are meant as **learning exercises** for anyone exploring embedded Linux programming:

- Split sensors into separate modules (`sensors.c/h`) to practice modular C programming.
- Add configuration for polling intervals and thresholds.
- Log sensor readings to a file for analysis.
- Replace virtual sensors with **real Raspberry Pi GPIO inputs**.
- Add more sensors or alert conditions to simulate a more complete system.

