
#ifndef pins_h
#define pins_h

/** Motor left */
#define PIN_ENABLE_LEFT 4
#define PIN_MOTOR_LEFT_A 5
#define PIN_MOTOR_LEFT_B 6

/** Motor right*/
#define PIN_ENABLE_RIGHT 3
#define PIN_MOTOR_RIGHT_A 11
#define PIN_MOTOR_RIGHT_B 12

/** LIDAR Stepper Motor */
#define PIN_STEPPER_0 A0 // (D14)
#define PIN_STEPPER_1 A1 // (D15)
#define PIN_STEPPER_2 A2 // (D16)
#define PIN_STEPPER_3 A3 // (D17)

/** Fan */
#define PIN_FAN A6 // (D20)

/** Zero Position Sensor */
#define PIN_ZEROING_SENSOR 13

/** DS18B20 Temperature Sensor - OneWire */
#define PIN_MOTOR_TEMPERATURE 2 //ADDRES: R=28 61 64 11 BD 93 D5 1

/** Analog Distance Sensor */
#define PIN_ANALOG_DISTANCE A9 // (D23)

/** TF MINI Serial Port (Serial 1) */
#define PIN_RX_TFMINI 0
#define PIN_TX_TFMINI 1

/** Port for I2C Communication to Raspberry PI - BUS 1*/
#define I2C_PI_SCL A18 // (D29)
#define I2c_PI_SDA A19 // (D30)

/** Port for I2C Communication to peripherals, e.g. GY-88 - BUS 0 */
#define I2C_PERIPHERY_SCL A5 // (D19)
#define I2c_PERIPHERY_SDA A4 // (D18)

#endif
