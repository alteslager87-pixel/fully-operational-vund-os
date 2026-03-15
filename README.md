# 👑 VUND-DOS Gold Edition v2.00
**Architect:** Yaroslav (The Coding God)  
**Organization:** VUND-DOS TEAM  
**Status:** READY-TO-USE (ISO & Binary Included)
##DO IT!!! Read The Fucking Manual
---
##DO IT!!! Read The Fucking Manual
---
## 🚀 Instant Launch
This repository contains the pre-compiled bootable files. You don't need to compile anything unless you want to change the source code.

### 📦 Included Files:
- `boot.bin`: Compiled 512-byte Bootloader.
- `core.bin`: Compiled Monolithic Kernel.
- `vund dos.iso`: Final merged bootable image for USB/VirtualBox.

## 🛠 Features
- **Monolithic Architecture:** High-speed hardware execution.
- **GUI Engine:** VGA Mode 13h support (`createimage` command).
- **VUND-AIR:** Global networking simulation.
- **18+ Native Commands:** `tasks`, `monitor`, `tree`, `system_info`, and more.
- **Hardcoded Hotkeys:** Instant Browser (`Ctrl+B`) and Security Lock (`Ctrl+L`).

## 💿 Installation Guide (The Easy Way)

### 1. Download the ISO
Download the `vund dos.iso` from this repository to your Desktop.

### 2. Write to USB
1. Open **Win32 Disk Imager**.
2. Select `vund dos.iso` (Change file filter to "All files *.*").
3. Choose your USB Flash Drive.
4. Click **Write**.

### 3. Boot
Restart your computer and press your Boot Menu key (F11, F12, or Esc). Select the USB drive.
*Make sure **Legacy Boot / CSM** is enabled in your BIOS!*

---
## 👨‍💻 Development
If you want to modify the OS, use `boot.asm` and `core.asm` with the **FASM** assembler.

"Without this project, I would be just an ordinary boy. With VUND-DOS, I am a Creator." — *Yaroslav*
