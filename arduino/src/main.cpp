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
static const uint8_t tempPin = A0;

String command;
unsigned long tempTimer = millis();
unsigned long lightsTimer = millis();

SevenSegmentDisplay display(pins);
LiftController liftController(2, stepper_pins);

bool autoLights = false;
int lightLevel = 0;
const int button1Pin = 30;
const int button2Pin = 31;
int button1State = 0;
int button2State = 0;

void setup()
{
  pinMode(13, OUTPUT);
  pinMode(ldrPin, INPUT);
  pinMode(tempPin, INPUT);
  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);
  Serial.begin(115200);
}

void sendTemperature()
{
  float valor_analog_lm35 = float(analogRead(tempPin)); // Obtém o valor analógico que varia de 0 a 1023
  float tensao = (valor_analog_lm35 * 2.5) / 1023;      // Vamos converter esse valor para tensão elétrica
  float temperatura = tensao / 0.010;
  Serial.print("temperature ");
  Serial.println(temperatura);
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

  button1State = digitalRead(button1Pin);

  if (button1State == HIGH)
  {
    display.empty();
    liftController.goToFloor(0);
    display.display(1);
  }

  button2State = digitalRead(button2Pin);

  if (button2State == HIGH)
  {
    display.empty();
    liftController.goToFloor(1);
    display.display(2);
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