#ifndef __CLOCK_HPP
#define __CLOCK_HPP

#include <Arduino.h>
#include "DigitClock.hpp"
#include "BinClock.hpp"
#include "Rotary.hpp"
#include <DS3231.h>
#include <SoftwareSerial.h>
typedef void (*func)(void);

class Clock
{
private:
  int hours;
  int minutes;
  int seconds;
  DigitClock* digitClock;
  BinClock* binClock;
  SoftwareSerial* esp;

public:
  Clock(int dataPin, int clockPin, int latchPin, int dataPinBin, int clockPinBin, int latchPinBin, int rx, int tx);
  ~Clock();
  void setTime(int  hours, int minutes, int seconds);
  void render(bool dots = false);
  void testClock();
  void setTimeEncoder(RotaryEncoder *rotary,DS3231 *rtc);
  int* getESPTime(func fun = NULL);
};

#endif
