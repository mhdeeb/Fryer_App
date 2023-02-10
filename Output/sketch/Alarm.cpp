#line 1 "c:\\Users\\eldeeb\\Desktop\\Fryer_APP\\Fryer_App\\Alarm.cpp"
#include "Alarm.h"

#include <Arduino.h>

Alarm::Alarm(u8 pin)
{
    this->pin = pin;
    pinMode(this->pin, OUTPUT);
}

void Alarm::Update()
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

bool Alarm::IsOn() const { return isOn; }

void Alarm::Start(Melody *melody, u32 playCount)
{
    Stop();
    this->playCount = playCount;
    this->melody = melody;
    isOn = true;
}

void Alarm::Stop()
{
    isOn = false;
    melodyIndex = 0;
    playIndex = 0;
    this->playCount = 0;
    this->melody = nullptr;
    digitalWrite(pin, LOW);
}

u8 Alarm::GetPin() const { return pin; }

Melody *Alarm::GetMelody()
{
    return melody;
}
