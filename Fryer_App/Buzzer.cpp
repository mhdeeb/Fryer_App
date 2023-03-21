// Date: 10/FEB/2022
// Path: Fryer_App/Alarm.cpp
// Link: https://github.com/mhdeeb/Fryer_App

#include "Buzzer.h"

#include <Arduino.h>

Buzzer::Buzzer(u8 pin)
{
    this->pin = pin;
    pinMode(this->pin, OUTPUT);
}

void Buzzer::Update()
{
    if (isOn)
    {
        if (melodyIndex == 0 || ((melodyIndex < melody->length) && ((millis() - lastTime) >= melody->melody[melodyIndex - 1].duration)))
        {
            analogWrite(pin, melody->melody[melodyIndex].note);
            melodyIndex++;
            lastTime = millis();
        }
        else if (melodyIndex >= melody->length && ((millis() - lastTime) >= melody->melody[melodyIndex - 1].duration))
        {
            if (playIndex < playCount - 1)
            {
                playIndex++;
                melodyIndex = 0;
            }
            else
            {
                isOn = false;
                melodyIndex = 0;
                playIndex = 0;
                digitalWrite(pin, LOW);
            }
        }
    }
}

bool Buzzer::IsOn() const { return isOn; }

void Buzzer::Play(Melody *melody, u32 playCount)
{
    Stop();
    this->playCount = playCount;
    this->melody = melody;
    isOn = true;
}

void Buzzer::Continue(Melody *melody, u32 playCount)
{
    if (!isOn)
        Play(melody, playCount);
}

void Buzzer::Stop()
{
    isOn = false;
    melodyIndex = 0;
    playIndex = 0;
    this->playCount = 0;
    this->melody = nullptr;
    digitalWrite(pin, LOW);
}

u8 Buzzer::GetPin() const { return pin; }

Melody *Buzzer::GetMelody()
{
    return melody;
}
