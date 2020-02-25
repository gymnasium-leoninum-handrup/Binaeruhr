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

void serialFlush() {
  while (Serial.available() > 0) {
    char t = Serial.read();
  }
}

int* Clock::getESPTime(func fun = NULL)
{
  int* times = new int[3];

  String ESPcmd[4] =
  {
    "AT+CWMODE=1",
    "AT+CWDHCP_CUR=1,1",
    "AT+CWJAP_CUR=\"" + String(WLAN) + "\",\"" + String(WLAN_PASSWORD) + "\"",
    "AT+CIPSNTPCFG=1,1,\"" + String(NTP_SERVER) + "\""
  };

  this->esp->setTimeout(ESP_TIMEOUT);
  this->render(true);
  for (int i = 0; i < 4; i++)
  {
    fun();
    delay(150);
    this->esp->println(ESPcmd[i]);
    if (esp->find("OK"))
    {
      Serial.print(ESPcmd[i]);
      Serial.println("Executed successfully");
    } else //PANIC MODE
    {
      Serial.print(ESPcmd[i]);
      Serial.println(" not worked. Please check ESP!");
      return NULL; // Error :(
    }
  }

  delay(500); // esp cooldown

  serialFlush();
  esp->println("AT+CIPSNTPTIME?");
  while (!esp->available()) {
    fun();
  }
  esp->readStringUntil('\n');
  esp->readStringUntil('\n');
  String zweiteZeile = esp->readStringUntil("\n");
  Serial.println(zweiteZeile);
  String yearString = zweiteZeile.substring(33, 37);
  Serial.print("Jahr: ");
  Serial.println(yearString);
  String uhrZeitString = zweiteZeile.substring(24, 32);
  Serial.println(uhrZeitString);
  // String* result = oe::splitme(uhrZeitString, ":");
  serialFlush();

  if (yearString.toInt() != 1970) {
    Serial.print("Stunden: ");
    //Serial.println(result[0]);
    Serial.println(uhrZeitString.substring(0, 2));
    times[0] = uhrZeitString.substring(0, 2).toInt();

    Serial.print("Minuten: ");
    // Serial.println(result[1]);
    Serial.println(uhrZeitString.substring(3, 5));
    times[1] = uhrZeitString.substring(3, 5).toInt();

    Serial.print("Sekunden: ");
    Serial.println(uhrZeitString.substring(6, 8));
    times[2] = uhrZeitString.substring(6, 8).toInt();
    this->render(false);
  } else {
    return NULL;
  }

  return times;
}

void Clock::render(bool dots = false)
{
  this->digitClock->setTime(this->hours, this->minutes, this->seconds);
  //this->binClock->setTime(this->hours, this->minutes, this->seconds);

  this->digitClock->renderTimeDigit(this->hours, this->minutes, this->seconds, dots);
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


  while (state < 4)
  {
    int ergebnis = rotary->poll();
    switch (state)
    {
      case (0):
        {
          if (ergebnis == LEFT)
          {
            hourBig--;
          }
          else if (ergebnis == RIGHT)
          {
            hourBig++;
          }
          if (hourBig > 2)
            hourBig = 0;

          if (hourBig < 0)
            hourBig = 2;
          break;
        }
      case (1):
        {
          if (ergebnis == LEFT)
          {
            hourSmall--;
          }
          else if (ergebnis == RIGHT)
          {
            hourSmall++;
          }

          if (hourBig == 2 && hourSmall > 3)
            hourSmall = 0;

          else if (hourSmall > 9)
            hourSmall = 0;

          else if (hourSmall < 0)
            hourSmall = 9;

          break;
        }
      case (2):
        {
          if (ergebnis == LEFT)
          {
            minuteBig--;
          }
          else if (ergebnis == RIGHT)
          {
            minuteBig++;
          }

          if (minuteBig > 5)
            minuteBig = 0;

          if (minuteBig < 0)
            minuteBig = 5;

          break;
        }
      case (3):
        {
          if (ergebnis == LEFT)
          {
            minuteSmall--;
          }
          else if (ergebnis == RIGHT)
          {
            minuteSmall++;
          }

          if (minuteSmall > 9)
            minuteSmall = 0;

          if (minuteSmall < 0)
            minuteSmall = 9;
          break;
        }
    }
    if (rotary->getTaste() == 1)
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
