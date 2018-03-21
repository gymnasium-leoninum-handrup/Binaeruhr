#ifndef _DIGITCLOCK_HPP
#define _DIGITCLOCK_HPP

#include <Arduino.h>

class DigitClock
{
private:
  int dataPin;
  int clockPin;
  int latchPin;

  int hours;
  int minutes;
  int seconds;

  byte values[11]=
    {
      0b01111011, // 0
      0b01100000, // 1
      0b01011101, // 2
      0b01110101, // 3
      0b01100110, // 4
      0b00110111, // 5
      0b00111111, // 6
      0b01100001, // 7
      0b01111111, // 8
      0b01110111,  // 9
      0b10000000  //dp
    };

  public:
    DigitClock(int dataPin, int clockPin, int latchPin);
    void testDigit();
    void renderTimeDigit(int hours, int minutes, int seconds);
    int getHours();
    int getMinutes();
    int getSeconds();
    void setHours(int hours);
    void setMinutes(int minutes);
    void setSeconds(int seconds);
    void setTime(int hours, int minutes, int seconds);
    void renderDots();
};
#endif
