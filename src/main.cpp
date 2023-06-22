#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>
#include "WebServer_JRC.h"
#include "SmartHome.h"

#define LED_PIN 18
#define FAN_PIN 19
#define RAIN_SENSOR_PIN 32
#define LDR_PIN 23
#define PIR_PIN 33
#define DHTPIN 02
#define DOOR_PIN 16
#define DOOR_OPEN 30
#define DOOR_CLOSE 145
#define WINDOW_PIN 15
#define WINDOW_OPEN 60
#define WINDOW_CLOSE 165

// WiFi server parameters
WiFiServer server(80); // web server in port 80
String local_ip;

// Initialize the LED
LED led = LED(LED_PIN);
Fan fan = Fan(FAN_PIN);

ButtonInverted rain_sensor = ButtonInverted(RAIN_SENSOR_PIN);
Button pir_sensor = Button(PIR_PIN);
Button ldr_sensor = Button(LDR_PIN);

DHTSensor dht = DHTSensor(DHTPIN);
ServoMotor door = ServoMotor(DOOR_PIN, DOOR_OPEN, DOOR_CLOSE);
ServoMotor window = ServoMotor(WINDOW_PIN, WINDOW_OPEN, WINDOW_CLOSE);

LiquidCrystal_I2C lcd(0x3F, 16, 2); // set the LCD address to 0x3F for a 16 chars and 2 line display

void setup()
{
  Serial.begin(115200);

  // Connect to wifi
  local_ip = connectToWiFi();

  // Show the local IP in Liquid Crystal Display
  lcd.init();
  lcd.clear();
  lcd.backlight(); // Make sure backlight is on

  // Print a message on both lines of the LCD.
  lcd.setCursor(0, 0); // Set cursor to character 2 on line 0
  lcd.print("JRC Smart Home");

  lcd.setCursor(0, 1); // Move cursor to character 2 on line 1
  lcd.print(local_ip);

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
  // Check for sensors for automation of the LED, Fan, Door and Window
  automate();
}
