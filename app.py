from flask import Flask, render_template, jsonify
import serial

app = Flask(__name__)

# Change COM port (Windows: COM3, Linux: /dev/ttyUSB0)
ser = serial.Serial('COM5', 115200, timeout=1)

latest_data = {}

def calculate_aqi(gas):
    if gas <= 150:
        return int((gas / 150) * 50)
    elif gas <= 300:
        return int(50 + (gas - 150) * (50 / 150))
    elif gas <= 500:
        return int(100 + (gas - 300) * (100 / 200))
    elif gas <= 700:
        return int(200 + (gas - 500) * (100 / 200))
    else:
        return 300

def get_category(aqi):
    if aqi <= 50:
        return "Good"
    elif aqi <= 100:
        return "Moderate"
    elif aqi <= 200:
        return "Poor"
    elif aqi <= 300:
        return "Unhealthy"
    else:
        return "Hazardous"

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/data')
def data():
    global latest_data

    if ser.in_waiting:
        line = ser.readline().decode().strip()
        try:
            gas, temp, hum, lat, lon = line.split(',')

            gas = int(gas)
            temp = float(temp)
            hum = float(hum)
            lat = float(lat)
            lon = float(lon)

            aqi = calculate_aqi(gas)
            category = get_category(aqi)

            latest_data = {
                "gas": gas,
                "temperature": temp,
                "humidity": hum,
                "latitude": lat,
                "longitude": lon,
                "aqi": aqi,
                "category": category
            }

        except:
            pass

    return jsonify(latest_data)

if __name__ == '__main__':
    app.run(debug=False)