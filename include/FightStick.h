#ifndef INCLUDED_FIGHTSTICK_H_
#define INCLUDED_FIGHTSTICK_H_
#include <Arduino.h>
#include <Joystick.h>
#include <Bounce2.h>
#include <FastLED.h>
#include <FlexiTimer2.h>

// Button num
#define BUTTON_NORMAL 13        // Number of normal input button num(Recognized as a gamepad button)
#define BUTTON_DIRECTION 4      // Number of direction input button. almost always 4(up, down, left, right)
#define BUTTON_LAYER 2          // Layer button that toggles the directional input type while pressing. default:2(Left and right stick)
#define BUTTON_ALL (BUTTON_NORMAL + BUTTON_DIRECTION + BUTTON_LAYER)
// Debounce
#define DEBOUNCE_TIME 2         
// LED
#define NUM_LEDS 20             // Number of LEDs of using LED strip
#define LED_PIN  16             // Pin for LED control
#define LED_BRIGHTNESS 33       // Brightness of LEDs
#define LED_TYPE WS2813         // Type of using LEDs
#define TIMER_INTERVAL 25       // Interval for LED color control

// Axis range 
enum {
	AXIS_RANGE_MIN,
	AXIS_RANDE_HOME,
	AXIS_RANGE_MAX
};
// Specific button number in array
enum {
	DIRECTION_LEFT = BUTTON_NORMAL,
	DIRECTION_DOWN,
	DIRECTION_RIGHT,
	DIRECTION_UP,
	LAYER_LS,
	LAYER_RS
};
// Input axis
enum {
	AXIS_Y,
	AXIS_X
};

// Pin assignment settings for all buttons.
// Normal button(1), ... , Normal button(n), Direction button(1), ... , Direction button(n), Layer button(1), ... Layer button(n)
const uint8_t buttonPin[BUTTON_ALL] = {12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 20, 21, 22, 23, 14, 15};

// Hatswitch direction Pattern
const int16_t hatPattern[3][3] = {{315,   0,  45},
								  {270,  -1,  90},
								  {225, 180, 135}};

Bounce debouncer[BUTTON_ALL];

// Joystick descriptor setting
Joystick_ Joystick = Joystick_(
	JOYSTICK_DEFAULT_REPORT_ID,       // hidReportId 
	JOYSTICK_TYPE_GAMEPAD,            // joystickType 
	BUTTON_NORMAL,                    // buttonCount 
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

CRGBArray<NUM_LEDS> leds;

#endif