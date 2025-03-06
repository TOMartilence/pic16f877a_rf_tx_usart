# Remote Healthcare Monitoring System

## Overview

This project provides a remote healthcare monitoring system designed for areas with limited or no internet connectivity. It uses a **PIC16F877A** microcontroller to collect data from multiple health sensors, transmits the data via **RF433** to an **ESP32**, which then forwards it using **MQTT** to a **Node.js server**. The data is stored in **MongoDB** and displayed on a **React** frontend.

## Features

- **Wireless Health Monitoring**: Collects patient data remotely using sensors.
- **RF433 Communication**: Enables data transmission without internet dependency.
- **ESP32 Bridge**: Relays data to an MQTT broker.
- **Cloud Storage & Visualization**: Stores health records in MongoDB and presents them in a React UI.
- **Real-time Monitoring**: Allows caregivers to track patient health remotely.

## System Architecture

1. **PIC16F877A**: Reads data from health sensors (e.g., heart rate, temperature, SpO2, etc.).
2. **RF433 Transmitter**: Sends sensor data wirelessly.
3. **ESP32 with RF433 Receiver**: Captures the transmitted data and forwards it via MQTT.
4. **Node.js Server**: Processes the received data and stores it in MongoDB.
5. **MongoDB**: Stores patient health records securely.
6. **React Frontend**: Displays the health data in a user-friendly interface.

## Hardware Components

- **PIC16F877A Microcontroller**
- **Health Sensors** (Heart Rate, Temperature, SpO2, etc.)
- **RF433 Transmitter & Receiver**
- **ESP32**
- **Power Supply**
- **Computer or Server for Node.js & MongoDB**

## Software Requirements

- **MPLAB X IDE & XC8 Compiler** (for PIC16F877A programming)
- **Arduino IDE** (for ESP32 firmware)
- **Node.js & Express** (backend server)
- **MQTT Broker (Mosquitto or similar)**
- **MongoDB** (database)
- **React** (frontend UI)

## Installation & Setup

### 1. Microcontroller Firmware

- Write and compile the **PIC16F877A** firmware using MPLAB X IDE.
- Program the microcontroller to read health sensor data and transmit via RF433.

### 2. ESP32 Configuration

- Install the necessary libraries for RF433 and MQTT in Arduino IDE.
- Upload the firmware to ESP32 to receive RF data and publish via MQTT.

### 3. Node.js Backend

- Install dependencies:
  ```bash
  npm install express mqtt mongoose cors
  ```
- Run the server:
  ```bash
  node server.js
  ```

### 4. MongoDB Database

- Set up MongoDB and ensure it is running.
- Define a schema to store health records.

### 5. React Frontend

- Install dependencies:
  ```bash
  npm install
  ```
- Start the frontend:
  ```bash
  npm start
  ```

## Future Enhancements

- Implement **LoRa** for long-range communication.
- Add **SMS alerts** for critical health data.
- Develop an **offline mode** for local data storage.

## Contribution

Feel free to fork and improve this project. Open issues and submit pull requests to enhance its functionality.

## License

This project is licensed under the MIT License.

