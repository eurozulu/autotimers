
/**
 * Open source library
 * Copyright Rob Gilham 2020
 */
 
#include "autotimer1.h"
// Frequency generater

const long BAUDRATE = 38400;


AutoTimer1 scaler = AutoTimer1();

void setOutputFreq(uint16_t hz) {

  scaler.setFrequency(hz);
  
  Serial.print("\n\nSetting frequency to ");
  Serial.print(scaler.Frequency());
  Serial.print(" hz:  ");
  Serial.print(" prescaler: ");
  Serial.println(scaler.Prescaler());
  Serial.print("Actual frequency: ");
  Serial.println(scaler.actualFrequency());

  scaler.startTimer1();
}




void setup() {
  Serial.begin(BAUDRATE);
  Serial.println("Beginning frequency generation");

  // Setup output signal pin and default to high
  pinMode(SIGNAL_PIN_OUT, OUTPUT);
  digitalWrite(SIGNAL_PIN_OUT, HIGH);
  setOutputFreq(0);
}

void loop() {

  if (Serial.available() > 0) {
    byte buf[255];
    int len = readHexEncodedSerial(Serial, buf, 255);

    if (len > 0) {
      uint16_t hz = buf[0];
      if (len > 1) {
        hz = (hz << 8) + buf[1];
      }

      setOutputFreq(hz);

      uint8_t top = OCR1A;
      Serial.print("OCR1A: ");
      Serial.println(top);

      top = OCR1B;
      Serial.print("OCR1B: ");
      Serial.println(top);

    }
  }
}
