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
    u16 lastUpdateTime;
    u16 updateDelay = 1000;
    bool isRunning = 0;
    bool isFinished = 0;

public:
    Timer(u16 initialValue);

    void Set(u16 value);

    void Start();

    void Stop();

    void Reset();

    void SetUpdateDelay(u16 updateDelay);

    void Update();

    bool IsRunning() const;

    bool IsFinished() const;

    u16 GetTime() const;
};

#endif // TIMER_H
