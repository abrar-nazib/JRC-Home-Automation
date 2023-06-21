#ifndef _SMARTHOME
#define _SMARTHOME

#include <Arduino.h>
#include <WiFi.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ESP32Servo.h>

class LED
{
private:
    int pin;
    bool state;
    bool automatedState;

public:
    LED(int pin);
    String getState();
    void setState(String state);

    String getAutomatedState();
    void setAutomatedState(String state);
};

// Class fan: child class of LED
class Fan : public LED
{
private:
    int speed;

public:
    Fan(int pin);
};

class Button
{
private:
    int pin;
    bool state;

public:
    Button(int pin);
    String getState();
};

// DHT22 sensor
class DHTSensor
{
private:
    int pin;
    DHT_Unified *dht;

public:
    DHTSensor(int pin);
    String getTemperature();
    String getHumidity();
};

// Class for door and window which are controlled by servo
class ServoMotor
{
private:
    int pin;
    int currentAngle;
    int openAngle;
    int closedAngle;
    bool state;
    bool automatedState;
    Servo servo;

public:
    ServoMotor(int pin, int openAngle, int closedAngle);
    int getCurrentAngle();
    void setCurrentAngle(int angle);
    void open();
    void close();
    String getState();
    void setState(String state);
    String getAutomatedState();
    void setAutomatedState(String state);
};

#endif // _SMARTHOME