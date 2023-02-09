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
    s32 value;
    s32 max;
    s32 min;
    s32 step;
    bool isCycleable;

public:
    Counter(s32 min, s32 max, s32 step, bool isCycleable = 1);

    void SetValue(s32 value);

    s32 GetValue() const;

    void SetMax(s32 max);

    s32 GetMax() const;

    void SetMin(s32 min);

    s32 GetMin() const;

    void SetStep(s32 step);

    s32 GetStep() const;

    void SetCycleable(bool isCycleable);

    void AddStep(s32 step);

    void Increment();

    void Decrement();
};

#endif // COUNTER_H
