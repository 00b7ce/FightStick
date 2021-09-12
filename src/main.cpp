#include <Arduino.h>
#include <Joystick.h>
#include <Bounce2.h>
#include <FastLED.h>
#include <FlexiTimer2.h>

// Button num
#define JOYSTICK_BUTTON_COUNT     13  // Normal input button
#define JOYSTICK_DIRECTIRON_COUNT  4  // Direction input button(up, down, left, right)
#define JOYSTICK_LAYER_COUNT       2  // Layer button for switching direction input type

// Axis range
#define AXIS_RANGE_MIN  0
#define AXIS_RANDE_HOME 1
#define AXIS_RANGE_MAX  2

// Direction button
#define DIRECTION_LEFT  (JOYSTICK_BUTTON_COUNT + 0)
#define DIRECTION_DOWN  (JOYSTICK_BUTTON_COUNT + 1)
#define DIRECTION_RIGHT (JOYSTICK_BUTTON_COUNT + 2)
#define DIRECTION_UP    (JOYSTICK_BUTTON_COUNT + 3)

#define AXIS_X 1
#define AXIS_Y 0

// Debounce
#define DEBOUNCE_TIME 2

// LED
#define NUM_LEDS 20
#define LED_PIN  16
#define LED_BRIGHTNESS 33
#define LED_TYPE WS2813
#define TIMER_INTERVAL 25

// button assign const
const uint8_t buttonCount = JOYSTICK_BUTTON_COUNT + JOYSTICK_DIRECTIRON_COUNT + JOYSTICK_LAYER_COUNT;
const uint8_t buttonPin[buttonCount] = {12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 20, 21, 22, 23, 14, 15};
const uint8_t layerLS = JOYSTICK_BUTTON_COUNT + JOYSTICK_DIRECTIRON_COUNT;
const uint8_t layerRS = JOYSTICK_BUTTON_COUNT + JOYSTICK_DIRECTIRON_COUNT + 1;
// Hatswitch direction Pattern
const int16_t hatPattern[3][3] = {{315,   0,  45},
								  								{270,  -1,  90},
								  								{225, 180, 135}};

// debounce const
Bounce debouncer[buttonCount];

// Joystick
Joystick_ Joystick = Joystick_(
	JOYSTICK_DEFAULT_REPORT_ID,       // hidReportId 
	JOYSTICK_TYPE_GAMEPAD,            // joystickType 
	JOYSTICK_BUTTON_COUNT,            // buttonCount 
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
uint8_t hue = 0;

// LED illumination
void timerLED(){
	leds.fill_rainbow(hue++);
}

// Initialize debouncer
void debouncer_init() {
	for (uint8_t i = 0; i < buttonCount; i++) {
		debouncer[i] = Bounce();
		debouncer[i].attach(buttonPin[i], INPUT_PULLUP);
		debouncer[i].interval(DEBOUNCE_TIME);
	}
}

// Initialize joystick
void joystick_init() {
	Joystick.begin();
	Joystick.setXAxisRange(AXIS_RANGE_MIN, AXIS_RANGE_MAX);
	Joystick.setYAxisRange(AXIS_RANGE_MIN, AXIS_RANGE_MAX);
	Joystick.setZAxisRange(AXIS_RANGE_MIN, AXIS_RANGE_MAX);
	Joystick.setRzAxisRange(AXIS_RANGE_MIN, AXIS_RANGE_MAX);
	Joystick.setXAxis(AXIS_RANDE_HOME);
	Joystick.setYAxis(AXIS_RANDE_HOME);
	Joystick.setZAxis(AXIS_RANDE_HOME);
	Joystick.setRzAxis(AXIS_RANDE_HOME);
	Joystick.setHatSwitch(0, hatPattern[AXIS_RANDE_HOME][AXIS_RANDE_HOME]);
}

// Initialize LED
void led_initialize() {
	FastLED.addLeds<LED_TYPE, LED_PIN>(leds, NUM_LEDS);
	FastLED.setBrightness(LED_BRIGHTNESS);
	FlexiTimer2::set(TIMER_INTERVAL, timerLED);
	FlexiTimer2::start();
}

void setup() {
	debouncer_init();
	joystick_init();
	led_initialize();
}

void loop() {

	uint8_t direction[2] = {AXIS_RANDE_HOME, AXIS_RANDE_HOME};

	while(1) {
		for (uint8_t i = 0; i < buttonCount; i++) {
			debouncer[i].update();
		}
		for (uint8_t i = 0; i < JOYSTICK_BUTTON_COUNT; i++) {
			Joystick.setButton(i, !debouncer[i].read());
		}
		
		direction[AXIS_Y] = AXIS_RANDE_HOME - !debouncer[DIRECTION_UP].read()   + !debouncer[DIRECTION_DOWN].read();
		direction[AXIS_X] = AXIS_RANDE_HOME - !debouncer[DIRECTION_LEFT].read() + !debouncer[DIRECTION_RIGHT].read();
		
		if (!debouncer[layerLS].read()) {
			Joystick.setYAxis(direction[AXIS_X]);
			Joystick.setXAxis(direction[AXIS_Y]);
			Joystick.setRzAxis(AXIS_RANDE_HOME);
			Joystick.setZAxis(AXIS_RANDE_HOME);
			Joystick.setHatSwitch(0, hatPattern[AXIS_RANDE_HOME][AXIS_RANDE_HOME]);
		}
		else if (!debouncer[layerRS].read()) {
			Joystick.setRzAxis(direction[AXIS_X]);
			Joystick.setZAxis(direction[AXIS_Y]);
			Joystick.setYAxis(AXIS_RANDE_HOME);
			Joystick.setXAxis(AXIS_RANDE_HOME);
			Joystick.setHatSwitch(0, hatPattern[AXIS_RANDE_HOME][AXIS_RANDE_HOME]);
		}
		else {
			Joystick.setHatSwitch(0, hatPattern[direction[AXIS_X]][direction[AXIS_Y]]);
			Joystick.setYAxis(AXIS_RANDE_HOME);
			Joystick.setXAxis(AXIS_RANDE_HOME);
			Joystick.setRzAxis(AXIS_RANDE_HOME);
			Joystick.setZAxis(AXIS_RANDE_HOME);
		}
		FastLED.show();
	}
}