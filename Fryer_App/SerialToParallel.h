// Date: 4/SEP/2023
// Path: Fryer_App/lib/SerialToParallel.h
// Link: https://github.com/mhdeeb/Fryer_App

#ifndef SERIAL_TO_PARALLEL_H
#define SERIAL_TO_PARALLEL_H

#include "def.h"
#include <Arduino.h>

class SerialToParallel
{
public:
    SerialToParallel(u8 latchPin, u8 clockPin, u8 dataPin);

    void Set(u16 data);

    void TurnOn(u8 index);

    void TurnOff(u8 index);

    bool isOn(u8 index);

    bool isBlinking(u8 index);

    void startBlinking(u8 index);

    void stopBlinking(u8 index);

    void Update();

private:
    u8 latchPin;
    u8 clockPin;
    u8 dataPin;
    u16 data = 0;
    u16 blinkData = 0;
    u32 lastBlink = 0;
    u32 blinkDuration = 400;
};

#endif // SERIAL_TO_PARALLEL_H