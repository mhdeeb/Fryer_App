// Date: 15/FEB/2022
// Path: Fryer_App/Heater.cpp
// Link: https://github.com/mhdeeb/Fryer_App

#include "Heater.h"

Heater::Heater(u8 heaterPin, u8 sensorPin, u8 targetTempC, u8 minTemp, u8 maxTemp) : targetTempCounter(minTemp, maxTemp, targetTempC, 1, false), sensor(sensorPin) { SetHeaterPin(heaterPin); }

s16 Heater::GetTemp() const { return sensor.GetTemp(); }

void Heater::SetHeaterPin(u8 heaterPin)
{
    this->heaterPin = heaterPin;
    pinMode(this->heaterPin, OUTPUT);
}

u8 Heater::GetHeaterPin() const { return heaterPin; }

Counter &Heater::GetTargetTempCounter() { return targetTempCounter; }

TempSensor &Heater::GetSensor() { return sensor; }

void Heater::Control()
{
}

void Heater::Update()
{
    sensor.Update();
    Control();
}