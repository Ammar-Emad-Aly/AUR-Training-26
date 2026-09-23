#include "DS1621.h"

DS1621::DS1621(uint8_t addr)
{
    address = addr;
}

void DS1621::begin()
{
    Wire.begin();
}

uint8_t DS1621::writeRegister(uint8_t reg, uint8_t data)
{
    Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.write(data);

    return Wire.endTransmission();
}

uint8_t DS1621::writeCommand(uint8_t command)
{
    Wire.beginTransmission(address);
    Wire.write(command);

    return Wire.endTransmission();
}

uint8_t DS1621::readRegister(uint8_t reg, uint8_t *data, uint8_t length)
{
    Wire.beginTransmission(address);
    Wire.write(reg);

    uint8_t error = Wire.endTransmission(false);

    if (error != 0)
        return error;

    uint8_t received = Wire.requestFrom(address, length);

    if (received != length)
        return 5;

    for (uint8_t i = 0; i < length; i++)
        data[i] = Wire.read();

    return 0;
}

uint8_t DS1621::configure(uint8_t config)
{
    return writeRegister(0xAC, config);
}

uint8_t DS1621::startConversion()
{
    return writeCommand(0xEE);
}

bool DS1621::readTemperature(float &temperature)
{
    uint8_t data[2];

    uint8_t error = readRegister(0xAA, data, 2);

    if (error != 0)
        return false;

    int8_t temp = (int8_t)data[0];

    temperature = temp;

    if (data[1] & 0x80)
        temperature += 0.5;

    return true;
}

