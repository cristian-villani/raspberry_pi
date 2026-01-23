#!/bin/bash
set -e

# Ensure script is run as root
if [[ $EUID -ne 0 ]]; then
   echo "This uninstall script must be run as root (use sudo)."
   exit 1
fi

echo "Starting uninstallation..."

# Stop and disable the timer
systemctl stop cpu-monitor.timer || true
systemctl disable cpu-monitor.timer || true

# Remove systemd unit files
rm -f /etc/systemd/system/cpu-monitor.timer
rm -f /etc/systemd/system/cpu-monitor.service

# Reload systemd
systemctl daemon-reload

# Remove main script
rm -f /usr/local/bin/log_cpu.sh

# Remove log directory
# rm -rf /var/log/cpu-monitor

echo "Uninstallation complete."

