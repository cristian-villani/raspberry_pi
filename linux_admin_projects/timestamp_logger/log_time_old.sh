#!/bin/bash

# Old version: created for learning / historical purposes.
# Writes logs to a local "logs" directory instead of journald.

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo working directory: $SCRIPT_DIR

LOG_DIR="$SCRIPT_DIR/logs"

echo log directory "("creating if necessary")": $LOG_DIR

mkdir -p "$LOG_DIR"

LOG_FILE="$LOG_DIR/timestamps.log"

echo log file: $LOG_FILE

#Adding date information to log file:

echo " "
echo " "
echo " "

echo "========================================="
echo "Service running, writing date to log file"
echo "========================================="
echo " "

echo "$(date '+%Y-%m-%d %H:%M:%S') - Service is running" >> "$LOG_FILE"

echo " "
echo "Timestamp successfully written to $LOG_FILE"
echo " "
