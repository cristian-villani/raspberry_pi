#!/bin/bash
set -e

# Ensure script is run as root
if [[ $EUID -ne 0 ]]; then
   echo "This uninstall script must be run as root (use sudo)."
   exit 1
fi

echo "Starting uninstallation..."

# Stop and disable the timer
systemctl stop timestamp-logger.timer || true
systemctl disable timestamp-logger.timer || true

# Remove systemd unit files
rm -f /etc/systemd/system/timestamp-logger.timer
rm -f /etc/systemd/system/timestamp-logger.service

# Reload systemd
systemctl daemon-reload

# Remove main script
rm -f /usr/local/bin/log_time.sh

# Remove log directory
rm -rf /var/log/timestamp-logger

echo "Uninstallation complete."

