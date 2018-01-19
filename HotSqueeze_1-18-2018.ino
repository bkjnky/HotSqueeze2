/*
**************************************************
  This is the main program loop for receiving commands from the TFT Display.
  Designed specifically to work Arduino Mega 2560
  ----> https://store-usa.arduino.cc/products/a000067

  Main Functions include:

***************************************************************************************************
  Class AirCylinder:
    AirCylinder(int pinCylinderDirection)               Names/defines an air cylinder by it's direction
    begin()                                             Initialize the air cyclinder
    moveAirPiston()                                     Siginals the selected air cylinder to apply air pressure and extend or retract (depending on its directional definition
    releaseAir()                                        Signals the selected air cylinder to release air pressure
    status()                                            Gets the status of the selected air cylinder 1= pressure is on, 0 = pressure bleed

***************************************************************************************************
  Class IRSensor:
  IRSensor(int pinSensor)                                Define DigitalRead pin for IR sensor
      void begin()                                      Initialize the IR Sensor
      bool status()                                     Read IR Sensor

***************************************************************************************************
  Class Heater:
    Heater(int pinHeater)                               Names/defines a Heater Band.  There are 3
    void begin()                                        Initialize the heater band
    void heaterOn()                                     Siginals selected heater to turn on
    void heaterOff()                                    Signals selected heater to turn off
    bool status()                                       returns the status On:Off of the selected heater

***************************************************************************************************

  Class: ThermoCouple:
    ThermoCouple(int pinTC, float volts, bool units)     Names/defines a ThermoCouple.  There are 3
    void begin()                                         Initalizes ThermoCouple.  There are 3
    int ReturnTemp()                                     Retruns temp in uinits spedified


  Written by Nate Schnase for Kerr Marketing, Inc.
  BSD license, all text above must be included in any redistribution
****************************************************
*/

#include "Nextion.h"
//#include "SoftwareSerial.h"
//#include "NextionButton.h"
//#include "NextionText.h"

#include "NexButton.h"
#include "NexText.h"
#include "NexNumber.h"


#include <DFRobotHighTemperatureSensor.h>
#include "Pin.h"
#include "AirCylinder.h"
#include "IR_Sensor.h"
#include "TempSensor.h"
#include "HeaterBand.h"
#include "HoldButton.h"
#include "E_Stop.h"



const bool relayOn  = HIGH;
const bool relayOff = LOW;
const int Extend_RAM = 1;
const int Retract_RAM = -1;
const int Release_Air = 0;
const int Error_State = 2;
const float TC_VoltRef = 5.000;                         //Defines the reference voltage for the ThermoCouple
const unsigned long heaterInterval = 1000;              //Defines time cycle to check heater temperature and update if nessessary

bool temp_unit = true;                                  //  True = C; False = F [sets units of measure for ThermoCouples]
int heat_temp = 0;
int temp_Set[3] = { 10, 250, 250 };                      //Defines desired temp for each heaterband as measured by ThermoCouple 0,1,2

String heaterLbl = "";
String sensorStatus = "";

volatile byte state = LOW;
int countPress = 0;

unsigned long heaterTimer;

//SoftwareSerial nextionSerial(17, 16); // RX, TX
//
//Nextion nex(nextionSerial);


//***********************************************
//***********8  Setup Air Cylinders  ************
//***********************************************

AirCylinder OpenMold(pinMoldOpen);                  //Defines the Mold Open Pin           [Shares same physical cylinder as CloseMold]
AirCylinder CloseMold(pinMoldClose);                //Defines the Mold Close Pin
AirCylinder EjectOn(pinEjectOn);                    //Defines the Extended Ejector Pin    [Shares same physical cyclinder as EjectOff]
AirCylinder EjectOff(pinEjectOff);                  //Defines the Retracted Ejector Pin
AirCylinder Inject(pinInjector);                    //Defines the Injection Pin           [Not a dual action cylinder]

//***********************************************
//*************  Setup IR Sensor  ***************
//***********************************************

//Define IR Sensor
IRSensor Sensor1(pinIRSense);                        //Defines the IR sensor Analog input pin

//***********************************************
//***********  Setup Thermo Couples  ************
//***********************************************

//Define ThermoCouples 1 thru 3


