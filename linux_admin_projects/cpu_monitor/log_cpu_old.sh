#!/bin/bash

# log_cpu.sh - simple CPU usage logger

# Old version: created for learning / historical purposes.
# Writes logs to a local "logs" directory instead of journald.

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo working directory: $SCRIPT_DIR

LOG_DIR="$SCRIPT_DIR/logs"

echo log directory "("creating if necessary")": $LOG_DIR

mkdir -p "$LOG_DIR"

LOG_FILE="$LOG_DIR/cpu.log"

echo log file: $LOG_FILE

echo " "
echo Getting time stamp
TIMESTAMP=$(date "+%Y-%m-%d %H:%M:%S")

#Adding date information to log file:

echo " "
echo " "
echo " "

echo "=============================================="
echo "Service running, writing cpu usage to log file"
echo "=============================================="
echo " "

echo "Service is running"
echo " "

CPU_TIME=$(top -b -n1 |grep "Cpu(s)" | sed 's/,/./g' | awk '{print $1, $2 + $4}')
echo $TIMESTAMP $CPU_TIME >> $LOG_FILE
echo "cpu time: " $CPU_TIME

echo " "
echo "Cpu usage successfully written to $LOG_FILE"
echo " "

