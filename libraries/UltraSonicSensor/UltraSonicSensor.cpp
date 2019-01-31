/* ======================================================== */
/* -- This is a distance calculation program for HC-SR04 -- */
/* -- ultra-sonic sensor.                                -- */
/* -- Author:Norman Chen                                 -- */
/* -- Ver: 0.0                                           -- */
/* ======================================================== */
#include <UltraSonicSensor.h>

void USS::init(int TrigPin, int EchoPin)
{
  _TrigPin = TrigPin;
  _EchoPin = EchoPin;
  pinMode(_TrigPin, OUTPUT);
  pinMode(_EchoPin, INPUT);
}

float USS::read()
{
  digitalWrite(_TrigPin, LOW); 
  delayMicroseconds(5);
  digitalWrite(_TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(_TrigPin, LOW);
  float duration=pulseIn(_EchoPin, HIGH, 60000);  //Set timeout as 60ms = 60000microsecons
  
  float distance = duration/58;

  //if (distance == 0) distance = 999;

  return distance;
} 
