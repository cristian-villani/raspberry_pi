# LDR-Based Light Gate (Speed Measurement)

This project demonstrates how to measure the speed of an object using two light-dependent resistors (LDRs) and an MCP3008 analog-to-digital converter connected to a Raspberry Pi.

## Overview

The setup uses two LDRs acting as a **light gate**. When an object passes over them, it blocks light and causes a measurable change in voltage. By detecting the time difference between the two events, the speed of the object can be calculated.

## Hardware Setup

* **Raspberry Pi**
* **MCP3008 ADC**
* **2 × LDR (Light Dependent Resistors)**
* **2 × 10 kΩ resistors** (used as voltage dividers)

### GPIO Connections

* **GPIO26** → used in:

  * `ldr_test.c` (single LDR test)
  * first LDR in `light_gate.c`

* **GPIO19** → used for:

  * second LDR in `light_gate.c`

### MCP3008 Channels

* **Channel 2 (CH2)** → first LDR
* **Channel 3 (CH3)** → second LDR

Each LDR is connected as part of a voltage divider with a 10 kΩ resistor, and the resulting analog signal is fed into the MCP3008.

## Physical Setup

* Both LDRs are **covered with dark paper** to reduce ambient light interference and improve detection reliability.

* The distance between the two LDRs is:

  **7 cm**

* When an object passes over the sensors, it blocks light sequentially:

  1. First LDR detects the object → timer starts
  2. Second LDR detects the object → timer stops

## Programs

### `ldr_test.c`

A simple test program to:

* Read a single LDR value
* Verify ADC functionality
* Observe light/dark transitions

### `light_gate.c`

Main application:

* Uses two LDRs
* Detects object passage across both sensors
* Measures time difference
* Forms the basis for speed calculation

## How It Works

1. Continuously read ADC values from both channels
2. Detect a significant change in light intensity (threshold-based)
3. Start timing when the first LDR is triggered
4. Stop timing when the second LDR is triggered
5. Use the known distance (7 cm) to compute speed

## Notes

* The detection logic currently uses a simple threshold;
 further improvements (e.g., filtering or debouncing) may increase accuracy.
* Covering the LDRs helps reduce noise from ambient light changes.

## Timing and Sensitivity Notes

The detection behavior depends on both the object speed and the
sampling/delay settings in the program.

If the object moves **too slowly**, it may not be reliably detected
unless the delay between ADC readings (e.g. `bcm2835_delay()` or similar
timing control) is adjusted. This is because the program samples the
sensor at discrete intervals rather than continuously.

### User Adjustments Required

For best results, the user should tune the following parameters based
on their own setup:

* **Distance between LDRs**

  * Should be adjusted depending on expected object size and speed range

* **Sampling / delay time**

  * Controls how frequently the ADC is read
  * Too slow: missed detections
  * Too fast: unnecessary CPU usage

These parameters should be adapted to match the specific experimental
conditions for reliable detection and timing accuracy.

## Demo

See `experiment.webm` for a short demonstration of the setup and measurement
process.

