# 🛡️ Arch USB Guardian

Hi there! This is a lightweight, low-level USB security scanner I built specifically for **Arch Linux**. It’s designed to keep an eye on your system's "backdoor" by automatically detecting and hashing files on any USB device the moment it's plugged in.

## 🤔 Why I Built This
Most modern desktops auto-mount USBs, but they don't tell you what's actually inside from a security perspective. I wanted to create a tool that:
1. Listens to the Linux Kernel (`udev`) directly.
2. Handles the "dirty work" of mounting partitions (NTFS, EXT4, etc.).
3. Provides a digital fingerprint (SHA-256) for every file to help identify potential threats.

## ✨ Key Features
- **Instant Detection:** No polling. It uses `libudev` to react the millisecond a device is connected.
- **Smart Mounting:** If your system doesn't auto-mount the USB, this tool will attempt to do it for you.
- **Crypto-Scanning:** Every file is processed through the SHA-256 algorithm via `PicoSHA2`.
- **Arch-Centric:** Built and tested on Arch Linux, but works on most distros with `libudev`.

## 🚀 Quick Start

### What you'll need:
- `libudev-dev` (for hardware monitoring)
- `ntfs-3g` (if you're scanning Windows-formatted USBs)
- A C++17 compliant compiler (`g++`)

### Compile:
Open your terminal and run:
```bash
g++ src/main.cpp src/scan.cpp -o usb_guardian -ludev -std=c++17 -Iinclude