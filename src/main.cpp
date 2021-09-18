#include "FightStick.hpp"

uint8_t ih = 174;
uint8_t is = 211;
uint8_t iv = 230;

CRGBPalette16 defaultPalette = {
	CHSV(ih, is, iv),
	CHSV( 0,  0, iv),
	CHSV(ih, is, iv),
	CHSV(ih, is, iv)
};

CRGBPalette16 currentPalette = defaultPalette;

// LED illumination
void led_rainbow() {
	static uint8_t hue = 0;
	fill_rainbow(leds, NUM_LEDS, hue++);
}

void led_solid() {
	fill_solid(leds, NUM_LEDS, CHSV(ih, is, iv));
}

void led_gradient() {
	static float offset = 0;
	for (uint8_t i = 0; i < NUM_LEDS; i++) {
		leds[i] = ColorFromPalette(currentPalette, (i * 255) / NUM_LEDS + offset);
		offset += 0.2;
	}
}

void led_breath() {
	float dV = ((exp(sin(millis()/2000.0*PI)) -0.36787944) * 108.0);
	fill_solid(leds, NUM_LEDS, CHSV(ih, is, dV));
}

uint8_t ledMode = LEDMODE_BREATH;
void timerLED(){
	switch (ledMode) {
	case LEDMODE_RAINBOW:
		led_rainbow();
		break;
	case LEDMODE_SOLID:
		led_solid();
		break;
	case LEDMODE_GRADIENT:
		led_gradient();
		break;	
	case LEDMODE_BREATH:
		led_breath();
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
	Joystick.setXAxis(AXIS_RANGE_HOME);
	Joystick.setYAxis(AXIS_RANGE_HOME);
	Joystick.setZAxis(AXIS_RANGE_HOME);
	Joystick.setRzAxis(AXIS_RANGE_HOME);
	Joystick.setHatSwitch(0, hatPattern[AXIS_RANGE_HOME][AXIS_RANGE_HOME]);
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

	uint8_t direction[2] = {AXIS_RANGE_HOME, AXIS_RANGE_HOME};
	uint8_t layer = 0;

	while(1) {
		for (uint8_t i = 0; i < NUM_BUTTON_ALL; i++) {
			debouncer[i].update();
		}
		for (uint8_t i = 0; i < NUM_BUTTON_NORMAL; i++) {
			Joystick.setButton(i, !debouncer[i].read());
		}
		
		direction[AXIS_Y] = (debouncer[DIRECTION_UP].read() && !debouncer[DIRECTION_DOWN].read()) + debouncer[DIRECTION_UP].read();
		direction[AXIS_X] = debouncer[DIRECTION_LEFT].read() + !debouncer[DIRECTION_RIGHT].read();
		
		layer = !debouncer[LAYER_LS].read() + (!debouncer[LAYER_RS].read() << 1);

		switch (layer) {
		case 0:
			Joystick.setHatSwitch(0, hatPattern[direction[AXIS_Y]][direction[AXIS_X]]);
			Joystick.setYAxis(AXIS_RANGE_HOME);
			Joystick.setXAxis(AXIS_RANGE_HOME);
			Joystick.setRzAxis(AXIS_RANGE_HOME);
			Joystick.setZAxis(AXIS_RANGE_HOME);
			break;
		case 1:
			Joystick.setYAxis(direction[AXIS_Y]);
			Joystick.setXAxis(direction[AXIS_X]);
			Joystick.setRzAxis(AXIS_RANGE_HOME);
			Joystick.setZAxis(AXIS_RANGE_HOME);
			Joystick.setHatSwitch(0, hatPattern[AXIS_RANGE_HOME][AXIS_RANGE_HOME]);
			break;
		case 2:
			Joystick.setRzAxis(direction[AXIS_Y]);
			Joystick.setZAxis(direction[AXIS_X]);
			Joystick.setYAxis(AXIS_RANGE_HOME);
			Joystick.setXAxis(AXIS_RANGE_HOME);
			Joystick.setHatSwitch(0, hatPattern[AXIS_RANGE_HOME][AXIS_RANGE_HOME]);
			break;
		case 3:
			if (!debouncer[DIRECTION_UP].read())	ledMode = LEDMODE_RAINBOW;
			if (!debouncer[DIRECTION_DOWN].read())	ledMode = LEDMODE_SOLID;
			if (!debouncer[DIRECTION_LEFT].read())	ledMode = LEDMODE_GRADIENT;
			if (!debouncer[DIRECTION_RIGHT].read())	ledMode = LEDMODE_BREATH;
			if (!debouncer[LEDSETTING_HUE_PLUS].read())  ih--;
			if (!debouncer[LEDSETTING_HUE_MINUS].read()) ih++;
			if (!debouncer[LEDSETTING_SAT_PLUS].read())  is--;
			if (!debouncer[LEDSETTING_SAT_MINUS].read()) is++;
			if (!debouncer[LEDSETTING_VAL_PLUS].read())  iv--;
			if (!debouncer[LEDSETTING_VAL_MINUS].read()) iv++;
			currentPalette = CRGBPalette16(CHSV(ih, is, iv), CHSV(0, 0, iv), CHSV(ih, is, iv), CHSV(ih, is, iv));
			break;
		default:
			break;
		}
		FastLED.show();
	}
}