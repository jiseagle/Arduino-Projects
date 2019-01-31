#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include <pins_arduino.h>
#endif

#include "Norn.h"
#include <Oscillator.h>
#include <UltraSonicSensor.h>
//#include <EEPROM.h>


/************************************************/
/* Norn Initialization function                 */
/************************************************/
void Norn::init(int YL, int YR, int RL, int RR, bool loadcalibration, int TrigPin, int EchoPin)
{
  servo_pins[0]=YL;
  servo_pins[1]=YR;
  servo_pins[2]=RL;
  servo_pins[3]=RR;
  uSS.init(TrigPin, EchoPin);
  
  attachServos();
  isResting=false;

  if(loadcalibration == true)
  {
      for(int i=0; i<4; i++)
      {
       int servo_trim=EEPROM.read(i);
       if (servo_trim > 128) servo_trim-=256;
       servo[i].SetTrim(servo_trim);
      }
   }  
  

  for(int i=0; i<4; i++){
    servo_position[i]=90;
  }
  
  if(loadcalibration == true){           //If load calibrated data, then reset position with trim value.
     int resetservos[4] = {90,90,90,90}; //All servos reset to 90 degree reset position.
     moveServos(500, resetservos);
   }

}

/*************************************************/
/* Servos attach and dettach functions           */
/*************************************************/

void Norn::attachServos()
{
  servo[0].attach(servo_pins[0]);
  servo[1].attach(servo_pins[1]);
  servo[2].attach(servo_pins[2]);
  servo[3].attach(servo_pins[3]);
}

void Norn::detachServos()
{
  servo[0].detach();
  servo[1].detach();
  servo[2].detach();
  servo[3].detach();
}

/***************************************************/
/* Oscillator Trims                                */
/***************************************************/
void Norn::setTrims(int YL, int YR, int RL, int RR)
{
  servo[0].SetTrim(YL);
  servo[1].SetTrim(YR);
  servo[2].SetTrim(RL);
  servo[3].SetTrim(RR);
}

void Norn::saveTrimsOnEEPROM()
{
  for(int i=0; i<4;i++)
  {
    EEPROM.write(i, servo[i].getTrim());
  }
}

/****************************************************/
/* BASIC MOTION                                     */
/****************************************************/
void Norn::moveServos(int time, int servo_target[4])
{

  attachServos();
  
  if(time>10){
    for(int i=0; i<4; i++){
      increment[i]=((servo_target[i])-servo_position[i])/(time/10.0);
    }

    final_time = millis() + time;
    
    for(int iteration = 1; millis() <final_time; iteration++){
      partial_time = millis()+10;
      for (int i=0; i<4;i++){
       servo[i].SetPosition(servo_position[i]+(iteration*increment[i]));
      }
      while (millis() < partial_time);  //pause
    }
   }

   else{
    for (int i =0; i<4; i++){
      servo[i].SetPosition(servo_target[i]);
    }
   }
   for (int i =0; i<4; i++){
     servo_position[i] = servo_target[i];
   }
}

void Norn::oscillateServos(int A[4], int O[4], int T, double phase_diff[4], float cycle=1)
{
  for(int i=0; i<4; i++){
    servo[i].SetA(A[i]);
    servo[i].SetO(O[i]);
    servo[i].SetT(T);
    servo[i].SetPh(phase_diff[i]);
  }
  double ref=millis();
  for (double x=ref; x<=T*cycle+ref; x=millis()){
    for (int i=0; i<4; i++){
      servo[i].refresh();
    }
   }
} 

void Norn::execution(int A[4], int O[4], int T, double phase_diff[4], float steps=1.0)
{
  attachServos();
  
  int cycles=(int)steps;

  if(cycles >= 1)
    {
      for(int i=0; i< cycles; i++)
       {
         oscillateServos(A, O,T, phase_diff);
       }
    }
  oscillateServos(A, O, T, phase_diff, (float)steps-cycles);  //steps - cycles is the final not complete cycle
}

/******************************************************/
/* Norn Rest Position for Servos                      */
/******************************************************/
void Norn::resetServos()
{
if(isResting==false){
  int resetservos[4] = {90,90,90,90}; //All servos reset to 90 degree reset position.
  moveServos(500, resetservos);

  detachServos();
  isResting=true;
  }
}

/******************************************************/
/* Norn Rest Request                                  */
/******************************************************/
void Norn::RestReq()
{
  if(isResting==true) isResting = false;
  else isResting = true;
}

/***********************************************************/
/* Pre-defined Motions                                     */
/***********************************************************/

/* ======================================================= */
/* Motion Name: Walk                                       */
/* Parameters:                                             */
/*      steps: Number of steps                             */
/*          T: Period                                      */
/*        dir: (Directions) FORWARD/BACKWARD               */
/* ======================================================= */
void Norn::walk(float steps, int T, int dir)
{
  /* -- oscillator parameters for walking               -- */
  /* -- Hip Servos are in phase                         -- */
  /* -- Feet Servos are in phase                        -- */
  /* -- Hip and Feet Servos are 90 degree out of phase  -- */
  /* -- FORWARD: -90, BACKWARD: 90                      -- */
  
  int A[4] = {30, 30, 20, 20};
  int O[4] = {0, 0, 4, -4};
  // phase_diff is radians = degree*(PI/180)
  double phase_diff[4] = {0, 0, (dir * -90)*(31415926/1000000/180), (dir * -90)*(31415926/1000000/180)};
 
  if(steps == 0){
   while(1){
     execution(A, O, T, phase_diff, 1);
    }
  }
  else execution(A, O, T, phase_diff, 1);
}

