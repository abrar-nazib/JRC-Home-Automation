#include <Arduino.h>
#include <WiFi.h>
#include "WebServer_JRC.h"

#define LED_PIN 13

// WiFi credentials
const char *ssid = "Archer";
const char *password = "mairala1234";

// WiFi server parameters
WiFiServer server(80); // web server in port 80

String ledState = "off";

void setup()
{
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

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
    readHeader(client);
    sendResponse(client, ledState);
    delay(1000);
    client.stop();
    Serial.println("Client disconnected");
    Serial.println();
    // }
  }
}

void sendResponse(WiFiClient client, String stateVar)
{

  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();
  client.println("<!DOCTYPE html><html>");
  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"icon\" href=\"data:,\">");

  // CSS to style the on/off buttons
  // Feel free to change the background-color and font-size attributes to fit your preferences
  client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
  client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
  client.println(".button2 {background-color: #77878A;}</style></head>");

  // Web Page Heading
  client.println("<body><h1>ESP32 Web Server</h1>");

  // Display current state, and ON/OFF buttons for GPIO 26
  client.println("<p>LED - State " + stateVar + "</p>");
  // If the output26State is off, it displays the ON button
  if (stateVar == "off")
  {
    client.println("<p><a href=\"/led/on\"><button class=\"button\">ON</button></a></p>");
  }
  else
  {
    client.println("<p><a href=\"/led/off\"><button class=\"button button2\">OFF</button></a></p>");
  }
  client.println("</body></html>");
  client.println();
}

String readHeader(WiFiClient client)
{
  String header = "";
  String currentLine = "";
  while (client.connected())
  {
    char c;
    if (client.available())
    {
      c = client.read();
      header += c;

      if (c == '\n') // check whether the request has finished. Handle \n character
      {
        if (currentLine.length() == 0)
        {
          // Serial.println("Header:\n" + header);
          return header;
        }
        else
        {
          currentLine = "";
        }
      }
      else if (c != '\r') // Handle \r character
      {
        currentLine += c;
      }
    }
  }
}