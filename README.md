# esp32-adafruit-fingerprint-system

ESP32 project that integrates an **Adafruit Fingerprint Sensor** using a clean, layered architecture:
**UART Communication → Protocol Handling → Business Logic → State Machine**.

This project focuses on structure, separation of concerns, and maintainability rather than a monolithic sketch.

## Features
- UART-based communication with **Adafruit Fingerprint Sensor**
- Encapsulation over `Adafruit_Fingerprint` library
- Clear separation between hardware, logic, and control flow
- Centralized finite state machine (FSM)
- Minimal logic in the entry file

## Based On
- Hardware: Adafruit-compatible fingerprint sensor (UART / TTL)
- Library: `Adafruit_Fingerprint`

The Adafruit library is wrapped rather than used directly across the codebase, to avoid tight coupling.

## Project Structure
- `fingerprint.ino` 
  Entry point. Initializes UART, services, and runs the controller loop.
- `FingerprintSensor.*`  
  Thin wrapper around `Adafruit_Fingerprint`. Handles sensor initialization and low-level calls.
- `FingerprintService.*`  
  High-level fingerprint operations (enroll, identify, delete, etc.).
- `SystemController.*`  
  Finite State Machine that drives the system flow.
- `SystemState.*`  
  State definitions used by the controller.

## Hardware Requirements
- ESP32 development board
- Adafruit Fingerprint Sensor (UART)
- Wiring example (UART2 recommended):
  - Sensor TX → ESP32 RX (e.g. GPIO16)
  - Sensor RX → ESP32 TX (e.g. GPIO17)
  - VCC / GND according to sensor specs

## Build & Upload

### Arduino IDE
1. Install ESP32 board support.
2. Install **Adafruit Fingerprint Sensor Library**.
3. Select board and port.
4. Upload.

### PlatformIO
- Add `adafruit/Adafruit Fingerprint Sensor Library` as a dependency.
- Build and upload normally.

## Configuration
- UART port and pins are configurable in the sensor initialization.
- Default baud rate should match the sensor configuration (commonly `57600`).

## Design Notes
- The Adafruit library is **not used directly** in the controller or state machine.
- All sensor interaction goes through `FingerprintSensor` to keep the system testable and extensible.
- The architecture allows replacing the sensor/library with minimal changes.
