# Raspberry Pi Projects – Cristian Villani

This repository contains a series of learning projects for Raspberry Pi, focusing on embedded Linux programming, C/C++ development, and virtual hardware simulation. The projects are designed to help understand GPIO control, sensor simulation, and system monitoring in an embedded Linux environment.

---

## Projects

### 1. **System Monitor (Virtual)**
- **Purpose:** Learn basic C programming and system interaction on Linux.
- **Description:** A simple C program that monitors system metrics such as CPU usage, memory, and process counts. It is entirely virtual and can be run on any Linux system.
- **Skills:** C programming, Linux system interaction, terminal output formatting.
- **Suggested Improvements:**
  - Logging to a file
  - More complex system monitoring logic
  - Additional metrics and alerts

### 2. **Virtual Sensors (C)**
- **Purpose:** Practice working with virtual sensors in C, simulating embedded development.
- **Description:** Program simulates multiple virtual sensors (temperature, light, pressure) and prints readings periodically. Serves as a bridge to real embedded GPIO programming.
- **Skills:** C programming, random data generation, loop control, modular coding.
- **Suggested Improvements:**
  - Logging sensor data
  - Adding new sensor types
  - More sophisticated data simulation

### 3. **BCM2835 Mock Sensors (Embedded Linux)**
- **Purpose:** Introduce embedded Linux GPIO programming with a mock setup.
- **Description:** Uses the BCM2835 C library to simulate GPIO operations on the Raspberry Pi. Three virtual sensors generate random readings and display them in the terminal.
- **Requirements:** Must be compiled and run on a Raspberry Pi.
- **Skills:** C programming, basic embedded Linux development, GPIO control, Makefile usage.
- **Suggested Improvements:**
  - Real GPIO integration with actual sensors
  - Additional virtual sensors
  - Logging to a file
  - More complex system monitoring logic

---

## Notes
- These projects are primarily for learning and demonstration purposes.
- The code is modular and structured to allow future extensions.

