cmake_minimum_required(VERSION 3.16)
project(autotimer)

set(CMAKE_CXX_STANDARD 14)

include_directories(.
        /Applications/Arduino.app/Contents//Java/hardware/arduino/avr/cores/arduino
        /Applications/Arduino.app/Contents//Java/hardware/tools/avr/avr/include)

add_library(
        autotimer.h
        autotimer.cpp
        autotimer1.h
        autotimer1.cpp
        autotimer2.h
        autotimer2.cpp
)
