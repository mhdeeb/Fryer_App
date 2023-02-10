// Date: 10/FEB/2022
// Path: Fryer_App/PushButton.h
// Link: https://github.com/mhdeeb/Fryer_App

#ifndef PUSH_BUTTON_H
#define PUSH_BUTTON_H

#include "def.h"

class PushButton
{
private:
    u8 pin;
    u8 state;
    u8 lastState;
    u32 holdTime;
    u32 holdStartTime;
    bool isPressed = false;
    bool isReleased = false;
    bool isHeld = false;
    bool isToggled = false;
    u32 timeHeld = 0;

public:
    PushButton(u8 pin, u32 holdTime);
    void Update();
    bool IsPressed() const;
    bool IsReleased() const;
    bool IsHeld() const;
    bool IsToggled() const;
    u32 GetTimeHeld() const;
    void SetHoldTime(u32 holdTime);
    u8 GetState() const;
    u32 GetHoldTime() const;
    void Reset();
};

#endif // PUSH_BUTTON_H