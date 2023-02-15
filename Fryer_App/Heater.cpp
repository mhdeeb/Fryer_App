// Date: 15/FEB/2022
// Path: Fryer_App/Heater.cpp
// Link: https://github.com/mhdeeb/Fryer_App

#include "Heater.h"
#include "Arduino.h"

Heater::Heater(u8 heaterPin, u8 sensorPin, u8 targetTempC, u8 minTemp, u8 maxTemp) : targetTempCounter(minTemp, maxTemp, targetTempC, 1, false)
{
    SetHeaterPin(heaterPin);
    SetSensorPin(sensorPin);
    Calibrate();
    UpdateTemp();
}

void Heater::UpdateTemp() { tempC = s16(analogRead(GetSensorPin()) * GetSlope() + GetOffset()); }

s16 Heater::GetTemp() const { return tempC; }

void Heater::SetOffset(f64 offset) { this->offset = offset; }

f64 Heater::GetOffset() const { return offset; }

void Heater::SetSlope(f64 slope) { this->slope = slope; }

f64 Heater::GetSlope() const { return slope; }

void Heater::SetHeaterPin(u8 heaterPin)
{
    this->heaterPin = heaterPin;
    pinMode(this->heaterPin, OUTPUT);
}

u8 Heater::GetHeaterPin() const { return heaterPin; }

void Heater::SetSensorPin(u8 sensorPin)
{
    this->sensorPin = sensorPin;
    pinMode(this->sensorPin, INPUT);
}

u8 Heater::GetSensorPin() const { return sensorPin; }

Counter &Heater::GetTargetTempCounter() { return targetTempCounter; }

void Heater::Calibrate()
{
    SetSlope(0.4839193597);
    SetOffset(-25.6);
}

void Heater::Update()
{
    UpdateTemp();
}
