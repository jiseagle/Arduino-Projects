/* ============================================================ */
/* This is a function for Norn robot.                           */
/* Author: Norman Chen                                          */
/* License:GPL
/* ============================================================ */

#ifndef Norn_h
#define Norn_h

#include <Servo.h>
#include <Oscillator.h>
#include <EEPROM.h>
#include <UltraSonicSensor.h>
#include <NornSing.h>

/* -- Define Constant -- */
#define FORWARD           1
#define BACKWARD         -1
#define LEFT              1
#define RIGHT            -1
#define _Oooh             1
#define _Hooo             2

class Norn
{
  public:

     /* --- Norn initialization --- */
     void init(int YL, int YR, int RL, int RR, bool loadcalibration, int TrigPin, int EchoPin, int SpkPin);

     /* --- Servo attach & dettach functions --- */
     void attachServos();
     void detachServos();
     
     /* --- Trim Calibration --- */
     void setTrims(int YL, int YR, int RL, int RR);
     void saveTrimsOnEEPROM(); 

     /* --- Basic Motion --- */
     void moveServos(int time, int servo_target[4]);
     void oscillateServos(int A[4], int O[4], int T, double phase_diff[4], float cycle);
     void execution(int A[4], int O[4], int T, double phase_diff[4], float steps);

     /* --- Reset Servos to reset (90degree) position --- */
     void resetServos();
     void RestReq();                                       // for ioResting reset to false

     /* --- Pre-defined Motions                       --- */
     void walk(float steps, int T, int dir);               //walk
     void Turn(float steps, int T, int Turn, int dir);     //Turn
     void LteralBend(int Steps, int T, int dir);           //Lateral Bend
     void UPDown(int Steps, int T, int h);                 //UPDown
     void swing(int Steps, int T, int h);                  //swing
     void TipToeSwing(int Steps, int T, int h);            //TipToe Swing
     void Jitter(int Steps, int T, int h);                 //Jitter
     void UpDownTurn(int Steps, int T, int h);             //TipToe Turn 
     void MoonWalk(int Steps, int T, int h, int dir);      //Moon Walk
     void WormWalk(int Steps, int T, int h, int dir);      //Worm Walk         
     void Crusatio(int Steps, int T, int h, int dir);      //Crusatio
     void FlapMoving(int Steps, int T, int h, int dir);    //Flap Move

     /* --- Avoidance                                  --- */
     float getDistance();                                  //getDistance() from Ultra Sonic Sensor
 
     void music(int song);
 
  private:

     Oscillator servo[4];
     USS uSS;
     NornSing sing;
     int servo_pins[4];
     int servo_position[4];
     int servo_trim[4];
     
     unsigned long final_time;
     unsigned long partial_time;
     float increment[4];
     bool isResting;
     bool loadcalibration;

};


#endif

