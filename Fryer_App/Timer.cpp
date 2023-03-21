// Date: 10/FEB/2022
// Path: Fryer_App/Timer.cpp
// Link: https://github.com/mhdeeb/Fryer_App

#include "Timer.h"

#include <Arduino.h>

Timer::Timer(u32 initialValue) : counter(0, initialValue, initialValue, 1, false), lastUpdateTime(millis())
{
    counter.SetValue(initialValue);
}

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
    if (isRunning && (millis() - lastUpdateTime) >= updateDelay || isFinished)
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

void Timer::IncrementMinutes(u32 minutes)
{
    counter.SetValue(counter.GetValue() + minutes * 60);
}

void Timer::IncrementSeconds(u32 seconds)
{
    counter.SetValue(counter.GetValue() + seconds);
}

void Timer::DecrementMinutes(u32 minutes)
{
    counter.SetValue(counter.GetValue() - minutes * 60);
}

void Timer::DecrementSeconds(u32 seconds)
{
    counter.SetValue(counter.GetValue() - seconds);
}

u32 Timer::GetMinutes() const { return counter.GetValue() / 60; }

u32 Timer::GetSeconds() const { return counter.GetValue() % 60; }

u32 Timer::GetTime() const { return counter.GetValue(); }

u32 Timer::GetMax() const { return counter.GetMax(); }