ThermoCouple TC0(pinTempA, TC_VoltRef, temp_unit);   //Defines the ThermoCouple Analog input pin
ThermoCouple TC1(pinTempB, TC_VoltRef, temp_unit);   //Defines the ThermoCouple Analog input pin
ThermoCouple TC2(pinTempC, TC_VoltRef, temp_unit);   //Defines the ThermoCouple Analog input pin
ThermoCouple TC[3] = { TC0, TC1, TC2 };              //Defines an Array of ThermoCouples
//********************************************
//*************  Setup Heaters  **************
//********************************************

//Define Heat Bands 1 thru 3

Heater HB0(pinHeatA);                               //Defines the Heater Band Analog input pin
Heater HB1(pinHeatB);                               //Defines the Heater Band Analog input pin
Heater HB2(pinHeatC);                               //Defines the Heater Band Analog input pin
Heater HB[3] = { HB0, HB1, HB2 };                   //Defintes an Array of heater bands

//*********************************************
//*************  Setup Hold Btn  **************
//*********************************************

HoldBtn H_Btn(pinHoldBtn);                          //Defines the E-Stop button Digital interupt pin

//*************************************************
//*************  Setup E Stop Light  **************
//*************************************************

EStop ESLight(pinEStopLight);



////*************************************************
////************* Setup TFT Buttons *****************
////*************************************************

// HOME SCREEN BUTTON DEFINITIONS

//NexButton b0 = NexButton(0, 1, "Btn_DMO"); Not needed - sends internal command in Display
//NexButton b1 = NexButton(0, 2, "Btn_Setup"); Not needed - sends internal command in Display
//NexButton b2 = NexButton(0, 3, "Btn_Run"); Not needed - sends internal command in Display

// MANUAL CONTROL SCREEN BUTTON DEFINITIONS

//NexButton b3 = NexButton(1, 1, "Btn_Home1"); Not needed - sends internal command in Display
//NextionButton b4(nex, 1, 4, "Btn_Mold_Open");
//NextionButton b5(nex, 1, 5, "Btn_Mold_Close");
//NextionButton b6(nex, 1, 6, "Btn_Ej_PinExt");
//NextionButton b7(nex, 1, 7, "Btn_Ej_PinRet");
//NextionButton b8(nex, 1, 8, "Btn_Extrude");
//NextionButton b9(nex, 1, 9, "Btn_MatFeed");
NexButton b4 = NexButton(1, 4, "Btn_Mold_Open");
NexButton b5 = NexButton(1, 5, "Btn_Mold_Close");
NexButton b6 = NexButton(1, 6, "Btn_Ej_PinExt");
NexButton b7 = NexButton(1, 7, "Btn_Ej_PinRet");
NexButton b8 = NexButton(1, 8, "Btn_Extrude");
NexButton b9 = NexButton(1, 9, "Btn_MatFeed");
//
//// SETUP HEAT SCREEN BUTTON DEFINITIONS
//
//NextionButton b10(nex, 2, 5, "Btn_temp1Up");
//NextionButton b11(nex, 2, 6, "Btn_temp2Up");
//NextionButton b12(nex, 2, 7, "Btn_temp3Up");
//NextionButton b13(nex, 2, 8, "Btn_temp1Dn");
//NextionButton b14(nex, 2, 9, "Btn_temp2Dn");
//NextionButton b15(nex, 2, 10, "Btn_temp3Dn");
NexButton b10 = NexButton(2, 5, "Btn_temp1Up");
NexButton b11 = NexButton(2, 6, "Btn_temp2Up");
NexButton b12 = NexButton(2, 7, "Btn_temp3Up");
NexButton b13 = NexButton(2, 8, "Btn_temp1Dn");
NexButton b14 = NexButton(2, 9, "Btn_temp2Dn");
NexButton b15 = NexButton(2, 10, "Btn_temp3Dn");

// RUN SCREEN BUTTON DEFINITIONS

//NexButton b10 = NexButton(3, 0, "Btn_temp1Up");

////*************************************************
////************* Setup TFT updates *****************
////*************************************************

