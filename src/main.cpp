#include "FightStick.hpp"

// LED illumination
void led_rainbow() {
	static uint8_t hue = 0;
	fill_rainbow(leds, NUM_LEDS, hue++);
}

void led_solid() {
	fill_solid(leds, NUM_LEDS, CHSV(ledParam.ih, ledParam.is, ledParam.iv));
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
	fill_solid(leds, NUM_LEDS, CHSV(ledParam.ih, ledParam.is, dV));
}

void timerLED(){
	switch (ledParam.mode) {
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
		led_solid();
		break;
	}
}

void set_direction(const int8_t& mode, uint8_t* direction) {

	int16_t hat = hatPattern[AXIS_RANGE_HOME][AXIS_RANGE_HOME];
	uint8_t axis[4] = {AXIS_RANGE_HOME, AXIS_RANGE_HOME, AXIS_RANGE_HOME, AXIS_RANGE_HOME};

	switch (mode) {
	case 0:
		hat = hatPattern[direction[AXIS_Y]][direction[AXIS_X]];
		break;
	case 1:
		axis[AXIS_X] = direction[AXIS_X];
		axis[AXIS_Y] = direction[AXIS_Y];
		break;
	case 2:
		axis[AXIS_Z]  = direction[AXIS_X];
		axis[AXIS_RZ] = direction[AXIS_Y];
		break;
	default:
		break;
	}
	Joystick.setHatSwitch(0, hat);
	Joystick.setXAxis(axis[AXIS_X]);
	Joystick.setYAxis(axis[AXIS_Y]);
	Joystick.setZAxis(axis[AXIS_Z]);
	Joystick.setRzAxis(axis[AXIS_RZ]);
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
	set_direction(-1, NULL);
}

// Initialize LED
void led_init() {
	FastLED.addLeds<LED_TYPE, LED_PIN>(leds, NUM_LEDS);
	FastLED.setBrightness(LED_BRIGHTNESS);
	EEPROM.get(EEPROM_ADDR_LEDPARAM, ledParam);
	currentPalette = GENERATE_PALETTE(ledParam.ih, ledParam.is, ledParam.iv);
}

void setup() {
	debouncer_init();
	joystick_init();
	led_init();
}

void setting_led() {
	while (!debouncer[LAYER_LS].read() || !debouncer[LAYER_RS].read()) {
		for (uint8_t i = 0; i < NUM_BUTTON_ALL; i++) {
			debouncer[i].update();
		}
		EVERY_N_MILLIS(10) {
			if (!debouncer[DIRECTION_UP].read())								ledParam.mode = LEDMODE_RAINBOW;
			if (!debouncer[DIRECTION_DOWN].read())								ledParam.mode = LEDMODE_SOLID;
			if (!debouncer[DIRECTION_LEFT].read())								ledParam.mode = LEDMODE_GRADIENT;
			if (!debouncer[DIRECTION_RIGHT].read())								ledParam.mode = LEDMODE_BREATH;
			if (!debouncer[LEDSETTING_HUE_MINUS].read())						ledParam.ih--;
			if (!debouncer[LEDSETTING_HUE_PLUS].read())							ledParam.ih++;
			if (!debouncer[LEDSETTING_SAT_MINUS].read() && ledParam.is >   0)	ledParam.is--;
			if (!debouncer[LEDSETTING_SAT_PLUS].read()  && ledParam.is < 255)	ledParam.is++;
			if (!debouncer[LEDSETTING_VAL_MINUS].read() && ledParam.iv >   0)	ledParam.iv--;
			if (!debouncer[LEDSETTING_VAL_PLUS].read()  && ledParam.iv < 255)	ledParam.iv++;
		}
		currentPalette = GENERATE_PALETTE(ledParam.ih, ledParam.is, ledParam.iv);
		EVERY_N_MILLIS(TIMER_INTERVAL) {
			timerLED();
			FastLED.show();
		}
	}
	EEPROM.put(EEPROM_ADDR_LEDPARAM, ledParam);
}

void loop() {

	uint8_t direction[2] = {AXIS_RANGE_HOME, AXIS_RANGE_HOME};
	int8_t  layer = 0;

	while(1) {
		for (uint8_t i = 0; i < NUM_BUTTON_ALL; i++) {
			debouncer[i].update();
			if (i < NUM_BUTTON_NORMAL)  Joystick.setButton(i, !debouncer[i].read());
		}

		direction[AXIS_Y] = (debouncer[DIRECTION_UP].read() && !debouncer[DIRECTION_DOWN].read()) + debouncer[DIRECTION_UP].read();
		direction[AXIS_X] = debouncer[DIRECTION_LEFT].read() + !debouncer[DIRECTION_RIGHT].read();
		layer = !debouncer[LAYER_LS].read() + (!debouncer[LAYER_RS].read() << 1);
		
		switch (layer) {
		case 3:
			setting_led();
			break;
		default:
			set_direction(layer, direction);
			break;
		}
		EVERY_N_MILLIS(TIMER_INTERVAL) {
			timerLED();
			FastLED.show();
		}
	}
}