// Date: 18/MAR/2023
// Path: Fryer_App/Multiplexer.cpp
// Link: https://github.com/mhdeeb/Fryer_App

#include "Multiplexer.h"

#include <Arduino.h>

Multiplexer::Multiplexer(const u8 *pins, u8 pinCount)
{
    this->pinCount = pinCount;
    this->pins = new u8[pinCount]{};
    for (u8 i = 0; i < pinCount; i++)
    {
        pinMode(pins[i], OUTPUT);
        this->pins[i] = pins[i];
    }
    lastUpdateTime = millis();
}

void Multiplexer::Set(bool *values)
{
    this->values = values;
}

void Multiplexer::Set(u32 index, bool value)
{
    if (index < (1 << pinCount))
        values[index] = value;
}

void Multiplexer::Update()
{
    if ((millis() - lastUpdateTime) < 1)
        return;

    if (currIndex == 7)
        for (u8 j = 0; j < pinCount; j++)
            digitalWrite(pins[j], LOW);
    else if (values[currIndex])
        for (u8 j = 0; j < pinCount; j++)
            digitalWrite(pins[j], (1 - ((currIndex + 1) >> j) & 1));

    currIndex = (currIndex + 1) % (1 << pinCount);

    if (values[currIndex])
        lastUpdateTime = millis();
}