#ifndef _BINCLOCK_HPP
#define _BINCLOCK_HPP

#include "Arduino.h"

class BinClock
{
private:
  int dataPin;
  int clockPin;
  int latchPin;

  const byte valueBin1[10]=
    {
     0b11111110,
     0b11111100,
     0b11111010,
     0b11111000,
     0b11110110,
     0b11110100,
     0b11110010,
     0b11110000,
     0b11101110,
     0b11101100
    };
    const byte valueBin10[6]=
    {
     0b11111110,
     0b11011110,
     0b10111110,
     0b10011110,
     0b01111110,
     0b01011110
    };

  public:
    BinClock(int dataPin, int clockPin, int latchPin);
    void testDigitBin();
    void renderTimeBin(int hours, int minutes, int seconds);
};
#endif
