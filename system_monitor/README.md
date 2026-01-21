# System Monitor

**Author:** Cristian Villani
**Project Type:** Embedded Linux / C Learning Project

---

## Purpose

A small embedded Linux project for Raspberry Pi, demonstrating GPIO handling,
  C programming, and system-level monitoring.

## Overview

This project simulates a simple system monitor running on a Raspberry Pi. It reads input values from virtual sensors (simulating GPIO pins) and logs outputs to the terminal. It demonstrates:

- Embedded Linux programming
- C language skills
- Basic GPIO handling
- Modular and maintainable project structure

## Features

- Periodic system monitoring loop
- Virtual GPIO input and output simulation
- Logging system state to terminal
- Modular design for adding more sensors in the future

## Requirements

- Raspberry Pi (any model)
- Linux-based OS (Raspberry Pi OS, Ubuntu, etc.)
- GCC compiler
- Make

## Build

Clone this repository and run:

```bash
cd system_monitor
make
```

## Run

```
./system_monitor
```

You should see terminal output simulating system and GPIO activity:

```
[12:01:03] GPIO0: LOW → IDLE
[12:01:04] GPIO1: HIGH → ACTIVE
```

## Suggested Improvements

These ideas are meant as **learning exercises** for anyone exploring embedded Linux programming:

- **Real GPIO Integration** – replace the virtual sensor functions with actual Raspberry Pi GPIO inputs.  
- **Additional Virtual Sensors** – add more simulated sensors to experiment with different data types and ranges.  
- **Logging to a File** – write sensor readings and alerts to a log file for later analysis.  
- **More Complex System Monitoring Logic** – implement alerts, thresholds, or computed metrics to simulate a real monitoring system.  
