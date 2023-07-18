#ifndef _SMARTHOME
#define _SMARTHOME

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>

#define ON 1
#define OFF 0
#define INVERTED 1

#define LED_PIN 15
#define FAN_PIN 16
#define DOOR_PIN 17
#define DOOR_OPEN_ANGLE 0
#define DOOR_CLOSED_ANGLE 90
#define WINDOW_PIN 4
#define WINDOW_OPEN_ANGLE 0
#define WINDOW_CLOSED_ANGLE 90

#define PIR_SENSOR_PIN 13
#define RAIN_SENSOR_PIN 5
#define LDR_SENSOR_PIN 23

#define DHT_SENSOR_PIN 2
#define DHTTYPE DHT22 // DHT 22 (AM2302)

/**
 * @brief The OutputDevice class represents an output device that can be controlled.
 */
class OutputDevice
{
protected:
    int pin;              /**< The pin number connected to the output device. */
    bool state;           /**< The current state of the output device. */
    bool automationState; /**< The automation state of the output device. */
public:
    /**
     * @brief Constructs an OutputDevice object with the specified pin.
     *
     * @param pin The pin number to which the output device is connected.
     */
    OutputDevice(int pin);

    /**
     * @brief Gets the current state of the output device.
     *
     * @return The current state (true for ON, false for OFF) of the output device.
     */
    bool getState();

    /**
     * @brief Gets the current state of the output device as a string.
     *
     * @return The current state ("ON" or "OFF") of the output device as a string.
     */
    String getStateStr();

    /**
     * @brief Sets the state of the output device.
     *
     * @param state The state (true for ON, false for OFF) to set the output device to.
     */
    virtual void setState(bool state);

    /**
     * @brief Sets the state of the output device using a string.
     *
     * @param state The state ("ON" or "OFF") to set the output device to as a string.
     */
    virtual void setState(String state);

    /**
     * @brief Gets the automation state of the output device.
     *
     * @return The automation state (true for automated, false for manual) of the output device.
     */
    bool getAutomationState();

    /**
     * @brief Gets the automation state of the output device as a string.
     *
     * @return The automation state ("ON" or "OFF") of the output device as a string.
     */
    String getAutomationStateStr();

    /**
     * @brief Sets the automation state of the output device.
     *
     * @param state The automation state (true for automated, false for manual) to set the output device to.
     */
    void setAutomationState(bool state);

    /**
     * @brief Set the Automation State object using a string.
     *
     * @param state The automation state  "ON" or "OFF" to set the automation state with string.
     */
    void setAutomationState(String state);
};

/**
 * @brief The LED class represents an LED output device that can be controlled.
 */
class LED : public OutputDevice
{
public:
    /**
     * @brief Constructs an LED object with the specified pin.
     *
     * @param pin The pin number to which the LED is connected.
     */
    LED(int pin) : OutputDevice(pin) {}
};

/**
 * @brief The Fan class represents a fan output device that can be controlled.
 */
class Fan : public OutputDevice
{
private:
    int speed; /**< The speed of the fan. */

public:
    /**
     * @brief Constructs a Fan object with the specified pin.
     *
     * @param pin The pin number to which the fan is connected.
     */
    Fan(int pin) : OutputDevice(pin) {}

    /**
     * @brief Gets the speed of the fan.
     *
     * @return The speed of the fan.
     */
    int getSpeed();

    /**
     * @brief Sets the speed of the fan.
     *
     * @param speed The speed of the fan.
     */
    void setSpeed(int speed);
};

/**
 * @brief The ServoMotor class represents a servo motor output device that can be controlled.
 *        It is derived from the OutputDevice class.
 */
class ServoMotor : public OutputDevice
{
private:
    int currentAngle; /**< The current angle of the servo motor. */
    int openAngle;    /**< The angle at which the servo motor is open. */
    int closedAngle;  /**< The angle at which the servo motor is closed. */
    Servo servo;      /**< The Servo object used to control the servo motor. */

    /**
     * @brief Get the current angle of the servo motor.
     *
     * @return The current angle of the servo motor.
     */
    int getCurrentAngle();

    /**
     * @brief Set the current angle of the servo motor.
     *
     * @param angle The angle to set as the current angle of the servo motor.
     */
    void setCurrentAngle(int angle);

    /**
     * @brief Open the servo motor.
     */
    void open();

    /**
     * @brief Close the servo motor.
     */
    void close();

public:
    /**
     * @brief Constructs a ServoMotor object with the specified pin, open angle, and closed angle.
     *
     * @param pin The pin number to which the servo motor is connected.
     * @param openAngle The angle at which the servo motor is open.
     * @param closedAngle The angle at which the servo motor is closed.
     */
    ServoMotor(int pin, int openAngle, int closedAngle);

