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

#include "STD_TYPES.h"
#include <Arduino.h>
#include <assert.h>

typedef struct Encoder_t
{
    u8 outputAPin;
    u8 outputBPin;
    u8 buttonPin;
    u8 AState;
    u8 BState;
    u8 ButtonState;
} Encoder;

typedef struct Counter_t
{
    int value;
    int max;
    int min;
    int step;
} Counter;

void Counter_Init(Counter *counter, int min, int max, int step)
{
    assert(max > min); // max must be greater than min (max > min
    counter->min = min;
    counter->max = max;
    counter->step = step;
    counter->value = min;
}

void Encoder_VidInit(Encoder *encoder, u8 outputAPin, u8 outputBPin, u8 buttonPin)
{
    encoder->outputAPin = outputAPin;
    encoder->outputBPin = outputBPin;
    encoder->buttonPin = buttonPin;
    pinMode(encoder->outputAPin, INPUT);
    pinMode(encoder->outputBPin, INPUT);
    pinMode(encoder->buttonPin, INPUT_PULLUP);
    encoder->AState = digitalRead(encoder->outputAPin);
    encoder->BState = digitalRead(encoder->outputBPin);
    encoder->ButtonState = digitalRead(encoder->buttonPin);
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
    }
    encoder->AState = A_State;
    encoder->BState = digitalRead(encoder->outputBPin);
    encoder->ButtonState = digitalRead(encoder->buttonPin);
}

#endif
