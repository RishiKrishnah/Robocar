# Robocar

An autonomous air quality monitoring robot that combines wireless motor control with real-time environmental sensing. The system collects gas, temperature, humidity, and GPS data from a mobile unit and streams it to a web dashboard, while a separate web interface provides remote directional control over Wi-Fi.

---

## Overview

The project consists of three independent subsystems that work together:

- **Motor Control (Arduino Uno R4 Wi-Fi)** — Hosts an HTTP server over Wi-Fi. Accepts directional commands (`/forward`, `/backward`, `/left`, `/right`, `/stop`) and drives a dual H-bridge motor driver.
- **Air Quality Sensor Unit (Arduino + Serial)** — Reads MQ135 gas sensor, DHT22 temperature/humidity sensor, and a GPS module. Streams CSV data over serial every 2 seconds.
- **Flask Web Server (Python)** — Reads the serial stream from the sensor unit, computes AQI, and serves two frontends: a control dashboard and an air quality dashboard.

---

## Repository Structure

```
rishikrishnah-robocar/
├── app.py                          # Flask server — serial reader + API
├── index.html                      # Robot control UI (D-pad)
├── air_polution/
│   ├── air_polution.ino            # Sensor unit firmware (MQ135 + DHT22 + GPS)
│   └── templates/
│       ├── index.html              # Basic air quality page
│       └── index1.html             # Polished air quality dashboard
└── sketch_mar13a/
    └── sketch_mar13a.ino           # Motor control firmware (Wi-Fi HTTP server)
```

---

## Hardware Requirements

### Motor Control Unit
| Component | Details |
|-----------|---------|
| Arduino Uno R4 Wi-Fi | Main controller |
| L298N Motor Driver | Dual H-bridge, ENA/ENB + IN1-IN4 |
| DC Motors (x2) | Connected to motor driver outputs |
| Power Supply | External battery for motors |

**Pin Mapping (sketch_mar13a.ino)**

| Arduino Pin | Function |
|-------------|----------|
| 5 | ENA (Motor A speed) |
| 6 | ENB (Motor B speed) |
| 8 | IN1 |
| 9 | IN2 |
| 10 | IN3 |
| 11 | IN4 |

### Sensor Unit
| Component | Details |
|-----------|---------|
| Arduino (Uno/Nano) | Main controller |
| MQ135 Gas Sensor | Analog pin A0 |
| DHT22 | Digital pin 2 |
| GPS Module (NMEA) | SoftwareSerial — RX: pin 4, TX: pin 3 |

---

## Software Requirements

- Python 3.8+
- Flask
- pyserial
- Arduino IDE 2.x

### Python Dependencies

```bash
pip install flask pyserial
```

### Arduino Libraries

Install via Arduino IDE Library Manager:

- `DHT sensor library` by Adafruit
- `TinyGPS++` by Mikal Hart
- `WiFiS3` (bundled with Arduino Uno R4 board package)

---

## Setup and Configuration

### 1. Motor Control Firmware

Open `sketch_mar13a/sketch_mar13a.ino` and update the Wi-Fi credentials:

```cpp
char ssid[] = "YOUR_NETWORK_NAME";
char pass[] = "YOUR_PASSWORD";
```

Flash to the Arduino Uno R4 Wi-Fi. After connecting, the assigned IP address is printed to Serial Monitor. Update the control UI with this address:

In `index.html`, find and update:

```javascript
const ARDUINO_IP = "http://<ARDUINO_IP_HERE>";
```

### 2. Sensor Unit Firmware

Open `air_polution/air_polution.ino` and flash to the sensor Arduino. No configuration required — it begins streaming CSV data over Serial at 115200 baud on power-up.

Output format:
```
gasRaw,temperature,humidity,latitude,longitude
```

### 3. Flask Server

Connect the sensor Arduino to your PC via USB. Update the serial port in `app.py`:

```python
ser = serial.Serial('COM5', 115200, timeout=1)  # Windows
# ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)  # Linux/macOS
```

Start the server:

```bash
python app.py
```

The server runs at `http://localhost:5000` by default.

---

## Usage

### Air Quality Dashboard

Navigate to `http://localhost:5000` to view:

- Live AQI index with color-coded gauge
- Temperature and humidity readings
- Raw gas sensor value
- GPS location embedded in a map (updates every 2 seconds)

### Robot Control

Open `index.html` directly in a browser (or serve it separately). Use the D-pad interface or keyboard shortcuts to send movement commands to the Arduino over Wi-Fi.

| Key | Action |
|-----|--------|
| Arrow Up / W | Forward |
| Arrow Down / S | Backward |
| Arrow Left / A | Left |
| Arrow Right / D | Right |
| Space | Stop |

---

## AQI Calculation

The AQI is derived from the raw MQ135 analog reading (0-1023) using a linear scale mapped to standard AQI bands:

| Gas Raw Value | AQI Range | Category |
|---------------|-----------|----------|
| 0 - 150 | 0 - 50 | Good |
| 151 - 300 | 51 - 100 | Moderate |
| 301 - 500 | 101 - 200 | Poor |
| 501 - 700 | 201 - 300 | Unhealthy |
| 700+ | 300+ | Hazardous |

> Note: The MQ135 requires a warmup period of approximately 24-48 hours for accurate baseline readings. Values during initial startup may not reflect actual air quality accurately.

---

## Known Limitations

- The MQ135 gas sensor is uncalibrated and provides a relative index rather than PPM readings for specific gases.
- GPS may take several minutes to acquire a fix indoors or in areas with poor satellite visibility. Location will display as `0.0, 0.0` until a valid fix is obtained.
- The Flask server and robot control Arduino must be on the same local network for the control interface to function.
- Serial port must be manually configured in `app.py` to match the host OS and connected port.

---

## License

This project is open source. See [LICENSE](LICENSE) for details.
