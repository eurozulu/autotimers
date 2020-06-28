
/**
   Open source library
   Copyright Rob Gilham 2020

  // TImer2 not yet implemented fully
*/

#include "autotimer2.h"
#include <stdlib.h>
#include <math.h>
#include <Arduino.h>


void AutoTimer2::startTimer() {
  cli();
  resetTimer();

  // set compare match registers
  uint8_t top = (uint8_t)Count();
  uint8_t start = top - (DUTY_CYCLE * top);
  if (start < MINIMUM_COUNT || start > top) {// Too low or rolledover/under to become bigger than top
    sei();
    return;
  }
  
  // Set outputcompares, OCR2A = state change point
  OCR2A = start;

  // turn on mode 8. (PWM, phase and frequency correct with ICR1 = TOP)
  TCCR2A |= (1 << WGM21);

  TIMSK2 |= (1 << OCIE2A) | (1 << OCIE2B);  // enable 'On compare match' for COMPB
  sei();
}


void AutoTimer2::resetTimer() {
  sei();
  // Clear control register
  TCCR2A = 0; // set entire TCCR2A register to 0
  TCCR2B = 0; // same for TCCR2B
  TCNT2  = 0; // initialize counter value to 0
  TIMSK2 = 0;
  cli();
}



// Set CS22-CS20 bits for PRESCALER
void AutoTimer2::setTimerPrescaler(uint16_t scaler) {
  switch (scaler) {
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
