#include <DHT.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// ----------- Pin Definitions -----------
#define MQ135_PIN A0
#define DHTPIN 2
#define DHTTYPE DHT22

#define GPS_RX 4   // Arduino receives from GPS TX
#define GPS_TX 3   // Arduino transmits to GPS RX

// ----------- Objects -----------
DHT dht(DHTPIN, DHTTYPE);
TinyGPSPlus gps;
SoftwareSerial gpsSerial(GPS_TX, GPS_RX);

// ----------- Variables -----------
unsigned long previousMillis = 0;
const long interval = 2000;  // 2 seconds

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600);
  dht.begin();

  Serial.println("Air Quality Mobile Unit Initialized");
}

void loop() {

  // Read GPS continuously
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // -------- MQ135 Reading --------
    int gasRaw = analogRead(MQ135_PIN);

    // -------- DHT22 Reading --------
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity)) {
      temperature = -1;
      humidity = -1;
    }

    // -------- GPS Data --------
    float latitude = 0.0;
    float longitude = 0.0;

    if (gps.location.isValid()) {
      latitude = gps.location.lat();
      longitude = gps.location.lng();
    }

    // -------- Send CSV Format --------
    // Format:
    // gasRaw,temperature,humidity,latitude,longitude

    Serial.print(gasRaw);
    Serial.print(",");
    Serial.print(temperature);
    Serial.print(",");
    Serial.print(humidity);
    Serial.print(",");
    Serial.print(latitude, 6);
    Serial.print(",");
    Serial.println(longitude, 6);
  }
}