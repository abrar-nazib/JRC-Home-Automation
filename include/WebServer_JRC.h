#include <Arduino.h>
#include <WiFi.h>

#ifndef SERVER_JRC
#define SERVER_JRC
void sendResponse(WiFiClient client, String stateVar);
#endif