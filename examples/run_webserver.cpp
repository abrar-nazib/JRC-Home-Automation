#include <Arduino.h>
#include "JRC_SmartHome.h"
#include "JRC_WebServer.h"

JRC_SmartHome jrc_home;
JRC_WebServer jrc_webserver(&jrc_home);

void setup()
{

  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("JRC Board Smart Home");

  // Connect to WiFi
  // bool connect = jrc_webserver.connectToWiFi("Bruh!", "mairala123");
  bool connect = jrc_webserver.connectToWiFi();

  if (connect)
    // Start the web server if connected to wifi
    jrc_webserver.startServer();
}

void loop()
{
  jrc_webserver.cleanClients();
  jrc_webserver.updateSensorData();
}
