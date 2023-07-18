#ifndef _JRC_WEBSERVER
#define _JRC_WEBSERVER
#include <Arduino.h>
#include <FS.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <SPIFFSEditor.h>

#include "JRC_SmartHome.h"

class JRC_WebServer
{
  JRC_SmartHome *jrc_home;
  bool wifi_connected = false;
  String ip = "Not Connected";

public:
  /**
   * @brief Construct a new JRC_WebServer object
   *
   * @param jrc_home
   */
  JRC_WebServer(JRC_SmartHome *jrc_home);

  /**
   * @brief Connect to WiFi
   *
   * @param ssid
   * @param password
   * @return true
   * @return false
   */
  bool connectToWiFi(const char *ssid = "Qubitech Solutions", const char *password = "RajshahiRuet");

  /**
   * @brief Start the web server
   *
   */
  void startServer();

  /**
   * @brief Construct a json document
   *
   * @param str
   * @return DynamicJsonDocument
   */
  DynamicJsonDocument stringToJSON(String str);

  /**
   * @brief Construct a String object
   *
   * @param doc
   * @return String
   */
  String JSONtoString(DynamicJsonDocument doc);

  /**
   * @brief Send the Web Socket Message
   *
   * @param doc
   */
  // void sendWebSocketMessage(DynamicJsonDocument doc);

  /**
   * @brief Construct a JSON Object containing all the input and output data
   *
   * @return DynamicJsonDocument
   */
  DynamicJsonDocument readAllData();

  /**
   * @brief Set the Input Device States according to the client message
   *
   * @param outputDevices
   */
  void setOutputDeviceStates(DynamicJsonDocument outputDevices);

  /**
   * @brief Update the sensor data and send it to the client if any change is noticed
   */
  void updateSensorData();

  /**
   * @brief Clean the websocket clients to free  up memory
   *
   */
  void cleanClients();
};

#endif // _JRC_WEBSERVER