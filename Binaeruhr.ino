#define latchPin 10
#define clockPin 9
#define dataPin 8
#define RIGHT true
#define LEFT false

#define rotaryKey 7
#define rotaryS2 6
#define rotaryS1 5

#include <DS3231.h>

DS3231 rtc(SDA, SCL);
Time t;
int sekunden;

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

bool getEncoderDirection()
{
  
}

void setTimeEncoder()
{
  int state = 0;
  int hour, minute, second;

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
        
        
        break;
      }
      case(1):
      {
        break;
      }
      case(2):
      {
        break;
      }
      case(3):
      {
        break;
      }
    }
  }
  rtc.setTime(hour, minute, second);
}

void renderTimeDigit(int hours, int minutes, int seconds)
{
  byte zeit[6];

  zeit[1] = hours/10;
  zeit[0] = hours%10;

  zeit[3] = minutes/10;
  zeit[2] = minutes%10;

  zeit[5] = seconds/10;
  zeit[4] = seconds%10;

  for(int i = 0; i<6; i++)
  {
    shiftOut(dataPin, clockPin, MSBFIRST, values[zeit[i]]);
  }
    digitalWrite(latchPin, LOW);
    digitalWrite(latchPin, HIGH);
  
}

void setup()
{
    Serial.begin(9600);
  
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(rotaryKey, INPUT);
    pinMode(rotaryS1, INPUT);
    pinMode(rotaryS2, INPUT);
    rtc.begin();
}

void loop()
{
    /*shiftOut(dataPin, clockPin, MSBFIRST, values[i]);
    Serial.print("i: ");
    Serial.print(i);
    Serial.print(" ");
    Serial.println(values[i], BIN);
    digitalWrite(latchPin, LOW);
    digitalWrite(latchPin, HIGH);*/

     sekunden = t.sec;

    t = rtc.getTime();
    Serial.println(t.hour);
    Serial.println(t.min);
    Serial.println(t.sec);

    if(sekunden != t.sec)
    {
    renderTimeDigit(t.hour, t.min, t.sec);
    }

    if(!digitalRead(rotaryKey))
    {
      setTimeEncoder();
    }
}

