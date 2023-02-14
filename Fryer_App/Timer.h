// Date: 10/FEB/2022
// Path: Fryer_App/Timer.cpp
// Link: https://github.com/mhdeeb/Fryer_App

#ifndef TIMER_H
#define TIMER_H

#include "def.h"

#include "Counter.h"

#include <LiquidCrystal_I2C.h>

#include "Buzzer.h"

class Timer
{
private:
    Counter counter;
    u32 col;
    u32 row;
    const char *name;
    u32 lastUpdateTime;
    LiquidCrystal_I2C *lcd;
    u32 updateDelay = 1000;
    bool isRunning = false;
    bool isFinished = false;
    Melody *melody = nullptr;
    u32 melodyIndex = 0;
    u32 lastBlinkTime = 0;
    u32 playIndex = 0;
    u32 playCount = 0;

public:
    Timer(u32 initialValue, u32 col, u32 row, const char *name, LiquidCrystal_I2C *lcd = nullptr);

    void Set(u32 value);

    void Start();

    void Stop();

    void Reset();

    void SetUpdateDelay(u32 updateDelay);

    void Draw();

    void Update();

    void SetLCD(LiquidCrystal_I2C *lcd);

    bool IsRunning() const;

    bool IsFinished() const;

    u32 GetTime() const;

    void StartBlinking(Melody *melody, u32 playCount);

    void StopBlinking();

    bool IsBlinking() const;
};

#endif // TIMER_H
