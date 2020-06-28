/**
 * Open source library
 * Copyright Rob Gilham 2020
 * 
 * Base class for AVR Timer
 */
#include <stdlib.h>
#include <stdint.h>

// 16mz cpu clock speed
#define CLOCK_SPEED 16000000
// tick = 0.0625μs | 62.5ns / clock tick)

// duty cycle, as a percentage of the complete cycle. 0.1 = 10% of the cycle
#define DUTY_CYCLE 0.2


// Pulse tick is divided by the prescaler to get the number of counts the pulse should be set.
// Width (in μs) / 0.0625μs = (50 / 0.0625) = 800
#define PULSE_TICK (PULSE_WIDTH_MIN / (1000000 / CLOCK_SPEED))
  
const uint8_t SIGNAL_PIN_OUT = 13;  // Pin sending out signal 

// AutoTimer is a wrapper class around a specific AVR Timer which assists in using that timer.
// AutoTimer itself is abstract, using specific subclasses for each real Timer (Timer1, Timer2)
// A user of the class sets the desired Frequency, in hz, and the AutoTimer calculates the best arrangement of
// Prescaler and TOP count for the Timer. to achieve that frequency, if possible.
// Output is on the respective OCn[A/B] pin for the respective Timer.
// A fixed pulse width is generated at the given frequency.
class AutoTimer {
  protected:

    uint16_t frequency;
    uint16_t prescaler;
    uint16_t count;

    // Calculates the timer count to use for the given frequency, using the given prescaller.
    // If the result is bigger than the Timer count register, (16-bit), or prescaler is not known to prescalers[], then zero is returned.
    uint16_t calculateCount(uint16_t prescale, float frequency);

    // Sets the Timer bits CS12, CS11 & CS10 in TCCR1B to define the given prescaler
    // if prescaler not known sets all bits to zero, disabling the Timer.
    virtual void setTimerPrescaler(uint16_t scaler);

    // Gets the total number of prescalers known to this timer
    virtual uint8_t prescalersLength();
    // Gets the decimal prescaler value for the given index == ( < prescalersLength)
    virtual uint16_t prescalers(uint8_t index);
  
    // Gets the TOP/MAX value for this Timer
    virtual uint16_t counterMaximum();

  public:
    AutoTimer();

    uint16_t Frequency() { return this->frequency; };
    uint16_t Prescaler() { return this->prescaler; };
    uint16_t Count() { return this->count; };

    // Set the frequency, in hz, the timer should generate a pulse.
    // The Timer will auto select the most accurate prescaler and count based on the frequency given.
    // If the given frequency os out of the range of the Timer, Frequency, Prescaler and Count will remain at zero.
    void setFrequency(uint16_t hz);

    // Gets the actual frequency as calculated by the current prescaler and count.
    // This may differ from the #Frequency depending on how the given frequency maps to the timer counts. Higher frequencies tend to be less accurate.
    uint32_t actualFrequency();

    // reset the timer, setting control registers to zero, turning off timer1
    virtual void resetTimer();

    // Sets Timer1 up to generate the currentfrequency, if that frequency is within the range of the timer.
    virtual void startTimer();

};
