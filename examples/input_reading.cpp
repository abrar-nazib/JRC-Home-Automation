/**
 * @file input_reading.cpp
 * @author Nazib Abrar
 * @brief Lesson to teach students how to read the temperature and humidity and LDR sensor from the DHT sensor and LDR sensor of JRC Smart Home kit
 * @version 1.0
 *
 */

#include <Arduino.h>
#include "JRC_SmartHome.h"

JRC_SmartHome jrc_home; // Create an object of JRC_SmartHome class

void setup()
{
    Serial.begin(115200); // Start the serial communication with the computer
    Serial.println("JRC Board Smart Home");
}

void read_dht_sensor()
{

    /**
     * @brief A function to demonstrate how to read the temperature and humidity from the DHT sensor of JRC Smart Home kit
     *
     */
    Serial.println();
    Serial.println("DHT sensor Reading");
    Serial.println();

    float temperature = jrc_home.dht_sensor.getTemperature();
    float humidity = jrc_home.dht_sensor.getHumidity();

    delay(1000);

    Serial.println("Temperature: " + String(temperature));
    Serial.println("Humidity: " + String(humidity));

    delay(3000);
}

void read_ldr_sensor()
{

    /**
     * @brief A function to demonstrate how to read the LDR sensor which returns HIGH or LOW in boolean
     *
     */
    Serial.println();
    Serial.println("LDR sensor Reading");
    Serial.println();

    bool ldr_state = jrc_home.ldr_sensor.getState();          // Returns 0 or 1
    String ldr_state_str = jrc_home.ldr_sensor.getStateStr(); // Returns HIGH or LOW
    Serial.println("LDR state boolean: " + String(ldr_state));
    Serial.println("LDR state String: " + ldr_state_str);

    delay(3000);
}

void loop()
{
    read_dht_sensor();
    read_ldr_sensor();
}
