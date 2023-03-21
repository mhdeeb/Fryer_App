// Date: 10/FEB/2022
// Path: Fryer_App/Timer.cpp
// Link: https://github.com/mhdeeb/Fryer_App

#ifndef TIMER_H
#define TIMER_H

#include "def.h"

#include "Counter.h"

#include "Buzzer.h"

class Timer
{
private:
    Counter counter;
    u32 lastUpdateTime;
    u32 updateDelay = 1000;
    bool isRunning = false;
    bool isFinished = false;

public:
    Timer(u32 initialValue);

    void Set(u32 value);

    void Start();

    void Stop();

    void Reset();

    void SetUpdateDelay(u32 updateDelay);

    void Update();

    bool IsRunning() const;

    bool IsFinished() const;

    void IncrementMinutes(u32 minutes);

    void IncrementSeconds(u32 seconds);

    void DecrementMinutes(u32 minutes);

    void DecrementSeconds(u32 seconds);

    u32 GetMinutes() const;

    u32 GetSeconds() const;

    u32 GetTime() const;

    u32 GetMax() const;
};

#endif // TIMER_H
