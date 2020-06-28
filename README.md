## Autotimers

#### Overview
A library to assist in using the atmega's in-built Timers to generate an output frequency square wave.
  
Automatically calculates the most suitable prescaler / counter TOP to achive the most accurate frequency.  
Uses the timers in-built On Output Compare registers and Pins to generate the signal, so hardly any CPU time used.  
Leaves the main 'loop' entirely empty for your code.  

Frequency can be changed at any time and as often as required.  
Timer will restart itself on change to ensure reliable frequency output.  


## Usage
The simpliest way to use the library is to copy the autotimer files into your project directory.  
Required files are the two base files:  
 - autotimer.h
 - autotimer.cpp  
And the two files for the Timer to be used:
 - autotimer1.h
 - autotimer1.cpp  
and/or
 - autotimer2.h
 - autotimer2.cpp

Include the relevant library, according to the Timer requried  
Options are:  
 - Timer1  A 16-bit timer  `#include "autotimer1.h"`
 - Timer2  A 8-bit timer  `#include "autotimer2.h"`  
 
 To use Timer1, include the timer1 library:  
 `#include "autotimer1.h"`  
 
 Using a global level reference to a timer:  
 `static AutoTimer *autoTimer = new AutoTimer1();`  
 
 Note the variable always refers to the base class: `AutoTimer`  
 The actual instance should be the Timer required, `AutoTimer1`  
 
 
 Set the frequency you desire from the timer, in hertz, in setup or loop:  
 `autoTimer->setFrequency(50);`  
 
When ready to start the timer, simply call:  
`autoTimer->startTimer();`

The Timer can be stopped by calling:  
`autoTimer->resetTimer();`
and restarted by calling `autoTimer->startTimer()` again.

#### Example
An example [`autotimer.ino`](https://github.com/eurozulu/autotimers/blob/master/autotimer.ino) is included in the project.  
This listens on the Serial port for a numeric value and uses this to set the frequency of the Timer output.
