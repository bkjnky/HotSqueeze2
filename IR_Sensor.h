#include "Arduino.h"

/*
************************************************** 
  This is a CLASS used to retrieve the state of the IR Sensor.  
  This sensor uses HIGH/LOW on an analog IN pin to report back
  an interupted state.  The interupted state is set mechanically
  on the injection molding machine.  
  
  Designed specifically to work Arduino Mega 2560 
  ----> https://store-usa.arduino.cc/products/a000067
 
  Written by Nate Schnase for Kerr Marketing, Inc.  
  BSD license, all text above must be included in any redistribution
 ****************************************************
Assumptions
* IR Sensor LOW = Triggered state.  Object sensed at calibrated distance.
* IR Sensor HIGH = Not Triggered State.  Object not sensed at calibrated distance. 


*/



class IRSensor
{
  public:
    IRSensor(int pinSensor);
      void begin();
      bool status();
      
  private:
    int _pin;
    bool _status;
};


//Class Members

    IRSensor::IRSensor(int SensorPin) //This is the constructor
    {
      _pin = SensorPin;
    }
    
    void IRSensor::begin()
    {
      pinMode(_pin, INPUT);
    }

    bool IRSensor::status()
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
    





