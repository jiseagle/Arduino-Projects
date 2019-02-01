//-- CC BY Norman
//-- Setup code to detect servo angle movement, and find ZERO position for assembly
//-------------------------------------------------------------------------------------------
#include <Servo.h>
#include <Oscillator.h>
#include <Norn.h>
#include <UltraSonicSensor.h>
#include <NornSing.h>
//#include <EEPROM.h>             //Remove annotation if you need to write trim value to EEPOM

//-- First Step: Configure the Servo Pin
/*
                ----------------------- 
                |                     |  
                |       O      O      |
                |                     |
                |---------------------|
        YR 3 ==>|                     | <== YL 2
                -----------------------
                     [  ]      [  ]
                     [  ]      [  ]
          RR 5 ==>{^^^^^^]    [^^^^^^]  ==> RL 4
                  [[[[[[[]    []]]]]]]

 */
#define PIN_YL            2     // define pin for servo[0]
#define PIN_YR            3     // define pin for servo[1]
#define PIN_RL            4     // define pin for servo[2]
#define PIN_RR            5     // define pin for servo[3]
#define TrigPin           6     // define Ultra Sonic Sensor Trig Pin
#define EchoPin           7     // define Ultra Sonic Sensor Echo Pin
#define SpkPin           11     // define speaker out pin

Norn Norn;                      //Creat a Norn object
int c;


void setup() {
  // Init Norn Robot.
  // 5th argument is true to load Servo calibrated trim values
  Norn.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, TrigPin, EchoPin, SpkPin); 
                                                                    
  //sing.init(11);  
  //sing.RobotOn();                                                                  
  Serial.begin(9600);                                               // remove annotation while you need serial port.
  //Norn.setTrims(-10,2,0,-5);                                      // Try calibrated trim manually if needed, then remove annotation mark
  //Norn.saveTrimsOnEEPROM();                                       // Only execute once while you confirm trim values to avoid EEPROM damaged
}

void loop() {
  // put your main code here, to run repeatedly:

  c = Serial.read();
  switch(c){
    case (48):
      Norn.RestReq();
      Norn.resetServos();
      break;

    case(49):

      break;

    case(50):
      int h=30;
      int A[4]= {12, 12, h, h};
      int O[4] = {0, 0, h -10, -h + 10};
      int phi = 1 *90;
      double phase_diff[4] = {DEG2RAD(0), DEG2RAD(180), DEG2RAD(-90*1), DEG2RAD(90*1)};
      Norn.execution(A,O,1000,phase_diff, 2);
      break;
  }

}
