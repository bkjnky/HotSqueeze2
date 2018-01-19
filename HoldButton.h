#include "Arduino.h"

/*
************************************************** 
  This is a CLASS used to retrieve the state of the Hold button.  
  This button uses digital read to determine state of the button
  and execute a feed hold on the machine.  

  Part is available https://www.dfrobot.com/product-1098.html
  Designed specifically to work Arduino Mega 2560 
  ----> https://store-usa.arduino.cc/products/a000067
 
  Written by Nate Schnase for Kerr Marketing, Inc.  
  BSD license, all text above must be included in any redistribution
 ****************************************************
Assumptions
* Button not pressed LOW = Not Triggered state.  
* Button Pressed HIGH = Triggered State.  


*/

class HoldBtn
{
  public:
    HoldBtn(int pinHold);
      void begin();
      bool status();
      
  private:
    int _pin;
    bool _status;
};


//Class Members

    HoldBtn::HoldBtn(int HoldPin) //This is the constructor
    {
      _pin = HoldPin;
    }
    
    void HoldBtn::begin()
    {
      pinMode(_pin, INPUT_PULLUP); // sets pin to interupt state
    }

    bool HoldBtn::status()
    {
      if (digitalRead(_pin) == HIGH)
      {
        _status=1;
      }
      else if (digitalRead(_pin) == LOW)
      {
       _status = 0;    
      }

        return _status;

    };
