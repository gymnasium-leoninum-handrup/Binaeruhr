#ifndef ROTARY_HPP
#define ROTARY_HPP

#define RIGHT 1
#define LEFT 2
#define NONE 0

class Rotary
{
private:
  int aLastState = 0;
  int aState = 0;
  int keyButton;
  int key1;
  int key2;

public:
  Rotary(int keyButton, int key1, int key2)
  {
    this->keyButton = keyButton;
    this->key1 = key1;
    this->key2 = key2;

    pinMode(keyButton, INPUT);
    pinMode(key1, INPUT);
    pinMode(key2, INPUT);

    digitalWrite(keyButton, HIGH);
    digitalWrite(key1, HIGH);
    digitalWrite(key2, HIGH);

    aLastState = digitalRead(this->key1);
  }

  bool buttonPressed()
  {
    if (digitalRead(keyButton) == LOW)
      return true;
    else
      return false;
  }
  int getTurn()
  {
    int result = 0;
    aState = digitalRead(key1); // Reads the "current" state of the outputA
    // If the previous and the current state of the outputA are different, that
    // means a Pulse has occured
    if (aState != aLastState && aState == LOW) {
      // If the outputB state is different to the outputA state, that means the
      // encoder is rotating clockwise
      if (digitalRead(key2) != aState) {
        result = 1;
      } else {
        result = 2;
      }
    } else {
      result = 0;
    }
    aLastState = aState; // Updates the previous state of the outputA with the
                         // current state

    return result;
  }
};

#endif
