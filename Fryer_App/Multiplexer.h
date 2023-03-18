// Date: 18/MAR/2023
// Path: Fryer_App/Multiplexer.h
// Link: https://github.com/mhdeeb/Fryer_App

#ifndef MULTIPLEXER_H
#define MULTIPLEXER_H

#include "def.h"
class Multiplexer
{
private:
    u8 pinCount;
    u8 *pins = nullptr;
    bool *values = nullptr;
    u32 currIndex = 0;
    u32 lastUpdateTime = 0;

public:
    Multiplexer(const u8 *pins, u8 pinCount);
    ~Multiplexer();
    void Set(const bool *values);
    void Set(u32 index, bool value);
    void Update();
};

#endif // MULTIPLEXER_H