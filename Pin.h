 /*
 * This section is to define all pins
 * Pin 24 high Signal Air Solenoid Open; Connected to Relay 2
 * Pin 22 high singal Air Solenoid Close; Connected to Relay 1
 * Pin 26 high ejector pin 1 to extend; Connected to Relay 3
 * Pin 28 high ejector Pin 2 to extend; Connected to Relay 4
 * Pin 30 high injection piston advance; Connected to Relay 5
 * Pin 2 PWM2 Rotate Screw Advamce; Connected to Servo Driver?????
 * Pin 32 heater band #1 ON; Connected to Relay 6
 * Pin 34 Heater Band #2 ON; Connected to Relay 7
 * Pin 36 Heater Band #3 ON; Connected to Relay 8
 * Pin A1 ThermoCouple #1; Read voltage from TC1 [convert via table and output temp in C]
 * Pin A2 ThermoCouple #2; Read voltage from TC1 [convert via table and output temp in C]
 * Pin A3 ThermoCouple #3; Read voltage from TC1 [convert via table and output temp in C]
 * Pin A4 ALL STOP EMERGENCY KILL [Normally open, when Closed signal received - Kill all heat all motion]
 * Pin [PIN to be defined] Proximity Sensor [IR] digital read LOW = detecting obstacle
 * Pin [PIN to be defined] Servo signal [uses Servo Library]
 */

#define pinMoldOpen      24
#define pinMoldClose     22
#define pinEjectOn       26
#define pinEjectOff      28
#define pinInjector      30   //single action piston.  
#define pinScrew          2
#define pinHeatA         32
#define pinHeatB         34
#define pinHeatC         36
#define pinTempA         A1
#define pinTempB         A2
#define pinTempC         A3
#define pinHoldBtn        3   // only allowed to use pins 2, 3, 18, 19, 20, 21 for interupt programming
#define pinIRSense       A8   //[needs digital pin to use digital read]
#define pinEStopLight    A12  //Light to indicate Estop Status
//#define pinServo         [Needs digital pin]

