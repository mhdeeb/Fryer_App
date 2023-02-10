// Date: 10/FEB/2022
// Path: Fryer_App/Heater.h
// Link: https://github.com/mhdeeb/Fryer_App

#ifndef HEATER_INTERFACE_H
#define HEATER_INTERFACE_H

#define TEMP_SENSOR A0
#define HEATER 4
#define ALARM 6

// Heater parameters
const int PT1000_PIN = A0;
const float vt_factor = 1.01;
const float offset = -25.6;

float temp_c;
int sensor_value;
float voltage;

void HEATER_CONTROL(void)
{
    sensor_value = analogRead(PT1000_PIN);
    voltage = sensor_value * (5.0 / 1023.0);
    temp_c = (((voltage * 100) / vt_factor) + offset);
    delay(500);
}

#endif // HEATER_INTERFACE_H
