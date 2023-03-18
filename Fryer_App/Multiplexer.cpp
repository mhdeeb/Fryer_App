// Date: 18/MAR/2023
// Path: Fryer_App/Multiplexer.cpp
// Link: https://github.com/mhdeeb/Fryer_App

#include "Multiplexer.h"

#include <Arduino.h>

Multiplexer::Multiplexer(const u8 *pins, u8 pinCount)
{
    this->pinCount = pinCount;
    this->pins = new u8[pinCount]{};
    this->values = new bool[1 << pinCount]{};
    for (u8 i = 0; i < pinCount; i++)
        this->pins[i] = pins[i];
    lastUpdateTime = millis();
}

Multiplexer::~Multiplexer()
{
    if (pins != nullptr)
        delete[] pins;
    if (values != nullptr)
        delete[] values;
}

void Multiplexer::Set(const bool *values)
{
    if (this->values != nullptr)
        delete[] this->values;
    this->values = new bool[1 << pinCount]{};
    for (u8 i = 0; i < (1 << pinCount); i++)
        this->values[i] = values[i];
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
    if (values[currIndex])
        for (u8 j = 0; j < pinCount; j++)
            digitalWrite(pins[j], (currIndex >> j) & 1);
    currIndex = (currIndex + 1) % (1 << pinCount);
    lastUpdateTime = millis();
}