    /**
     * @brief Sets the state of the servo motor based on a boolean value.
     *
     * @param state The state (true for ON, false for OFF) to set the servo motor to.
     */
    void setState(bool state) override;

    /**
     * @brief Sets the state of the servo motor based on a string value.
     *
     * @param state The state ("ON" or "OFF") to set the servo motor to as a string.
     */
    void setState(String state) override;
};

/**
 * @brief The LCD_Display class represents an LCD display output device that can be controlled.
 */
class LCD_Display
{
private:
    LiquidCrystal_I2C lcd; /**< The LiquidCrystal_I2C object used to control the LCD display. */
    String line1 = "";
    String line2 = "";

public:
    /**
     * @brief Constructs an LCD_Display object.
     */
    LCD_Display();

    /**
     * @brief Write a string to the LCD display. Centers the string on the specified line
     *
     * @param line The line number to write to.
     * @param text The string to write.
     *
     * @return bool 1 if successful, 0 if not
     */
    bool printLine(int line, String text);

    /**
     * @brief Write a string to the LCD display. Centers the string on the specified line
     *
     * @param line The line number to write to.
     * @param text The string to write.
     *
     * @return bool 1 if successful, 0 if not
     */
    bool printLineCentered(int line, String text);
};

/**
 * @brief The InputDevice class represents an input device (Sensor) that can be read.
 */
class InputDevice
{
protected:
    int pin;          /**< The pin number connected to the input device */
    bool state = 0;   /**< The current state of the input device */
    bool is_inverted; /**< Whether the input device is inverted or not */
    bool previous_state = 0;

public:
    /**
     * @brief Construct a new InputDevice object
     *
     * @param pin
     */
    InputDevice(int pin);

    /**
     * @brief Construct a new InputDevice object
     *
     * @param pin
     * @param is_inverted
     */
    InputDevice(int pin, bool is_inverted);

    /**
     * @brief Get the state of the input device
     *
     * @return bool
     */
    bool getState();

    /**
     * @brief Get the state of the input device as a string
     *
     * @return String
     */
    String getStateStr(); // Returns HIGH or LOW

    bool getDifference();
};

/**
 * @brief DHT Sensor
 *
 */
class DHTSensor
{
private:
    int pin;                 /**< The pin number connected to the DHT sensor */
    float threshold = 30.00; /**< The temperature threshold */
    DHT_Unified *dht;        /**< The DHT_Unified object used to read the temperature and humidity */

    float temperature = 0;
    float previousTemperature = 0; /**< The previous temperature reading */

    float humidity = 0;
    float previousHumidity = 0; /**< The previous humidity reading */

public:
    /**
     * @brief Construct a new DHTSensor object
     *
     * @param pin
     */
    DHTSensor(int pin);

    /**
     * @brief Get the Temperature in Celsius
     *
     * @return float
     */
    float getTemperature();

    /**
     * @brief Get the Temperature as a string
     *
     * @return String
     */
    String getTemperatureStr();

    /**
     * @brief Set the Temperature Threshold
     *
     * @param threshold
     */
    void setTemperatureThreshold(float threshold);

    /**
     * @brief Get the Temperature Threshold
     *
     * @return float
     */
    float getTemperatureThreshold();

    /**
     * @brief Get the Humidity percentange
     *
     * @return float
     */
    float getHumidity();

    /**
     * @brief Get the Humidity percentange as a string
     *
     * @return String
     */
    String getHumidityStr();

    /**
     * @brief Get the Temperature Difference
     *
     * @return float
     */
    bool getTemperatureDifference();

    /**
     * @brief Get the Humidity Difference
     *
     * @return float
     */
    bool getHumidityDifference();
};

/**
 * @brief The JRC_SmartHome class represents a smart home that can be controlled.
 */

class JRC_SmartHome
{
private:
public:
    LED light = LED(LED_PIN);
    Fan fan = Fan(FAN_PIN);
    ServoMotor door = ServoMotor(DOOR_PIN, DOOR_OPEN_ANGLE, DOOR_CLOSED_ANGLE);
    ServoMotor window = ServoMotor(WINDOW_PIN, WINDOW_OPEN_ANGLE, WINDOW_CLOSED_ANGLE);
    InputDevice pir_sensor = InputDevice(PIR_SENSOR_PIN);
    InputDevice ldr_sensor = InputDevice(LDR_SENSOR_PIN, INVERTED);
    InputDevice rain_sensor = InputDevice(RAIN_SENSOR_PIN);
    DHTSensor dht_sensor = DHTSensor(DHT_SENSOR_PIN);
};

#endif // _SMARTHOME