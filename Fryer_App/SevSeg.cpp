// Date: 18/MAR/2023
// Path: Fryer_App/SevSeg.h
// Link: https://github.com/mhdeeb/Fryer_App

#include "SevSeg.h"

#include <Arduino.h>

SevSeg::SevSeg(u8 clk, u8 dio) : display(clk, dio) {}

void SevSeg::Set(Timer *timer)
{
    this->timer = timer;
}

void SevSeg::Update()
{

    if ((millis() - lastUpdateTime) < 500)
        return;
    if (timer == nullptr)
        return;
    if (timer->IsRunning())
        display.showNumberDecEx(timer->GetMinutes() * 100 + timer->GetSeconds(), (blink != blink) ? 0b01000000 : 0, true, 4, 0);
    else
        display.showNumberDecEx(timer->GetMinutes() * 100 + timer->GetSeconds(), 0b01000000, true, 4, 0);
    lastUpdateTime = millis();
}

TM1637Display *SevSeg::GetDisplay()
{
    return &display;
}