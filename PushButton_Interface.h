/*****************************************/
/*****************************************/
/****** By  : Abdelrahman Mohamed ********/
/****** Date:     7/DEC/2022      ********/
/****** SWC :      Encoder        ********/
/****** VER :        1.00         ********/
/*****************************************/
/*****************************************/
#ifndef PUSH_BUTTON_INTERFACE_H
#define PUSH_BUTTON_INTERFACE_H

#include "STD_TYPES.h"
#include <Arduino.h>

typedef struct PushButton_t
{
    u8 pin;
    u8 state;
    u8 lastState;
    u8 isPressed = 0;
    u8 isReleased = 0;
    u8 isHeld = 0;
    u8 isToggled = 0;
    u32 counter = 0;
    u32 holdTime;
    u32 _startTime;
    u32 _debounceDelay = 50;
} PushButton;

void PushButton_Init(PushButton *button, u8 pin, u32 holdTime)
{
    button->pin = pin;
    pinMode(button->pin, INPUT_PULLUP);
    button->state = digitalRead(button->pin);
    button->lastState = button->state;
    button->holdTime = holdTime;
    button->_startTime = millis();
}

void PushButton_Update(PushButton *button)
{
    if (millis() - button->_startTime < button->_debounceDelay)
    {
        return;
    }

    button->lastState = button->state;
    button->state = digitalRead(button->pin);

    if (button->state == LOW)
    {
        button->counter = millis() - button->_startTime;
        if (button->counter <= button->holdTime)
        {
            button->isHeld = 0;
        }
        else
        {
            button->isHeld = 1;
        }
    }

    if (button->state == LOW && button->lastState == HIGH)
    {
        button->isPressed = 1;
        button->isToggled = !button->isToggled;
        button->_startTime = millis();
    }
    else if (button->state == HIGH && button->lastState == LOW)
    {
        button->isReleased = 1;
        button->isHeld = 0;
        button->counter = 0;
    }
    else if (button->state == button->lastState)
    {
        button->isPressed = 0;
        button->isReleased = 0;
    }
}

#endif // PUSH_BUTTON_INTERFACE_H