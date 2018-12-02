#include "Clock.hpp"
#include "Configuration.h"

//‎A4 (SDA), A5 (SCL)

#include <DS3231.h> // real time clock lib
#include "Rotary.hpp"

DS3231 rtc(SDA, SCL);
Time t;
int seconds;
Clock clock(DATA_PIN, CLOCK_PIN, LATCH_PIN, DATA_BIN, CLOCK_BIN, LATCH_BIN, 10, 11);
RotaryEncoder rotary(ROTARY_S1, ROTARY_S2, ROTARY_KEY);

void setup()
{
    Serial.begin(9600);
    Serial.println("Serielle Verbindung hergestellt. Guten Tag!");
    Serial.println("Initialisiere RTC...");
    rtc.begin(); // init rtc connection
    Serial.println("Versuche, Zeit vom Schulserver über ESP8266 zu lesen...");
    int* result = clock.getESPTime();
    if(result == NULL)
    {
      Serial.println("Konnte Uhrzeit übers Schulnetz nicht lesen! Bitte ESP und/oder Schulserver prüfen!");
    }else 
    {
      Serial.println("OK");
      Serial.println("Setze RTC");
      rtc.setTime(result[0], result[1], result[2]);
    }

    Serial.println("Beginne mit Selbsttest... Bitte warten");
    clock.testClock();
    Serial.println("Selbsttest abgeschlossen");
}

void loop()
{
    seconds = t.sec;

    t = rtc.getTime();

    if (seconds != t.sec)
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
