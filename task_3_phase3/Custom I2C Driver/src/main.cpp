#include <Arduino.h>
#include "DS1621.h"

DS1621 sensor(0x48);

void setup()
{
    Serial.begin(9600);

    sensor.begin();

    uint8_t error;

    error = sensor.configure(0x00);

    if (error != 0)
    {
        Serial.print("Config Error: ");
        Serial.println(error);
        return;
    }

    error = sensor.startConversion();

    if (error != 0)
    {
        Serial.print("Start Error: ");
        Serial.println(error);
        return;
    }
}

void loop()
{
    float temperature;

    if (sensor.readTemperature(temperature))
    {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.println(" C");
    }
    else
    {
        Serial.println("Temperature Read Error");
    }

    delay(1000);
}

