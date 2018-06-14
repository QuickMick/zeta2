
#ifndef fan_h
#define fan_h

class Fan
{
  public:
    Fan(int pin);

    /**
     * use in the setup method, sets up the pins
     */
    void begin();

    /**
     * update the fanspeed depending on the temperature
     */
    void update(float temperature);

  private:
    int pin;
    float temperature;
};
#endif
