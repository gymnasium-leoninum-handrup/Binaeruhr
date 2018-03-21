#ifndef __CLOCK_HPP
#define __CLOCK_HPP

#include <Arduino.h>
#include "DigitClock.hpp"
#include "BinClock.hpp"
#include "Rotary.hpp"
#include <DS3231.h>

class Clock
{
private:
  int hours;
  int minutes;
  int seconds;
  DigitClock* digitClock;
  BinClock* binClock;

public:
  Clock(int dataPin, int clockPin, int latchPin, int dataPinBin, int clockPinBin, int latchPinBin);
  ~Clock();
  void setTime(int  hours, int minutes, int seconds);
  void render();
  void testClock();
  void setTimeEncoder(Rotary *rotary,DS3231 *rtc);
};
#endif
