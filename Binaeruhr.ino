#include "Clock.hpp"

#define latchPin 10
#define clockPin 9
#define dataPin 8

#define dataBin 11
#define clockBin 12
#define latchBin 13

#define RIGHT 1
#define LEFT -1

#define rotaryKey 7
#define rotaryS2 6
#define rotaryS1 5

//‎A4 (SDA), A5 (SCL)

#include <DS3231.h>
#include "Rotary.hpp"

DS3231 rtc(SDA, SCL);
Time t;
Clock clock(dataPin, clockPin, latchPin, dataBin, clockBin, latchBin, 10, 11);
int sekunden;
RotaryEncoder rotary(rotaryS1, rotaryS2, rotaryKey);

void setup()
{
    Serial.begin(9600);
    Serial.println("OK");
    int* result = clock.getESPTime();

    pinMode(latchBin, OUTPUT);
    pinMode(clockBin, OUTPUT);
    pinMode(dataBin, OUTPUT);

    pinMode(rotaryKey, INPUT);
    pinMode(rotaryS1, INPUT);
    pinMode(rotaryS2, INPUT);
    digitalWrite(rotaryKey, HIGH);
    rtc.begin();

    // clock.testClock();
}

void loop()
{
    sekunden = t.sec;

    t = rtc.getTime();

    if (sekunden != t.sec)
    {
        Serial.println("Uhr aktualisieren");
        clock.setTime(t.hour, t.min, t.sec);
        clock.render();
    }

    int taste = rotary.getTaste();
    if (taste == 1)
    {
        Serial.println("kurzer Click");
        rotary.setCount(0);
        clock.testClock();
    }
    if (taste > 1)
    {
        Serial.println("Langer Click");
        clock.setTimeEncoder(&rotary, &rtc);
    }
}
