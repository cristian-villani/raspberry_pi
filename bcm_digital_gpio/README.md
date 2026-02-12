# BCM Digital GPIO Sensor System

This project enables the control and monitoring of digital GPIO pins on a Raspberry Pi using the **BCM2835** library. It interfaces with mock sensors (temperature, light, pressure) and handles actuators (GPIO pins) to manage the state of these sensors based on predefined thresholds.

### Features

- **GPIO Control:** Use the Raspberry Pi’s BCM GPIO pins to control external sensors.
- **Mock Sensors:** Simulates readings for temperature, light, and pressure.
- **Threshold Handling:** Prints alerts when sensor readings go above or below defined thresholds.
- **Polling and Updating:** Continuously updates and checks sensor values at regular intervals.

### Files

- `mock_sensor.c`: Simulates sensor data (temperature, light, pressure) and updates GPIO pins based on thresholds.
- `read_sensor.c`: Reads simulated sensor data from `/tmp/temperature`, `/tmp/light`, and `/tmp/pressure`. It also monitors the corresponding GPIO pins for state changes.
- `bcm2835.h`: Required BCM2835 library for GPIO control. The library must be downloaded manually from the [BCM2835 GitHub page](https://github.com/Screenly/raspberry-pi-bcm2835) or another source.

1. Setup

   - Clone or download the repository to your local machine.

   - Install the BCM2835 library


2.  Build


```
cd bcm_digital_gpio/

make
```

3. Run

```
./mult_sensor

```
You should see something like:

```
29  degC  Thu Feb 12 09:27:39 PM CET 2026
Temperatur set to LOW
962  hPa  Thu Feb 12 09:27:39 PM CET 2026
Pressure set to LOW
189  lx  Thu Feb 12 09:28:09 PM CET 2026
Light value set to LOW
48  degC  Thu Feb 12 09:28:09 PM CET 2026
Temperatur set to HIGH
992  hPa  Thu Feb 12 09:28:09 PM CET 2026
Pressure set to LOW

```

And, from an other terminal:

```
./read_sensor
```

You will see something like:

```
Temperature below threshold
Light below threshold
Pressure below threshold
Temperature below threshold
Light below threshold
Pressure below threshold
=========================================
    Warning!                      
=========================================
Temperature above threshold
48  degC  Thu Feb 12 09:28:10 PM CET 2026
=========================================
Light below threshold
Pressure below threshold
=========================================
    Warning!                      
=========================================
Temperature above threshold
48  degC  Thu Feb 12 09:28:15 PM CET 2026
=========================================
Light below threshold
Pressure below threshold
```

Note: sudo may be required for GPIO access.


4. Troubleshooting

"Error opening file" or "No integer found": Ensure the /tmp files (e.g., /tmp/temperature, /tmp/light, /tmp/pressure) exist and contain valid integer values.

GPIO pins not updating: Check the physical GPIO connections and ensure the BCM2835 library is correctly installed.

Program not responding to sensor changes: Ensure your mock sensors and timing intervals are correctly configured.

5. License

This project is open for educational purposes. Feel free to modify and use it for your own projects.
