// Date: 10/FEB/2022
// Path: Fryer_App/PushButton.cpp
// Link: https://github.com/mhdeeb/Fryer_App

#include "PushButton.h"

#include <Arduino.h>

PushButton::PushButton(u8 pin, u32 holdTime)
{
    this->pin = pin;
    pinMode(this->pin, INPUT_PULLUP);
    state = digitalRead(this->pin);
    lastState = state;
    this->holdTime = holdTime;
}

void PushButton::Update()
{
    lastState = state;
    state = digitalRead(pin);
    if (lastState != state)
        delay(DEBOUNCE_DELAY);
    if (lastState == HIGH && state == LOW)
    {
        isPressed = true;
        isToggled = !isToggled;
        holdStartTime = millis();
    }
    else if (lastState == LOW && state == LOW)
    {
        isPressed = false;
        timeHeld = millis() - holdStartTime;
        if (timeHeld > holdTime)
            isHeld = true;
        else
            isHeld = false;
    }
    else if (lastState == LOW && state == HIGH)
    {
        isPressed = false;
        isReleased = true;
        isHeld = false;
        timeHeld = 0;
    }
    else
    {
        isPressed = false;
        isReleased = false;
        isHeld = false;
        timeHeld = 0;
    }
}

bool PushButton::IsPressed() const { return isPressed; }

bool PushButton::IsReleased() const { return isReleased; }

bool PushButton::IsHeld() const { return isHeld; }

bool PushButton::IsToggled() const { return isToggled; }

u32 PushButton::GetTimeHeld() const { return timeHeld; }

u8 PushButton::GetState() const { return state; }

u32 PushButton::GetHoldTime() const { return holdTime; }

void PushButton::SetHoldTime(u32 holdTime) { this->holdTime = holdTime; }

void PushButton::Reset()
{
    isPressed = false;
    isReleased = false;
    isHeld = false;
    isToggled = false;
    timeHeld = 0;
}
