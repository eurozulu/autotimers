
/**
  Using Wave Gen mode 8: PWM, Phase and Frequency Corrected with ICR1 as TOP
  Frequency (Period) is defined by ICR1 acting as TOP.  All frequency changes are preformed on a stopped Timer.
  Pulse width (duty cycle) is defined by OCR1A.  OCR1A match clears OC1A on upslope and sets it on the downslope.
  OC1A flag is registered to digital pin 9.
  Timer operates at twice the desired frequency so a complete upcount and downcount = singel period of desired frequency.
  
  Open source library
  Copyright Rob Gilham 2020
*/
#include <Arduino.h>

#include "autotimer1.h"
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
//#include <avr/iom328p.h>

#define DDR_OC1A B00000010  // Pin definition linked to DDR_OC1A (B00000010 / PB1 = DigitalPin 9)

AutoTimer1::AutoTimer1() : AutoTimer() { };

// Sets the CS12, CS11 & CS10 bits on TCCR1B for the given prescaler.
// If not a valid prescaler (found in prescalers[]) then disables the Timer by setting all bits to zero.
void AutoTimer1::setTimerPrescaler(uint16_t scaler) {
  switch (scaler) {
    case 1 : {
        TCCR1B |= (1 << CS10);
        break;
      }
    case 8 : {
        TCCR1B |= (1 << CS11);
        break;
      }
    case 64 : {
        TCCR1B |= (1 << CS11);
        TCCR1B |= (1 << CS10);
        break;
      }
    case 256 : {
        TCCR1B |= (1 << CS12);
        break;
      }
    case 1024 : {
        TCCR1B |= (1 << CS12);
        TCCR1B |= (1 << CS10);
        break;
      }
    default: {  // disable timer
        TCCR1B &= (0 << CS12);
        TCCR1B &= (0 << CS11);
        TCCR1B &= (0 << CS10);
      }
  }
}


void AutoTimer1::startTimer() {
  resetTimer();
  if (prescaler == 0 || count == 0) {
    return;
  }
  cli();

  // wave gen mode 8, PWM freq corrected, TOP = ICR1, OCR1A defines pulse width
  TCCR1B |= (1 << WGM13);

  //set the output pin DDR to output
  DDRB |= DDR_OC1A;

  // COM1A1 + COM1A0 B10 = clear OC1A on upcount match and set on downcount match.
  // setting B11 (1 << COM1A0) will invert this
  TCCR1A |= (1 << COM1A1);

  // Define the frequency of the counter by defining TOP as the count.
  uint16_t top = Count();
  ICR1 = top;
  
  // set compare match register OCR1A to mark the change state point in the count.
  // When matching on the upcount, clears OC1A and sets it again when matching on the downcount.
  // The closer OCR1A is to TOP (ICR1), the shorter the pulse width will be.
  uint16_t start = top - (DUTY_CYCLE * top);
  OCR1A = start;

  // No inturrupts are used.
  //TIMSK1 |= (1 << OCIE1A) | (1 << OCIE1B);  // enable 'On compare match' for COMPA & B

  // Turn on the timer.
  setTimerPrescaler(prescaler);

  sei();
}


void AutoTimer1::resetTimer() {
  cli();

  // reset prescaler to zero, turns off timer clock.
  setTimerPrescaler(0);

  // Clear control register
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1  = 0; // initialize counter value to 0
  TIFR1 = 0;  // Clear any Flags set
  TIMSK1 = 0; // Clear all interrupt masks

  OCR1A = 0;
  OCR1B = 0;
  sei();
}
