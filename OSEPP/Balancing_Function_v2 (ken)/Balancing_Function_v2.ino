
/*
 * TWO WHEELER BALANCING
 * 
 * Need to have "I2C dev" folder in Arduino libraries
 * Need to have "MPU6050" folder in Arduino libraries
 * Robot will try and balance itself using the MPU6050 sensor.
 *
 * Best used on OSEPP Mechanical Kits.
 * 2WBAL-01
 *
 * created 08 Nov 2016
 * by Sui Wei
 *
 */

//include motor code
#include "TBMotor.h"

OseppTBMotor Motor1(12, 11);
OseppTBMotor Motor2(8, 3);
//OseppTBMotor Motor3(7, 6);
//OseppTBMotor Motor4(4, 5);


const float offset_a = 2;

int leftSpeed;
int rightSpeed;
float ctrlTurn = 0;
float ctrlwalk = 0;
float offset = 0;
float equilibrium = 0;

extern float angle;

void setup() {
  Serial.begin(9600);
  updateIMU();
  //Change the timer frequency
  //To avoid the frequency of hearing.
  TCCR2B &= ~7;
  TCCR2B |= 1;
  delay(5000);
  PIDCalibrate();
}


float PID_Balance(float e, float kp, float ki, float kd)
{
  static float es = 0, sum = 0;
  float r;
  sum += e;
  if(sum>180)sum=180;else if(sum<-180)sum=-180;
  r = kp * e + ki * sum + kd * (e - es);
  es = e;
  return r;
}

void PIDCalibrate()
{
  //Calibration variables
  //number of samples per calibration run
  int totalSamples = 20000;
  //Initial run length
  int totalInitial = 5000;
  //number of calibration runs
  int totalRuns = 3;
  //angle sum
  float angleSum =0;

    for (int h = 0; h<totalInitial; h++)
    {
      //get the angle
      updateIMU(); //assume it is working
      
      leftSpeed = rightSpeed = PID_Balance(angle+offset, 20, 1, 60);
      offset += 0.00001*float(leftSpeed);

      if (leftSpeed > 255)leftSpeed = 255; else if (leftSpeed < -255)leftSpeed = -255;
      if (rightSpeed > 255)rightSpeed = 255; else if (rightSpeed < -255)rightSpeed = -255;

      if (abs(angle) < 45)
      {
        Motor1.SetSpeed(-leftSpeed);
        Motor2.SetSpeed(rightSpeed);
  //    Serial.print(a);
  //    Serial.print(",");
  //    Serial.print(-leftSpeed);
  //    Serial.print(",");
  //    Serial.print(-rightSpeed);
  //    Serial.println(",");
      } else {
        Motor1.SetSpeed(-0);
        Motor2.SetSpeed(-0);
      }
    }
  Motor1.SetSpeed(-0);
  Motor2.SetSpeed(-0);
  delay(5000);
  Motor1.SetSpeed(-leftSpeed);
  Motor2.SetSpeed(rightSpeed);

  //Outer calibration loop
  for (int k = 0; k<totalRuns; k++)
  {
    angleSum =0;
    //calibration loop
    for (int i = 0; i<totalSamples; i++)
    {
      //get the angle
      updateIMU(); //assume it is working
      //sum the angle
      angleSum += angle;
      
      leftSpeed = rightSpeed = PID_Balance(angle+offset, 20, 1, 60);
      offset += 0.00005*float(leftSpeed);

      if (leftSpeed > 255)leftSpeed = 255; else if (leftSpeed < -255)leftSpeed = -255;
      if (rightSpeed > 255)rightSpeed = 255; else if (rightSpeed < -255)rightSpeed = -255;

      if (abs(angle) < 45)
      {
        Motor1.SetSpeed(-leftSpeed);
        Motor2.SetSpeed(rightSpeed);
  //    Serial.print(a);
  //    Serial.print(",");
  //    Serial.print(-leftSpeed);
  //    Serial.print(",");
  //    Serial.print(-rightSpeed);
  //    Serial.println(",");
      } else {
        Motor1.SetSpeed(-0);
        Motor2.SetSpeed(-0);
      }

    }
    //find new equilibrium
    equilibrium = 0.5*equilibrium+0.5*angleSum/totalSamples;
  }
  Motor1.SetSpeed(-0);
  Motor2.SetSpeed(-0);

  delay(5000);
}


void PIDSetSpeed()
{
  //leftSpeed = rightSpeed = PID_Balance(angle+ctrlwalk+offset, 20, 1, 60);
  leftSpeed = rightSpeed = PID_Balance(angle+ctrlwalk-equilibrium, 20, 1, 60);
  leftSpeed+=ctrlTurn;
  rightSpeed-=ctrlTurn;

  ctrlTurn *= 0.95;
  ctrlwalk *= 0.9;

  if (leftSpeed > 255)leftSpeed = 255; else if (leftSpeed < -255)leftSpeed = -255;
  if (rightSpeed > 255)rightSpeed = 255; else if (rightSpeed < -255)rightSpeed = -255;
}

void loop() {
  ProcessCommand();
  if(!updateIMU())return;
  
  PIDSetSpeed();

  if (abs(angle) < 45)
  {
    Motor1.SetSpeed(-leftSpeed);
    Motor2.SetSpeed(rightSpeed);
//    Serial.print(a);
//    Serial.print(",");
//    Serial.print(-leftSpeed);
//    Serial.print(",");
//    Serial.print(-rightSpeed);
//    Serial.println(",");
  } else {
    Motor1.SetSpeed(-0);
    Motor2.SetSpeed(-0);
  }
}

void ProcessCommand()
{
  static unsigned long lastCmdIn = 0;
  int c;
  if (Serial.available() > 0 )
  {
    c = Serial.read();
    lastCmdIn = millis();
    switch (c)
    {
      case 'U': case '2':
        ctrlwalk = 2;
        break;
      case '4':
        ctrlwalk = - 2;
        break;
      case 'L': case '1':
        ctrlTurn = -45;
        break;
      case 'R': case '3':
        ctrlTurn = 45;
        break;
      case 'A':
        break;
      case 'C': case 'c':
        break;
      case 'B': case 'b':
        break;
      case 'D':
        break;
      default:
        break;
    }
  } else if (millis() - lastCmdIn > 300) {

  }
}




