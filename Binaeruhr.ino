#include "Clock.hpp"

#define latchPin 10
#define clockPin 9
#define dataPin 8

#define dataBin 11
#define clockBin 12
#define latchBin 13

#define RIGHT true
#define LEFT false

#define rotaryKey 7
#define rotaryS2 6
#define rotaryS1 5

//‎A4 (SDA), A5 (SCL)

#include <DS3231.h>
#include "Rotary.hpp"

DS3231 rtc(SDA, SCL);
Time t;
Clock clock(dataPin, clockPin, latchPin, dataBin, clockBin, latchBin);
int sekunden;
Rotary rotary(rotaryKey, rotaryS1, rotaryS2);

void setup()
{
    Serial.begin(9600);


    pinMode(latchBin, OUTPUT);
    pinMode(clockBin, OUTPUT);
    pinMode(dataBin, OUTPUT);

    pinMode(rotaryKey, INPUT);
    pinMode(rotaryS1, INPUT);
    pinMode(rotaryS2, INPUT);
    digitalWrite(rotaryKey,HIGH);
    rtc.begin();
    clock.testClock();
}

void loop()
{
   /* shiftOut(dataPin, clockPin, MSBFIRST, values[i]);
    Serial.print("i: ");
    Serial.print(i);
    Serial.print(" ");
    Serial.println(values[i], BIN);
    digitalWrite(latchPin, LOW);
    digitalWrite(latchPin, HIGH);*/

    sekunden = t.sec;

    t = rtc.getTime();
    /*Serial.print(t.hour);
    Serial.print(":");
    Serial.print(t.min);
    Serial.print(":");
    Serial.print(t.sec);
    Serial.println();*/

    if(sekunden != t.sec )
    {
      Serial.println("Uhr setzen");
      clock.setTime(t.hour, t.min, t.sec);
      clock.render();
    }

   /* if(rotary.buttonPressed())
    {
      //Serial.println("Testdigit");
      testDigit();

      setTimeEncoder();
    }*/

     if(rotary.buttonPressed())
     {
       int keyT = rotary.keyTime();
       if(keyT> 0)
       Serial.println("longkey");
       if(keyT >= 2000)
       {
        clock.setTimeEncoder(&rotary, &rtc);
       }
       else
       {
             clock.testClock();
        }
     }
}
