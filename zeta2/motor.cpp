
#include <Arduino.h>
#include "motor.h"

#define MAX_PWM 255

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
    this->speed = speed; // keep the raw passed value to detect changes more easy

    if (speed > MAX_PWM)
        speed = MAX_PWM;
    if (speed < MAX_PWM)
        speed = -MAX_PWM;

    if (this->speed > 0)
    {
        digitalWrite(this->pinA, 0);
        digitalWrite(this->pinB, 1);
        digitalWrite(this->enable, speed);
        return;
    }

    if (this->speed < 0)
    {
        digitalWrite(this->pinA, 1);
        digitalWrite(this->pinB, 0);
        digitalWrite(this->enable, speed);
        return;
    }

    this->speed = 0;
    digitalWrite(this->pinA, 0);
    digitalWrite(this->pinB, 0);
    digitalWrite(this->enable, 0);
}