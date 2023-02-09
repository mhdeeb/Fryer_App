#include "Counter.h"

#include <assert.h>

Counter::Counter(s32 min, s32 max, s32 step, bool isCycleable)
{
    assert(max > min);
    this->min = min;
    this->max = max;
    this->step = step;
    if (step > 0)
        this->value = min;
    else
        this->value = max;
    this->isCycleable = isCycleable;
}

void Counter::SetValue(s32 value) { value = value; }

s32 Counter::GetValue() const { return value; }

void Counter::SetMax(s32 max) { this->max = max; }

s32 Counter::GetMax() const { return max; }

void Counter::SetMin(s32 min) { this->min = min; }

s32 Counter::GetMin() const { return min; }

void Counter::SetStep(s32 step) { this->step = step; }

s32 Counter::GetStep() const { return step; }

void Counter::SetCycleable(bool isCycleable) { this->isCycleable = isCycleable; }

void Counter::AddStep(s32 step)
{
    value += step;
    if (value > max)
    {
        if (isCycleable)
            value -= max - min + 1;
        else
            value = max;
    }
    else if (value < min)
    {
        if (isCycleable)
            value += max - min + 1;
        else
            value = min;
    }
}

void Counter::Increment() { AddStep(step); }

void Counter::Decrement() { AddStep(-step); }