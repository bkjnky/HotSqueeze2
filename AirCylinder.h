#include "Arduino.h"

/*
************************************************** 
  This is a CLASS used to control pneumatic ram postions as well as to 
  get the current state of one or more pins used to control relays that apply or
  cut off 120v AC power to solenoids.
  
  Designed specifically to work Arduino Mega 2560 
  ----> https://store-usa.arduino.cc/products/a000067
 
  Written by Nate Schnase for Kerr Marketing, Inc.  
  BSD license, all text above must be included in any redistribution
 ****************************************************

Assumptions
* both relays OPEN = let air out
* both relays CLOSED = error
* Open mold = AV1 is OPEN, AV2 is CLOSED
* Close Mold = AV1 is CLOSED, AV2 is OPEN
* IR Sensor Low = trigger stop Closing mold
*IR Sensor HIGH = Trigger Stop Opening


*/





class AirCylinder
{
  public:
    AirCylinder(int pinCylinderDirection);
    void begin();
    void PsiUp();
    void PsiDown();
    bool status();
  private:
    int _pin;
    bool _status;
};

//Class Members

AirCylinder::AirCylinder(int pinCylinderDirection) //This is the constructor
{
  _pin = pinCylinderDirection;
}


void AirCylinder::begin()
{
  pinMode(_pin, OUTPUT);
}

void AirCylinder::PsiUp()
{
  digitalWrite(_pin, HIGH);
  _status = 1;
}

void AirCylinder::PsiDown()
{
  digitalWrite(_pin, LOW);
  _status = 0;
}

bool AirCylinder::status()
{
  return _status;
}

