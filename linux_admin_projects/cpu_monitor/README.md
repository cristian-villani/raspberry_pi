# Timestamp Logger

A small learning project for Linux administration and embedded Linux basics.  
It demonstrates:

- Writing a simple logging script in Bash
- Managing periodic tasks with **systemd timers**
- Logging via systemd and journald (stdout/stderr)
- Installation and uninstallation procedures
- Organization of scripts and systemd unit files

---

## Repository Structure

```
cpu-monitor/
├── log_cpu.sh             # Main logging script
├── install.sh             # Install script (copies files, enables timer)
├── uninstall.sh           # Uninstall script (removes files and timer)
├── systemd/
│ ├── cpu-monitor.service
│ └── cpu-monitor.timer
└── README.md              # Current instructions
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
log_cpu.sh is copied to /usr/local/bin/ and made executable

Logs are written to journald via systemd (stdout/stderr)

Systemd unit files are copied to /etc/systemd/system/

The timer is enabled and started automatically
```

## Checking the Timer

To see the timer status:

```
systemctl list-timers | grep cpu-monitor
```

To see logs written by the script:

```
journalctl -u cpu-monitor.service
```


## Uninstallation

If you want to remove the project completetly:


```
sudo ./uninstall.sh
```

This will:

```
Stop and disable the cpu monitor

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
