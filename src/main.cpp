#include "FightStick.hpp"

uint8_t red = 39;
uint8_t green = 230;
uint8_t blue = 214;

CRGBPalette16 palette_color = {
	CRGB(green, red, blue),
	CRGB::White,
	CRGB(green, red, blue),
	CRGB(green, red, blue)
};

// LED illumination
void led_rainbow() {
	static uint8_t hue = 0;
	leds.fill_rainbow(hue++);
}

void led_solid() {
	leds.fill_solid(CRGB(green, red, blue));
}

void led_gradient() {
	static float offset = 0;
	for (uint16_t i = 0; i < NUM_LEDS; i++) {
		leds[i] = ColorFromPalette(palette_color, (i * 255) / NUM_LEDS + offset);
		offset += 0.2;
	}
}

uint8_t led_mode = 0;
void timerLED(){
	switch (led_mode) {
	case 0:
		led_rainbow();
		break;
	case 1:
		led_solid();
		break;
	case 2:
		led_gradient();
		break;	
	case 3:
		break;
	default:
		break;
	}
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
	uint8_t layer = 0;

	while(1) {
		for (uint8_t i = 0; i < NUM_BUTTON_ALL; i++) {
			debouncer[i].update();
		}
		for (uint8_t i = 0; i < NUM_BUTTON_NORMAL; i++) {
			Joystick.setButton(i, !debouncer[i].read());
		}
		
		direction[AXIS_Y] = AXIS_RANDE_HOME - !debouncer[DIRECTION_UP].read()   + !debouncer[DIRECTION_DOWN].read();
		direction[AXIS_X] = AXIS_RANDE_HOME - !debouncer[DIRECTION_LEFT].read() + !debouncer[DIRECTION_RIGHT].read();
		
		layer = !debouncer[LAYER_LS].read() + (!debouncer[LAYER_RS].read() << 1);

		switch (layer) {
		case 0:
			Joystick.setHatSwitch(0, hatPattern[direction[AXIS_X]][direction[AXIS_Y]]);
			Joystick.setYAxis(AXIS_RANDE_HOME);
			Joystick.setXAxis(AXIS_RANDE_HOME);
			Joystick.setRzAxis(AXIS_RANDE_HOME);
			Joystick.setZAxis(AXIS_RANDE_HOME);
			break;
		case 1:
			Joystick.setYAxis(direction[AXIS_X]);
			Joystick.setXAxis(direction[AXIS_Y]);
			Joystick.setRzAxis(AXIS_RANDE_HOME);
			Joystick.setZAxis(AXIS_RANDE_HOME);
			Joystick.setHatSwitch(0, hatPattern[AXIS_RANDE_HOME][AXIS_RANDE_HOME]);
			break;
		case 2:
			Joystick.setRzAxis(direction[AXIS_X]);
			Joystick.setZAxis(direction[AXIS_Y]);
			Joystick.setYAxis(AXIS_RANDE_HOME);
			Joystick.setXAxis(AXIS_RANDE_HOME);
			Joystick.setHatSwitch(0, hatPattern[AXIS_RANDE_HOME][AXIS_RANDE_HOME]);
			break;
		case 3:
			if (!debouncer[DIRECTION_UP].read())	led_mode = 0;
			if (!debouncer[DIRECTION_DOWN].read())	led_mode = 1;
			if (!debouncer[DIRECTION_LEFT].read())	led_mode = 2;
			if (!debouncer[DIRECTION_RIGHT].read())	led_mode = 3;
			if (!debouncer[0].read())
			break;
		default:
			break;
		}
		FastLED.show();
	}
}