## System Overview

This project controls a Raspberry Pi robot car using two independent interfaces:

- HTTP control server (WiFi-based control)
- Bluetooth control server (RFCOMM-based control)

Both interfaces control the same motor driver (L298N) via a shared motor library.

Only one control interface should be used at a time.
