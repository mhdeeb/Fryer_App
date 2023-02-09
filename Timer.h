/*****************************************/
/*****************************************/
/****** By  : Abdelrahman Mohamed ********/
/****** Date:     7/DEC/2022      ********/
/****** SWC :       TIMER         ********/
/****** VER :       1.00          ********/
/*****************************************/
/*****************************************/
#ifndef TIMER_H
#define TIMER_H

#include "tdef.h"

#include "Counter.h"

class Timer
{
private:
    Counter counter;
    u32 lastUpdateTime;
    u32 updateDelay = 1000;
    bool isRunning = 0;
    bool isFinished = 0;

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

    u32 GetTime() const;
};

#endif // TIMER_H
