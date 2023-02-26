// Date: 15/FEB/2022
// Path: Fryer_App/TempSensor.cpp
// Link: https://github.com/mhdeeb/Fryer_App

#include "TempSensor.h"

#include "Arduino.h"

TempSensor::TempSensor(u8 pin) : pin(pin)
{
    Calibrate();
    lastUpdate = millis();
}

void TempSensor::SetOffset(f64 offset) { this->offset = offset; }

f64 TempSensor::GetOffset() const { return offset; }

void TempSensor::SetSlope(f64 slope) { this->slope = slope; }

f64 TempSensor::GetSlope() const { return slope; }

void TempSensor::SetPin(u8 pin)
{
    this->pin = pin;
    pinMode(this->pin, INPUT);
}

u8 TempSensor::GetPin() const { return pin; }

void TempSensor::Update()
{
    if (millis() - lastUpdate < updateInterval)
        return;
    tempC = s16(analogRead(GetPin()) * GetSlope() + GetOffset());
    lastUpdate = millis();
}

s16 TempSensor::GetTemp() const { return tempC; }

void TempSensor::Calibrate()
{
    SetSlope(0.4839193597);
    SetOffset(-25.6);
}