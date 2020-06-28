/**
   Open source library
   Copyright Rob Gilham 2020

   Timer2 uses WaveGen mode 5, PWM, phase correct with TOP set by OCRA
   Running at double the required frequency, it clears OC2A on upcount hitting OCR1A and sets when counts back to BOTTOM.
   Fixed at 50% DUTY Cycle.
   
*/
#include "autotimer.h"


// Output pit is pin OC2A = Digital pin 11
#define DDR_OC2A 8 // B00001000 Pin definition linked to DDR_OC2A (B00001000 / PB3 = DigitalPin 11)

#define PRESCALER_COUNT 7     // The number of prescalers available to this timer

// AutoTimer2 is a helper class to select the most appropriate Prescaler and count for a 8 bit timer.
// Given a frequency, it calculates the most accurate Prescaler to use to achieve that frequency.
class AutoTimer2 : public AutoTimer {
  private:
    // A fixed list of the available prescalers for this timer
    const uint16_t PRESCALERS[PRESCALER_COUNT] {1, 8, 32, 64, 128, 256, 1024};

  protected:
    // Sets the Timer bits CS22, CS21 & CS20 in TCCR2B to define the given prescaler
    // if prescaler not known sets all bits to zero, disabling the Timer.
    void setTimerPrescaler(uint16_t scaler);

    // Gets the total number of prescalers known to this timer
    uint8_t prescalersLength() {
      return PRESCALER_COUNT;
    }

    // Gets the decimal prescaler value for the given index == ( < prescalersLength)
    // returns the prescaler value at the given index or zero if index is out of range.
    uint16_t prescalers(uint8_t index) {
      return (index >= 0 && index < PRESCALER_COUNT) ? PRESCALERS[index] : 0;
    }

    // Gets the TOP/MAX value for this Timer
    uint16_t counterMaximum() {
      return 0xFF; // 8-bit timer
    }

  public:
    AutoTimer2() : AutoTimer() {};

    // Sets Timer2 up to generate the currentfrequency, if that frequency is within the range of the timer.
    void startTimer();
    
    // reset the timer, setting control registers to zero, turning off timer2
    void resetTimer();

};
