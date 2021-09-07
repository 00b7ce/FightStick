#include <Arduino.h>
#include <Joystick.h>

#define JOYSTICK_BUTTON_COUNT 13

Joystick_ Joystick = Joystick_(
  JOYSTICK_DEFAULT_REPORT_ID,         // hidReportId 
  JOYSTICK_TYPE_GAMEPAD,              // joystickType 
  JOYSTICK_BUTTON_COUNT,              // buttonCount 
  1,                                  // hatSwitchCount 
  true,                               // includeXAxis
  true,                               // includeYAxis 
  true,                               // includeZAxis 
  false,                              // includeRxAxis
  false,                              // includeRyAxis
  true,                               // includeRzAxis
  false,                              // includeRudder
  false,                              // includeThrottle
  false,                              // includeAccelerator
  false,                              // includeBrake
  false                               // includeSteering
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
}

void loop() {
  while(1){
  }
}