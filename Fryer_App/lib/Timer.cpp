// Date: 10/FEB/2022
// Path: Fryer_App/Timer.cpp
// Link: https://github.com/mhdeeb/Fryer_App

#include "Timer.h"

#include <Arduino.h>

Timer::Timer() : counter(0, 0, 0, 1, false) {}

Timer::Timer(u32 initialValue) : counter(0, initialValue, initialValue, 1, false) {}

void Timer::Set(u32 value)
{
    counter.SetMax(value);
    counter.SetValue(value);

    Stop();
}

void Timer::Start()
{
    isRunning = true;
    lastUpdateTime = millis();
}

void Timer::Stop()
{
    isRunning = false;
    isFinished = false;
}

void Timer::Reset()
{
    Set(counter.GetMax());
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
