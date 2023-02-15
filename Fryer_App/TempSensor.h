// Date: 15/FEB/2022
// Path: Fryer_App/TempSensor.h
// Link: https://github.com/mhdeeb/Fryer_App

#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

#include "def.h"

class TempSensor
{
private:
    u8 pin;
    f64 offset;
    f64 slope;
    s16 tempC;

public:
    TempSensor(u8 pin);
    void SetOffset(f64 offset);
    f64 GetOffset() const;
    void SetSlope(f64 slope);
    f64 GetSlope() const;
    void SetPin(u8 pin);
    u8 GetPin() const;
    void Update();
    s16 GetTemp() const;
    void Calibrate(); // To be implemented
};

#endif // TEMP_SENSOR_H