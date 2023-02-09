/*****************************************/
/*****************************************/
/****** By  : Abdelrahman Mohamed ********/
/****** Date:     7/DEC/2022      ********/
/****** SWC :       COUNTER       ********/
/****** VER :        1.00         ********/
/*****************************************/
/*****************************************/
#ifndef COUNTER_H
#define COUNTER_H

#include "tdef.h"

class Counter
{
private:
    u32 value;
    u32 max;
    u32 min;
    u32 step;
    bool isCycleable;

public:
    Counter(u32 min, u32 max, u32 step, bool isCycleable = 1);

    void SetValue(u32 value);

    u32 GetValue() const;

    void SetMax(u32 max);

    u32 GetMax() const;

    void SetMin(u32 min);

    u32 GetMin() const;

    void SetStep(u32 step);

    u32 GetStep() const;

    void SetCycleable(bool isCycleable);

    void Increment();

    void Decrement();
};

#endif // COUNTER_H
