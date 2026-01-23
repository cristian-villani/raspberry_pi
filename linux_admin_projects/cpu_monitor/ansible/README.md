# Ansible Automation for Timestamp Logger

This folder contains an optional Ansible extension for the CPU Monitor project. It automates the installation and uninstallation of the service on a Raspberry Pi (or any Linux system).

## What is Ansible?

Ansible is a configuration management and automation tool. Instead of manually copying files and running commands, you define what you want your system to do in a playbook (playbook.yml), and Ansible executes it automatically.

For this project, Ansible automates:

```
Copying the log_cpu.sh script to /usr/local/bin

Copying the systemd unit files (.service and .timer) to /etc/systemd/system/

Reloading systemd so it recognizes the new units

Enabling and starting the timer automatically

(Optional) Uninstalling everything safely
```

Folder Structure

```
ansible/
├── playbook.yml               # Main playbook to install cpu-monitor
├── uninstall.yml              # Optional playbook to uninstall
└── files/
    ├── log_cpu.sh            # Service script used by playbook
    └── systemd/
        ├── cpu-monitor.service
        └── cpu-monitor.timer

    playbook.yml: Defines the tasks Ansible performs

    uninstall.yml: Tasks to remove the service safely

    files/log_cpu.sh: Source script to be installed

    files/systemd/: Contains the systemd unit files for the service and timer
```

## How to Use

1. Install Ansible

On Raspberry Pi / Linux:

```
sudo apt update
sudo apt install ansible
ansible --version
```

2. Run the install playbook

From the ansible/ folder:

```
ansible-playbook playbook.yml
```

This will:

```
Copy the script to /usr/local/bin (executable)

Copy the .service and .timer files to /etc/systemd/system/

Reload systemd

Enable and start the timer
```

3. Verify the service

Check timer status:

```
systemctl status cpu-monitor.timer
```


Follow logs:

```
journalctl -u cpu-monitor.service -f
```

4. Run the uninstall playbook

```
ansible-playbook uninstall.yml
```


This will:

```
Stop and disable the timer

Remove the systemd unit files

Remove the script from /usr/local/bin

Reload systemd
```
