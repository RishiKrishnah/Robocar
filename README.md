# Robocar: WiFi-Controlled Robotic Vehicle with Air Quality Monitoring

## Overview

Robocar is an integrated IoT and robotics system that combines real-time robotic control with environmental monitoring. The system enables remote navigation of a robotic vehicle over WiFi while simultaneously collecting and visualizing air quality data, including gas concentration, temperature, humidity, and geographic location.

The architecture integrates embedded systems, networking, backend processing, and modern web interfaces to provide a complete end-to-end solution.

---

## Key Features

- Real-time robot control via web-based interface
- WiFi-based communication between client and robot
- Air quality monitoring using MQ135 gas sensor
- Temperature and humidity sensing using DHT22
- GPS-based location tracking
- Live AQI computation and categorization
- Interactive dashboard with dynamic visualization
- Google Maps integration for location tracking
- Keyboard and touch-based control support

---

## System Architecture

### Robot Control Flow
Web UI (Browser)
↓
HTTP Requests (WiFi)
↓
Arduino WiFi Server
↓
Motor Driver Control
↓
Robot Movement


### Sensor Data Flow
Sensors (MQ135, DHT22, GPS)
↓
Arduino Serial Output
↓
Flask Backend
↓
AQI Computation
↓
REST API (/data)
↓
Dashboard UI

---

## Project Structure
rishikrishnah-robocar/
├── README.md
├── app.py
├── index.html
├── air_polution/
│ ├── air_polution.ino
│ └── templates/
│ ├── index.html
│ └── index1.html
└── sketch_mar13a/
└── sketch_mar13a.ino


---

## Technologies Used

### Hardware
- Arduino (WiFi-enabled board)
- MQ135 Gas Sensor
- DHT22 Temperature and Humidity Sensor
- GPS Module
- Motor Driver Module

### Software
- Python (Flask)
- HTML, CSS, JavaScript
- Arduino C/C++
- Serial Communication
- REST APIs

---

## Installation and Setup

### 1. Arduino Setup (Robot Control)

- Open `sketch_mar13a/sketch_mar13a.ino`
- Update WiFi credentials:
  ```cpp
  char ssid[] = "YOUR_SSID";
  char pass[] = "YOUR_PASSWORD";

Upload the code to the Arduino
Note the IP address printed in the serial monitor
Arduino Setup (Air Quality Unit)
Open air_polution/air_polution.ino
Connect:
MQ135 → A0
DHT22 → Pin 2
GPS → Pins 3 and 4
Upload the code

Flask Backend

Install dependencies:

pip install flask pyserial

Update COM port in app.py:

ser = serial.Serial('COM5', 115200, timeout=1)

Run the server:

python app.py

Web Interface
Robot Control
Open index.html

Update Arduino IP:

const ARDUINO_IP = "http://<YOUR_ARDUINO_IP>";
Dashboard

Open in browser:

http://127.0.0.1:5000/

API Endpoints
GET /data

Returns latest sensor readings and computed AQI.

Response Format
{
  "gas": 250,
  "temperature": 30,
  "humidity": 60,
  "latitude": 12.9716,
  "longitude": 80.2431,
  "aqi": 120,
  "category": "Poor"
}

AQI Calculation

The AQI is computed using a piecewise linear approximation based on gas sensor values.

Gas Value Range	AQI Range
0–150	0–50
150–300	50–100
300–500	100–200
500–700	200–300
>700	300+
AQI Categories
AQI Range	Category
0–50	Good
51–100	Moderate
101–200	Poor
201–300	Unhealthy
>300	Hazardous
Control Commands
Command	Endpoint
Forward	/forward
Backward	/backward
Left	/left
Right	/right
Stop	/stop

Limitations
AQI is estimated using raw MQ135 values and is not calibrated to standard environmental metrics
Serial communication assumes stable connection and consistent data format
No authentication mechanism for robot control
WiFi credentials are hardcoded in firmware
Limited error handling in backend
Future Improvements
Sensor calibration for accurate AQI measurement
Cloud integration for remote monitoring and data storage
Authentication and security enhancements
Mobile application interface
Autonomous navigation capabilities
Historical data logging and analytics
Real-time alerts for hazardous air quality
License

This project is intended for academic and research purposes.
