
#ifndef motor_h
#define motor_h

class Motor
{
  public:
    Motor(int pinA, int pinB, int enable);

    /**
     * use in the setup method, sets up the pins
     */
    void begin();

    void set(int speed);

    int getSpeed() { return this->speed; }
    //int setSpeed(int speed) { this->speed = speed; }

  protected:
    void step(int this_step);

  private:
    int pinA;
    int pinB;
    int enable;
    int speed;
};
#endif
