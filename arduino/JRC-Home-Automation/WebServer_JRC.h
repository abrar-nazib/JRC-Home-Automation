#include <Arduino.h>
#include <WiFi.h>

#ifndef SERVER_JRC
#define SERVER_JRC
void sendResponse(WiFiClient client, String stateVar);
class Header
{
public:
    String text;
    String method;
    String uri;
    // String version;
    String host;
    // String connection;
    // String cacheControl;
    // String upgradeInsecureRequests;
    // String userAgent;
    // String accept;
    // String acceptEncoding;
    // String acceptLanguage;
    // String cookie;
    Header(String header);
};

Header readHeader(WiFiClient client);

String connectToWiFi();

void handleRequest(WiFiClient client, String uri);

#endif