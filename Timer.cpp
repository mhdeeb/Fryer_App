#include "Timer.h"

#include <Arduino.h>

Timer::Timer(u32 initialValue) : counter(0, initialValue, 1, false), lastUpdateTime(millis()) { counter.SetValue(initialValue); }

void Timer::Set(u32 value)
{
    counter.SetMax(value);
    counter.SetValue(value);
}

void Timer::Start()
{
    isRunning = true;
    isFinished = false;
}

void Timer::Stop()
{
    isRunning = false;
    isFinished = false;
}

void Timer::Reset()
{
    counter.SetValue(counter.GetMax());
    isRunning = false;
    isFinished = false;
}

void Timer::SetUpdateDelay(u32 updateDelay) { this->updateDelay = updateDelay; }

void Timer::Update()
{
    if (isRunning && (millis() - lastUpdateTime) >= updateDelay)
    {
        counter.Decrement();
        if (counter.GetValue() == counter.GetMin())
        {
            isRunning = false;
            isFinished = true;
        }
        lastUpdateTime = millis();
    }
}

bool Timer::IsRunning() const { return isRunning; }

bool Timer::IsFinished() const { return isFinished; }

u32 Timer::GetTime() const { return counter.GetValue(); }
