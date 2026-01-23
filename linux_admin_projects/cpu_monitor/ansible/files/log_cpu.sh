#!/bin/bash

# log_cpu.sh - simple CPU usage logger

TIMESTAMP=$(date "+%Y-%m-%d %H:%M:%S")

CPU_TIME=$(top -b -n1 |grep "Cpu(s)" | sed 's/,/./g' | awk '{print $1, $2 + $4}')
echo "$TIMESTAMP $CPU_TIME - service is running"