//SETUP HEAT SCREEN TEMP READINGS DEFINITIONS
//NextionText  setTemp0(nex, 2, 11, "setTemp0"); //Set Temp Read out 1
//NextionText  setTemp1(nex, 2, 12, "setTemp1"); //Set Temp Read out 2
//NextionText  setTemp2(nex, 2, 13, "setTemp2"); //Set Temp Read out 3
//NextionText  readTemp0(nex, 2, 14, "readTemp0"); //Read Temp Read out 1
//NextionText  readTemp1(nex, 2, 15, "readTemp1"); //Read Temp Read out 2
//NextionText  readTemp2(nex, 2, 16, "readTemp2"); //Read Temp Read out 3
//
NexText setTemp0 = NexText(2, 11, "setTemp0"); //Set Temp Read out 1
NexText setTemp1 = NexText(2, 12, "setTemp1"); //Set Temp Read out 2
NexText setTemp2 = NexText(2, 13, "setTemp2"); //Set Temp Read out 3
NexText readTemp0 = NexText(2, 14, "readTemp0"); //Read Temp Read out 1
NexText readTemp1 = NexText(2, 15, "readTemp1"); //Read Temp Read out 2
NexText readTemp2 = NexText(2, 16, "readTemp2"); //Read Temp Read out 3

//NexText *tempSet[] = { &setTemp0, &setTemp1, &setTemp2 };
//
NexText *tempRead[] = { &readTemp0, &readTemp1, &readTemp2 };

char buffer[10] = {0};

NexTouch *nex_listen_list[] =
{
    //&b0,
    //&b1,
    //&b2,
    //&b3,
    &b4,
    &b5,
    &b6,
    &b8,
    &b9,
    &b10,
    &b11,
    &b12,
    &b13,
    &b14,
    &b15,
//    &setTemp0,  Delete these
//    &setTemp1,
//    &setTemp2,
//    &readTemp0,
//    &readTemp1,
//    &readTemp2,
    NULL
};



void setup()
{

  unsigned long heatTimer = millis();

  //nexLoop(nex_listen_list);
  // put your setup code here, to run once:
  //Initialize Air Cylinders
  //Sets pins per the pin.h file
  OpenMold.begin();
  CloseMold.begin();
  EjectOn.begin();
  EjectOff.begin();
  Inject.begin();
  //Initialize IR Sensor
  Sensor1.begin();
  //Initialize ThermoCouples
  TC0.begin();
  TC1.begin();
  TC2.begin();
  //Initialize Heat Bands 1 thru 3
  HB0.begin();
  HB1.begin();
  HB2.begin();
  // Initialize Interupt for hold button
  H_Btn.begin();
  attachInterrupt(digitalPinToInterrupt(pinHoldBtn), interupt1, CHANGE);

  // Initialize E Stop Light
  ESLight.begin();
  ESLight.eStopOff();




  // *******************************
  // *** TEST CODE TFT DISPLAY   ***
  // *******************************
//  nextionSerial.begin(115200);
//  nex.init();
      nexInit();                            //attaches functions to button presses
  //    b0.attachPop(b0PopCallback, &b0);
  //    b1.attachPop(b1PopCallback, &b1);
  //    b2.attachPop(b2PopCallback, &b2);
  //    b3.attachPop(b3PopCallback, &b3);
  b4.attachPop(b4PopCallback, &b4);
  b5.attachPop(b5PopCallback, &b5);
  b6.attachPop(b6PopCallback, &b6);
  b7.attachPop(b7PopCallback, &b7);
  b8.attachPop(b8PopCallback, &b8);
  b9.attachPop(b9PopCallback, &b9);
  // Temp Up/Down buttons
  b10.attachPop(b10PopCallback, &b10);
  b11.attachPop(b11PopCallback, &b11);
  b12.attachPop(b12PopCallback, &b12);
  b13.attachPop(b13PopCallback, &b13);
  b14.attachPop(b14PopCallback, &b14);
  b15.attachPop(b15PopCallback, &b15);
  // Send Data to display

        dbSerial.begin(115200);
        nexSerial.begin(115200);
}