/* ======================================================= */
/* Motion Name:Trun                                        */
/* Parameters:                                             */
/*      steps: Number of steps                             */
/*          T: Period                                      */
/*        dir: (Directions) FORWARD/BACKWARD               */
/*       turn: Left/Right                                  */
/* ======================================================= */
void Norn::Turn(float steps, int T, int turn, int dir)
{

  /* -- oscillator parameters for turn                  -- */
  /* -- Hip Servos are in phase                         -- */
  /* -- Feet Servos are in phase                        -- */
  /* -- Hip and Feet Servos are 90 degree out of phase  -- */
  /* -- Left Hip amp. is larger than Right hip amp.     -- */
  /* -- And vice versa.                                 -- */
  /* -- turn Left: 1, Right: -1                         -- */
  /* -- Direction: FORWARD 1, BACKWARD -1               -- */

  int A[4] = {30, 30, 20, 20};
  int O[4] = {0, 0, 7, -7};
  // phase_diff is radians = degree*(PI/180)
  double phase_diff[4] = {0, 0, (dir * -90)*(31415926/1000000/180), (dir * -90)*(31415926/1000000/180)};
  
  if(turn == 1)  // Turn Left
    {
      A[0] = 35;
      A[1] = 10;
    }
  else if(turn == -1) // Turn Right
    {
      A[0] = 10; 
      A[1] = 35;
    }

   // execute oscillateServos
   execution(A, O, T, phase_diff, steps);
}

/* ======================================================= */
/* Motion Name:Lateral Bend                                */
/* Parameters:                                             */
/*      steps: Number of bends                             */
/*          T: Period of one bend                          */
/*        dir: (Directions) Right(-1) or Left(1) bend      */
/* ======================================================= */
void Norn::LteralBend(int Steps, int T, int dir)
{
  //Parameters of all the movements. Default: Left Bend
  int bend1[4];
  int bend2[4];
  int resets[4]={90, 90, 90, 90};
  
  if(dir == -1){ 
    bend1[0]=90;bend1[1]=90;bend1[2]=168;bend1[3]=138;
    bend2[0]=90;bend2[1]=90;bend2[2]=75;bend2[3]=138;
  }
  else if(dir == 1){
    bend1[0]=90;bend1[1]=90;bend1[2]=52;bend1[3]=35;
    bend2[0]=90;bend2[1]=90;bend2[2]=52;bend2[3]=105;
  }

  //To avoid too short bend movements, set T=800 as minimum time.
  T = max(T, 800);

  //Bend movement
  for(int i=0; i<Steps; i++){
    moveServos(T/2, bend1);
    moveServos(T/2, bend2);
    delay(T*0.8);
    moveServos(T, resets);
   }
}

/* ======================================================= */
/* Motion Name: Up & Down                                  */
/* Parameters:                                             */
/*      steps: Number of Up & Down                         */
/*          T: Period                                      */
/*          h: Level of Up & Down (SMALL, MID, BIG)        */
/*             or between 0~90 degree                      */
/* ======================================================= */
void Norn::UPDown(int Steps, int T, int h)
{
  /* --- Both feet are 180 degree out of phase  --- */
  /* --- Both feet Amp. and offset are the same --- */


  int A[4] = {0,0,h,h};
  int O[4] = {0,0,h,-h};
  double phase_diff[4] = {0,0,(-90)*(31415926/1000000/180),(90)*(31415926/1000000/180)};

  execution(A, O, T, phase_diff, Steps);
}

/* ======================================================= */
/* Motion Name: Swing                                      */
/* Parameters:                                             */
/*      steps: Number of Up & Down                         */
/*          T: Period                                      */
/*          h: Amount of Swing                             */
/* ======================================================= */
void Norn::swing(int Steps, int T, int h)
{
  /* --- Both feet are in phase, the offset is half --- */
  /* --- the amplitude to let Robot swing from side --- */
  /* --- to side.                                   --- */

  int A[4] = {0,0,h, h};
  int O[4] = {0,0,h/2, -h/2};
  double phase_diff[4] = {0,0,0,0};

  execution(A, O, T, phase_diff, Steps);
}

/* ======================================================= */
/* Motion Name: TipToeSwing                                */
/* Parameters:                                             */
/*      steps: Number of Steps                             */
/*          T: Period                                      */
/*          h: Amount of Swing                             */
/* ======================================================= */
void Norn::TipToeSwing(int Steps, int T, int h)
{
  /* -- two feet are in the phase                      -- */
  /* -- offset is out of phase                         -- */
  /* -- phase_diff is zero                             -- */
  int A[4]= {0, 0, h, h};
  int O[4] = {0, 0, h, -h};
  double phase_diff[4] = {0, 0, 0, 0};
  
  execution(A,O,T,phase_diff,Steps);
}

/* ======================================================= */
/* getDistance from UltraSonicSensor                       */
/* ======================================================= */
float Norn::getDistance()
{
 return uSS.read();
}



