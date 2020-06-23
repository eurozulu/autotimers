/**
 * Open source library
 * Copyright Rob Gilham 2020
 */
 #include <Arduino.h>

#define CLOCK_SPEED 16000000
const float PULSE_WIDTH = 0.25;

const byte SIGNAL_PIN_OUT = 13;  // Pin sending signal to speedo


// AutoTimer2 is a helper class to select the most appropriate Prescaler and count for a 8 bit timer.
// Given a frequency, it calculates the most accurate Prescaler to use to achieve that frequency.
class AutoTimer2 {
  private:
    const uint8_t prescalers_count = 7;
    const uint16_t prescalers[7] {1, 8, 32, 64, 128, 256, 1024};

    uint32_t frequency;
    uint16_t prescaler;
    uint8_t count;
    
    uint32_t AutoTimer2::calculateCount(uint16_t prescale, uint32_t frequency);
    void selectPrescale();
    void setPrescaler();
    
  public:
    AutoTimer2() {};

    // The frequency desired, set with #setFrequency
    uint32_t Frequency() { return frequency; };
    
    // The currently selected prescaler
    uint16_t Prescaler() { return prescaler; };

    // The timer count for the current frequency
    uint8_t Count(){ return count; };

    // Set the scaler frequency, adjusts the prescaler and count for the timer to achive the nearest frequency to the given one.
    void setFrequency(uint32_t frequency);

    // Gets the actual frequency being generated.
    uint32_t actualFrequency();

    // reset the timer, setting control registers to zero, turning off timer2
    void resetTimer2();

    // Sets Timer2 up to generate the currentfrequency, if that frequency is within the range of the timer.
    void startTimer2();
};
