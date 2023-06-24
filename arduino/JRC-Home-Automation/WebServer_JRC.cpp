#include <Arduino.h>
#include <WiFi.h>
#include "WebServer_JRC.h"
#include <ArduinoJson.h>
#include "SmartHome.h"
#include "html.h"

String connectToWiFi()
{

  // const char *ssid = "JRC Board";
  // const char *password = "robotics";
  const char *ssid = "NAF Tech_WiFi";
  const char *password = "N@f Tech";
  // const char *password = "null_byte0x00";
  // Connect to WiFi
  WiFi.begin(ssid, password);
  // Serial.println("Connected to the WiFi network");
  // Serial.print("IP address: ");
  // Serial.println(WiFi.localIP());
  return WiFi.localIP().toString();
}

void sendResponse(WiFiClient client)
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type: text/html");
  client.println("Connection: close");
  client.println();
  client.println(html);
  client.println();
}

void sendJSONResponse(WiFiClient client, String stateVar)
{
  StaticJsonDocument<200> jsonDocument;

  jsonDocument["state"] = stateVar;

  String json;
  serializeJson(jsonDocument, json);

  client.println("HTTP/1.1 200 OK");
  client.println("Content-type: application/json");
  client.println("Connection: close");
  client.println();
  client.println(json);
}

Header readHeader(WiFiClient client)
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
          return Header(header);
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
  return Header(header);
}

Header::Header(String header)
{
  text = header;

  // Get the first line of the header
  int firstLineIndex = header.indexOf('\n');
  String firstLine = header.substring(0, firstLineIndex);

  // Get the request method
  int methodIndex = firstLine.indexOf(' ');
  method = firstLine.substring(0, methodIndex);

  // Get the request URI
  int uriIndex = firstLine.indexOf(' ', methodIndex + 1);
  uri = firstLine.substring(methodIndex + 1, uriIndex);

  // Get the second line of the header
  int secondLineIndex = header.indexOf('\n', firstLineIndex + 1);
  String secondLine = header.substring(firstLineIndex + 1, secondLineIndex);

  // Get the host from the second line
  int hostIndex = secondLine.indexOf(' ');
  host = secondLine.substring(hostIndex + 1);
}

// Route Handler function
extern LED led;
extern Fan fan;

extern ButtonInverted rain_sensor;

extern Button pir_sensor;
extern Button ldr_sensor;

extern DHTSensor dht;

extern ServoMotor door;
extern ServoMotor window;

void handleRequest(WiFiClient client, String uri)
{
  if (uri == "/")
  {
    sendResponse(client);
  }
  else if (uri == "/api/light/toggle")
  {
    // get the state of the LED1
    String state = led.getState();
    // toggle the state
    if (state == "off")
    {
      led.setState("on");
    }
    else
    {
      led.setState("off");
    }
    // send the response
    sendJSONResponse(client, led.getState());
  }
  else if (uri == "/api/light/get-state")
  {
    sendJSONResponse(client, led.getState());
  }
  // api/light/get-automate-state
  else if (uri == "/api/light/get-automate-state")
  {
    sendJSONResponse(client, led.getAutomatedState());
  }
  // api/light/get-automate-state
  else if (uri == "/api/light/toggle-automate")
  {
    // get the state of the LED1
    String state = led.getAutomatedState();
    // toggle the state
    if (state == "off")
    {
      led.setAutomatedState("on");
    }
    else
    {
      led.setAutomatedState("off");
    }
    // send the response
    sendJSONResponse(client, led.getAutomatedState());
  }

  // API Endpoint for fan
  else if (uri == "/api/fan/toggle")
  {
    // get the state of the fan
    String state = fan.getState();
    // toggle the state
    if (state == "off")
    {
      fan.setState("on");
    }
    else
    {
      fan.setState("off");
    }
    // send the response
    sendJSONResponse(client, fan.getState());
  }
  else if (uri == "/api/fan/get-state")
  {
    sendJSONResponse(client, fan.getState());
  }

  // api/fan/get-automate-state
  else if (uri == "/api/fan/get-automate-state")
  {
    sendJSONResponse(client, fan.getAutomatedState());
  }

  // api/fan/toggle-automate
  else if (uri == "/api/fan/toggle-automate")
  {
    // get the state of the fan
    String state = fan.getAutomatedState();
    // toggle the state
    if (state == "off")
    {
      fan.setAutomatedState("on");
    }
    else
    {
      fan.setAutomatedState("off");
    }
    // send the response
    sendJSONResponse(client, fan.getAutomatedState());
  }

  // api/door/toggle
  else if (uri == "/api/door/toggle")
  {
    // get the state of the door
    String state = door.getState();
    // toggle the state
    if (state == "closed")
    {
      door.setState("open");
    }
    else
    {
      door.setState("closed");
    }
    // send the response
    sendJSONResponse(client, door.getState());
  }

  // api/door/get-state
  else if (uri == "/api/door/get-state")
  {
    sendJSONResponse(client, door.getState());
  }

  else if (uri == "/api/door/get-state")
  {
    sendJSONResponse(client, door.getState());
  }

  // Get automate state
  else if (uri == "/api/door/get-automate-state")
  {
    sendJSONResponse(client, door.getAutomatedState());
  }
  // Toggle automate
  else if (uri == "/api/door/toggle-automate")
  {
    // get the state of the door
    String state = door.getAutomatedState();
    // toggle the state
    if (state == "off")
    {
      door.setAutomatedState("on");
    }
    else
    {
      door.setAutomatedState("off");
    }
    // send the response
    sendJSONResponse(client, door.getAutomatedState());
  }

  // Handle window APIs
  // api/window/toggle
  else if (uri == "/api/window/toggle")
  {
    // get the state of the window
    String state = window.getState();
    // toggle the state
    if (state == "closed")
    {
      window.setState("open");
    }
    else
    {
      window.setState("closed");
    }
    // send the response
    sendJSONResponse(client, window.getState());
  }

  // Get Automate State
  else if (uri == "/api/window/get-automate-state")
  {
    sendJSONResponse(client, window.getAutomatedState());
  }

  // Window toggle automate
  else if (uri == "/api/window/toggle-automate")
  {
    // get the state of the window
    String state = window.getAutomatedState();
    // toggle the state
    if (state == "off")
    {
      window.setAutomatedState("on");
    }
    else
    {
      window.setAutomatedState("off");
    }
    // send the response
    sendJSONResponse(client, window.getAutomatedState());
  }

  // api/window/get-state
  else if (uri == "/api/window/get-state")
  {
    sendJSONResponse(client, window.getState());
  }

  // handle /api/sensors
  else if (uri == "/api/sensors")
  {
    StaticJsonDocument<200> jsonDocument;

    jsonDocument["rainState"] = rain_sensor.getState();
    jsonDocument["pirState"] = pir_sensor.getState();
    jsonDocument["ldrState"] = ldr_sensor.getState();
    jsonDocument["tempState"] = dht.getTemperature();
    jsonDocument["humidState"] = dht.getHumidity();

    String json;
    serializeJson(jsonDocument, json);

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type: application/json");
    client.println("Connection: close");
    client.println();
    client.println(json);
  }

  else
  {
    client.println("HTTP/1.1 404 Not Found");
    client.println("Content-type: text/html");
    client.println("Connection: close");
    client.println();
    client.println("<html><body><h1>404 Not Found</h1></body></html>");
  }
}