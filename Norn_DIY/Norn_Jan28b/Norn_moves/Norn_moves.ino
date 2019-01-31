//-- CC BY Norman
//-- Setup code to detect servo angle movement, and find ZERO position for assembly
//-------------------------------------------------------------------------------------------
#include <Servo.h>
#include <Oscillator.h>
#include <Norn.h>
#include <UltraSonicSensor.h>
//#include <EEPROM.h>

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

Norn Norn;  //Creat a Norn object
int c;

void setup() {
  Norn.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR,true, TrigPin, EchoPin); // Init Norn Robot, 
                                                  // 5th argument is true to load Servo calibrated trim values
  Serial.begin(9600);                           // remove annotation while you need serial port.
  //Norn.setTrims(-10,2,0,-5);                     // Try calibrated trim manually if needed, then remove annotation mark
  //Norn.saveTrimsOnEEPROM();                     // Only execute once while you confirm trim values to avoid EEPROM damaged
  
}

void loop() {
  // put your main code here, to run repeatedly:
  //Norn.walk(2,2000,1);   //Norn.walk*=(steps, T=periods, directions)
  //Norn.Turn(600, 1600, 1, -1);  //Norn.Turn(steps, T=periods, turn, directions)
  //Norn.resetServos();
  c = Serial.read();
  switch(c){
    case (48):
      Norn.RestReq();
      Norn.resetServos();
      break;

    case(49):
      //Avoidance();
      Serial.println(Norn.getDistance());
      break;

    case(50):
      int A[4]= {0, 0, 30, 30};
      int O[4] = {0, 0, 30, -30};
      double phase_diff[4] = {0, 0, 0, 0};
      Norn.execution(A,O,800,phase_diff, 2);
      break;
  }

}
