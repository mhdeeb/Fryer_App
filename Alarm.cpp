#include "Alarm.h"

#include <Arduino.h>

Alarm::Alarm(u8 pin, Melody melody, u32 playCount)
{
    this->pin = pin;
    pinMode(this->pin, OUTPUT);
    this->melody = melody;
    this->playCount = playCount;
}

void Alarm::Update()
{
    if (isOn)
    {
        if (melodyIndex == 0 || ((melodyIndex < melody.length) && ((millis() - lastTime) >= melody.melody[melodyIndex - 1].duration)))
        {
            analogWrite(pin, melody.melody[melodyIndex].note);
            melodyIndex++;
            lastTime = millis();
        }
        else if (melodyIndex >= melody.length && ((millis() - lastTime) >= melody.melody[melodyIndex - 1].duration))
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

void Alarm::Start()
{
    Stop();
    isOn = true;
}

void Alarm::Stop()
{
    isOn = false;
    melodyIndex = 0;
    playIndex = 0;
    digitalWrite(pin, LOW);
}

u8 Alarm::GetPin() const { return pin; }

Melody Alarm::GetMelody()
{
    return melody;
}
