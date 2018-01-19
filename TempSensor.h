#include <DFRobotHighTemperatureSensor.h>
#include "Arduino.h"

/*
************************************************** 
  This is a class for getting temperature readings 
  from the  Gravity: Analog High Temperature Sensor
  Designed specifically to work Arduino Mega 2560 
  ----> https://www.dfrobot.com/index.php?route=product/product&product_id=1474
  ----> https://store-usa.arduino.cc/products/a000067
  These sensors enable the user to read temperatures of up to 350C

INCLUDES USE OF:
**************************************************
 DFRobotHighTemperatureSensor.cpp - DFRobotHighTemperatureSensor library
  Developed by Bernie - 2016/1/13

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

HighTemperature Sensor info found at
http://www.dfrobot.com/

Version 1.0: 13 Jan 2016 by bernie
- Updated to build against Arduino 1.0.6
- Made accessors inline in the header so they can be optimized away
****************************************************
  
  Written by Nate Schnase for Kerr Marketing, Inc.  
  BSD license, all text above must be included in any redistribution
 ****************************************************
#define pinTempA         A1
#define pinTempB         A2
#define pinTempC         A3
*/





class ThermoCouple
{
  public:
    ThermoCouple(int pinTC, float volts, bool units);
    void begin();
    int ReturnTemp();
 
  private:
    bool _units;
    int _pin;
    int _temp;
    float _voltageRef;
};

//Class Members

ThermoCouple::ThermoCouple(int pinTC, float volts, bool units) //This is the constructor
{
  _pin = pinTC;
  _voltageRef = volts;
}

void ThermoCouple::begin()
{
  pinMode(_pin, INPUT);
 }


int ThermoCouple::ReturnTemp()
{
  DFRobotHighTemperature PT100 = DFRobotHighTemperature(_voltageRef); //Define an PT100 object
      if (_units = true)
      {
      _temp = PT100.readTemperature(_pin);
      }
      else
      {
      _temp = PT100.readTemperature(_pin);
      _temp = ((_temp*(9/5))+32);               //Convert C value to F
      }

         return _temp;
};






