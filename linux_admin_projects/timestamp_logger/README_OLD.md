# Old README

This README describes an older version of the project.  
For the current installation and usage instructions, see `README.md`.

# Timestamp Logger Service

This is a small demonstration project showing basic Linux administration skills on a Raspberry Pi.

### Historical versions

- `log_time_old.sh`: initial version, created for learning and experimenting with local logs.
- `log_time.sh`: cleaned-up version for systemd and journald.


## Purpose

This is a small learning project intended to explore:
- systemd services and timers
- Linux filesystem conventions
- basic installation and removal scripts

The focus is on correctness, structure, and understanding rather than feature complexity.


## What it does
- Logs the current timestamp to a file (`logs/timestamps.log`) every time the script is executed.
- Demonstrates:
  - Writing portable Bash scripts
  - Using relative paths
  - Creating log directories dynamically
  - Preparing a script to be run as a systemd service

## How to use

1. Make the script executable:
```bash
chmod +x log_time.sh
```

2. Run manually:
```
./log_time.sh
```

3. Check the log:
```
cat logs/timestamps.log
```


### System integration notes

For demonstration purposes, the script lives in this repository.
When installed system-wide on a Linux system, it is typically placed under:

- `/usr/local/bin/` for the executable script
- `/etc/systemd/system/` for the systemd service file
- `/var/log/` for log files

This follows standard Linux filesystem conventions and keeps configuration,
executables, and runtime data clearly separated.

## Installation steps:

```
1. Copy log_time.sh to /usr/local/bin
2. Copy unit files to /etc/systemd/system
3. Reload systemd
4. Enable and start the timer
```


## Notes

The logs/ directory is ignored by Git (.gitignore) because it contains runtime data.

This project is intended as a small demonstration of Linux admin concepts and can be extended to run automatically as a systemd service.
