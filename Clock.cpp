#include "Clock.hpp"
#include "split.hpp"
#include "Configuration.h"

Clock::Clock(int dataPin, int clockPin, int latchPin, int dataPinBin, int clockPinBin, int latchPinBin, int rx, int tx)
  {
    this->digitClock = new DigitClock(dataPin, clockPin, latchPin);
    this->binClock = new BinClock(dataPinBin, clockPinBin, latchPinBin);
    this->esp = new SoftwareSerial(rx, tx);
    this->esp->begin(57600);
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

int* Clock::getESPTime()
{
  int* times = new int[3];
  
  String ESPcmd[4] = 
  {
      "AT+CWMODE=1",
      "AT+CWDHCP_CUR=1,1",
      "AT+CWJAP_CUR=\"" + String(WLAN) +"\",\"" + String(WLAN_PASSWORD) + "\"",
      "AT+CIPSNTPCFG=1,2,\"" + String(NTP_SERVER) + "\""
  };

    this->esp->setTimeout(ESP_TIMEOUT);
    for(int i = 0; i<4; i++)
    {
        this->esp->println(ESPcmd[i]);
        if(esp->find("OK")) 
        {
          Serial.print(ESPcmd[i]);
          Serial.println("Executed successfully");
        }else //PANIC MODE
        {
          Serial.print(ESPcmd[i]);
          Serial.println(" not worked. Please check ESP!");
          return NULL; // Error :(
        }
    }

    delay(50); // esp cooldown

    esp->println("AT+CIPSNTPTIME?");
    String x = esp->readStringUntil("OK");
    Serial.println(x);

    String* result = oe::splitme(oe::splitme(oe::splitme(x, "\r\n")[2], " ")[3], ":");

    Serial.print("Stunden: ");
    Serial.println(result[0]);
    times[0] = result[0].toInt();

    Serial.print("Minuten: ");
    Serial.println(result[1]);
    times[1] = result[1].toInt();

    Serial.print("Sekunden: ");
    Serial.println(result[2]);
    times[2] = result[2].toInt();

    return times;
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

void Clock::setTimeEncoder(RotaryEncoder *rotary, DS3231 *rtc)
{
  int state = 0;
  int hourBig, hourSmall, minuteBig, minuteSmall = 0;

  this->digitClock->renderDots();


  while(state < 4)
  {
    int ergebnis = rotary->poll();
    switch(state)
    {
      case(0):
      {
        if(ergebnis == LEFT)
        {
          hourBig--;
        }
        else if(ergebnis == RIGHT)
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
        if(ergebnis == LEFT)
        {
          hourSmall--;
        }
        else if(ergebnis == RIGHT)
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
         if(ergebnis == LEFT)
        {
          minuteBig--;
        }
        else if(ergebnis == RIGHT)
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
         if(ergebnis == LEFT)
        {
          minuteSmall--;
        }
        else if(ergebnis == RIGHT)
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
      if(rotary->getTaste() == 1)
      {
        rotary->setCount(0);
        state++;
        delay(100);
      }
      setTime(hourBig * 10 + hourSmall, minuteBig * 10 + minuteSmall, 0);
      render();
    }
      rtc->setTime(hourBig * 10 + hourSmall, minuteBig * 10 + minuteSmall, 0);
      setTime(hourBig * 10 + hourSmall, minuteBig * 10 + minuteSmall, 0);
  }
