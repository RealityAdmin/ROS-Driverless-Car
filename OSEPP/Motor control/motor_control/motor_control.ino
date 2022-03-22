//include motor code
#include "TBMotor.h"

#define DRIVE_POWER 100
#define TURN_POWER 50
#define COMMAND_DELAY 200

OseppTBMotor Motor1(12, 11);
OseppTBMotor Motor2(8, 3);
//OseppTBMotor Motor3(7, 6);
//OseppTBMotor Motor4(4, 5);


void setup() {
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  ProcessCommand();
}


void ProcessCommand()
{
  int c;
  if (Serial.available() > 0 )
  {
    c = Serial.read();
    switch (c)
    {
      case 'U': case '2':
        forward(DRIVE_POWER);
        delay(COMMAND_DELAY);
        forward(0);
        break;
      case '4':
        backwards(DRIVE_POWER);
        delay(COMMAND_DELAY);
        backwards(0);
        break;
      case 'L': case '1':
        turn(TURN_POWER);
        delay(COMMAND_DELAY);
        turn(0);
        break;
      case 'R': case '3':
        turn(-TURN_POWER);
        delay(COMMAND_DELAY);
        turn(0);
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
  }
}

void forward(int power)
{
        Motor1.SetSpeed(power);
        Motor2.SetSpeed(-power);
}

void backwards(int power)
{
        Motor1.SetSpeed(-power);
        Motor2.SetSpeed(power);
}

void turn(int power)
{
        Motor1.SetSpeed(power);
        Motor2.SetSpeed(power);
}



