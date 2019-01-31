//-- CC BY Norman
//-- Setup code to detect servo angle movement, and find ZERO position for assembly
//-------------------------------------------------------------------------------------------
#include <Servo.h>

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

Servo YL_Servo; 
Servo YR_Servo; 
Servo RL_Servo; 
Servo RR_Servo;

int minimum = 10;
int maximum = 170;

void setup() {
  YL_Servo.attach(2);
  YR_Servo.attach(3);
  RL_Servo.attach(4);
  RR_Servo.attach(5);

  YL_Servo.write(minimum);
  YR_Servo.write(minimum);
  RL_Servo.write(minimum);
  RR_Servo.write(minimum);
  delay(3000);

  YL_Servo.write(maximum);
  YR_Servo.write(maximum);
  RL_Servo.write(maximum);
  RR_Servo.write(maximum);
  delay(3000);  
}

void loop() {
  // put your main code here, to run repeatedly:
  YL_Servo.write(90);
  YR_Servo.write(90);
  RL_Servo.write(90);
  RR_Servo.write(90);
}
