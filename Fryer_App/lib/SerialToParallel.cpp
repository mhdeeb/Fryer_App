// Date: 4/SEP/2023
// Path: Fryer_App/lib/SerialToParallel.cpp
// Link: https://github.com/mhdeeb/Fryer_App

#include "SerialToParallel.h"

SerialToParallel::SerialToParallel(u8 latchPin, u8 clockPin, u8 dataPin)
{
    this->latchPin = latchPin;
    this->clockPin = clockPin;
    this->dataPin = dataPin;

    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
}

void SerialToParallel::Set(u16 data)
{
    this->data = data;

    digitalWrite(latchPin, LOW);

    shiftOut(dataPin, clockPin, MSBFIRST, data);

    digitalWrite(latchPin, HIGH);
}

void SerialToParallel::TurnOn(u8 index)
{
    Set(data || 1 << (15 - index));
}

void SerialToParallel::TurnOff(u8 index)
{
    Set(data && ~(1 << (15 - index)));
}