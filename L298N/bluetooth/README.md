# Bluetooth version

The Bluetooth control system allows direct control via an Android application.

## Requirements
- Raspberry Pi paired with Android device
- Bluetooth enabled
- BlueZ installed

## System services and configuration

Two systemd services are used:

- Bluetooth auto-connect service
- RFCOMM listener service

## Configuration notes

For simplicity, some values are hardcoded and must be adjusted manually:

- The Bluetooth MAC address is defined in `bluetooth` scripts
- The systemd service files assume the repository is located at:
  `~/raspberry_pi/L298N/`
- The `bluetooth-auto-connect.sh` script path must match the location used in:
  `bluetooth-auto-connect.service`

If the project is moved to a different directory, update the service files accordingly.

## Bluetooth auto-connect script

The `bluetooth-auto-connect.sh` script is responsible for maintaining the Bluetooth connection with the paired Android device.

It continuously checks whether the phone is connected and ensures that the RFCOMM listener is started only when a valid connection exists.

### Behavior

- Periodically checks Bluetooth connection status using `bluetoothctl`
- If the phone is connected:
  - ensures no stale `rfcomm` processes are running
  - starts the RFCOMM listener (`rfcomm listen`)
- If the phone disconnects:
  - releases the RFCOMM interface
  - waits for reconnection

### Important note

- The phone MAC address is hardcoded inside this script
- The script is executed automatically by the `bluetooth-auto-connect.service`
- It must remain executable:

```
chmod +x bluetooth-auto-connect.sh
```

## Installation

To install:

```
sudo cp systemd/*.service /etc/systemd/system/
sudo systemctl daemon-reload
sudo systemctl enable bluetooth-auto-connect.service
sudo systemctl enable rfcomm-listen.service
```

To start:

```
sudo systemctl start bluetooth-auto-connect.service
sudo systemctl start rfcomm-listen.service
```

Once running, the system exposes a single RFCOMM channel for control.

Please not that only one mobile app can control the car at a time.

## Android app

The control logic is implemented in:

- `android/CarController.kt`
