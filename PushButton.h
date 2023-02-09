/*****************************************/
/*****************************************/
/****** By  : Abdelrahman Mohamed ********/
/****** Date:     7/DEC/2022      ********/
/****** SWC :     PushButton      ********/
/****** VER :        1.00         ********/
/*****************************************/
/*****************************************/
#ifndef PUSH_BUTTON_H
#define PUSH_BUTTON_H

#include "tdef.h"

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
    bool IsPressed();
    bool IsReleased();
    bool IsHeld();
    bool IsToggled();
    u32 GetTimeHeld();
    void SetHoldTime(u32 holdTime);
    u8 GetState();
    u32 GetHoldTime();
    void Reset();
};

#endif // PUSH_BUTTON_H