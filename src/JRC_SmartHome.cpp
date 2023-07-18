#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>
#include "JRC_SmartHome.h"

// -----------------------------------------------
//          OutputDevice class implementation
// ------------------------------------------------

// Output Device class constructor
OutputDevice::OutputDevice(int pin)
{
    this->pin = pin;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    this->state = false;
    this->automationState = true;
}

// Get the state of the output device
bool OutputDevice::getState()
{
    return this->state;
}

// Get the state of the output device as a string
String OutputDevice::getStateStr()
{
    return this->state ? "ON" : "OFF";
}

// Set the state of the output device
void OutputDevice::setState(bool state)
{
    if (state == true)
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

// Set the state of the output device using a string
void OutputDevice::setState(String state)
{
    if (state == "ON")
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

// Get the automation state of the output device
bool OutputDevice::getAutomationState()
{
    return this->automationState;
}

// Get the automation state of the output device as a string
String OutputDevice::getAutomationStateStr()
{
    return this->automationState ? "ON" : "OFF";
}

// Set the automation state of the output device
void OutputDevice::setAutomationState(bool state)
{
    this->automationState = state;
}

// Set the automation state of the output device using a string
void OutputDevice::setAutomationState(String state)
{
    if (state == "ON")
    {
        this->automationState = true;
    }
    else if (state == "OFF")
    {
        this->automationState = false;
    }
}

int Fan::getSpeed()
{
    return this->speed;
}

void Fan::setSpeed(int speed)
{
    this->speed = speed;
}

ServoMotor::ServoMotor(int pin, int openAngle, int closedAngle) : OutputDevice(pin), openAngle(openAngle), closedAngle(closedAngle)
{
    this->openAngle = openAngle;
    this->closedAngle = closedAngle;
    this->servo = Servo();
    this->servo.attach(this->pin);
    this->servo.write(this->closedAngle);
    this->currentAngle = this->closedAngle;
}

int ServoMotor::getCurrentAngle()
{
    return this->currentAngle;
}

void ServoMotor::setCurrentAngle(int angle)
{
    // clamp the angle between 0 to 180
    if (angle < 0)
    {
        angle = 0;
    }
    else if (angle > 180)
    {
        angle = 180;
    }

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

void ServoMotor::setState(bool state)
{
    if (state == true)
    {
        this->open();
        this->state = true;
    }
    else
    {
        this->close();
        this->state = false;
    }
}

void ServoMotor::setState(String state)
{
    if (state == "ON")
    {
        this->open();
        this->state = true;
    }
    else
    {
        this->close();
        this->state = false;
    }
}

LCD_Display::LCD_Display() : lcd(0x27, 16, 2)
{
    lcd.init();
    lcd.backlight();
    lcd.clear();
}

bool LCD_Display::printLine(int line, String text)
{
    // Check whether the line number is greater than 2. Throw error if it is.
    if (line > 2 or line < 1)
    {
        Serial.println("Error: Line number cannot be greater than 2");
        throw "Error: Line number cannot be greater than 2";
        return false;
    }

    // Check whether the text is greater than 16 characters. Throw error if it is.
    if (text.length() > 16)
    {
        Serial.println("Error: Text cannot be greater than 16 characters");
        throw "Error: Text cannot be greater than 16 characters";
        return false;
    }

    // Print the text on the LCD
    if (line == 1)
    {
        line1 = text;
    }
    else
    {
        line2 = text;
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
    return true;
}

bool LCD_Display::printLineCentered(int line, String text)
{
    // Check whether the line number is greater than 2. Throw error if it is.
    if (line > 2 or line < 1)
    {
        Serial.println("Error: Line number cannot be greater than 2");
        throw "Error: Line number cannot be greater than 2";
        return false;
    }

    // Check whether the text is greater than 16 characters. Throw error if it is.
    if (text.length() > 16)
    {
        Serial.println("Error: Text cannot be greater than 16 characters");
        throw "Error: Text cannot be greater than 16 characters";
        return false;
    }

    // Print the text on the LCD
    if (line == 1)
    {
        // Put spaces before the text to center it according to the text length
        int spaces = (16 - text.length()) / 2;
        for (int i = 0; i < spaces; i++)
        {
            line1 += " ";
        }
        line1 += text;
    }
    else
    {
        int spaces = (16 - text.length()) / 2;
        for (int i = 0; i < spaces; i++)
        {
            line2 += " ";
        }
        line2 += text;
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
    return true;
}

// -----------------------------------------------
//          InputDevice class implementation
// ------------------------------------------------

//
InputDevice::InputDevice(int pin) : pin(pin)
{
    pinMode(pin, INPUT_PULLDOWN);
    is_inverted = false;
}

InputDevice::InputDevice(int pin, bool is_inverted) : pin(pin), is_inverted(is_inverted)
{
    pinMode(pin, INPUT_PULLDOWN);
}

bool InputDevice::getState()
{
    state = digitalRead(pin);
    if (is_inverted)
    {
        return !state;
    }
    return state;
}

String InputDevice::getStateStr()
{
    if (getState())
    {
        return "HIGH";
    }
    else
    {
        return "LOW";
    }
}

bool InputDevice::getDifference()
{
    getState();
    int difference = abs(state - previous_state);
    if (difference >= 1)
    {
        previous_state = state;
    }
    return difference >= 1 ? 1 : 0;
}

// Class for DHT Sensor
DHTSensor::DHTSensor(int pin)
{
    this->pin = pin;
    this->dht = new DHT_Unified(this->pin, DHTTYPE);
    this->dht->begin();
}

float DHTSensor::getTemperature()
{
    sensors_event_t event;
    this->dht->temperature().getEvent(&event);
    temperature = event.temperature;
    return temperature;
}

String DHTSensor::getTemperatureStr()
{
    sensors_event_t event;
    this->dht->temperature().getEvent(&event);
    temperature = event.temperature;
    // Return up to 2 decimal places
    String temp_str = String((int)(temperature * 100) / 100.0);
    return temp_str;
}

float DHTSensor::getHumidity()
{
    sensors_event_t event;
    this->dht->humidity().getEvent(&event);
    humidity = event.relative_humidity;
    return humidity;
}

String DHTSensor::getHumidityStr()
{
    sensors_event_t event;
    this->dht->humidity().getEvent(&event);
    humidity = event.relative_humidity;
    // Return up to 2 decimal places
    String hum_str = String((int)(humidity * 100) / 100.0);
    return hum_str;
}

// // set the threshold
void DHTSensor::setTemperatureThreshold(float threshold)
{
    this->threshold = threshold;
}

float DHTSensor::getTemperatureThreshold()
{
    return this->threshold;
}

bool DHTSensor::getTemperatureDifference()
{
    getTemperature(); // To update the temperature variable
    float difference = abs(temperature - previousTemperature);
    if (difference >= 1)
    {
        previousTemperature = temperature;
    }
    return difference >= 1 ? 1 : 0;
}

bool DHTSensor::getHumidityDifference()
{
    getHumidity(); // to update the humidity variable
    float difference = abs(humidity - previousHumidity);
    if (difference >= 1)
    {
        previousHumidity = humidity;
    }
    return difference >= 1 ? 1 : 0;
}