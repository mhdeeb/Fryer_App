/*****************************************/
/*****************************************/
/****** By  : Abdelrahman Mohamed ********/
/****** Date:     7/DEC/2022      ********/
/****** SWC :       Alarm         ********/
/****** VER :       1.00          ********/
/*****************************************/
/*****************************************/
#ifndef ALARM_H
#define ALARM_H_H

#include "tdef.h"

struct note
{
    u8 note;
    u32 duration;
};

struct Melody
{
    note *melody;
    u32 length;
};

class Alarm
{
private:
    u8 pin;
    Melody melody;
    u32 playCount;
    bool isOn = false;
    u32 melodyIndex = 0;
    u32 playIndex = 0;
    u32 lastTime = 0;

public:
    Alarm(u8 pin, Melody melody, u32 playCount = 1);
    void Update();
    bool IsOn() const;
    void Start();
    void Stop();
    u8 GetPin() const;
    Melody GetMelody();
};

#endif // ALARMs_H