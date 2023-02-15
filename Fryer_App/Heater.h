// Date: 10/FEB/2022
// Path: Fryer_App/Heater.h
// Link: https://github.com/mhdeeb/Fryer_App

#ifndef HEATER_H
#define HEATER_H

#include "def.h"
#include "TempSensor.h"
#include "Counter.h"

class Heater
{
private:
    u8 heaterPin;
    TempSensor sensor;
    Counter targetTempCounter;

public:
    Heater(u8 heaterPin, u8 sensorPin, u8 targetTempC, u8 minTemp, u8 maxTemp);
    s16 GetTemp() const;
    void SetHeaterPin(u8 heaterPin);
    u8 GetHeaterPin() const;
    Counter &GetTargetTempCounter();
    TempSensor &GetSensor();
    void Control(); // To be implemented
    void Update();
};

#endif // HEATER_H
