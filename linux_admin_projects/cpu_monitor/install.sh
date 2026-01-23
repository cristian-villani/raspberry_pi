#!/bin/bash
set -e

# Ensure script is run as root
if [[ $EUID -ne 0 ]]; then
   echo "This install script must be run as root." 
   exit 1
fi

echo "Starting installation of CPU monitor..."

# Paths
SCRIPT_SRC="./log_cpu.sh"
SCRIPT_DEST="/usr/local/bin/log_cpu.sh"

echo "Copying log_cpu.sh to $SCRIPT_DEST"
cp "$SCRIPT_SRC" "$SCRIPT_DEST"
chmod +x "$SCRIPT_DEST"

#LOG_DIR="/var/log/cpu-monitor"
#echo "Creating log directory: $LOG_DIR"
#mkdir -p "$LOG_DIR"
#chown root:root "$LOG_DIR"
#chmod 755 "$LOG_DIR"

# Copy systemd unit files from repo
SYSTEMD_DIR="./systemd"
SERVICE_FILE="$SYSTEMD_DIR/cpu-monitor.service"
TIMER_FILE="$SYSTEMD_DIR/cpu-monitor.timer"

echo "Copying systemd service and timer files..."
cp "$SERVICE_FILE" /etc/systemd/system/
cp "$TIMER_FILE" /etc/systemd/system/

# Reload systemd and enable cpu monitor
systemctl daemon-reload
systemctl enable --now cpu-monitor.timer

echo "Installation complete. Cpu-monitor is active!"

