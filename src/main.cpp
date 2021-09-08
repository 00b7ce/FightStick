#include <Arduino.h>
#include <Joystick.h>
#include <Bounce2.h>

// button assign const
const uint8_t buttonCount = 13;
const uint8_t directionCount = 4;
const uint8_t buttonPin[buttonCount] = {12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
const uint8_t directionPin[directionCount] = {20, 21, 22, 23};
const uint8_t lsPin = 14;
const uint8_t rsPin = 15;

// debounce const
Bounce buttonDebouncer[buttonCount];
Bounce directionDebouncer[directionCount];
const uint8_t debounceTime = 2;

Joystick_ Joystick = Joystick_(
  JOYSTICK_DEFAULT_REPORT_ID,       // hidReportId 
  JOYSTICK_TYPE_GAMEPAD,            // joystickType 
  buttonCount,              // buttonCount 
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
  for (uint8_t i = 0; i < buttonCount; i++) {
    buttonDebouncer[i] = Bounce();
    buttonDebouncer[i].attach(buttonPin[i], INPUT_PULLUP);
    buttonDebouncer[i].interval(debounceTime);
    buttonDebouncer[i].update();
  }

  for (uint8_t i = 0; i < directionCount; i++) {
    directionDebouncer[i] = Bounce();
    directionDebouncer[i].attach(directionPin[i], INPUT_PULLUP);
    directionDebouncer[i].interval(debounceTime);
    directionDebouncer[i].update();
  }

  pinMode(lsPin, INPUT_PULLUP);
  pinMode(rsPin, INPUT_PULLUP);

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
  while(1) {
    for (uint8_t i = 0; i < buttonCount; i++) {
      buttonDebouncer[i].update();
      Joystick.setButton(i, !buttonDebouncer[i].read());
    }
    for (uint8_t i = 0; i < directionCount; i++) {
      directionDebouncer[i].update();
    }
    // ToDo:方向入力の判定
  }
}