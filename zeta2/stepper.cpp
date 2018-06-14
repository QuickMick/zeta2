
#include <Arduino.h>
#include "stepper.h"

Stepper::Stepper(int pin0, int pin1, int pin2, int pin3)
{
    this->pin0 = pin0;
    this->pin1 = pin1;
    this->pin2 = pin2;
    this->pin3 = pin3;
}

void Stepper::begin()
{
    pinMode(this->pin0, OUTPUT);
    pinMode(this->pin1, OUTPUT);
    pinMode(this->pin2, OUTPUT);
    pinMode(this->pin3, OUTPUT);
}
void Stepper::previous()
{
    this->step(this->currentStep);
    this->currentStep--;

    if (this->currentStep == -1)
    {
        this->currentStep = 7;
    }
}

void Stepper::next()
{
    this->step(this->currentStep);
    this->currentStep++;
    if (this->currentStep == 8)
    {
        this->currentStep = 0;
    }
}

void Stepper::step(int step)
{
    switch (step)
    {
    case -2:
        digitalWrite(pin0, LOW); //BLU
        digitalWrite(pin1, LOW); //Pink
        digitalWrite(pin2, LOW); //Yellow
        digitalWrite(pin3, LOW); //Orange
        break;
    case 0:
        digitalWrite(pin0, LOW);  //BLU
        digitalWrite(pin1, LOW);  //Pink
        digitalWrite(pin2, LOW);  //Yellow
        digitalWrite(pin3, HIGH); //Orange
        break;
    case 1:
        digitalWrite(pin0, LOW);  //BLU
        digitalWrite(pin1, LOW);  //Pink
        digitalWrite(pin2, HIGH); //Yellow
        digitalWrite(pin3, HIGH); //Orange
        break;
    case 2:
        digitalWrite(pin0, LOW);  //BLU
        digitalWrite(pin1, LOW);  //Pink
        digitalWrite(pin2, HIGH); //Yellow
        digitalWrite(pin3, LOW);  //Orange
        break;
    case 3:
        digitalWrite(pin0, LOW);  //BLU
        digitalWrite(pin1, HIGH); //Pink
        digitalWrite(pin2, HIGH); //Yellow
        digitalWrite(pin3, LOW);  //Orange
        break;
    case 4:
        digitalWrite(pin0, LOW);  //BLU
        digitalWrite(pin1, HIGH); //Pink
        digitalWrite(pin2, LOW);  //Yellow
        digitalWrite(pin3, LOW);  //Orange
        break;
    case 5:
        digitalWrite(pin0, HIGH); //BLU
        digitalWrite(pin1, HIGH); //Pink
        digitalWrite(pin2, LOW);  //Yellow
        digitalWrite(pin3, LOW);  //Orange
        break;
    case 6:
        digitalWrite(pin0, HIGH); //BLU
        digitalWrite(pin1, LOW);  //Pink
        digitalWrite(pin2, LOW);  //Yellow
        digitalWrite(pin3, LOW);  //Orange
        break;
    case 7:
        digitalWrite(pin0, HIGH); //BLU
        digitalWrite(pin1, LOW);  //Pink
        digitalWrite(pin2, LOW);  //Yellow
        digitalWrite(pin3, HIGH); //Orange
        break;
    }
}