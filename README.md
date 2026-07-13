# NFC OTA Bootloader for ATtiny1616

**A bare-metal, C-based Over-The-Air (OTA) bootloader for the ATtiny1616, designed for NFC smart devices using the ST25DV64K.**

## 📖 Project Overview
-TODO-

## 📂 Directory Structure
```text
.
├── include/            # Global header files
├── src/                # Source code
│   ├── hal/            # Hardware Abstraction Layer (Drivers for NFC, LED Matrix)
│   ├── mcal_lite/      # Microcontroller Abstraction Layer (Low-level register drivers)
│   ├── secure/         # OTA flow control and security logic
│   └── main.c          # Application entry point and state machine
├── Makefile            # Build system and hardware configuration targets
└── README.md
```

## 🛠 Hardware Architecture
The custom hardware:
* **Microcontroller:** Microchip ATtiny1616 
* **NFC Interface:** STMicroelectronics ST25DV64KC Dynamic NFC/RFID Tag 
* **LED Matrix Driver:** IS31FL3730 (I2C) 
* **Operating Voltage:** 3.3V

## 🧠 Software Architecture
The firmware is developed in **C** using bare-metal approach.

* **MCAL (Microcontroller Abstraction Layer):**  
* **HAL (Hardware Abstraction Layer):** Modular ST25DV64K and IS31FL3730 device drivers. 
* **APP (OTA Flow Control):**  

### Memory Map (ATtiny1616)
The hardware is partitioned using UPDI-programmed fuses:
* `0x0000 - 0x0FFF`: **Secure Bootloader** (4KB Partition via `BOOTEND` fuse) 
* `0x1000 - 0x3FFF`: **Application Space** (e.g., Snake Game or LED Matrix App) 

## ⚙️ Toolchain & Prerequisites
To compile and flash this project, you need:
* **AVR-GCC Toolchain** (`avr-gcc` 15.1.0+ / `avr-libc`) 
* **Make** * **UPDI Programmer** ( Atmel-ICE)  
* **avrdude** (v7.0+) for flashing ELF/HEX files 

## 🚀 Build Instructions
The project utilizes a makefile 
