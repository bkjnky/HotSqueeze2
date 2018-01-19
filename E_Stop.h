#include "Arduino.h"

/*
************************************************** 
  This is a CLASS used to define and set the status of the E Stop Light.  
  This sensor uses HIGH/LOW on an analog IN pin to report back
  an interupted state.  
  
  Designed specifically to work Arduino Mega 2560 
  ----> https://store-usa.arduino.cc/products/a000067
 
  Written by Nate Schnase for Kerr Marketing, Inc.  
  BSD license, all text above must be included in any redistribution
 ****************************************************
Assumptions


*/



class EStop
{
  public:
    EStop(int pinEStopLight);
      void begin();
      void eStopOn();
      void eStopOff();
      bool status();
      
  private:
    int _pin;
    bool _status;
};


//Class Members

    EStop::EStop(int pinEStopLight) //This is the constructor
    {
      _pin = pinEStopLight;
    }
    
    void EStop::begin()
    {
      pinMode(_pin, OUTPUT);
    }

void EStop::eStopOn()
    {
      digitalWrite(_pin, HIGH);
      _status = 0;
    }
    
    void EStop::eStopOff()
    {
      digitalWrite(_pin, LOW);
      _status = 1;
    }

    bool EStop::status()
    {
      return _status;
    };

 
