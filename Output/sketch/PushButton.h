#line 1 "c:\\Users\\eldeeb\\Desktop\\Fryer_APP\\Fryer_App\\PushButton.h"
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