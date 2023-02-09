#include "PushButton.h"

#include <Arduino.h>

PushButton::PushButton(u8 pin, u32 holdTime)
{
    this->pin = pin;
    pinMode(this->pin, INPUT_PULLUP);
    state = digitalRead(this->pin);
    lastState = state;
    this->holdTime = holdTime;
    startTime = millis();
}

void PushButton::Update()
{
    if (millis() - startTime < debounceDelay)
    {
        return;
    }

    lastState = state;
    state = digitalRead(pin);

    if (state == LOW)
    {
        counter = millis() - startTime;
        if (counter <= holdTime)
        {
            isHeld = 0;
        }
        else
        {
            isHeld = 1;
        }
    }

    if (state == LOW && lastState == HIGH)
    {
        isPressed = 1;
        isToggled = !isToggled;
        startTime = millis();
    }
    else if (state == HIGH && lastState == LOW)
    {
        isReleased = 1;
        isHeld = 0;
        counter = 0;
    }
    else if (state == lastState)
    {
        isPressed = 0;
        isReleased = 0;
    }
}

bool PushButton::IsPressed() { return isPressed; }

bool PushButton::IsReleased() { return isReleased; }

bool PushButton::IsHeld() { return isHeld; }

bool PushButton::IsToggled() { return isToggled; }

u32 PushButton::GetCounter() { return counter; }

void PushButton::SetDebounceDelay(u32 delay) { debounceDelay = delay; }

void PushButton::SetHoldTime(u32 holdTime) { this->holdTime = holdTime; }

u8 PushButton::GetState() { return state; }

u32 PushButton::GetHoldTime() { return holdTime; }

u32 PushButton::GetDebounceDelay() { return debounceDelay; }

void PushButton::Reset()
{
    isPressed = 0;
    isReleased = 0;
    isHeld = 0;
    isToggled = 0;
    counter = 0;
}
