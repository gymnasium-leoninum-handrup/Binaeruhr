#include "Clock.hpp"

Clock::Clock(int dataPin, int clockPin, int latchPin, int dataPinBin, int clockPinBin, int latchPinBin)
  {
    this->digitClock = new DigitClock(dataPin, clockPin, latchPin);
    this->binClock = new BinClock(dataPinBin, clockPinBin, latchPinBin);
  }

Clock::~Clock()
{
  delete this->digitClock;
  delete this->binClock;
}

void Clock::setTime(int hours, int minutes, int seconds)
{
  this->hours = hours;
  this->minutes = minutes;
  this->seconds = seconds;
}

void Clock::render()
{
  this->digitClock->setTime(this->hours, this->minutes, this->seconds);
  //this->binClock->setTime(this->hours, this->minutes, this->seconds);

  this->digitClock->renderTimeDigit(this->hours, this->minutes, this->seconds);
  this->binClock->renderTimeBin(this->hours, this->minutes, this->seconds);
}

void Clock::testClock()
{
  this->digitClock->testDigit();
  this->binClock->testDigitBin();
}

void Clock::setTimeEncoder(Rotary *rotary, DS3231 *rtc)
{
  int state = 0;
  int hourBig, hourSmall, minuteBig, minuteSmall = 0;

  Serial.println("Setting time...");
  this->digitClock->renderDots();


  while(state < 4)
  {
    switch(state)
    {
      case(0):
      {
        if(rotary->getTurn() == LEFT)
        {
          hourBig--;
        }
        else if(rotary->getTurn() == RIGHT)
        {
          hourBig++;
        }
        if(hourBig>2)
          hourBig = 0;

        if(hourBig < 0)
          hourBig = 2;
        break;
      }
      case(1):
      {
        if(rotary->getTurn() == LEFT)
        {
          hourSmall--;
        }
        else if(rotary->getTurn() == RIGHT)
        {
          hourSmall++;
        }

        if(hourBig == 2 && hourSmall> 3)
          hourSmall = 0;

        else if(hourSmall > 9)
          hourSmall = 0;

        else if(hourSmall < 0)
          hourSmall = 9;

        break;
      }
      case(2):
      {
         if(rotary->getTurn() == LEFT)
        {
          minuteBig--;
        }
        else if(rotary->getTurn() == RIGHT)
        {
          minuteBig++;
        }

        if(minuteBig > 5)
          minuteBig = 0;

        if(minuteBig < 0)
          minuteBig = 5;

        break;
      }
      case(3):
      {
         if(rotary->getTurn() == LEFT)
        {
          minuteSmall--;
        }
        else if(rotary->getTurn() == RIGHT)
        {
          minuteSmall++;
        }

        if(minuteSmall > 9)
          minuteSmall = 0;

        if(minuteSmall < 0)
          minuteSmall = 9;
        break;
      }
    }
      if(rotary->buttonPressed())
      {
        state++;
        delay(1000);
      }
      setTime(hourBig * 10 + hourSmall, minuteBig * 10 + minuteSmall, 0);
      render();
    }
      rtc->setTime(hourBig * 10 + hourSmall, minuteBig * 10 + minuteSmall, 0);
      setTime(hourBig * 10 + hourSmall, minuteBig * 10 + minuteSmall, 0);
  }
