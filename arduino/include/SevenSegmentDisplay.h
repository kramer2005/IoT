#ifndef __SEVEN_SEGMENT_DISPLAY_HPP__
#define __SEVEN_SEGMENT_DISPLAY_HPP__
#include <Arduino.h>

class SevenSegmentDisplay
{
public:
    SevenSegmentDisplay(uint8_t pins[7]);
    void display(size_t number);
    void reset();

private:
    uint8_t *pins;
};

#endif