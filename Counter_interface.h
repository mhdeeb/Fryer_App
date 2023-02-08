/*****************************************/
/*****************************************/
/****** By  : Abdelrahman Mohamed ********/
/****** Date:     7/DEC/2022      ********/
/****** SWC :      Encoder        ********/
/****** VER :        1.00         ********/
/*****************************************/
/*****************************************/
#ifndef COUNTER_INTERFACE_H
#define COUNTER_INTERFACE_H

#include "STD_TYPES.h"
#include <assert.h>

typedef struct Counter_t
{
    int value;
    int max;
    int min;
    int step;
} Counter;

void Counter_Init(Counter *counter, int min, int max, int step)
{
    assert(max > min); // max must be greater than min
    counter->min = min;
    counter->max = max;
    counter->step = step;
    counter->value = min;
}

#endif // COUNTER_INTERFACE_H
