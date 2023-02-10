// Date: 10/FEB/2022
// Path: Fryer_App/Encoder.cpp
// Link: https://github.com/mhdeeb/Fryer_App


#ifndef ENCODER_H
#define ENCODER_H

#include "PushButton.h"

class Encoder
{
private:
    u8 outputAPin;
    u8 outputBPin;
    u8 AState;
    u8 BState;
    PushButton button;
    s8 AChange = 0;

public:
    Encoder(u8 outputAPin, u8 outputBPin, u8 buttonPin);

    void Update();

    s8 popRotationChange();

    PushButton &GetButton();
};

#endif // ENCODER_H
