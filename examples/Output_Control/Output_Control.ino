/**
 * @file output_control.cpp
 * @author Nazib Abrar
 * @brief Lesson to teach students how to control light and door using JRC_SmartHome Library in JRC Smart Home kit
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

void light_control()
{

    /**
     * @brief A simple example of how to control light using the JRC_SmartHome library in JRC Smart Home kit
     *
     */
    Serial.println();
    Serial.println("Light control");
    Serial.println();

    bool light_state = jrc_home.light.getState();          // get the state of the light in boolean: true/false
    Serial.println("Light state: " + String(light_state)); // Print the state of the light on the serial monitor

    // Turn on the light
    Serial.println("Turning on the light");
    delay(1000);
    jrc_home.light.setState(ON);

    // Check the state of the light
    light_state = jrc_home.light.getState();
    Serial.println("Light state: " + String(light_state));

    // Turn off the light
    Serial.println("Turning off the light");
    delay(1000);
    jrc_home.light.setState(OFF);

    // Check the state of the light
    light_state = jrc_home.light.getState();
    Serial.println("Light state: " + String(light_state));

    delay(3000);
}

void door_control()
{
    /**
     * @brief A simple example of how to control door using the JRC_SmartHome library in JRC Smart Home kit
     *
     */

    Serial.println();
    Serial.println("Door control");
    Serial.println();

    bool door_state = jrc_home.door.getState();          // get the state of the door in boolean: true/false
    Serial.println("Door state: " + String(door_state)); // Print the state of the door on the serial monitor

    // Open the door
    Serial.println("Opening the door");
    delay(1000);
    jrc_home.door.setState(ON);

    // Check the state of the door
    door_state = jrc_home.door.getState();
    Serial.println("Door state: " + String(door_state));

    // Close the door
    Serial.println("Closing the door");
    delay(1000);

    jrc_home.door.setState(OFF);

    // Check the state of the door
    door_state = jrc_home.door.getState();
    Serial.println("Door state: " + String(door_state));

    delay(3000);
}

void loop()
{
    light_control();
    door_control();
}
