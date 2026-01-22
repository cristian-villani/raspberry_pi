# Timestamp Logger

A small learning project for Linux administration and embedded Linux basics.  
It demonstrates:

- Writing a simple logging script in Bash
- Managing periodic tasks with **systemd timers**
- Proper logging to `/var/log` and optional journald integration
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

Log directory /var/log/timestamp-logger/ is created

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
cat /var/log/timestamp-logger/timestamps.log
```

Or, if using journald output:

```
journalctl -u timestamp-logger.service


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

Remove the log directory /var/log/timestamp-logger/

Reload systemd to clean up
```

## Automation

The raspberry_pi/timestamp-logger folder contains a small Linux service for learning embedded administration.
A simple Ansible playbook is included as an optional automation layer, showing reproducible installation and service management.

## Notes

```
This project is intended for learning and demonstration purposes

Logs are stored in /var/log/timestamp-logger/ by default

Systemd units are managed under /etc/systemd/system/

The previous version of the project (without systemd integration) is documented in README_OLD.md
```

## License

This project is open for learning and portfolio purposes. Use freely.
