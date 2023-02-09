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

class Counter
{
private:
    int value;
    int max;
    int min;
    int step;
    bool isCycleable;

public:
    Counter(int min, int max, int step, bool isCycleable = 1);

    void SetValue(int value);

    int GetValue() const;

    void SetMax(int max);

    int GetMax() const;

    void SetMin(int min);

    int GetMin() const;

    void SetStep(int step);

    int GetStep() const;

    void SetCycleable(bool isCycleable);

    void AddStep(int step);

    void Increment();

    void Decrement();
};

#endif // COUNTER_H
