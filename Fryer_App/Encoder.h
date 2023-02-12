// Date: 10/FEB/2022
// Path: Fryer_App/Encoder.cpp
// Link: https://github.com/mhdeeb/Fryer_App

#ifndef ENCODER_H
#define ENCODER_H

#include "PushButton.h"
#include "Counter.h"

class Encoder
{
private:
    u8 outputAPin;
    u8 outputBPin;
    u8 AState;
    u8 BState;
    PushButton button;
    Counter *counter = nullptr;
    s8 AChange = 0;

public:
    Encoder(u8 outputAPin, u8 outputBPin, u8 buttonPin);

    void Update();

    s8 popRotationChange();

    PushButton &GetButton();

    void Reset();

    void SetCounter(Counter *counter);

    Counter *GetCounter();

    void LockCounter();

    void UnlockCounter();
};

#endif // ENCODER_H
