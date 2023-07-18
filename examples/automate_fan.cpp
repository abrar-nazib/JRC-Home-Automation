/**
 * @file automate_fan.cpp
 * @author Nazib Abrar
 * @brief Lesson to teach students how to automate the fan using JRC_SmartHome Library in JRC Smart Home kit
 * @version 1.0
 *
 *
 */

#include <Arduino.h>
#include "JRC_SmartHome.h"

JRC_SmartHome jrc_home;

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.println("JRC Board Smart Home");
}

void automate_fan()
{

    /**
     * @brief A function to demonstrate how to automate the fan using the DHT sensor of JRC Smart Home kit
     *
     */
    Serial.println();
    Serial.println("Automate Fan");
    Serial.println();

    float temperature = jrc_home.dht_sensor.getTemperature();                   // Get the temperature
    float temperatureThreshold = jrc_home.dht_sensor.getTemperatureThreshold(); // Get the temperature threshold

    // Automate the fan if automation state of fan is on
    if (jrc_home.fan.getAutomationState() == ON)
    {
        if (temperature > temperatureThreshold)
        {
            jrc_home.fan.setState(ON);
        }
        else
        {
            jrc_home.fan.setState(OFF);
        }
    }
}

void loop()
{
    // Set the temperature threshold
    jrc_home.dht_sensor.setTemperatureThreshold(30);
    automate_fan();
}
