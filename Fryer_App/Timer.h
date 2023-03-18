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

    // u32 GetMilliseconds() const;

    // u32 GetHours() const;

    // u32 GetMinutes() const;

    // u32 GetSeconds() const;

    // void SetTime(u32 hours, u32 minutes, u32 seconds, u32 milliseconds);

    // void SetMinutes(u32 minutes);

    // void SetSeconds(u32 seconds);

    // void SetHours(u32 hours);

    // void SetMilliseconds(u32 milliseconds);

    // void IncrementMinutes(u32 minutes);

    // void IncrementSeconds(u32 seconds);

    // void IncrementHours(u32 hours);

    // void IncrementMilliseconds(u32 milliseconds);

    // void DecrementMinutes(u32 minutes);

    // void DecrementSeconds(u32 seconds);

    // void DecrementHours(u32 hours);

    // void DecrementMilliseconds(u32 milliseconds);

    void IncrementMinutes(u32 minutes);

    void IncrementSeconds(u32 seconds);

    void DecrementMinutes(u32 minutes);

    void DecrementSeconds(u32 seconds);

    u32 GetValue() const;

    u32 GetMinutes() const;

    u32 GetSeconds() const;

    u32 GetTime() const;

    u32 GetMax() const;
};

#endif // TIMER_H
