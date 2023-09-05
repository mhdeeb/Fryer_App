// Date: 18/MAR/2023
// Path: Fryer_App/SevSeg.h
// Link: https://github.com/mhdeeb/Fryer_App

#ifndef SEVSEG_H
#define SEVSEG_H

#include "def.h"
#include "Timer.h"

#include <TM1637Display.h>

class SevSeg
{
private:
    TM1637Display display;
    Timer *timer = nullptr;
    u32 lastUpdateTime = 0;
    bool blink = false;
    bool blinkOn = false;

public:
    SevSeg(u8 clk, u8 dio);
    void Set(Timer *timer);
    Timer *Get();
    void Update();
    TM1637Display *GetDisplay();
};

#endif // SEVSEG_H