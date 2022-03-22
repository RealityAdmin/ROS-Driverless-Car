//include motor code
#include "TBMotor.h"
#include <SPI.h>

//MOTOR VARIABLES
int LEFT_DRIVE_POWER=100;
int RIGHT_DRIVE_POWER=100;
#define TURN_POWER 50
#define COMMAND_DELAY 200

OseppTBMotor Motor1(4, 5);
//OseppTBMotor Motor2(8, 3);
OseppTBMotor Motor2(7, 6);
//OseppTBMotor Motor4(4, 5);

//Encoder variables
#define ENC_IN_L 2
#define ENC_IN_R 3

//Pulse counts from encoders
volatile long encoderValueL = 0;
volatile long encoderValueR = 0;

//rpm
int rpm_L=0;
int rpm_R=0;

//Encoder pulses per rotation (can change as necessary)
#define ENC_COUNT_REV 480

//interval of time to check rotation speed (RPM)
int interval = 1000;

//variables to keep track of time:
int cycleMillis = 0;

char c;
bool process;

void setup() {
  //initializing SPI slave mode
  Serial.begin(115200);
  SPCR |= bit(SPE);
  SPI.attachInterrupt();

  //initialize encoders as input with internal pullup
  pinMode(ENC_IN_L, INPUT_PULLUP);
  pinMode(ENC_IN_R, INPUT_PULLUP);

  //Attach interrupt to update pulse count for both encoders
  attachInterrupt(digitalPinToInterrupt(ENC_IN_L), updateEncoderLeft, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_IN_R), updateEncoderRight, RISING);
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

  //Loop to calculate RPM of each wheel every specified interval
  if (cycleMillis > interval){
    cycleMillis = 0;

    //Calculate RPM of each wheel
    rpm_L = (float)(encoderValueL*60/ENC_COUNT_REV);
    rpm_R = (float)(encoderValueR*60/ENC_COUNT_REV);
  }

  Serial.println(rpm_L);
  Serial.println(rpm_R);

  //Serial.println(encoderValue1);
  //Serial.println(encoderValue2);
  //Update timer
  cycleMillis++;
  //Reset pulse count
  encoderValueL=0;
  encoderValueR=0;
}


void ProcessCommand()
{
  if (process)
  {
    process = false;
    switch (c)
    {
      case 'f': case '2':
        forward(LEFT_DRIVE_POWER, RIGHT_DRIVE_POWER);
        delay(COMMAND_DELAY);
        Serial.println('F');
        forward(0,0);
        break;
      case '4':
        backwards(LEFT_DRIVE_POWER, RIGHT_DRIVE_POWER);
        delay(COMMAND_DELAY);
        backwards(0,0);
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

void updateEncoderLeft()
{
  encoderValueL++;
}

void updateEncoderRight()
{
  encoderValueR++;
}

void forward(int leftpower, int rightpower)
{
        Motor1.SetSpeed(-(leftpower));
        Motor2.SetSpeed(-(rightpower));
}

void backwards(int leftpower, int rightpower)
{
        Motor1.SetSpeed(leftpower);
        Motor2.SetSpeed(rightpower);
}

void turn(int power)
{
        Motor1.SetSpeed(-power);
        Motor2.SetSpeed(power);
}
