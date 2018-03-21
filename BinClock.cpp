#include "BinClock.hpp"

BinClock::BinClock(int dataPin, int clockPin, int latchPin): dataPin(dataPin), clockPin(clockPin), latchPin(latchPin)
{
  pinMode(this->clockPin, OUTPUT);
  pinMode(this->dataPin, OUTPUT);
  pinMode(this->latchPin, OUTPUT);
}

void BinClock::testDigitBin()
{
  byte data;
  for(int i = 0; i<10; i++)
  {
    data = valueBin1[i];
    shiftOut(dataPin, clockPin, MSBFIRST, data);
    shiftOut(dataPin, clockPin, MSBFIRST, data);
    shiftOut(dataPin, clockPin, MSBFIRST, data);
    digitalWrite(latchPin, LOW);
    digitalWrite(latchPin, HIGH);
    delay(500);
  }
   for(int i = 0; i<6; i++)
  {
    data = valueBin10[i];
    shiftOut(dataPin, clockPin, MSBFIRST, data);
    shiftOut(dataPin, clockPin, MSBFIRST, data);
    shiftOut(dataPin, clockPin, MSBFIRST, data);
    digitalWrite(latchPin, LOW);
    digitalWrite(latchPin, HIGH);
    delay(500);
  }
}

void BinClock::renderTimeBin(int hours, int minutes, int seconds)
{
  byte h10,h1,m10,m1,s10,s1;

  h10 = hours/10; // hours big
  h1 = hours%10; // hours small

  m10 = minutes/10; // minutes big
  m1 = minutes%10; // minutes small

  s10 = seconds/10; // seconds big
  s1 = seconds%10; // seconds small

  shiftOut(dataPin,clockPin,MSBFIRST,valueBin10[s10]&valueBin1[s1]);
  shiftOut(dataPin,clockPin,MSBFIRST, valueBin10[m10]&valueBin1[m1]);
  shiftOut(dataPin,clockPin,MSBFIRST,valueBin10[h10]&valueBin1[h1]);
  digitalWrite(latchPin, LOW);
  digitalWrite(latchPin, HIGH);
}
