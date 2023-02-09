#include "Timer.h"

#include <Arduino.h>

Timer::Timer(u16 initialValue) : counter(0, initialValue, -1, false), lastUpdateTime(millis()) {}

void Timer::Set(u16 value) { counter.SetValue(value); }

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

void Timer::SetUpdateDelay(u16 updateDelay) { this->updateDelay = updateDelay; }

void Timer::Update()
{
    if (isRunning && (millis() - lastUpdateTime) >= updateDelay)
    {
        counter.Increment();
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

u16 Timer::GetTime() const { return counter.GetValue(); }
