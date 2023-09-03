// Date: 4/SEP/2023
// Path: Fryer_App/lib/SerialToParallel.h
// Link: https://github.com/mhdeeb/Fryer_App

#ifndef SERIAL_TO_PARALLEL_H
#define SERIAL_TO_PARALLEL_H

class SerialToParallel
{
public:
    SerialToParallel(u8 latchPin, u8 clockPin, u8 dataPin);

    void Set(u16 data);

    void TurnOn(u8 index);

    void TurnOff(u8 index);

private:
    u8 latchPin;
    u8 clockPin;
    u8 dataPin;
    u16 data = 0;
};

#endif // SERIAL_TO_PARALLEL_H