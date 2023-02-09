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
    u32 updateDelay;
    u32 lastUpdateTime;
    bool isRunning = 0;
    bool isFinished = 0;

public:
    Timer(int initialValue);

    void Start();

    void Stop();

    void Reset();

    void SetUpdateDelay(u32 updateDelay);

    void Update();
};

#endif // TIMER_H
