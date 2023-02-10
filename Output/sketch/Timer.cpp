#line 1 "c:\\Users\\eldeeb\\Desktop\\Fryer_APP\\Fryer_App\\Timer.cpp"
#include "Timer.h"

#include <Arduino.h>

Timer::Timer(u32 initialValue, u32 col, u32 row, const char *name, LiquidCrystal_I2C *lcd) : counter(0, initialValue, 1, false), col(col), row(row), name(name), lcd(lcd), lastUpdateTime(millis())
{
    counter.SetValue(initialValue);
    Draw();
}

void Timer::Set(u32 value)
{
    counter.SetMax(value);
    counter.SetValue(value);
    Draw();
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
    StopBlinking();
    Draw();
    isRunning = false;
    isFinished = false;
}

void Timer::SetUpdateDelay(u32 updateDelay) { this->updateDelay = updateDelay; }

void Timer::Draw()
{
    if (lcd)
    {
        char string[14];
        lcd->setCursor(col, row);
        sprintf(string, "%s %02lu:%02lu", name, GetTime() / 60, GetTime() % 60);

        if (IsBlinking())
        {
            if (melodyIndex == 0 || ((melodyIndex < melody->length) && ((millis() - lastBlinkTime) >= melody->melody[melodyIndex - 1].duration)))
            {
                if (!melody->melody[melodyIndex].note)
                {
                    lcd->printstr("        ");
                    lastBlinkTime = millis();
                }
                else
                    lcd->printstr(string);
                melodyIndex++;
                lastBlinkTime = millis();
            }
            else if (melodyIndex >= melody->length && ((millis() - lastBlinkTime) >= melody->melody[melodyIndex - 1].duration))
            {
                if (playIndex < playCount - 1)
                {
                    playIndex++;
                    melodyIndex = 0;
                }
                else
                    Reset();
            }
        }
        else
            lcd->printstr(string);
    }
}

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

        Draw();

        lastUpdateTime = millis();
    }
}

void Timer::SetLCD(LiquidCrystal_I2C *lcd)
{
    this->lcd = lcd;
    Draw();
}

bool Timer::IsRunning() const { return isRunning; }

bool Timer::IsFinished() const { return isFinished; }

u32 Timer::GetTime() const { return counter.GetValue(); }

void Timer::StartBlinking(Melody *melody, u32 playCount = 1)
{
    this->playCount = playCount;
    this->playIndex = 0;
    this->melody = melody;
}

void Timer::StopBlinking()
{
    this->playCount = 0;
    this->playIndex = 0;
    melody = nullptr;
}

bool Timer::IsBlinking() const
{
    return playCount;
}
