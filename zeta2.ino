#include <Wire.h>
#include <TeensyThreads.h>
#include "pins.h"
#include "Stepper.h"
#include "Motor.h"
#include "BMP085.h"
#include "I2Cdev.h"
#include "MPU6050.h"
asd
    BMP085 pressure_m;
MPU6050 accelgyro;

Stepper lidarStepper(PIN_STEPPER_0, PIN_STEPPER_1, PIN_STEPPER_2, PIN_STEPPER_3);

bool blinkState = false;

/*
gelb: gyp-distance-sensor
braun: zero-position-sensor
grün: grün von tfmini distance-sensor tx
orange: weiß von tfmini distance-sensor rx
*/

/*
800 ma alle 4motoren
23ma teensy
*/

void setup()
{
  Serial.begin(9600);
  Serial1.begin(115200); // TFMINI serial
  Wire.begin();          // I2C

  while (!Serial && !Serial1)
  {
  }

  /**
   *  configure OUTPUT pins
   */
  pinMode(PIN_ENABLE_LEFT, OUTPUT);
  pinMode(PIN_MOTOR_LEFT_A, OUTPUT);
  pinMode(PIN_MOTOR_LEFT_B, OUTPUT);

  pinMode(PIN_ENABLE_RIGHT, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_A, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_B, OUTPUT);

  pinMode(PIN_FAN, OUTPUT);

  lidarStepper.begin();

  /**
   *  configure INPUT pins
   */
  pinMode(PIN_ZEROING_SENSOR, INPUT);
  pinMode(PIN_ANALOG_DISTANCE, INPUT);

  /**
   * Start Sensor connections
   */
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();
  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  pressure_m.bmp085Calibration();

  /**
 * Start Threads
 */
  threads.addThread(blinkthread);
}

void loop()
{
  /*  float temperature = pressure_m.bmp085GetTemperature(); //MUST be called first
  float pressure = pressure_m.bmp085GetPressure();
  float altitude = pressure_m.calcAltitude(pressure);
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  Serial.print("Temperature: ");
  Serial.print(temperature, 2); //display 2 decimal places
  Serial.println("deg C");

  Serial.print("Pressure: ");
  Serial.print(pressure, 0); //whole number only.
  Serial.println(" Pa");

  Serial.print("Altitude: ");
  Serial.print(altitude, 2); //display 2 decimal places
  Serial.println(" M");

  Serial.println(); //line break

  delay(1000); //wait a second and get values again.

  // these methods (and a few others) are also available
  //accelgyro.getAcceleration(&ax, &ay, &az);
  //accelgyro.getRotation(&gx, &gy, &gz);

  // display tab-separated accel/gyro x/y/z values
  Serial.print("a/g:\t");
  Serial.print(ax);
  Serial.print("\t");
  Serial.print(ay);
  Serial.print("\t");
  Serial.print(az);
  Serial.print("\t");
  Serial.print(gx);
  Serial.print("\t");
  Serial.print(gy);
  Serial.print("\t");
  Serial.println(gz);

  // blink LED to indicate activity
  blinkState = !blinkState;
  digitalWrite(LED_PIN, blinkState);*/
}

void blinkthread()
{
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  float temperature;
  float pressure;
  float altitude;
  while (1)
  {

    temperature = pressure_m.bmp085GetTemperature(); //MUST be called first
    pressure = pressure_m.bmp085GetPressure();
    altitude = pressure_m.calcAltitude(pressure);
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    /* digitalWrite(LED, HIGH);
    threads.delay(150);
    digitalWrite(LED, LOW);
    threads.delay(150);*/

    // threads.yield();
  }
}

void motorcontrol()
{
}

void controllLIDAR()
{
}

void readCommands()
{
}

/*
#include <Arduino.h>


const int LED = 13;

volatile int blinkcode = 0;

void blinkthread() {
  while(1) {
    if (blinkcode) {
      for (int i=0; i<blinkcode; i++) {
        digitalWrite(LED, HIGH);
        threads.delay(150);
        digitalWrite(LED, LOW);
        threads.delay(150);
      }
      blinkcode = 0;
    }
    threads.yield();
  }
}

void setup() {
  delay(1000);
  pinMode(LED, OUTPUT);
  threads.addThread(blinkthread);
}

int count = 0;

void loop() {
  count++;
  blinkcode = count;
  delay(5000);
}
*/

/*
#include <TeensyThreads.h>

volatile long int count = 0;

void thread_func(int inc) {
  while(1) count += inc;
}

void setup() {
  threads.addThread(thread_func, 1);
}

void loop() {
  Serial.println(count);
  threads.delay(1000);
}*/

/*
 * 
 * #include "SoftwareSerial.h"

#include "TFMini.h"


// Setup software serial port

SoftwareSerial mySerial(10, 11);      // Uno RX (TFMINI TX), Uno TX (TFMINI RX)

TFMini tfmini;

void setup() {

  // Step 1: Initialize hardware serial port (serial debug port)

  Serial.begin(115200);

  // wait for serial port to connect. Needed for native USB port only

  while (!Serial);

     

  Serial.println ("Initializing...");

  // Step 2: Initialize the data rate for the SoftwareSerial port

  mySerial.begin(TFMINI_BAUDRATE);

  delay(10);

  // Step 3: Initialize the TF Mini sensor

  tfmini.begin(&mySerial);    

}

void loop() {

  // Take one TF Mini distance measurement

  uint16_t dist = tfmini.getDistance();

  uint16_t strength = tfmini.getRecentSignalStrength();

  // Display the measurement

  Serial.print(dist);

  Serial.print(" cm      sigstr: ");

  Serial.println(strength);

  // Wait some short time before taking the next measurement

  delay(25);  

}
 * */