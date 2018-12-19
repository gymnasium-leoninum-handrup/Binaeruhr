#include "Clock.hpp"
#include "Configuration.h"

//‎A4 (SDA), A5 (SCL)

#include <DS3231.h> // real time clock lib
#include "Rotary.hpp"

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

DS3231 rtc(SDA, SCL);
Time t;
int seconds;
int syncSeconds = 0;
bool synced = false;
//Clock clock(DATA_PIN, CLOCK_PIN, LATCH_PIN, DATA_BIN, CLOCK_BIN, LATCH_BIN, 10, 11);
Clock clock(dataPin, clockPin, latchPin, dataBin, clockBin, latchBin, 2, 3);
RotaryEncoder rotary(ROTARY_S1, ROTARY_S2, ROTARY_KEY);

#define DIMENSION 3
int greenTime[DIMENSION][4] = {
  {12, 12, 12, 13},
  {13,3, 13,28 },
  {13, 32, 13, 33}
};

void setup()
{
    pinMode(4, OUTPUT);
    Serial.begin(9600);
    Serial.println("Serielle Verbindung hergestellt. Guten Tag!");
    Serial.println("Initialisiere RTC...");
    rtc.begin(); // init rtc connection
    Serial.println("Beginne mit Selbsttest... Bitte warten");
    clock.testClock();
    Serial.println("Selbsttest abgeschlossen");
}

void loop()
{

    if(syncSeconds == 0) 
    {
      Serial.println("Versuche, Zeit vom Schulserver über ESP8266 zu lesen...");
      int* result = clock.getESPTime();
      Serial.print(result[0]);
            Serial.print(result[1]);
                  Serial.println(result[2]);
      if(result == NULL)
      {
        Serial.println("Konnte Uhrzeit übers Schulnetz nicht lesen! Bitte ESP und/oder Schulserver prüfen!");
        syncSeconds = 600;
        synced = false;
      }else 
      {
      //  Serial.println("OK");
        //Serial.println("Setze RTC");
        rtc.setTime(result[0], result[1], result[2]);
        syncSeconds = 21600;
        synced = true;
      }      
    }

    
    
    seconds = t.sec;

    t = rtc.getTime();

    if (seconds != t.sec)
    {
        Serial.println("Uhr aktualisieren");
        clock.setTime(t.hour, t.min, t.sec);
        clock.render(!synced);
        syncSeconds--;

        for(int i = 0; i<DIMENSION; i++)
        {
          //Serial.println(greenTimes[0] >= t.hour);
          if((t.hour * 60 + t.min) >= (greenTime[i][0] * 60 + greenTime[i][1]) && (t.hour * 60 + t.min) <= (greenTime[i][2] * 60 + greenTime[i][3]))
          {
            Serial.println("Es ist Pause ;)");
             digitalWrite(4, HIGH);
             break;
          }else {
                digitalWrite(4, LOW);
          }
        }
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
