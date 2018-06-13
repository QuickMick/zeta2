
#ifndef stepper_h
#define stepper_h

class Stepper
{
public:
  Stepper(int pin0, int pin1, int pin2, int pin3);

  /**
     * use in the setup method, sets up the pins
     */
  void begin();

  /**
     * move motor a step further
     */
  void next();

  /**
     * move to previous step
     */
  void previous();
  int currentStep = 0;

protected:
  void step(int this_step);

private:
  int pin0;
  int pin1;
  int pin2;
  int pin3;
};
#endif
