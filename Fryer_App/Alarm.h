// Date: 10/FEB/2022
// Path: Fryer_App/Alarm.h
// Link: https://github.com/mhdeeb/Fryer_App

#ifndef ALARM_H
#define ALARM_H_H

#include "def.h"

struct Note
{
    u8 note;
    u32 duration;
};

struct Melody
{
    Note *melody;
    u32 length;
};

class Alarm
{
private:
    u8 pin;
    Melody *melody;
    u32 playCount;
    bool isOn = false;
    u32 melodyIndex = 0;
    u32 playIndex = 0;
    u32 lastTime = 0;

public:
    Alarm(u8 pin);
    void Update();
    bool IsOn() const;
    void Start(Melody *melody, u32 playCount = 1);
    void Stop();
    u8 GetPin() const;
    Melody *GetMelody();
};

#endif // ALARMs_H