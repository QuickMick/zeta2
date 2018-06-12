
#include <Arduino.h>
#include "motor.h"

Motor::Motor(int pinA, int pinB, int enable)
{
    this->pinA = pinA;
    this->pinB = pinB;
    this->enable = enable;
    this->speed = 0;
}

void Motor::begin()
{
    pinMode(this->pinA, OUTPUT);
    pinMode(this->pinB, OUTPUT);
    pinMode(this->enable, OUTPUT);
}

void Motor::set(int speed)
{
    if (speed > 0)
    {
    }

    switch (true)
    {
    case 0:
        break;
    }
}