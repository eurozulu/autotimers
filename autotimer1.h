
/**
 * Open source library
 * Copyright Rob Gilham 2020
 */
 
#include <stdlib.h>
#include <stdint.h>



#define CLOCK_SPEED 16000000
const float PULSE_WIDTH = 0.25;

const uint8_t SIGNAL_PIN_OUT = 13;  // Pin sending signal to speedo
#define PRESCALER_COUNT 5

// AutoTimer1 is a wrapper class for the Timer1.
// Given a frequency, it calculates the most accurate Prescaler / Count comnbination to use to achieve that frequency.
class AutoTimer1 {
  private:
    const uint16_t prescalers[PRESCALER_COUNT] {1, 8, 64, 256, 1024};

    uint32_t frequency;
    uint16_t prescaler;
    uint16_t count;

    // Calculates the timer count to use for the current frequency, using the given prescaller.
    // IF the result is bigger than the Timer count register (16-bit) then zero is returned.
    uint32_t AutoTimer1::calculateCount(uint16_t prescale, uint32_t frequency);
    void selectPrescaleCount();
    void setTimerPrescaler(uint16_t scaler);

  public:
    AutoTimer1();

    uint32_t Frequency() { return frequency; };
    uint16_t Prescaler() { return prescaler; };
    uint16_t Count() { return count; };

    // Set the frequency, in hz, the timer should generate a pulse.
    // The Timer will auto select the most accurate prescaler and count based on the frequency given.
    // If the given frequency os out of the range of the Timer, Frequency, Prescaler and Count will remain at zero.
    void setFrequency(uint32_t hz);

    // Gets the actual frequency as calculated by the current prescaler and count.
    // This may differ from the #Frequency depending on how the given frequency maps to the timer counts. Higher frequencies tend to be less accurate.
    uint32_t actualFrequency();

    // reset the timer, setting control registers to zero, turning off timer1
    void resetTimer1();

    // Sets Timer1 up to generate the currentfrequency, if that frequency is within the range of the timer.
    void startTimer1();

};
