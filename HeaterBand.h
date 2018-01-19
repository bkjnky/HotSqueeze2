/*
************************************************** 
  This is a set of functions used to control duty cycle 120v AC power to
  three heater bands via relays controlled by 3 digital pins.  
  
  Designed specifically to work Arduino Mega 2560 
  ----> https://store-usa.arduino.cc/products/a000067
 
  Written by Nate Schnase for Kerr Marketing, Inc.  
  BSD license, all text above must be included in any redistribution
 ****************************************************

#define pinHeatA         32
#define pinHeatB         34
#define pinHeatC         36

*/

class Heater
//Class Members

{
public:
    Heater(int pinHeater);  //pin for heater band
    void begin();           //initialize band
    void heaterOn();        //heater band on 
    void heaterOff();       //heater band off
    bool status();
 
  private:
    int _pin;
    bool _status;

};



Heater::Heater(int pinHeater) //This is the constructor
{
  _pin = pinHeater;
}


    void Heater::begin()
    {
      pinMode(_pin, OUTPUT);
    }
    
    void Heater::heaterOn()
    {
      digitalWrite(_pin, HIGH);
      _status = 0;
    }
    
    void Heater::heaterOff()
    {
      digitalWrite(_pin, LOW);
      _status = 1;
    }

    bool Heater::status()
    {
      return _status;
    };




    