//int Get_Solenoid_State(int pinExtendCyclinder, int pinRetractCyclinder = NULL) //Currently assuming that to open the Solenoid, one relay must be closed allowing 120vac to trigger either the extend or retract air valve to open.
//{
//  //int currentState = 0;
//  //if ((digitalRead(pinMoldOpen) == relayOn) && (digitalRead(pinMoldClose) == relayOn) && (digitalRead(pinIRSense == HIGH)))  //NOT SURE IF THIS IS NEEDED ASSUMING NO
//  //if ((digitalRead(pinMoldOpen) == relayOn) && (digitalRead(pinMoldClose) == relayOn))
//  if (pinRetractCyclinder != NULL)
//  {
//    if ((digitalRead(pinExtendCyclinder) == relayOn) && (digitalRead(pinRetractCyclinder) == relayOn)) //if both pins high - both 120v relays are closed sending 120v through both
//    {
//      currentState = Error_State;
//    }
//  
//    //else if ((digitalRead(pinMoldOpen) == relayOff) && (digitalRead(pinMoldClose) == relayOff ))
//    else if ((digitalRead(pinExtendCyclinder) == relayOff) && (digitalRead(pinRetractCyclinder) == relayOff )) //assuming that both relays open would allow the Solenoid to center, releasing air from both the extend and retract air circut
//    {
//      currentState = Release_Air;
//    }
//  
//    //else if ((digitalRead(pinMoldOpen) == relayOff) && (digitalRead(pinMoldClose) == relayOn ))
//    else if ((digitalRead(pinExtendCyclinder) == relayOff) && (digitalRead(pinRetractCyclinder) == relayOn )) // Assuming Closed relay sends 120vac to Solenoid causing extend air piston circut to open
//    {
//      currentState = Retract_RAM;
//    }
//  
//    //else if ((digitalRead(pinMoldOpen)) == relayOn && (digitalRead(pinMoldClose) == relayOff ))
//    else if ((digitalRead(pinExtendCyclinder)) == relayOn && (digitalRead(pinRetractCyclinder) == relayOff )) // Assuming Closed relay sends 120vac to Solenoid causing Retract Air piston circut to open
//    {
//      currentState = Extend_RAM;
//    }
//  
//    return currentState;
//  }
//    else
//    {
//     if ((digitalRead(pinExtendCyclinder) == relayOff))
//     {
//      currentState = Release_Air;
//     }
//     else if ((digitalRead(pinExtendCyclinder) == relayOn))
//     {
//      currentState = Extend_RAM;
//     }
//      return currentState;
//    }
//}
//
//
////****************************************************** SET MOLD CYCLINDER STATE ************************************************
//
///*
// * this function opens and closes the mold pistons
// * there is a delay set post close
// * 
// */
//
//void Set_Mold_State(int SetMoldPosition)    //Setting up now to theoretically run until the sensor is low or high
//{
//  if ((SetMoldPosition != Get_Solenoid_State(pinMoldOpen, pinMoldClose)) && (Get_Solenoid_State(pinMoldOpen, pinMoldClose) != Error_State))  // make sure that your not trying to double open or double close the mold
//  {
//    if (SetMoldPosition == Retract_RAM)           //Fires RAM to Open the mold
//    {
//      while (digitalRead(pinIRSense == HIGH))     
//      {
//       digitalWrite(pinMoldOpen, relayOff);       
//       digitalWrite(pinMoldClose, relayOn);
//      }
//       delay(500);
//       digitalWrite(pinMoldOpen, relayOff);       //Cut Power to both Extend and Retract Theoretically this releases the air
//       digitalWrite(pinMoldClose, relayOff);
//        
//    }
//    else if (SetMoldPosition == Extend_RAM)       //Fires RAM to Close the mold
//    {
//      while(digitalRead(pinIRSense == LOW))
//      {
//        digitalWrite(pinMoldOpen, relayOn);       
//        digitalWrite(pinMoldClose, relayOff);
//      }
//       delay(500);
//       digitalWrite(pinMoldOpen, relayOff);       //Cut Power to both Extend and Retract Theoretically this releases the air
//       digitalWrite(pinMoldClose, relayOff);
//    }
//    else if (SetMoldPosition == Release_Air)      //Let the air go, Manually manipulate the mould state by hand
//       digitalWrite(pinMoldOpen, relayOff);       
//       digitalWrite(pinMoldClose, relayOff);
//       delay(3000);                              // This sets the delay will need to adjust code to accept variable
//    
//   }
//}
//
////****************************************************** SET EJECTOR PIN STATE ************************************************
//
///*
// * This function fires the ejector pin and then retracts it after a set delay
// * may want to implement a variable delay
// */
//
//void Set_Ejector_State(int SetEjectorPosition)
//{
//  if (SetEjectorPosition != (Get_Solenoid_State(pinEjectUp, pinEjectDown)) && (Get_Solenoid_State(pinEjectUp, pinEjectDown) != Error_State)) // make sure that your not trying to double extend or double retract the ejector pin
//   { 
//    if (SetEjectorPosition == Retract_RAM)        // fires Ejector pin Retract
//    {
//      digitalWrite(pinEjectUp, relayOn);
//      digitalWrite(pinEjectDown, relayOff);
//    }
//    else if (SetEjectorPosition == Extend_RAM)    // fires Ejector pin Extend
//    {
//      digitalWrite(pinEjectUp, relayOff);
//      digitalWrite(pinEjectDown, relayOn);
//      delay(1000);                                //this sets the delay.  May need to implement variable delay setting here                     
//      digitalWrite(pinEjectUp, relayOn);
//      digitalWrite(pinEjectDown, relayOff);
//    }
//   }
//}
//
//
////****************************************************** SET INJECTION RAM PIN STATE ************************************************
///*
// * this function extends/retracts the injection ram
// * we are missing a retract pin and relay  I made this up but this will not retract the piston
// */
//
//void Set_Injector_Piston_State(int SetInjectionPistonPosition)
//{
//  if (SetInjectionPistonPosition != (Get_Solenoid_State(pinPistonInject)) && (Get_Solenoid_State(pinPistonInject) != Error_State))  // make sure that your not trying to double push or double pull the injector piston
//  {
//    if (SetInjectionPistonPosition == Retract_RAM)        // fires Injection RAM Retract
//    {
//      digitalWrite(pinPistonInject, relayOff);
//    }
//    else if (SetInjectionPistonPosition == Extend_RAM)    // fires Injection RAM Extend
//    {
//      digitalWrite(pinPistonInject, relayOn);
//      delay(1000);                                        //this sets the delay.  May need to implement variable delay setting here    
//      digitalWrite(pinPistonInject, relayOff);
//    }
//  }
//}



