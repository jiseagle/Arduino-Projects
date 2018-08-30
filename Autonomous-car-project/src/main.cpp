//Write By Norman Chen
//Controlled by UNO Rev 3
//Program: Self-propelled Car

#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
#include <SoftwareSerial.h>

// Declare a servo pin
#define servoPinr 9
#define servoPinl 10

//Create a Servo project
Servo servoR;   // Nominal 1475
Servo servoL;   // Nominal 1535

//Define UltraSonic Pins
#define sonicTrig_right 7
#define sonicEcho_right 8
#define sonicTrig_left  5
#define sonicEcho_left  6
unsigned int distRight, distLeft;

//Below settings are for STK3210
#define stk3210Address 0x48    // stk3210 I2C address

// Define Register Map for stk3210
#define stkState_reg       0x00
//#define stkAlsctrl_reg     0x02
#define stkPSctrl_reg      0x01
#define stkLedCtrl_reg     0x03
#define stkInt_reg         0x04
#define PS_THDH1_reg       0x06
#define PS_THDH2_reg       0x07
//#define alsData1           0x13
//#define alsData2           0x14
#define psData1            0x11
#define psData2            0x12
#define data1Offset_reg    0x15
#define data2Offset_reg    0x16

// Define Register Value for stk3210
#define stkState_val       0x02 // Enable ALS only w/o wait time
#define stkState_val2      0x01 // Eanble PS w/offset compensation w/o wait time
//#define stkAlsctrl_val     0x79
#define stkPSctrl_val      0x31
#define stkLedCtrl_val     0xF0  //4On period  = 8/64 of 0.57ms
#define stkInt_val         0x05
#define PS_THDH1_val       0x00
#define PS_THDH2_val       0x00
#define data1Offset_val    0x00
#define data2Offset_val    0x00

// Define PS Threshold
#define PS_L1            0x00
#define PS_L2            0x40
// Define Ultra Sonic DistCal (unit:cm)
#define distLimit              6
#define distH                 20
//define PS Limit

// Define
// delcare sub-Function
void forward(int time);
void slowForward(int time);
void reverse(int time);
void turnLeft(int time);
void turnRight(int time);
void reAttach();
void allStop(int time);
void proximity();
void print_PS_data();
void print_dis();
unsigned int RightDistCal(),LeftDistCal();

//Define SoftwareSerial Parameters
#define txPin 3
#define rxPin 2
SoftwareSerial BT(rxPin, txPin);  //set up a new serial port obejct
#define btBaud 38400

//Global Variables
#define baudRate 9600
char psd1, psd2;
char btRead = 'S';
int ctrlState = 0;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(baudRate);                   //Set Baud Rate for Arduino UNO
    pinMode(sonicTrig_right, OUTPUT);        //Define sonic trig and echo
    pinMode(sonicEcho_right, INPUT_PULLUP);
    pinMode(sonicTrig_left, OUTPUT);        //Define sonic trig and echo
    pinMode(sonicEcho_left, INPUT_PULLUP);

    Wire.begin();                           //Join I2C bus as Master

    // define tx, rx pin for serial port object
    pinMode(txPin, OUTPUT);
    pinMode(rxPin, INPUT);
    BT.begin(btBaud);                        // begin for serial tx/rx of BT
}

void loop() {
      // Check if BT transmisson data.
      if (BT.available() >0)  btRead = BT.read();
      
      // Set Robot-auto-controlled.
      if (btRead == 'A') {                
            
            //Read Sensor Data
            //distRight = RightDistCal();     // Read right ultrasonic sensor data
            //distLeft = LeftDistCal();       // Read left ultrasonic sensor data
            proximity();                    // Read front proximity sesor data
            //print_dis();
            //print_PS_data();
            //int DistOffset1 = distLeft - distRight;
            //int DistOffset2 = distRight - distLeft;

            //Check if there is block in front of Robot
            if (psd2 <= PS_L1)                // No Block in front of Robot
            {
              slowForward(5);
              distRight = RightDistCal();     // Read right ultrasonic sensor data
              distLeft = LeftDistCal();       // Read left ultrasonic sensor data
              
              //Determine which direction by road branch
              if (distLeft > distH && distRight < distH)          // left&Straight --> turn left
              {
                turnLeft(1050);
              }
              else if ((distRight > distH) && (distLeft < distH))      // right&straight --> go straight
              {
                forward(0);
                if (distLeft <= distLimit)         // slightly turn left
                {
                  turnRight(50);
                }
                else if (distRight <= distLimit)   // slightly turn right
                {
                  turnLeft(50);
                }
              }
              else if ((distRight > distH) && (distLeft > distH))      // cross section --> turn left
              {
                turnLeft(1050);
              }
              else                                                     // straight road
              {
                forward(0);
                if (distLeft <= distLimit)         // slightly turn left
                {
                  turnRight(50);
                }
                else if (distRight <= distLimit)   // slightly turn right
                {
                  turnLeft(50);
                }
              }
            }

            if (psd2 > PS_L1)
            {
              distRight = RightDistCal();     // Read right ultrasonic sensor data
              distLeft = LeftDistCal();       // Read left ultrasonic sensor data

              if ((distRight < distH) && (distLeft > distH))           // only can turn left
              {
                turnLeft(1050);
              }
              else if ((distRight > distH) && (distLeft > distH))      // only can turn right
              {
                turnLeft(1050);
              }
              else if ((distRight > distH) && (distLeft < distH))      // only can turn right
              {
                turnRight(1100);
              }
              else                                                     // dead road, U turn.
              {
                turnLeft(2400);
              }
            }
        }
      
     // Manaul Cotnrol Robot
      if (btRead == 'F') {
        forward(0);
      }
      else if (btRead == 'f') {
        allStop(0);
      }
      else if (btRead == 'R') {
        turnRight(0);
      }
      else if (btRead == 'r') {
        allStop(0);
      }
      else if (btRead == 'L') {
        turnLeft(0);
      }
      else if (btRead == 'l') {
        allStop(0);
      }
      else if (btRead == 'V') {
        reverse(0);
      }
      else if (btRead == 'v') {
        allStop(0);
      }
      else if (btRead == 'S') {
        allStop(0);
      }

}

