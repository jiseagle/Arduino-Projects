// Include the Servo library
#include <Servo.h>
#include <SoftwareSerial.h>

// Define HC-SR04 Trig pin, and Echo pin
const int trigPin = 10; 
const int echoPin = 11; 

// Variables for calculate distance and duration
unsigned int distance;


// Other parameter definition
#define baudrate 9600
#define BTbaudrate 38400
#define txPin 3          // define Serial Port TX PIN
#define rxPin 2          // define Serial Port RX PIN

int j = 0;
// Incudes sub functions
unsigned int distCal();
void FootInit();
void MoveForward1();
void MoveForward2();
void MoveForward3();
void MoveBackward2();
void SlideStep();
void BalletStand();
void ShakingLeftLeg();
void ShakingRightLeg();
void SumoRight();
void SumoLeft();
void ContSumo();

// Create Servo project
Servo LeftHipServo;
Servo LeftAnkleServo;
Servo RightHipServo;
Servo RightAnkleServo; 

// Create BT Project
SoftwareSerial BT(txPin, rxPin);

void setup() {
  pinMode(trigPin, OUTPUT);     // set pinMode for Trig pin as output
  pinMode(echoPin, INPUT);      // set pinMode for Echo pin as input
  Serial.begin(baudrate);           // set serial port start, and baud rate=9600
  
  pinMode(txPin, OUTPUT);
  pinMode(rxPin, INPUT);
  BT.begin(BTbaudrate);

  // attach pin 4, 5, 6, 7 for Sevo
  LeftHipServo.attach(4);  
  LeftAnkleServo.attach(5);
  RightHipServo.attach(6);
  RightAnkleServo.attach(7);
  FootInit(); 
  delay(1000);
}

int c = 0; 

void loop() {

  if(BT.available() > 0)  //Check if BT send message to Norn_DIY
  {
    c = BT.read();
    Serial.println(c);
  }

  else if (Serial.available() > 0) // Check if Serial Port send message to Norn_DIY
  {
    c = Serial.read();
    Serial.println(c);
    if(c==48)
    {
      FootInit();
    }

    if(c==49)
    {
     MoveForward2();
     }

    if(c==50)
    {
      while(1){
      distance=distCal();

      if (distance >= 10) MoveForward2();
      else if (distance <= 10) MoveBackward2();

      c= Serial.read();
      if(c == 48) break;
      }
    }

    if(c==51)
    {
      

    }

    if(c==52)
    {
     SumoRight();
     }
  }

}



void FootInit(){
  LeftHipServo.write(88);  
  LeftAnkleServo.write(92);
  RightHipServo.write(90);
  RightAnkleServo.write(90);
}

void MoveForward1(){          // 兩腳反向平移往前
  LeftHipServo.write(45);
  RightHipServo.write(135);
  delay(300);
  LeftAnkleServo.write(60);
  RightAnkleServo.write(120);
  delay(300);
  LeftHipServo.write(120);
  RightHipServo.write(60);
  delay(300);
  LeftAnkleServo.write(90);
  RightAnkleServo.write(90);
  delay(300);  
}

void MoveForward2(){            // 墊腳尖往前走
      for (int i=90; i < 151; i++){
        LeftHipServo.write(i);
        RightAnkleServo.write(90-(i-90));
        RightHipServo.write(i);
        delay(8);
      }

      for (int i = 30; i < 91; i++) {
        RightAnkleServo.write(i);
        delay(8);
      } 

     for (int i= 150; i > 89; i--){
        LeftHipServo.write(i);
        RightHipServo.write(i);
        delay(8);
      }
      
      int j = 0;
      for (int i=90; i > 30; i--){
        RightHipServo.write(i);
        LeftAnkleServo.write(i+j);
        LeftHipServo.write(i);
        j=j+2;
        delay(8);
      }

      for (int i = 150; i > 89; i--) {
        LeftAnkleServo.write(i);
        delay(8);
      } 

     for (int i= 30; i < 91; i++){
        LeftHipServo.write(i);
        RightHipServo.write(i);
        delay(8);
      }
}

