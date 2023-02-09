#include "Counter.h"

#include <assert.h>

Counter::Counter(int min, int max, int step, bool isCycleable)
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

void Counter::SetValue(int value) { value = value; }

int Counter::GetValue() const { return value; }

void Counter::SetMax(int max) { this->max = max; }

int Counter::GetMax() const { return max; }

void Counter::SetMin(int min) { this->min = min; }

int Counter::GetMin() const { return min; }

void Counter::SetStep(int step) { this->step = step; }

int Counter::GetStep() const { return step; }

void Counter::SetCycleable(bool isCycleable) { this->isCycleable = isCycleable; }

void Counter::AddStep(int step)
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