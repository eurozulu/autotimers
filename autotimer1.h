/**
   Open source library
   Copyright Rob Gilham 2020
*/
#include "autotimer.h"

#define PRESCALER_COUNT 5     // The number of prescalers available to this timer

// Output pit is pin OC1A = Digital pin 9
#define DDR_OC1A B00000010  // Pin definition linked to DDR_OC1A (B00000010 / PB1 = DigitalPin 9)


// AutoTimer1 is a wrapper class for the 16-bit Timer1.
class AutoTimer1 : public AutoTimer {
  private:
    // A fixed list of the available prescalers for this timer
    const uint16_t PRESCALERS[PRESCALER_COUNT] {1, 8, 64, 256, 1024};

    // Sets the Timer bits CS12, CS11 & CS10 in TCCR1B to define the given prescaler
    // if prescaler not known sets all bits to zero, disabling the Timer.
    virtual void setTimerPrescaler(uint16_t scaler);

  protected:
    // Gets the total number of prescalers known to this timer
    uint8_t prescalersLength() { return PRESCALER_COUNT; };
    // Gets the decimal prescaler value for the given index == ( < prescalersLength)
    uint16_t prescalers(uint8_t index) {
      return index >= 0 && index < PRESCALER_COUNT ? PRESCALERS[index] : 0;
    };
    
    // Gets the TOP/MAX value for this Timer
    uint16_t counterMaximum() { return 0xFFFF;};  // 16-bit timer

  public:
    AutoTimer1();

    // reset the timer, setting control registers to zero, turning off timer1
    void resetTimer();

    // Sets Timer1 up to generate the currentfrequency, if that frequency is within the range of the timer.
    void startTimer();

};
