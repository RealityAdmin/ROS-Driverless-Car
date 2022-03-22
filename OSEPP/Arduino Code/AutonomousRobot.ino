#include <SPI.h>

char c;
bool process;
void setup() {
  Serial.begin(115200);
  
  // Turn on slave mode
  SPCR |= bit(SPE);

  // Turn on interrupts
  SPI.attachInterrupt();
}

//Interrupt Routine
ISR(SPI_STC_vect){
  // Grab byte
  c = SPDR;
  process = true;
}

void loop () {
  if (process){
    Serial.println(c);
    ProcessCommand(c);
    process = false;
  }
}
