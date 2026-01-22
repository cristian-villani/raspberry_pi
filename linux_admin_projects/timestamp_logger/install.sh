#!/bin/bash
set -e

# Ensure script is run as root
if [[ $EUID -ne 0 ]]; then
   echo "This install script must be run as root." 
   exit 1
fi

echo "Starting installation of Timestamp Logger..."

# Paths
SCRIPT_SRC="./log_time.sh"
SCRIPT_DEST="/usr/local/bin/log_time.sh"

echo "Copying log_time.sh to $SCRIPT_DEST"
cp "$SCRIPT_SRC" "$SCRIPT_DEST"
chmod +x "$SCRIPT_DEST"

LOG_DIR="/var/log/timestamp-logger"
echo "Creating log directory: $LOG_DIR"
mkdir -p "$LOG_DIR"
chown root:root "$LOG_DIR"
chmod 755 "$LOG_DIR"

# Copy systemd unit files from repo
SYSTEMD_DIR="./systemd"
SERVICE_FILE="$SYSTEMD_DIR/timestamp-logger.service"
TIMER_FILE="$SYSTEMD_DIR/timestamp-logger.timer"

echo "Copying systemd service and timer files..."
cp "$SERVICE_FILE" /etc/systemd/system/
cp "$TIMER_FILE" /etc/systemd/system/

# Reload systemd and enable timer
systemctl daemon-reload
systemctl enable --now timestamp-logger.timer

echo "Installation complete. Timer is active!"

