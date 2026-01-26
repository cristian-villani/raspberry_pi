# Timestamp Logger

A small learning project for Linux administration and embedded Linux basics.  
It demonstrates:

- Writing a simple logging script in Bash
- Managing periodic tasks with **systemd timers**
- Logging via systemd and journald (stdout/stderr)
- File-based logging demonstrated in historical scripts
- Installation and uninstallation procedures
- Organization of scripts and systemd unit files

---

## Repository Structure

```
timestamp-logger/
├── log_time.sh # Main logging script
├── install.sh # Install script (copies files, enables timer)
├── uninstall.sh # Uninstall script (removes files and timer)
├── systemd/
│ ├── timestamp-logger.service
│ └── timestamp-logger.timer
├── README.md # Current instructions
└── README_OLD.md # Previous version of README for reference
```


---

## Installation

1. Make the installer executable:

```
chmod +x install.sh
```

2. Run the installer as root:

```
sudo ./install.sh
```

What happens:

```
log_time.sh is copied to /usr/local/bin/ and made executable

Logs are written to journald via systemd (stdout/stderr)

Systemd unit files are copied to /etc/systemd/system/

The timer is enabled and started automatically
```

## Checking the Timer

To see the timer status:

```
systemctl list-timers | grep timestamp-logger
```

To see logs written by the script:

```
journalctl -u timestamp-logger.service
```

## Example Output

```
2026-01-22 13:55:24 - Service is running
2026-01-22 13:56:11 - Service is running
2026-01-22 13:57:29 - Service is running
2026-01-22 14:00:33 - Service is running
2026-01-22 14:00:56 - Service is running
2026-01-26 12:54:40 - Service is running
2026-01-26 12:54:49 - Service is running
2026-01-26 12:54:50 - Service is running
```


## Uninstallation

If you want to remove the project completetly:


```
sudo ./uninstall.sh
```

This will:

```
Stop and disable the timer

Remove unit files from /etc/systemd/system/

Remove the script from /usr/local/bin/

Reload systemd to clean up
```

## Automation

A simple Ansible playbook is included as an optional automation layer, showing reproducible installation and service management.

## Notes

```
This project is intended for learning and demonstration purposes

Logs are written to journald via systemd (stdout/stderr)

Systemd units are managed under /etc/systemd/system/

```

## License

This project is open for learning and portfolio purposes. Use freely.
