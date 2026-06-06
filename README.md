
# Toll Gate System - ESP32

## Overview
Vehicle detection + RFID payment card reader → automatic gate control

## Features
- ✅ RFID card reader for payment verification
- ✅ Ultrasonic sensor for vehicle detection
- ✅ Servo motor for gate control
- ✅ Buzzer alarm for unauthorized access
- ✅ RGB LED status indicator
- ✅ Smooth servo movement

## Hardware
- ESP32 microcontroller
- MFRC522 RFID reader
- HC-SR04 Ultrasonic sensor
- SG90 Servo motor
- Buzzer module
- RGB LED (common cathode)

## How It Works
1. Car approaches → ultrasonic detects
2. Driver taps RFID card
3. If authorized → gate opens (5 sec)
4. If not authorized → alarm sounds
5. Gate auto-closes after timeout

## Pin Configuration
- RFID: GPIO 2 (RST), GPIO 5 (SS)
- Ultrasonic: GPIO 13 (TRIG), GPIO 34 (ECHO)
- Servo: GPIO 4
- Buzzer: GPIO 15
- RGB LED: GPIO 21 (RED), 22 (GREEN), 23 (BLUE)

## Installation
1. Install Arduino IDE
2. Install MFRC522 library
3. Upload `TollgateSystem.ino` to ESP32
4. Add authorized card UIDs in code

## Status Indicators
- 🔵 BLUE: Ready / Gate Closed
- 🟢 GREEN: Car Detected / Gate Opening
- 🟡 YELLOW: Car Approaching
- 🔴 RED: Alarm / Unauthorized Access

## Author
Shreyaz Bijukchhe
