#include <Arduino.h>
#include <SevenSegmentDisplay.h>
#include <LiftController.h>

#define timedOp(TIMER, DELAY) if (millis() - TIMER >= (unsigned int)DELAY)
#define DATA_COLLECT_DELAY 1000
#define NUMBER_OF_STEPS 65
#define AUTO_LIGHT_THRESHOLD 200

uint8_t pins[7] = {2, 3, 4, 5, 6, 7, 8};
int stepper_pins[4] = {10, 12, 11, 13};
static const uint8_t ldrPin = A5;

String command;
unsigned long tempTimer = millis();
unsigned long lightsTimer = millis();

SevenSegmentDisplay display(pins);
LiftController liftController(2, stepper_pins);

bool autoLights = false;
int lightLevel = 0;

void setup()
{
  pinMode(13, OUTPUT);
  pinMode(ldrPin, INPUT);
  Serial.begin(115200);
}

void sendTemperature()
{
  Serial.println("temperature 12.05");
}

void sendLights()
{
  lightLevel = 600 - analogRead(ldrPin);
  Serial.print("light ");
  Serial.println(lightLevel); // inverse pin

  if (autoLights)
  {
    if (lightLevel <= AUTO_LIGHT_THRESHOLD)
    {
      digitalWrite(13, HIGH);
    }
    else
    {
      digitalWrite(13, LOW);
    }
  }
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
    if (command.startsWith("auto"))
    {
      autoLights = command.charAt(5) == '1';
    }

    if (command.startsWith("lights"))
    {
      autoLights = false;
      digitalWrite(13, command.charAt(7) == '1' ? HIGH : LOW);
    }

    if (command.startsWith("floor"))
    {
      int floor = command.charAt(6) - '0';
      display.empty();
      liftController.goToFloor(floor);
      display.display(floor + 1);
    }
  }
}