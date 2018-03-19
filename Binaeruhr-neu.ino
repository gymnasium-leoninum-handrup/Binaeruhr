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
int sekunden;
Rotary rotary(rotaryKey, rotaryS1, rotaryS2);

const byte values[]=
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

const byte valueBin1[]=
  {0b11111110,
   0b11111100,
   0b11111010,
   0b11111000,
   0b11110110,
   0b11110100,
   0b11110010,
   0b11110000,
   0b11101110,
   0b11101100
  };
  const byte valueBin10[]=
  {0b11111110,
   0b11011110,
   0b10111110,
   0b10011110,
   0b01111110,
   0b01011110
  };

byte data;  

void testDigitBin()
{
  for(int i = 0; i<10; i++)
  {
    data = valueBin1[i]; 
    shiftOut(dataBin, clockBin, MSBFIRST, data);
    shiftOut(dataBin, clockBin, MSBFIRST, data);
    shiftOut(dataBin, clockBin, MSBFIRST, data);
    digitalWrite(latchBin, LOW);
    digitalWrite(latchBin, HIGH);
    delay(500);
  }
   for(int i = 0; i<6; i++)
  {
    data = valueBin10[i]; 
    shiftOut(dataBin, clockBin, MSBFIRST, data);
    shiftOut(dataBin, clockBin, MSBFIRST, data);
    shiftOut(dataBin, clockBin, MSBFIRST, data);
    digitalWrite(latchBin, LOW);
    digitalWrite(latchBin, HIGH);
    delay(500);
  }
}

   
void setTimeEncoder()
{
  int state = 0;
  int hourBig, hourSmall, minuteBig, minuteSmall = 0;

  Serial.println("Setting time...");
  for(int i = 0; i< 6; i++)
  {
  shiftOut(dataPin, clockPin, MSBFIRST, values[10]);
  }
  digitalWrite(latchPin, LOW);
  digitalWrite(latchPin, HIGH);
  

  while(state < 4)
  {
    switch(state)
    {
      case(0):
      {
        if(rotary.getTurn() == LEFT)
        {
          hourBig--;        
        }
        else if(rotary.getTurn() == RIGHT)
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
        if(rotary.getTurn() == LEFT)
        {
          hourSmall--;        
        }
        else if(rotary.getTurn() == RIGHT)
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
         if(rotary.getTurn() == LEFT)
        {
          minuteBig--;        
        }
        else if(rotary.getTurn() == RIGHT)
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
         if(rotary.getTurn() == LEFT)
        {
          minuteSmall--;        
        }
        else if(rotary.getTurn() == RIGHT)
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
    
    if(rotary.buttonPressed())
    {
      state++;
      delay(1000);
    }
    renderTimeDigit(hourBig * 10 + hourSmall, minuteBig * 10 + minuteSmall, 0);
  }
  rtc.setTime(hourBig * 10 + hourSmall, minuteBig * 10 + minuteSmall, 0);
}

void testDigit()
{
  for(int i = 0; i<=9; i++)
  {
    renderTimeDigit(i*10 + i, i*10 + i, i*10 + i);
    delay(1000);
  }
}

void renderTimeDigit(int hours, int minutes, int seconds)
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


void renderTimeBin(int hours, int minutes, int seconds)
{
  byte h10,h1,m10,m1,s10,s1;
 
  h10 = hours/10; // hours big
  h1 = hours%10; // hours small

  m10 = minutes/10; // minutes big
  m1 = minutes%10; // minutes small

  s10 = seconds/10; // seconds big
  s1 = seconds%10; // seconds small

  shiftOut(dataBin,clockBin,MSBFIRST,valueBin10[s10]&valueBin1[s1]);
  shiftOut(dataBin,clockBin,MSBFIRST, valueBin10[m10]&valueBin1[m1]);
  shiftOut(dataBin,clockBin,MSBFIRST,valueBin10[h10]&valueBin1[h1]);
  digitalWrite(latchBin, LOW);
  digitalWrite(latchBin, HIGH);
}



void setup()
{
    Serial.begin(9600);
    
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);

    
    pinMode(latchBin, OUTPUT);
    pinMode(clockBin, OUTPUT);
    pinMode(dataBin, OUTPUT);
    
    pinMode(rotaryKey, INPUT);
    pinMode(rotaryS1, INPUT);
    pinMode(rotaryS2, INPUT);
    digitalWrite(rotaryKey,HIGH);
    rtc.begin();
    testDigit();
    testDigitBin();
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
      renderTimeDigit(t.hour, t.min, t.sec);
      renderTimeBin(t.hour, t.min, t.sec);
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
        setTimeEncoder();
       } 
       else
       {
             testDigit();
             testDigitBin();
        }
     }
}