void loop(void)
{



  /*
     check sensor1 status = open or closed, which ever it should
     get TC1 value then turn HB1 on or off
     get TC2 value then turn HB2 on or off
     get TC3 value then tunr HB3 on or off
     check buttons
     BUTTON - RUN


  */

  //**************************************************************************************************
  //*****************************  TFT DISPLAY BUTTON CATCH LOOP  ************************************
  /*
     When a pop or push event occured every time,
     the corresponding component[right page id and component id] in touch event list will be asked.
  */

  nexLoop(nex_listen_list);


  //**************************************************************************************************
  //**************************************************************************************************



  // ***************************
  // *** TEST CODE IR SENSOR ***
  // ***************************

  if (Sensor1.status() == 0)
  {
    sensorStatus = "Tripped";
  }
  else if (Sensor1.status() == 1)
  {
    sensorStatus = "Open";
  }

  //Serial.println("SENSOR1 STATUS " + sensorStatus);


  // *******************************
  // *** TEST CODE HEATER ON/OFF ***
  // *******************************
//  Serial.println(millis());
//  Serial.println(heaterTimer);
  if ( (millis() - heaterTimer) >= heaterInterval)
  {

    updateTempRead(); //NEED TO ADD CODE TO VERIFY SCREEN NUMBER AND ONLY RUN IF SCREEN 2 IS DISPLAYED
    holdTemp();
    heaterTimer = millis ();
  }


  // *******************************
  // *** TEST CODE ESTOP Button  ***
  // *******************************
  if (countPress > 2)                                             //each press and release adds 1 to the count.  2 == kill (may need to set this to 4 ???)
  {
    countPress = 0;
    ESLight.eStopOff();
  }

  while (countPress == 2)
  {
    ESLight.eStopOn();                                            // Turn on ESTOP Light
    for (int i = 0; i < 3; i++)
    {
      temp_Set[i] = 0;                                           //Defines desired temp for each heaterband as measured by ThermoCouple 0,1,2
      HB[i].heaterOff();                                         // Kill All heaters
      heaterLbl = String("Heater Number " + String(i));
      //Serial.println(heaterLbl);
      //Serial.println(" Heater OFF - EMERGENCY KILL" );
    }

  }

  //updateTempRead(TC , *tempRead );
  //updateTempSet(temp_Set, *tempSet);



} //End of VOID LOOP MAIN PROGRAM

/*
********************************************************************************************
********************************************************************************************
***                         FUNCTIONS ARE LISTED BELOW THIS POINT                        ***
***                                                                                      ***
***                                                                                      ***
***                                                                                      ***
********************************************************************************************
********************************************************************************************
*/

//void holdTemp( ThermoCouple TCs[3], Heater HBs[3], int desiredTemp[3])    //Test temp for each heater Turn off or on as needed as measured by associated ThermoCouple
void holdTemp( void )

{


  for (int i = 0; i < 3; i++)
  {

    if (TC[i].ReturnTemp() >= temp_Set[i])
    {
      HB[i].heaterOff();
      heaterLbl = String("Heater Number " + String(i));
      Serial.println(heaterLbl);
      Serial.println(" Heater OFF" );

    }
    else
    {
      HB[i].heaterOn();
      heaterLbl = String("Heater Number " + String(i));
      Serial.println(heaterLbl);
      Serial.println(" Heater ON" );
      Serial.println("Temp Sensor = " + String(TC[i].ReturnTemp()));

    }

  }

}

void updateTempRead(void)
{

  for (int i = 0; i < 3; i++)
  {

    uint32_t temp;
    memset(buffer, 0, sizeof(buffer));
    // inject your code to retrieve value from temp sensor
    temp = TC[i].ReturnTemp();
    // set value you retrieved from temp sensor
    itoa(temp, buffer, 10);
    //Serial.println("TempRead I " + String(i));
    //dbSerialPrintln(buffer);
    tempRead[i]->setText(buffer);
    //delay(50);

  }

}

/*
 ***********************************
 *******  Interupt #1 ESTOP  *******
 ***********************************
*/


void interupt1()                                                              //  INTERUPT FOR E STOP BUTTON
{
  countPress ++;
}


/*
 ***************************************
 *******  TFT Operations Funct.  *******
 ***************************************
*/
/*
 *****************************************
 ******* Open/Close Mold Cylinder  *******
 *****************************************
*/
void b4PopCallback(void *ptr)
{
  NexButton *btn = (NexButton *)ptr;
  OpenMold.PsiUp();     //  Turn on valve that opens air Cylinder
  CloseMold.PsiDown();  //  Turn off valve that closes air Cylinder
}

void b5PopCallback(void *ptr)
{
  NexButton *btn = (NexButton *)ptr;
  OpenMold.PsiDown();  // turn off valve that opens Air Cylinder
  CloseMold.PsiUp();   // Turn on Valve that Closes Air Cylinder
}

