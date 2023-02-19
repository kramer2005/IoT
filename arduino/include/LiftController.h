#ifndef __LIFT_CONTROLLER_H__
#define __LIFT_CONTROLLER_H__
#include <Arduino.h>
#include <Stepper.h>

class LiftController
{
public:
    LiftController(int floorCount, int pins[4]);
    void goToFloor(int floor);

private:
    Stepper *stepper;
    int floorCount;
    int actualFloor;
};

#endif