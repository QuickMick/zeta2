
#include <Arduino.h>
#include "fan.h"

Fan::Fan(int pin)
{
    this->pin = pin;
}

void Fan::begin()
{
    pinMode(this->pin, OUTPUT);
}

/**
     * update the fanspeed depending on the temperature
     */
void Fan::update(float temperature)
{
    if (((int)this->temperature) == ((int)temperature))
    {
        return;
    }

    this->temperature = temperature;
    if (temperature < 20)
    {
        analogWrite(this->pin, 0);
          return;
    }
    else if (temperature < 30)
    {
        analogWrite(this->pin, 50);
          return;
    }
    else if (temperature < 40)
    {
        analogWrite(this->pin, 100);
          return;
    }
    else if (temperature < 50)
    {
        analogWrite(this->pin, 150);
        return;
    }
    analogWrite(this->pin, 255);
}
