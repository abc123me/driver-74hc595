#include "595.h"

#define _INIT_MACRO(pin) if(pin > 0){\
	uint8_t res = tryInitGPIO(pin, 1);\
	if(res) return res;\
}

/* All functions follow this error code table
	0: Success
	1: Invalid pin (gpio_is_valid)
	2: Failed GPIO request (gpio_request)
	3: Failed to set direction (gpio_direction_output)
	4: Pointer to chip is null
	5: Invalid value
	255: Unknown
*/
char* getReason595(uint8_t error){
        if(error == 0) return "no error";
        if(error == 1) return "invalid pin";
        if(error == 2) return "gpio_request() failed";
        if(error == 3) return "gpio_direction_input/output() failed";
        if(error == 4) return "pointer to chip is null";
        if(error == 5) return "invalid value";
        return "unknown";
}
uint8_t tryInitGPIO(uint8_t pin, bool output){
	if(gpio_is_valid(pin)){
		char buf[9];
		snprintf(buf, 8, "gpio%i", pin);
		buf[8] = 0x00;
		if(gpio_request(pin, buf)) return 2;
		int res = output ? gpio_direction_output(pin, 0) : gpio_direction_input(pin);
		if(res) return 3;
	} else return 1;
	return 0;
}
uint8_t tryFreeGPIO(uint8_t pin){
	if(gpio_is_valid(pin)){
		gpio_free(pin);
	} else return 1;
	return 0;
}

uint8_t initSingle595(struct Chip74HC595* chip, uint8_t clock, uint8_t data, uint8_t latch){
	return init595(chip, clock, data, latch, 1);
}
uint8_t init595(struct Chip74HC595* chip, uint8_t clock, uint8_t data, uint8_t latch, uint8_t chainLength){
	if(chip == NULL) return 4;
	chip->clockPin = clock;
	chip->dataPin = data;
	chip->latchPin = latch;
	chip->chainLength = chainLength;

	_INIT_MACRO(chip->clockPin);
	_INIT_MACRO(chip->dataPin);
	_INIT_MACRO(chip->latchPin);
	return 0;
}

uint8_t free595(struct Chip74HC595* chip){
	if(chip == NULL) return 4;
	tryFreeGPIO(chip->clockPin);
	tryFreeGPIO(chip->dataPin);
	tryFreeGPIO(chip->latchPin);
	return 0;
}
uint8_t reset595(struct Chip74HC595* chip){
	if(chip == NULL) return 4;
	uint8_t i;
	for(i = 0; i < chip->chainLength; i++)
		writeb595(chip, 0);
	latch595(chip);
	return 0;
}
uint8_t writeb595(struct Chip74HC595* chip, uint8_t data){
	if(chip == NULL) return 4;
	int8_t i;
	for(i = 7; i >= 0; i--){
		uint8_t val = 0;
		if(data & (1 << i)) val = 255;
		gpio_set_value(chip->dataPin, val);
		clock595(chip);
	}
	return 0;
}
uint8_t write595(struct Chip74HC595* chip, uint8_t* data, uint8_t len){
	if(chip == NULL) return 4;
	uint8_t i;
	for(i = 0; i < len; i++)
		writeb595(chip, data[i]);
	return 0;
}
void clock595(struct Chip74HC595* chip){
	gpio_set_value(chip->clockPin, 1);
	ndelay(chip->delay);
	gpio_set_value(chip->clockPin, 0);
	ndelay(chip->delay);
}
void latch595(struct Chip74HC595* chip){
	gpio_set_value(chip->latchPin, 1);
	ndelay(chip->delay * 2);
	gpio_set_value(chip->latchPin, 0);
	ndelay(chip->delay * 2);
}
uint8_t setSpeed595(struct Chip74HC595* chip, uint32_t delay){
	if(!chip)
		return 4;
	chip->delay = delay;
	return 0;
}
