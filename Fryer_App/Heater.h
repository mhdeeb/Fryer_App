// Date: 10/FEB/2022
// Path: Fryer_App/Heater.h
// Link: https://github.com/mhdeeb/Fryer_App

#ifndef HEATER_INTERFACE_H
#define HEATER_INTERFACE_H

#include "def.h"
#include "Counter.h"

class Heater
{
private:
    u8 heaterPin;
    u8 sensorPin;
    s16 tempC;
    Counter targetTempCounter;
    f64 offset;
    f64 slope;

public:
    Heater(u8 heaterPin, u8 sensorPin, u8 targetTempC, u8 minTemp, u8 maxTemp);
    void UpdateTemp();
    s16 GetTemp() const;
    void SetOffset(f64 offset);
    f64 GetOffset() const;
    void SetSlope(f64 slope);
    f64 GetSlope() const;
    void SetHeaterPin(u8 heaterPin);
    u8 GetHeaterPin() const;
    void SetSensorPin(u8 sensorPin);
    u8 GetSensorPin() const;
    Counter &GetTargetTempCounter();
    void Calibrate();
    void Update();
};

#endif // HEATER_INTERFACE_H
