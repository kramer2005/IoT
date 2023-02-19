#include "SevenSegmentDisplay.h"

SevenSegmentDisplay::SevenSegmentDisplay(uint8_t pins[7])
{
    this->pins = pins;
}

const uint8_t sevenSegmtDisplay[11][7] = {
    {1, 1, 1, 1, 1, 1, 0}, // 0
    {0, 1, 1, 0, 0, 0, 0}, // 1
    {1, 1, 0, 1, 1, 0, 1}, // 2
    {1, 1, 1, 1, 0, 0, 1}, // 3
    {0, 1, 1, 0, 0, 1, 1}, // 4
    {1, 0, 1, 1, 0, 1, 1}, // 5
    {1, 0, 1, 1, 1, 1, 1}, // 6
    {1, 1, 1, 0, 0, 0, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1}, // 8
    {1, 1, 1, 1, 0, 1, 1}, // 9
    {0, 0, 0, 0, 0, 0, 1}  // "-" : indicates transition}
};

void SevenSegmentDisplay::display(size_t number)
{
    for (size_t i = 0; i < 7; i++)
        digitalWrite(this->pins[i], sevenSegmtDisplay[number][i]);
}

void SevenSegmentDisplay::reset()
{
    for (size_t i = 0; i < 7; i++)
        digitalWrite(this->pins[i], sevenSegmtDisplay[10][i]);
}