void MoveForward3(){            // 翹腳尖往前走
      LeftAnkleServo.write(60);
      delay(300);
      LeftHipServo.write(120);
      RightHipServo.write(120);
      delay(300);
      LeftAnkleServo.write(90);
      RightAnkleServo.write(120);
      LeftHipServo.write(60);
      RightHipServo.write(60);
      delay(300);
      RightAnkleServo.write(90);
      delay(300);
}

void MoveBackward2(){
      LeftAnkleServo.write(120);
      delay(300);
      LeftHipServo.write(120);
      RightHipServo.write(120);
      delay(300);
      LeftAnkleServo.write(90);
      RightAnkleServo.write(60);
      LeftHipServo.write(60);
      RightHipServo.write(60);
      delay(300);
      RightAnkleServo.write(90);
      delay(300);
}

void SlideStep(){
        for (int i = 90; i <141; i++){
        LeftHipServo.write(i);
        LeftAnkleServo.write(i);
        delay(5);
      }

      for (int i = 90; i > 49; i--){
        RightAnkleServo.write(i);
        delay(5);
      }
      
      for (int i = 140; i > 89; i--){
        LeftAnkleServo.write(i);
        LeftHipServo.write(i);
        delay(5);
      }

      for(int i = 50; i <91; i++){
        RightAnkleServo.write(90);
        delay(5);
      }

      for (int i = 90; i > 41; i--){
        RightHipServo.write(i);
        RightAnkleServo.write(i);
        delay(5);
      }
      
      for (int i = 90; i < 141; i++){
        LeftAnkleServo.write(i);
        delay(5);
      }
      
      for (int i = 40; i < 91; i++){
        RightAnkleServo.write(i);
        RightHipServo.write(i);
        delay(5);
      }
      
      for(int i = 140; i > 89; i--){
        LeftAnkleServo.write(90);
        delay(5);
      }
}

void BalletStand(){
  for (int i = 0; i<61; i++){
    LeftAnkleServo.write(90+i);
    RightAnkleServo.write(90-i);
    delay(5);
  }
}

void ShakingLeftLeg(){
       LeftAnkleServo.write(60);
       delay(70);
       LeftAnkleServo.write(90);
       delay(70);
       LeftAnkleServo.write(120);
       delay(70);
       LeftAnkleServo.write(90);
       delay(70);
}

void ShakingRightLeg(){
       RightAnkleServo.write(120);
       delay(70);
       RightAnkleServo.write(90);
       delay(70);
       RightAnkleServo.write(60);
       delay(70);
       RightAnkleServo.write(90);
       delay(70);
}

void ContSumo(){
  SumoRight();
  delay(3000);
  for (int i = 140; i > 90; i--){
    RightAnkleServo.write(i);
    delay(50);
  }
  SumoLeft();
  delay(3000);
  for (int i = 40; i < 90; i++){
    LeftAnkleServo.write(i);
    delay(50);
  }  
}
void SumoRight(){
     for (int i = 90; i <160; i++){
      LeftAnkleServo.write(i);
      delay(5);
     }

     for(int i = 90; i <120; i++){
      LeftHipServo.write(i);
     }
     

     for(int i = 90; i <140; i++){
      RightAnkleServo.write(i);
      delay(10);
     }

     for(int i = 160; i > 90; i--){
      LeftAnkleServo.write(i);
      delay(5);
     }

     for(int i= 120; i > 90; i--){
      LeftHipServo.write(i);
     }
}

void SumoLeft(){
     for (int i = 90; i > 20; i--){
      RightAnkleServo.write(i);
      delay(5);
     }

     for(int i = 90; i > 50; i--){
      RightHipServo.write(i);
     }
     

     for(int i = 90; i > 40; i--){
      LeftAnkleServo.write(i);
      delay(10);
     }

     for(int i = 20; i <91; i++){
      RightAnkleServo.write(i);
      delay(5);
     }

     for(int i= 50; i <91; i++){
      RightHipServo.write(i);
     }
}


unsigned int distCal() {
  unsigned int duration = 0;
  unsigned int dist = 0;
  // Init Trig 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Receive Echo and calculate distance
  duration = pulseIn(echoPin, HIGH);
  dist = duration /58;
  return dist;
}
