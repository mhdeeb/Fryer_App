#include "Timer.h"

#include "Counter.h"

#include <Arduino.h>

Timer::Timer(int initialValue) : counter(0, initialValue, -1, false), lastUpdateTime(millis()) {}

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
        counter.Increment();
        if (counter.GetValue() == counter.GetMin())
        {
            isRunning = false;
            isFinished = true;
        }
        lastUpdateTime = millis();
    }
}
