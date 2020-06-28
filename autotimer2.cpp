
/**
   Open source library
   Copyright Rob Gilham 2020
*/

#include "autotimer2.h"
#include <stdlib.h>
#include <math.h>


void AutoTimer2::startTimer() {
  resetTimer();
  if (prescaler == 0 || count == 0) {
    return;
  }
  cli();

  // wave gen mode 5, PWM freq corrected, TOP = OCR1A - Fixed 50% pulse width
  TCCR2A |= (1 << WGM20);
  TCCR2B |= (1 << WGM22);

  //set the output pin DDR to output
  DDRB |= DDR_OC2A;

  // COM2A1 + COM2A0 B10 = clear OC2A on upcount match and set on downcount.
  // setting B11 (1 << COM2A0) will invert this
  TCCR2A |= (1 << COM2A1);

  // Define the frequency of the counter by defining TOP as the count.
  uint16_t top = Count();
  OCR1A = top;
  
  // No inturrupts are used.
  //TIMSK2 = 0;

  // Turn on the timer.
  setTimerPrescaler(prescaler);

  sei();
}


void AutoTimer2::resetTimer() {
  cli();
  // Clear control register
  TCCR2A = 0; // set entire TCCR2A register to 0
  TCCR2B = 0; // same for TCCR2B
  TCNT2  = 0; // initialize counter value to 0
  TIMSK2 = 0;
  sei();
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
      TCCR2B |= (1 << CS21) | (1 << CS20);
      break;

    case 64 :
      TCCR2B |= (1 << CS22);
      break;

    case 128 :
      TCCR2B |= (1 << CS22) | (1 << CS20);
      break;

    case 256 :
      TCCR2B |= (1 << CS22) | (1 << CS21);
      break;

    case 1024 :
      TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
      break;

    default: // disable timer
      TCCR2B &= (0 << CS22);
      TCCR2B &= (0 << CS21);
      TCCR2B &= (0 << CS20);
  }
}
