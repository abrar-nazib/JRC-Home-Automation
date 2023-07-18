#include <Arduino.h>
#include <FS.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <SPIFFSEditor.h>

#include "JRC_WebServer.h"

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

JRC_WebServer::JRC_WebServer(JRC_SmartHome *jrc_home) : jrc_home(jrc_home)
{
}

bool JRC_WebServer::connectToWiFi(const char *ssid, const char *password)
{

  Serial.println();
  Serial.println("Connecting to WiFi");
  Serial.println();

  WiFi.begin(ssid, password);

  // while (WiFi.status() != WL_CONNECTED)
  for (int i = 0; i < 20; i++)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
    if (WiFi.status() == WL_CONNECTED)
    {
      break;
    }
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Connection failed");
    this->ip = "Not Connected";
    this->wifi_connected = false;
    return false;
  }

  Serial.println("Connected to the WiFi network");
  Serial.println("IP address: " + WiFi.localIP().toString());

  this->wifi_connected = true;
  this->ip = WiFi.localIP().toString();
  return true;
}

void JRC_WebServer::startServer()
{
  SPIFFS.begin();

  ws.onEvent([this](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
             {
               if (type == WS_EVT_CONNECT)
               {
                 Serial.println("Client connected");
                 // Construct the json object
                 DynamicJsonDocument doc(1024);
                 doc = readAllData();
                 String str = JSONtoString(doc);

                 client->text(str);
               }
               else if (type == WS_EVT_DISCONNECT)
               {
                 Serial.println("Client disconnected");
               }
               else if (type == WS_EVT_DATA)
               {
                 AwsFrameInfo *info = (AwsFrameInfo *)arg;
                 String msg = "";

                 // Put the message into a string
                 for (size_t i = 0; i < info->len; i++)
                 {
                   msg += (char)data[i];
                 }

                 // Construct the json object
                 DynamicJsonDocument clientMessage(1024);
                 clientMessage = stringToJSON(msg);
                 setOutputDeviceStates(clientMessage);
               }

               // thing
             });

  server.addHandler(&ws);

  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
  server.serveStatic("/index.html", SPIFFS, "/index.html");
  server.serveStatic("/script.js", SPIFFS, "/script.js");
  server.onNotFound([](AsyncWebServerRequest *request)
                    { request->send(404, "text/plain", "Not found"); });

  server.begin();
}

String JRC_WebServer::JSONtoString(DynamicJsonDocument doc)
{
  String output;
  serializeJson(doc, output);
  return output;
}

DynamicJsonDocument JRC_WebServer::stringToJSON(String str)
{

  DynamicJsonDocument d(1024);
  deserializeJson(d, str);
  return d;
}

DynamicJsonDocument JRC_WebServer::readAllData()
{
  // Get the current state of the output devices
  DynamicJsonDocument light(1024);
  light["state"] = jrc_home->light.getStateStr();
  light["automationState"] = jrc_home->light.getAutomationStateStr();
  DynamicJsonDocument fan(1024);
  fan["state"] = jrc_home->fan.getStateStr();
  fan["automationState"] = jrc_home->fan.getAutomationStateStr();
  DynamicJsonDocument door(1024);
  door["state"] = jrc_home->door.getStateStr();
  door["automationState"] = jrc_home->door.getAutomationStateStr();
  DynamicJsonDocument window(1024);
  window["state"] = jrc_home->window.getStateStr();
  window["automationState"] = jrc_home->window.getAutomationStateStr();

  // Get the current reading of sensors
  DynamicJsonDocument sensorReadings(1024);
  sensorReadings["temp"] = jrc_home->dht_sensor.getTemperatureStr();
  sensorReadings["humid"] = jrc_home->dht_sensor.getHumidityStr();
  sensorReadings["pir"] = jrc_home->pir_sensor.getStateStr();
  sensorReadings["ldr"] = jrc_home->ldr_sensor.getStateStr();
  sensorReadings["rain"] = jrc_home->rain_sensor.getStateStr();

  DynamicJsonDocument outputDevices(1024);
  outputDevices["light"] = light;
  outputDevices["fan"] = fan;
  outputDevices["door"] = door;
  outputDevices["window"] = window;

  DynamicJsonDocument message(1024);
  message["outputDevices"] = outputDevices;
  message["sensorReadings"] = sensorReadings;

  return message;
}

void JRC_WebServer::setOutputDeviceStates(DynamicJsonDocument outputDevices)
{

  jrc_home->light.setState(outputDevices["light"]["state"].as<String>());
  jrc_home->light.setAutomationState(outputDevices["light"]["automationState"].as<String>());

  jrc_home->fan.setState(outputDevices["fan"]["state"].as<String>());
  jrc_home->fan.setAutomationState(outputDevices["fan"]["automationState"].as<String>());

  jrc_home->door.setState(outputDevices["door"]["state"].as<String>());
  jrc_home->door.setAutomationState(outputDevices["door"]["automationState"].as<String>());

  jrc_home->window.setState(outputDevices["window"]["state"].as<String>());
  jrc_home->window.setAutomationState(outputDevices["window"]["automationState"].as<String>());
}
float prev = millis();
void JRC_WebServer::updateSensorData()
{
  if (millis() - prev > 400)
  {
    DynamicJsonDocument message = readAllData();
    String msg = JSONtoString(message);
    if (
        jrc_home->dht_sensor.getTemperatureDifference() || jrc_home->dht_sensor.getHumidityDifference() || jrc_home->ldr_sensor.getDifference() || jrc_home->rain_sensor.getDifference() || jrc_home->pir_sensor.getDifference())
    {
      Serial.println(msg);
      if (wifi_connected) // Only emit websocket message if wifi is connected
        ws.textAll(msg);
    }
    prev = millis();
  }
}

void JRC_WebServer::cleanClients()
{
  ws.cleanupClients();
}