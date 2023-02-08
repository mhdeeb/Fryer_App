/*****************************************/
/*****************************************/
/****** By  : Abdelrahman Mohamed ********/
/****** Date:     7/DEC/2022      ********/
/****** SWC :      Encoder        ********/
/****** VER :        1.00         ********/
/*****************************************/
/*****************************************/
#ifndef ENCODER_INTERFACE_H
#define ENCODER_INTERFACE_H

#include "Counter_interface.h"
#include "PushButton_Interface.h"
#include <Arduino.h>

typedef struct Encoder_t
{
    u8 outputAPin;
    u8 outputBPin;
    u8 AState;
    u8 BState;
    PushButton button;
    bool isAChanged = false;
} Encoder;

void Encoder_VidInit(Encoder *encoder, u8 outputAPin, u8 outputBPin, u8 buttonPin)
{
    encoder->outputAPin = outputAPin;
    encoder->outputBPin = outputBPin;
    pinMode(encoder->outputAPin, INPUT);
    pinMode(encoder->outputBPin, INPUT);
    encoder->AState = digitalRead(encoder->outputAPin);
    encoder->BState = digitalRead(encoder->outputBPin);
    PushButton_Init(&encoder->button, buttonPin, 100);
}

void Encoder_VidControl(Encoder *encoder, Counter *counter)
{
    u8 A_State = digitalRead(encoder->outputAPin);
    if (A_State != encoder->AState)
    {
        if (digitalRead(encoder->outputBPin) != A_State)
        {
            counter->value += counter->step;
            if (counter->value > counter->max)
                counter->value -= counter->max - counter->min + 1;
        }
        else
        {
            counter->value -= counter->step;
            if (counter->value < counter->min)
                counter->value += counter->max - counter->min + 1;
        }

        encoder->isAChanged = true;
    }
    encoder->AState = A_State;
}

bool Encoder_popRotationChange(Encoder *encoder)
{
    bool isChanged = encoder->isAChanged;
    encoder->isAChanged = false;
    return isChanged;
}

#endif // ENCODER_INTERFACE_H
