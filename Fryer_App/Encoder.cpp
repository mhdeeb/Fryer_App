// Date: 10/FEB/2022
// Path: Fryer_App/Encoder.cpp
// Link: https://github.com/mhdeeb/Fryer_App

#include "Encoder.h"

#include "PushButton.h"

#include <Arduino.h>

Encoder::Encoder(u8 outputAPin, u8 outputBPin, u8 buttonPin) : button(buttonPin, 100)
{
    this->outputAPin = outputAPin;
    this->outputBPin = outputBPin;
    pinMode(this->outputAPin, INPUT);
    pinMode(this->outputBPin, INPUT);
    this->AState = digitalRead(this->outputAPin);
    this->BState = digitalRead(this->outputBPin);
}

void Encoder::Update()
{
    u8 A_State = digitalRead(this->outputAPin);
    if (A_State != this->AState)
    {
        if (digitalRead(this->outputBPin) != A_State)
            this->AChange = 1;
        else
            this->AChange = -1;
    }
    this->AState = A_State;
    button.Update();
}

s8 Encoder::popRotationChange()
{
    s8 isChanged = AChange;
    AChange = 0;
    return isChanged;
}

PushButton &Encoder::GetButton()
{
    return button;
}
