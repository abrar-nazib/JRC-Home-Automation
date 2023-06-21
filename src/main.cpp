#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "WebServer_JRC.h"
#include "SmartHome.h"

#define LED_PIN 13
#define FAN_PIN 14
#define RAIN_SENSOR_PIN 15
#define LDR_PIN 23
#define PIR_PIN 16
#define DHTPIN 5
#define DOOR_PIN 17
#define WINDOW_PIN 4

// WiFi server parameters
WiFiServer server(80); // web server in port 80
String local_ip;

// Initialize the LED
LED led = LED(LED_PIN);
Fan fan = Fan(FAN_PIN);

Button rain_sensor = Button(RAIN_SENSOR_PIN);
Button pir_sensor = Button(PIR_PIN);
Button ldr_sensor = Button(LDR_PIN);

DHTSensor dht = DHTSensor(DHTPIN);
ServoMotor door = ServoMotor(DOOR_PIN, 0, 90);     // 0 degree is open, 90 degree is closed
ServoMotor window = ServoMotor(WINDOW_PIN, 0, 90); // 0 degree is open, 90 degree is closed

void setup()
{
  Serial.begin(115200);

  // Connect to wifi
  local_ip = connectToWiFi();

  // Start the server
  server.begin();
}

void loop()
{

  WiFiClient client = server.available(); // listen for incoming clients
  if (client)
  {
    Serial.println("New client connected");
    Serial.print("Client IP: ");
    Serial.println(client.remoteIP());
    Header header = readHeader(client);

    handleRequest(client, header.uri);

    client.stop();
    Serial.println("Client disconnected");
    Serial.println();
  }
}
