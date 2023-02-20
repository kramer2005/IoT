#include <LiftController.h>

#define FLOOR_STEP 4000

LiftController::LiftController(int floorCount, int pins[4])
{
    this->floorCount = floorCount;
    this->actualFloor = 0;
    this->stepper = new Stepper(65, pins[0], pins[1], pins[2], pins[3]);
    this->stepper->setSpeed(500);
}

void LiftController::goToFloor(int floor)
{
    if (floor >= floorCount || floor < 0)
    {
        return;
    }


    int step = (floor - this->actualFloor) * FLOOR_STEP;

    this->stepper->step(step);

    this->actualFloor = floor;
}