/* ============================================================ */
/* This is a sing function for Norn robot.                      */
/* Author: Norman Chen                                          */
/* License:GPL                                                  */
/* ============================================================ */

#include <Arduino.h>
#include <NornSing.h>

/* ============================================================ */
/* define SpkPin and initialize                                 */
/* ============================================================ */
void NornSing::init(int SpkPin)
{
 _SpkPin = SpkPin;
 pinMode(_SpkPin, OUTPUT);
}

/* ============================================================ */
/* function for generate tone with frequency, and duration      */
/* ============================================================ */
void NornSing::beep(float freq, long duration)
{

  // Convert the frequency to microseconds T=1/freq/1000000 = 100000/freq (microseconds)
  float microsecondsPerWave = 1000000 / freq;
  
  // Calculate how many milliseconds there are Per HIGH/LOW cycles.
  // t (milliseconds)= 1/2T (microseconds) * 1000
  float millisecondsPerCycle = 1000 / (microsecondsPerWave * 2); 
  
  // Calculate total loop time = duration * millisecondsPerCycle
  float loopTime = duration * millisecondsPerCycle;
  
  //Play the note for the calculated looptime
  for (int i = 0; i < loopTime ; i ++) {
     digitalWrite(_SpkPin, HIGH);
     delayMicroseconds(microsecondsPerWave);

     digitalWrite(_SpkPin, LOW);
     delayMicroseconds(microsecondsPerWave);
     }
}

/* ============================================================= */
/* Tone for RobotOn initial.                                     */
/* ============================================================= */
void NornSing::RobotOn()
{
      beep(1933, 100);
      beep(2156, 100);
      beep(1863, 100);
      beep(1505, 100);
      beep(1816, 100);
      beep(1933, 100);
      beep(1729, 100);
      beep(2291, 100);
}

void NornSing::Oooh()
{
  for (float i=note_C6; i < note_C7; i=i*1.01) {
     beep(i, 10);
  }
}

void NornSing::Hooo()
{
  for (float i=note_C7; i > note_C6; i=i*0.99) {
     beep(i, 10);
  }
}

