#include "DigitClock.hpp"

DigitClock::DigitClock(int dataPin, int clockPin, int latchPin): dataPin(dataPin), clockPin(clockPin), latchPin(latchPin)
{
  pinMode(this->clockPin, OUTPUT);
  pinMode(this->dataPin, OUTPUT);
  pinMode(this->latchPin, OUTPUT);
}

void DigitClock::testDigit()
{
  for(int i = 0; i<=9; i++)
  {
    renderTimeDigit(i*10 + i, i*10 + i, i*10 + i);
    delay(1000);
  }
}

void DigitClock::renderTimeDigit(int hours, int minutes, int seconds)
{
  byte zeit[6];

  zeit[0] = hours/10; // hours big
  zeit[1] = hours%10; // hours small

  zeit[2] = minutes/10; // minutes big
  zeit[3] = minutes%10; // minutes small

  zeit[4] = seconds/10; // seconds big
  zeit[5] = seconds%10; // seconds small

  for(int i = 5; i>=0; i--)
  {
    shiftOut(dataPin, clockPin, MSBFIRST, values[zeit[i]]);
  }
    digitalWrite(latchPin, LOW);
    delay(100);
    digitalWrite(latchPin, HIGH);
    delay(100);

}

int DigitClock::getHours()
{
  return this->hours;
}

int DigitClock::getMinutes()
{
  return this->minutes;
}

int DigitClock::getSeconds()
{
  return this->seconds;
}

void DigitClock::setHours(int hours)
{
  this->hours = hours;
}

void DigitClock::setMinutes(int minutes)
{
  this->minutes = minutes;
}

void DigitClock::setSeconds(int seconds)
{
  this->seconds = seconds;
}

void DigitClock::setTime(int hours, int minutes, int seconds)
{
    setHours(hours);
    setMinutes(minutes);
    setSeconds(seconds);
}

void DigitClock::renderDots()
{
  for(int i = 0; i< 6; i++)
  {
    shiftOut(dataPin, clockPin, MSBFIRST, values[10]);
  }
  digitalWrite(latchPin, LOW);
  digitalWrite(latchPin, HIGH);
}
