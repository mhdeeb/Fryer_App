#include "Counter.h"

Counter::Counter(u32 min, u32 max, u32 step, bool isCycleable)
{
    SetMin(min);
    SetMax(max);
    SetStep(step);
    SetValue(min);
    SetCycleable(isCycleable);
}

void Counter::SetValue(u32 value)
{
    this->value = value;
}

u32 Counter::GetValue() const { return value; }

void Counter::SetMax(u32 max) { this->max = max; }

u32 Counter::GetMax() const { return max; }

void Counter::SetMin(u32 min) { this->min = min; }

u32 Counter::GetMin() const { return min; }

void Counter::SetStep(u32 step) { this->step = step; }

u32 Counter::GetStep() const { return step; }

void Counter::SetCycleable(bool isCycleable) { this->isCycleable = isCycleable; }

void Counter::Increment()
{
    if (GetValue() + step < GetValue())
    {
        if (isCycleable)
            SetValue(min + GetValue() + step);
        else
            SetValue(-1);
    }
    else
        SetValue(GetValue() + step);
    if (GetValue() > max)
    {
        if (isCycleable)
            SetValue(min + GetValue() - max - 1);
        else
            SetValue(max);
    }
}

void Counter::Decrement()
{
    if (GetValue() < step)
    {
        if (isCycleable)
            SetValue(max - (step - GetValue()) + 1);
        else
            SetValue(0);
    }
    else
        SetValue(GetValue() - step);

    if (GetValue() < min)
    {
        if (isCycleable)
            SetValue(max - min + GetValue() + 1);
        else
            SetValue(min);
    }
}