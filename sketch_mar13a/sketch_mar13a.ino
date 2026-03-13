#include <WiFiS3.h>

char ssid[] = "OnePlus Nord 3 5G";
char pass[] = "d8hq8can";

WiFiServer server(80);

// Motor Pins
int ena = 5;
int enb = 6;
int in1 = 8;
int in2 = 9;
int in3 = 10;
int in4 = 11;

void setup() {

  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  analogWrite(ena,200);
  analogWrite(enb,200);

  Serial.begin(115200);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  Serial.println("Connected!");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {

  WiFiClient client = server.available();
  if (!client) return;

  String request = client.readStringUntil('\r');
  client.flush();

  if (request.indexOf("/forward") != -1) forward();
  if (request.indexOf("/backward") != -1) backward();
  if (request.indexOf("/left") != -1) left();
  if (request.indexOf("/right") != -1) right();
  if (request.indexOf("/stop") != -1) stopCar();

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println();
  client.println("{\"status\":\"ok\"}");
}

void forward(){
 digitalWrite(in1,HIGH);
 digitalWrite(in2,LOW);
 digitalWrite(in3,HIGH);
 digitalWrite(in4,LOW);
}

void backward(){
 digitalWrite(in1,LOW);
 digitalWrite(in2,HIGH);
 digitalWrite(in3,LOW);
 digitalWrite(in4,HIGH);
}

void left(){
 digitalWrite(in1,LOW);
 digitalWrite(in2,HIGH);
 digitalWrite(in3,HIGH);
 digitalWrite(in4,LOW);
}

void right(){
 digitalWrite(in1,HIGH);
 digitalWrite(in2,LOW);
 digitalWrite(in3,LOW);
 digitalWrite(in4,HIGH);
}

void stopCar(){
 digitalWrite(in1,LOW);
 digitalWrite(in2,LOW);
 digitalWrite(in3,LOW);
 digitalWrite(in4,LOW);
}