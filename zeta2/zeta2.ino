#include <Wire.h>
#include <TeensyThreads.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "pins.h"
#include "stepper.h"
#include "motor.h"
#include "fan.h"
#include "BMP085.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "TFMini.h"

BMP085 pressure_m;
MPU6050 accelgyro;
TFMini tfmini;

Stepper lidarStepper(PIN_STEPPER_0, PIN_STEPPER_1, PIN_STEPPER_2, PIN_STEPPER_3);
Motor motorLeft(PIN_MOTOR_LEFT_A, PIN_MOTOR_LEFT_B, PIN_ENABLE_LEFT);
Motor motorRight(PIN_MOTOR_RIGHT_A, PIN_MOTOR_RIGHT_B, PIN_ENABLE_RIGHT);
Fan fan(PIN_FAN);

OneWire ds(PIN_MOTOR_TEMPERATURE);
DallasTemperature sensors(&ds);
DeviceAddress insideThermometer = {0x28, 0x61, 0x64, 0x11, 0xBD, 0x93, 0xD5, 0x1};

/*
gelb: gyp-distance-sensor
braun: zero-position-sensor
grün: grün von tfmini distance-sensor tx
orange: weiß von tfmini distance-sensor rx
*/

/*
 1,1 ma alle 4motoren+stepper+fan
23ma teensy
*/
byte addr[8];
void setup()
{
  Serial.begin(9600);    // debug serial
  Serial1.begin(115200); // TFMINI serial
  Wire.begin();          // I2C

  // wait till serial is here
  while (!Serial && !Serial1)
  {
  }

  tfmini.begin(&Serial1);

  /**
   *  configure OUTPUT pins
   */
  fan.begin();
  lidarStepper.begin();
  motorLeft.begin();
  motorRight.begin();

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
  // threads.addThread(blinkthread);
  // threads.addThread(stepMotorTest);
  /*
  ds.search(addr);

  Serial.print("R=");
  for (int i = 0; i < 8; i++)
  {
    Serial.print(addr[i], HEX);
    Serial.print(" ");
  }

  if (OneWire::crc8(addr, 7) != addr[7])
  {
    Serial.print("CRC is not valid!\n");
    return;
  }

  ds.select(addr);
  ds.write(0x44, 1); // start conversion, with parasite power on at the end
  delay(1000);
  ds.reset();
  ds.select(addr);
  ds.write(0xBE); // Read Scratchpad
  */
  sensors.begin();
  sensors.setResolution(insideThermometer, 9);
  if (sensors.isParasitePowerMode())
    Serial.println("Dallas ParasitePower: ON");
  else
    Serial.println("Dallas ParasitePower: OFF");
  if (!sensors.getAddress(insideThermometer, 0))
    Serial.println("Unable to find address for Device 0");
}

void loop()
{
  lidarStepper.next();
  delay(1);

  /*Serial.print(" Requesting temperatures...");
  int x = millis();
  sensors.requestTemperatures(); // Send the command to get temperature readings
  int x2 = millis();

  Serial.print("Temperature is: ");
  Serial.print(sensors.getTempC(insideThermometer)); //sensors.getTempCByIndex(0)); // Why "byIndex"?
  Serial.print("    time:");
  Serial.print(x2 - x);
  // You can have more than one DS18B20 on the same bus.
  // 0 refers to the first IC on the wire

  Serial.println("  DONE");
  delay(1000);
  */
}

void stepMotorTest()
{
  while (1)
  {
    Serial.print(millis());
    Serial.print(" ");
    Serial.print("R=");
    for (int i = 0; i < 8; i++)
    {
      Serial.print(addr[i], HEX);
      Serial.print(" ");
    }

    Serial.println(" ");
    delay(1000);
    threads.delay(50);

    /*  for(int i=0;i<256;i++){
    lidarStepper.next();
    delay(1);
    }*/
    /*  Serial.print(millis());
    Serial.print("STEP ");
   
    Serial.print(lidarStepper.currentStep);*/
  }
}

void blinkthread()
{

  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  float temperature;
  float pressure;
  float altitude;
  byte data[12];

  while (1)
  {
    Serial.print("P=");
    for (int i = 0; i < 9; i++)
    { // we need 9 bytes
      data[i] = ds.read();
      Serial.print(data[i], HEX);
      Serial.print(" ");
    }
    Serial.print(" CRC=");
    Serial.print(OneWire::crc8(data, 8), HEX);
    Serial.println();

    temperature = pressure_m.bmp085GetTemperature(); //MUST be called first
    pressure = pressure_m.bmp085GetPressure();
    altitude = pressure_m.calcAltitude(pressure);
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    motorLeft.set(255);
    motorRight.set(255);
    fan.update(1000);

    Serial.print("ZEROING: ");
    Serial.print(digitalRead(PIN_ZEROING_SENSOR));
    Serial.println();

    /* digitalWrite(LED, HIGH);
    threads.delay(150);
    digitalWrite(LED, LOW);
    threads.delay(150);*/

    // threads.yield();

    uint16_t dist = tfmini.getDistance();
    uint16_t strength = tfmini.getRecentSignalStrength();
    // Display the measurement
    Serial.print(dist);
    Serial.print(" cm      sigstr: ");
    Serial.println(strength);

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

    threads.delay(500);
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
