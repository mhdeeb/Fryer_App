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
        if (timeHeld > holdTime && !wasReseted)
            isHeld = true;
        else
            isHeld = false;
    }
    else if (lastState == LOW && state == HIGH)
    {
        isPressed = false;
        if (!wasReseted)
            isReleased = true;
        else
            wasReseted = false;
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

bool PushButton::Get(PushButtonInterface state) const
{
    switch (state)
    {
    case PushButtonInterface::PRESSED:
        return isPressed;
    case PushButtonInterface::RELEASED:
        return isReleased;
    case PushButtonInterface::HELD:
        return isHeld;
    case PushButtonInterface::TOGGLED:
        return isToggled;
    case PushButtonInterface::RESETED:
        return wasReseted;
    default:
        return false;
    }
}

void PushButton::Set(PushButtonInterface state, bool value)
{
    switch (state)
    {
    case PushButtonInterface::PRESSED:
        isPressed = value;
        break;
    case PushButtonInterface::RELEASED:
        isReleased = value;
        break;
    case PushButtonInterface::HELD:
        isHeld = value;
        break;
    case PushButtonInterface::TOGGLED:
        isToggled = value;
        break;
    case PushButtonInterface::RESETED:
        wasReseted = value;
        break;
    case PushButtonInterface::ALL:
    default:
        isPressed = value;
        isReleased = value;
        isHeld = value;
        isToggled = value;
        wasReseted = value;
    }
}

void PushButton::Reset(PushButtonInterface state)
{
    switch (state)
    {
    case PushButtonInterface::PRESSED:
        isPressed = false;
        break;
    case PushButtonInterface::RELEASED:
        isReleased = false;
        break;
    case PushButtonInterface::HELD:
        isHeld = false;
        break;
    case PushButtonInterface::TOGGLED:
        isToggled = false;
        break;
    case PushButtonInterface::RESETED:
        wasReseted = true;
        break;
    case PushButtonInterface::ALL:
    default:
        isPressed = false;
        isReleased = false;
        isHeld = false;
        isToggled = false;
        wasReseted = true;
    }
}

u32 PushButton::GetTimeHeld() const { return timeHeld; }

void PushButton::ResetTimeHeld()
{
    timeHeld = 0;
    wasReseted = true;
}

u8 PushButton::GetState() const { return state; }

u32 PushButton::GetHoldTime() const { return holdTime; }

void PushButton::SetHoldTime(u32 holdTime) { this->holdTime = holdTime; }