
/**
   Open source library
   Copyright Rob Gilham 2020
*/

#include "autotimer1.h"
// Frequency generater

const long BAUDRATE = 38400;

// use AutoTimer1 for 16 bit timer or AutoTimer2 for 8bit timer
static AutoTimer *autoTimer = new AutoTimer1();

// debug routine to show behaviour of the #autoTimer->setFrequency(hz) method
void setOutputFreq(uint16_t hz) {

  autoTimer->setFrequency(hz);

  Serial.print("\nSet frequency to ");
  Serial.print(autoTimer->Frequency());
  Serial.print(" hz:  ");
  Serial.print("  prescaler: ");
  Serial.print(autoTimer->Prescaler());
  Serial.print("  count: ");
  Serial.print(autoTimer->Count());
  Serial.print("  on pin: ");
  Serial.println(SIGNAL_PIN_OUT);

  autoTimer->startTimer();
}


void setup() {
  Serial.begin(BAUDRATE);
  Serial.println("Beginning frequency generation");

  // Setup output signal pin and default to high
  pinMode(SIGNAL_PIN_OUT, OUTPUT);
  //digitalWrite(SIGNAL_PIN_OUT, HIGH);

  // Set output to zero (off) and wait for number on serial.
  setOutputFreq(0);
}

void loop() {

  // Read serial for hex encoded frequency, upto 2 bytes:
  // e.g. 03E8 == 1000hz
  if (Serial.available() > 0) {
    uint32_t hz = readNumber(Serial);
    if (hz > 0xFFFF) {
      Serial.print("frequency ");
      Serial.print(hz);
      Serial.print(" is too large.  Maximum is ");
      Serial.println(0xFFFF);
      return;
    }
    
    Serial.print("Setting output frequency to ");
    Serial.print(hz);
    Serial.println(" hz");
    setOutputFreq((uint16_t) hz);

    Serial.print("Achieved frequency ");
    Serial.print(autoTimer->actualFrequency());
    Serial.println(" hz");
  }

}
