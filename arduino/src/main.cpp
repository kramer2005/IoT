#include <Arduino.h>
#include <SevenSegmentDisplay.h>
#include <LiftController.h>

#define timedOp(TIMER, DELAY) if (millis() - TIMER >= (unsigned int)DELAY)
#define DATA_COLLECT_DELAY 1000
#define NUMBER_OF_STEPS 65

uint8_t pins[7] = {2, 3, 4, 5, 6, 7, 8};
int stepper_pins[4] = {10, 11, 12, 13};

String command;
unsigned long tempTimer = millis();
unsigned long lightsTimer = millis();

SevenSegmentDisplay display(pins);
LiftController liftController(2, stepper_pins);

void setup()
{
  pinMode(13, OUTPUT);
  Serial.begin(115200);
}

void sendTemperature()
{
  Serial.println("temperature 12.05");
}

void sendLights()
{
  Serial.println("light 12.05");
}

void loop()
{
  timedOp(tempTimer, 10000)
  {
    sendTemperature();
    tempTimer = millis();
  }

  timedOp(lightsTimer, 1000)
  {
    sendLights();
    lightsTimer = millis();
  }

  if (Serial.available())
  {
    command = Serial.readString();
    if (command.startsWith("lights"))
    {
      digitalWrite(13, command.charAt(7) == '1' ? HIGH : LOW);
    }

    if (command.startsWith("floor"))
    {
      liftController.goToFloor(command.charAt(6) - '0');
    }
  }
}