/*
 ******************************************
 ******* Ejector Pin Extend Retract  *******
 ******************************************
*/

void b6PopCallback(void *ptr)
{
  NexButton *btn = (NexButton *)ptr;

}

void b7PopCallback(void *ptr)
{
  NexButton *btn = (NexButton *)ptr;
  /*
      for (int i=0 ; i < 3; i++)
      {
        temp_Set[i] = temp_Set[i] - 1;  //set all temps down 1
        Serial.println(temp_Set[i]);
      }
  */

}

/*
 ****************************
    Extrude and Screw Feed
 *  *************************

*/

void b8PopCallback(void *ptr)
{
  NexButton *btn = (NexButton *)ptr;


  //    {
  //   //PLACEHOLDER for Extrude
  //    }


}

void b9PopCallback(void *ptr)
{
  NexButton *btn = (NexButton *)ptr;


  //    {
  //   //PLACEHOLDER for Feed Material
  //    }


}

/*
 *******************************************
 *******     Set Temps 1 thru 3      *******
 *******************************************
*/


void b10PopCallback(void *ptr)  //SET TEMP 1 UP 1 DEGREE C
{
  NexButton *btn = (NexButton *)ptr;
  temp_Set[0]++ ;
  //updateTempSet(temp_Set, *tempSet);
  uint32_t temp;
  memset(buffer, 0, sizeof(buffer));
  // inject your code to retrieve value from temp sensor
  temp = temp_Set[0];
  // set value you retrieved from temp sensor
  itoa(temp, buffer, 10);
  setTemp0.setText(buffer);
}

void b11PopCallback(void *ptr)  //SET TEMP 2 UP 1 DEGREE C
{
  NexButton *btn = (NexButton *)ptr;
  temp_Set[1]++ ;
  //updateTempSet(temp_Set, *tempSet);
  uint32_t temp;
  memset(buffer, 0, sizeof(buffer));
  // inject your code to retrieve value from temp sensor
  temp = temp_Set[1];
  // set value you retrieved from temp sensor
  itoa(temp, buffer, 10);
  setTemp1.setText(buffer);
}

void b12PopCallback(void *ptr)  //SET TEMP 3 UP 1 DEGREE C
{
  NexButton *btn = (NexButton *)ptr;
  temp_Set[2]++ ;
  //updateTempSet(temp_Set, *tempSet);
  uint32_t temp;
  memset(buffer, 0, sizeof(buffer));
  // inject your code to retrieve value from temp sensor
  temp = temp_Set[2];
  // set value you retrieved from temp sensor
  itoa(temp, buffer, 10);
  setTemp2.setText(buffer);
}

/*
 *******************************************
 *******     Set Temps 1 thru 3      *******
 *******************************************
*/

void b13PopCallback(void *ptr)  //SET TEMP 1 Down 1 DEGREE C
{

  NexButton *btn = (NexButton *)ptr;
  temp_Set[0]-- ;
  //updateTempSet(temp_Set, *tempSet);
  uint32_t temp;
  memset(buffer, 0, sizeof(buffer));
  // inject your code to retrieve value from temp sensor
  temp = temp_Set[0];
  // set value you retrieved from temp sensor
  itoa(temp, buffer, 10);
  setTemp0.setText(buffer);
}

void b14PopCallback(void *ptr)  //SET TEMP 2 Down 1 DEGREE C
{
  NexButton *btn = (NexButton *)ptr;
  temp_Set[1]-- ;
  //updateTempSet(temp_Set, *tempSet);
  uint32_t temp;
  memset(buffer, 0, sizeof(buffer));
  // inject your code to retrieve value from temp sensor
  temp = temp_Set[1];
  // set value you retrieved from temp sensor
  itoa(temp, buffer, 10);
  setTemp1.setText(buffer);
}

void b15PopCallback(void *ptr)  //SET TEMP 3 Down 1 DEGREE C
{
  NexButton *btn = (NexButton *)ptr;
  temp_Set[2]-- ;
  //updateTempSet(temp_Set, *tempSet);
  uint32_t temp;
  memset(buffer, 0, sizeof(buffer));
  // inject your code to retrieve value from temp sensor
  temp = temp_Set[2];
  // set value you retrieved from temp sensor
  itoa(temp, buffer, 10);
  setTemp2.setText(buffer);
}




