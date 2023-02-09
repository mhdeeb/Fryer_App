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
    u32 startTime;
    bool isPressed = 0;
    bool isReleased = 0;
    bool isHeld = 0;
    bool isToggled = 0;
    u32 counter = 0;
    u32 debounceDelay = DEBOUNCE_DELAY;

public:
    PushButton(u8 pin, u32 holdTime);
    void Update();
    bool IsPressed();
    bool IsReleased();
    bool IsHeld();
    bool IsToggled();
    u32 GetCounter();
    void SetDebounceDelay(u32 delay);
    void SetHoldTime(u32 holdTime);
    u8 GetState();
    u32 GetHoldTime();
    u32 GetDebounceDelay();
    void Reset();
};

#endif // PUSH_BUTTON_H