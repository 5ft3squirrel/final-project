//a damn lot of libraries
#include <Wire.h>
//#include <Pushbutton.h>
//#include <Zumo32U4ProximitySensors.h>
#include <Zumo32U4Encoders.h>
//#include <Zumo32U4IRPulses.h>
//#include <PololuBuzzer.h>
//#include <FastGPIO.h>
#include <Zumo32U4Motors.h>
#include <LSM303.h>
#include <Zumo32U4.h>
#include <Zumo32U4LCD.h>
#include <TurnSensor.h>
//#include <Zumo32U4LineSensors.h>
#include <PololuHD44780.h>
//#include <USBPause.h>
#include <Zumo32U4Buttons.h>
#include <L3G.h>
#include <Zumo32U4Buzzer.h>
#include <QTRSensors.h>
#include <AccelStepper.h>
#include <SPI.h>

// YL: the code below is modified from https://github.com/pvcraven/zumo_32u4_examples/blob/master/GyroSensorExample/GyroSensorExample.ino
// and https://github.com/pvcraven/zumo_32u4_examples/blob/master/TurnExample/TurnExample.ino
// and https://github.com/pvcraven/zumo_32u4_examples/blob/master/MotorEncoders/MotorEncoders.ino

L3G gyro;
Zumo32U4LCD lcd;
Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;
Zumo32U4Encoders encoders;

int turnSpeed = 150;
int motorSpeed = 250;
int x = 10;

// --- Setup Method
void setup() {
  buttonA.waitForButton();
  delay(100);
  turnSensorSetup();
  delay(500);
  turnSensorReset();
}

void turnLeft(int degrees) {
  turnSensorReset();
  motors.setSpeeds(-turnSpeed, turnSpeed);
  int angle = 0;
  do {
    delay(1);
    turnSensorUpdate();
    angle = (((int32_t)turnAngle >> 16) * 360) >> 16;
    lcd.gotoXY(0, 0);
    lcd.print(angle);
    lcd.print(" ");
  } while (angle < degrees);
  motors.setSpeeds(0, 0);
}

// Turn right
void turnRight(int degrees) {
  turnSensorReset();
  motors.setSpeeds(turnSpeed, -turnSpeed);
  int angle = 0;
  do {
    delay(1);
    turnSensorUpdate();
    angle = (((int32_t)turnAngle >> 16) * 360) >> 16;
    lcd.gotoXY(0, 0);
    lcd.print(angle);
    lcd.print(" ");
  } while (angle > -degrees);
  motors.setSpeeds(0, 0);
}

void forward(long count) {
  encoders.getCountsAndResetLeft();
  encoders.getCountsAndResetRight();
  long countsLeft = 0;
  long countsRight = 0;
  motors.setSpeeds(motorSpeed, motorSpeed);
  while(countsLeft < count) {
    countsLeft += encoders.getCountsAndResetLeft();
    countsRight += encoders.getCountsAndResetRight();
    lcd.gotoXY(0, 1);
    lcd.print(countsLeft);
    lcd.print(" ");
    delay(2);
  };
  motors.setSpeeds(0, 0);
}

void reverse(long count) {
  encoders.getCountsAndResetLeft();
  encoders.getCountsAndResetRight();
  long countsLeft = 0;
  long countsRight = 0;
  motors.setSpeeds(-motorSpeed, -motorSpeed);
  while(countsLeft < count) {
    countsLeft -= encoders.getCountsAndResetLeft();
    countsRight -= encoders.getCountsAndResetRight();
    lcd.gotoXY(0, 1);
    lcd.print(countsLeft);
    lcd.print(" ");
    delay(2);
  };
  motors.setSpeeds(0, 0);
}

void loop() {
  // Read the sensors
  turnSensorUpdate();
  int angle = (((int32_t)turnAngle >> 16) * 360) >> 16;
  lcd.gotoXY(0, 0);
  lcd.print(angle);
  lcd.print(" ");
  //if we press A, then it starts:
  bool buttonPress = buttonA.getSingleDebouncedPress();
  if (buttonPress) {
     delay(5000);
     // because 0.0927in
     forward(927/x);
     delay(1000);
     turnLeft(96);
     delay(1000);
     forward(580/x);
     delay(1000);
     turnLeft(45);
     delay(1000);
     forward(710/x);
     turnRight(21);
     delay(1000);
     forward(1474/x);
      //should be pupper head and front limbs  
     delay(1000);
     turnRight(180);
     forward(1474/x);
     delay(1000);
     //go back
     turnLeft(76);
     delay(1000);
     forward(2899/x);
     delay(1000);
     turnRight(76);
     delay(1000);
     forward(1229/x);
     delay(1000);
     turnRight(180);
     delay(1000);
     forward(1229/x);
     delay(1000);
     turnRight(93);
     delay(1000);
     forward(918/x);     
}
}