//DistCal
unsigned int RightDistCal() {
   unsigned int duration = 0;

   digitalWrite(sonicTrig_right, LOW);
   delayMicroseconds(2);
   digitalWrite(sonicTrig_right, HIGH);
   delayMicroseconds(10);
   digitalWrite(sonicTrig_right, LOW);
   //Set Echo receiver
   duration = pulseIn(sonicEcho_right, HIGH);
   distRight = duration / 58;
      return distRight;
}

unsigned int LeftDistCal() {
   unsigned int duration = 0;

   digitalWrite(sonicTrig_left, LOW);
   delayMicroseconds(2);
   digitalWrite(sonicTrig_left, HIGH);
   delayMicroseconds(10);
   digitalWrite(sonicTrig_left, LOW);
   //Set Echo receiver
   duration = pulseIn(sonicEcho_left, HIGH);
   distLeft = duration / 58;
      return distLeft;
}
// Reverse Sub Function
void forward(int time) {
  reAttach();
  servoR.write(1100);
  servoL.write(1775);
  delay(time);
}

//Forward Sub Function
void reverse(int time) {
  reAttach();
  servoR.write(180);
  servoL.write(0);
  delay(time);
}

//Slow Forward Sub Function
void slowForward(int time) {
  reAttach();
  servoR.write(1418);
  servoL.write(1595);
  delay(time);
}

//Trun Left Sub Function
void turnLeft(int time) {
  reAttach();
  servoR.write(0);
  servoL.write(0);
  delay(time);
}

//Turn Right Sub Function
void turnRight(int time) {
  reAttach();
  servoR.write(180);
  servoL.write(180);
  delay(time);
}

// Stop Function, due to set 90 maybe not completely stopped,
// use detach to completely stop servo.
void allStop(int time) {
  servoR.detach();
  servoL.detach();
  delay(time);
}

// Setup Re-attach after detach
void reAttach() {
  if (!servoR.attached())
    servoR.attach(servoPinr);   //attach Pin 3 to right servo, e.g. right wheel.
  if (!servoL.attached())
    servoL.attach(servoPinl);   //attach Pin 5 to left servo, e.g. left wheel.
}

void proximity() {
  //Enable PS
  // Start I2C transmission
  Wire.beginTransmission(stk3210Address);
  // Write State to enable PS
  Wire.write(stkState_reg);
  Wire.write(stkState_val2);
  Wire.endTransmission();

  Wire.beginTransmission(stk3210Address);
  //Write LED CTRL
  Wire.write(stkLedCtrl_reg);
  Wire.write(stkLedCtrl_val);
  Wire.endTransmission();

  //Write INT State
  Wire.beginTransmission(stk3210Address);
  Wire.write(stkInt_reg);
  Wire.write(stkInt_val);
  Wire.endTransmission();

  //Write PS Data Threshold HIGH
  Wire.beginTransmission(stk3210Address);
  Wire.write(PS_THDH1_reg);
  Wire.write(PS_THDH1_val);
  Wire.endTransmission();
  Wire.beginTransmission(stk3210Address);
  Wire.write(PS_THDH2_reg);
  Wire.write(PS_THDH2_val);
  Wire.endTransmission();

  //Write PS DATA1 Offset
  Wire.beginTransmission(stk3210Address);
  Wire.write(data1Offset_reg);
  Wire.write(data1Offset_val);
  Wire.endTransmission();
  //Write PS DATA2 Offset
  Wire.beginTransmission(stk3210Address);
  Wire.write(data2Offset_reg);
  Wire.write(data2Offset_val);
  Wire.endTransmission();

  // Read PS DATA1
  Wire.beginTransmission(stk3210Address);
  Wire.write(psData1);
  Wire.endTransmission();
  Wire.requestFrom(stk3210Address,1);
  psd1 = Wire.read();

  // Read PS DATA2
  Wire.beginTransmission(stk3210Address);
  Wire.write(psData2);
  //Wire.write(stkState_reg);
  Wire.endTransmission();
  Wire.requestFrom(stk3210Address,1);
  psd2 = Wire.read();
}

void print_PS_data() {
  // Print PS DATA1 & DATA2
  Serial.print("PS_DATA1 --> 0x");
  if(psd1 < 16) Serial.print("0");
  Serial.println(psd1,HEX);

  // Print PS DATA1 & DATA2
  Serial.print("PS_DATA2 --> 0x");
  if(psd2 < 16) Serial.print("0");
  Serial.println(psd2,HEX);
  delay(300);
}

void print_dis() {
  Serial.print("The Right distance is: ");
  Serial.print(distRight);
  Serial.println("cm");
  Serial.print("The Left distance is: ");
  Serial.print(distLeft);
  Serial.println("cm");
  delay(300);
}
