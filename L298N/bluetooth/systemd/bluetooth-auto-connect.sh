#!/bin/bash

PHONE_MAC="XX:XX:XX:XX:XX:XX" # Replace with MAC address of your mobile phone

check_connection() {
    bluetoothctl info $PHONE_MAC | grep -q 'Connected: yes'
}

while true; do
    if check_connection; then
        echo "Phone is connected."

        # Clean up before starting
        sudo pkill rfcomm
        sudo rfcomm release all

        echo "Starting RFCOMM listener..."

        sudo rfcomm listen /dev/rfcomm0 1

        echo "Connection lost. Cleaning up..."

        sudo rfcomm release all
    else
        echo "Phone not connected, waiting..."
        sleep 5
    fi
done
