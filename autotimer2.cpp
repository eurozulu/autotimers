
/**
 * Open source library
 * Copyright Rob Gilham 2020
 */
 
#include "autotimer2.h"
#include <stdlib.h>
#include <math.h>
#include <Arduino.h>

uint32_t Scaler8::actualFrequency() {
  if (count == 0 || prescaler == 0)
    return 0;
  uint32_t scaleCount = (uint32_t)prescaler * (uint32_t)count;
  return CLOCK_SPEED / scaleCount;
};


void Scaler8::setFrequency(uint32_t frequency) {
  this->frequency = frequency;
  this->prescaler = 0;
  this->count = 0;

  if (frequency < 1) {
    return;
  }
  selectPrescale();
};



void Scaler8::startTimer2() {
  noInterrupts();

  resetTimer2();

  // turn on CTC mode. (reset timer counter when TOP(OCR1A) reached)
  TCCR2A |= (1 << WGM21);

  // set compare match registers
  uint8_t top = Count();
  uint8_t start = top * PULSE_WIDTH;
  if (start > top)
    start = 1;

  // Set outputcompares, OCR2A = full cycle, OCR2B when pulse starts.
  OCR2A = top;
  OCR2B = start;

  TIMSK2 |= (1 << OCIE2A) | (1 << OCIE2B);  // enable 'On compare match' for COMPB

  interrupts();
}


void Scaler8::resetTimer2() {
  noInterrupts();

  // Clear control register
  TCCR2A = 0; // set entire TCCR2A register to 0
  TCCR2B = 0; // same for TCCR2B
  TCNT2  = 0; // initialize counter value to 0
  TIMSK2 = 0;

  interrupts();
}

// Try each prescaler to get the closest result.
void Scaler8::selectPrescale() {
  double resultFreq = 0;
  uint16_t resultScale = 0;
  uint8_t resultCount = 0;

  for (int index = 0; index < prescalers_count; index++) {
    uint32_t cnt = calculateCount((double)prescalers[index], (double)this->frequency);
    Serial.print("calc count: ");
    Serial.print(cnt);
    Serial.print(" for prescale: ");
    Serial.println(prescalers[index]);

    if (cnt == 0) // Can't prescale for this frequency
      continue;

    double actual = CLOCK_SPEED / (prescalers[index] * cnt);
    Serial.print(" actual : ");
    Serial.println(actual , 8);
    if (resultCount == 0 || (abs(frequency - actual) < abs(frequency - resultFreq)) ) {
      resultFreq = actual;
      resultScale = prescalers[index];
      resultCount = cnt;
      if (actual == this->frequency) {
        break;
      }
    }
  }
  this->prescaler = resultScale;
  this->count = round(resultCount);
}

// Calculate the count required to get given frequency using given prescaler.
// If result is bigger than 0xFFFF, returns zero.
uint32_t Scaler8::calculateCount(uint16_t prescale, uint32_t frequency) {
  if (prescale == 0 || frequency == 0)
    return 0;

  uint32_t cnt = CLOCK_SPEED / (prescale * frequency) - 1;
  return (cnt < 0xFF) ? cnt : 0;
}

// Set CS22-CS20 bits for PRESCALER
void Scaler8::setPrescaler() {
  switch (prescaler) {
    case 1 :
      TCCR2B |= (1 << CS20);
      break;

    case 8 :
      TCCR2B |= (1 << CS21);
      break;

    case 32 :
      TCCR2B |= (1 << CS21);
      TCCR2B |= (1 << CS20);
      break;

    case 64 :
      TCCR2B |= (1 << CS22);
      break;

    case 128 :
      TCCR2B |= (1 << CS22);
      TCCR2B |= (1 << CS20);
      break;

    case 256 :
      TCCR2B |= (1 << CS22);
      TCCR2B |= (1 << CS21);
      break;

    case 1024 :
      TCCR2B |= (1 << CS22);
      TCCR2B |= (1 << CS21);
      TCCR2B |= (1 << CS20);
      break;

    default: // disable timer
      TCCR2B &= (0 << CS22);
      TCCR2B &= (0 << CS21);
      TCCR2B &= (0 << CS20);
  }
}

ISR(TIMER2_COMPA_vect) {
  digitalWrite(SIGNAL_PIN_OUT, HIGH);
}

ISR(TIMER2_COMPB_vect) {
  digitalWrite(SIGNAL_PIN_OUT, LOW);

}
