# 👑 VUND-OS v2.00 GOLD EDITION

![Version](https://img.shields.io/badge/Version-2.00_GOLD-gold)
![Status](https://img.shields.io/badge/Status-STABLE-green)
![Author](https://img.shields.io/badge/Author-Coding_God_Yaroslav-blue)
![Architecture](https://img.shields.io/badge/Arch-x86_Monolithic-orange)
![License](https://img.shields.io/badge/License-VUND_Private-red)

**VUND-OS** is a high-performance monolithic operating system designed for total hardware control. No bloatware, no lag—just pure code and direct port manipulation.

---

## 🚀 Key Features

* 📦 **Monolithic Architecture:** All drivers (Video, Keyboard, IDE, PC Speaker) are baked directly into the kernel for maximum execution speed.
* 🛠 **VUND Setup:** Custom pre-build installer to configure driver modules (Mouse, Speaker, Ethernet).
* 💾 **Filesystem v2:** Integrated commands like `create fi`, `create fo`, and the legendary `goto dsk` for drive switching.
* 📟 **KIM-1 Monitor:** Built-in low-level HEX memory monitor for true system hackers.
* 🎹 **Mechanical Keyboard Optimization:** High-polling PS/2 driver for the best tactile typing experience.

---

## 🛠 How to Build

Requirements: **GCC** and **Binutils** (i386-elf-gcc recommended).

1. **Configuration:**
   ```bash
   gcc setup.c -o setup.exe
   ./setup.exe
