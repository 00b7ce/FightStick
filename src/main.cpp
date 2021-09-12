#include "FightStick.hpp"

// LED illumination
void timerLED(){
	static uint8_t hue = 0;
	leds.fill_rainbow(hue++);
}

// Initialize debouncer
void debouncer_init() {
	for (uint8_t i = 0; i < NUM_BUTTON_ALL; i++) {
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
void led_init() {
	FastLED.addLeds<LED_TYPE, LED_PIN>(leds, NUM_LEDS);
	FastLED.setBrightness(LED_BRIGHTNESS);
	FlexiTimer2::set(TIMER_INTERVAL, timerLED);
	FlexiTimer2::start();
}

void setup() {
	debouncer_init();
	joystick_init();
	led_init();
}

void loop() {

	uint8_t direction[2] = {AXIS_RANDE_HOME, AXIS_RANDE_HOME};

	while(1) {
		for (uint8_t i = 0; i < NUM_BUTTON_ALL; i++) {
			debouncer[i].update();
		}
		for (uint8_t i = 0; i < NUM_BUTTON_NORMAL; i++) {
			Joystick.setButton(i, !debouncer[i].read());
		}
		
		direction[AXIS_Y] = AXIS_RANDE_HOME - !debouncer[DIRECTION_UP].read()   + !debouncer[DIRECTION_DOWN].read();
		direction[AXIS_X] = AXIS_RANDE_HOME - !debouncer[DIRECTION_LEFT].read() + !debouncer[DIRECTION_RIGHT].read();
		
		if (!debouncer[LAYER_LS].read()) {
			Joystick.setYAxis(direction[AXIS_X]);
			Joystick.setXAxis(direction[AXIS_Y]);
			Joystick.setRzAxis(AXIS_RANDE_HOME);
			Joystick.setZAxis(AXIS_RANDE_HOME);
			Joystick.setHatSwitch(0, hatPattern[AXIS_RANDE_HOME][AXIS_RANDE_HOME]);
		}
		else if (!debouncer[LAYER_RS].read()) {
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