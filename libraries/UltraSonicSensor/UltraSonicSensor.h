#ifndef UltraSonicSensor_h
#define UltraSonicSensor_h
#include <Arduino.h>

class USS
{
  public:
         void init(int TrigPin, int EchoPin);
         float read();

  private:
         int _TrigPin;
         int _EchoPin;
};

#endif
         


