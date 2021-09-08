#include <Arduino.h>
#include <Joystick.h>

const uint8_t joystickButtonCount = 13;
const uint8_t buttonPin[joystickButtonCount] = {12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
const uint8_t arrowPin[4] = {20, 21, 22, 23};
const uint8_t lsPin = 14;
const uint8_t rsPin = 15;

Joystick_ Joystick = Joystick_(
  JOYSTICK_DEFAULT_REPORT_ID,       // hidReportId 
  JOYSTICK_TYPE_GAMEPAD,            // joystickType 
  joystickButtonCount,              // buttonCount 
  1,                                // hatSwitchCount 
  true,                             // includeXAxis
  true,                             // includeYAxis 
  true,                             // includeZAxis 
  false,                            // includeRxAxis
  false,                            // includeRyAxis
  true,                             // includeRzAxis
  false,                            // includeRudder
  false,                            // includeThrottle
  false,                            // includeAccelerator
  false,                            // includeBrake
  false                             // includeSteering
);

void setup() {
  Joystick.begin();
  Joystick.setXAxisRange(0, 2);
  Joystick.setYAxisRange(0, 2);
  Joystick.setZAxisRange(0, 2);
  Joystick.setRzAxisRange(0, 2);
  Joystick.setXAxis(1);
  Joystick.setYAxis(1);
  Joystick.setZAxis(1);
  Joystick.setRzAxis(1);
  Joystick.setHatSwitch(0, -1);
}

void loop() {
  while(1){
  }
}