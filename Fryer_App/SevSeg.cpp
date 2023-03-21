// Date: 18/MAR/2023
// Path: Fryer_App/SevSeg.h
// Link: https://github.com/mhdeeb/Fryer_App

#include "SevSeg.h"

#include <Arduino.h>

SevSeg::SevSeg(u8 clk, u8 dio) : display(clk, dio)
{
    display.setBrightness(0x05);
}

void SevSeg::Set(Timer *timer)
{
    this->timer = timer;
}

void SevSeg::Update()
{
    if ((millis() - lastUpdateTime) < 500 || !timer)
        return;

    if (timer->IsRunning() || timer->IsFinished())
        blink = !blink;
    else
        blink = false;

    if (timer->IsFinished() && blink)
        display.setBrightness(0x05, false);
    else
        display.setBrightness(0x05, true);

    display.showNumberDecEx(timer->GetMinutes() * 100 + timer->GetSeconds(), blink ? 0 : 0b01000000, true, 4, 0);

    lastUpdateTime = millis();
}

TM1637Display *SevSeg::GetDisplay()
{
    return &display;
}