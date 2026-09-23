#ifndef DS1621_H
#define DS1621_H

#include <Arduino.h>
#include <Wire.h>

class DS1621
{
private:
    uint8_t address;

public:
    DS1621(uint8_t addr = 0x48);

    void begin();

    uint8_t writeRegister(uint8_t reg, uint8_t data);
    uint8_t writeCommand(uint8_t command);

    uint8_t readRegister(uint8_t reg, uint8_t *data, uint8_t length);

    uint8_t configure(uint8_t config);
    uint8_t startConversion();

    bool readTemperature(float &temperature);
};

#endif

