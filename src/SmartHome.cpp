#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ESP32Servo.h>
#include "SmartHome.h"

#define DHTTYPE DHT22 // DHT 22 (AM2302)

// LED class constructor
LED::LED(int pin)
{
    this->pin = pin;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    this->state = false;
    this->automatedState = true;
}

// Get the state of the LED
String LED::getState()
{
    return this->state ? "on" : "off";
}

// Set the state of the LED
void LED::setState(String state)
{
    if (state == "on")
    {
        digitalWrite(this->pin, HIGH);
        this->state = true;
    }
    else
    {
        digitalWrite(this->pin, LOW);
        this->state = false;
    }
}

// Get the automated state of the LED
String LED::getAutomatedState()
{
    return this->automatedState ? "on" : "off";
}

// Set the automated state of the LED
void LED::setAutomatedState(String state)
{
    if (state == "on")
    {
        this->automatedState = true;
    }
    else
    {
        this->automatedState = false;
    }
}

// Constructor of fan is exactly the same as LED
Fan::Fan(int pin) : LED(pin)
{
    this->speed = 0;
}

// Button class constructor
Button::Button(int pin)
{
    this->pin = pin;
    pinMode(pin, INPUT_PULLDOWN);

    this->state = false;
}

// Get the state of the button
String Button::getState()
{
    state = digitalRead(this->pin);
    return this->state ? "on" : "off";
}

ButtonInverted::ButtonInverted(int pin) : Button(pin)
{
}

String ButtonInverted::getState()
{
    state = digitalRead(this->pin);
    return this->state ? "off" : "on";
}

// DHT22 class constructor
DHTSensor::DHTSensor(int pin)
{
    this->pin = pin;
    this->dht = new DHT_Unified(this->pin, DHTTYPE);
    this->dht->begin();
}

// Get the temperature
String DHTSensor::getTemperature()
{
    sensors_event_t event;
    this->dht->temperature().getEvent(&event);
    // Return up to 2 decimal places
    String temp = String((int)(event.temperature * 100) / 100.0);
    return temp;
}

// Get the humidity
String DHTSensor::getHumidity()
{
    sensors_event_t event;
    this->dht->humidity().getEvent(&event);
    // Return up to 2 decimal places
    String hum = String((int)(event.relative_humidity * 100) / 100.0);
    return hum;
}

ServoMotor::ServoMotor(int pin, int openAngle, int closedAngle)
{
    this->pin = pin;
    this->openAngle = openAngle;
    this->closedAngle = closedAngle;
    this->servo = Servo();
    this->servo.attach(this->pin);
    this->servo.write(this->closedAngle);
    this->currentAngle = this->closedAngle;
    this->automatedState = true;
}

int ServoMotor::getCurrentAngle()
{
    return this->currentAngle;
}

void ServoMotor::setCurrentAngle(int angle)
{
    this->servo.write(angle);
    this->currentAngle = angle;
}

void ServoMotor::open()
{
    // Use setCurrentAngle() to set the angle to openAngle
    this->setCurrentAngle(this->openAngle);
}

void ServoMotor::close()
{
    // Use setCurrentAngle() to set the angle to closedAngle
    this->setCurrentAngle(this->closedAngle);
}

String ServoMotor::getState()
{
    return this->state ? "open" : "closed";
}

void ServoMotor::setState(String state)
{
    if (state == "open")
    {
        this->state = true;
        this->open();
    }
    else
    {
        this->state = false;
        this->close();
    }
}

String ServoMotor::getAutomatedState()
{
    return this->automatedState ? "on" : "off";
}

void ServoMotor::setAutomatedState(String state)
{
    if (state == "on")
    {
        this->automatedState = true;
    }
    else
    {
        this->automatedState = false;
    }
}

extern LED led;
extern Fan fan;

extern ButtonInverted rain_sensor;

extern Button pir_sensor;
extern Button ldr_sensor;

extern DHTSensor dht;

extern ServoMotor door;
extern ServoMotor window;

// Automate function
void automate()
{
    // Handle the automated state of the LED Depending on ldr_sensor
    if (led.getAutomatedState() == "on")
    {
        if (ldr_sensor.getState() == "on")
        {
            led.setState("on");
        }
        else
        {
            led.setState("off");
        }
    }
    // Handle the automated state of window depending on rain_sensor
    if (window.getAutomatedState() == "on")
    {
        if (rain_sensor.getState() == "on")
        {
            window.setState("closed");
        }
        else
        {
            window.setState("open");
        }
    }
    // Handle the automated state of Door depending on pir_sensor
    if (door.getAutomatedState() == "on")
    {
        if (pir_sensor.getState() == "on")
        {
            door.setState("open");
        }
        else
        {
            door.setState("closed");
        }
    }
}