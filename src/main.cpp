#include <Arduino.h>
#include <Joystick.h>

#define JOYSTICK_BUTTON_COUNT 17

Joystick_ Joystick = Joystick_(
  JOYSTICK_DEFAULT_REPORT_ID,         // hidReportId 
  JOYSTICK_TYPE_GAMEPAD,              // joystickType 
  JOYSTICK_BUTTON_COUNT,              // buttonCount 
  JOYSTICK_DEFAULT_HATSWITCH_COUNT,   // hatSwitchCount 
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
}

void loop() {
}