//include motor code
#include "TBMotor.h"
#include <SPI.h>

#define DRIVE_POWER 100
#define TURN_POWER 50
#define COMMAND_DELAY 200

OseppTBMotor Motor1(4, 5);
OseppTBMotor Motor2(8, 3);
//OseppTBMotor Motor3(7, 6);
//OseppTBMotor Motor4(4, 5);

char c;
bool process;

void setup() {
  Serial.begin(115200);
  SPCR |= bit(SPE);
  SPI.attachInterrupt();
}

ISR(SPI_STC_vect){
  // Grab byte
  c = SPDR;
  process = true;
  Serial.println(c);
}

void loop() {
  // put your main code here, to run repeatedly:
  ProcessCommand();
}


void ProcessCommand()
{
  if (process)
  {
    process = false;
    switch (c)
    {
      case 'f': case '2':
        forward(DRIVE_POWER);
        delay(COMMAND_DELAY);
        Serial.println('F');
        forward(0);
        break;
      case '4':
        backwards(DRIVE_POWER);
        delay(COMMAND_DELAY);
        backwards(0);
        break;
      case 'r': case '1':
        turn(TURN_POWER);
        delay(COMMAND_DELAY);
        Serial.println('R');
        turn(0);
        break;
      case 'l': case '3':
        turn(-TURN_POWER);
        delay(COMMAND_DELAY);
        Serial.println('L');
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
        Motor1.SetSpeed(-(power));
        Motor2.SetSpeed(-(power/2-5));
}

void backwards(int power)
{
        Motor1.SetSpeed(power);
        Motor2.SetSpeed(power);
}

void turn(int power)
{
        Motor1.SetSpeed(-power);
        Motor2.SetSpeed(power);
}
