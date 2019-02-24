#ifndef _595_H
#define _595_H

#include "linux/gpio.h"
#include "linux/delay.h"

enum Error595{
	E_Success, E_InvalidPin, E_InvalidValue, E_NullPointer, E_RequestFailed, E_SetDirectionFailed
};
typedef struct Chip74HC595{
	uint8_t latchPin, clockPin, dataPin;
	uint8_t chainLength;
	int16_t resetPin, outputEnablePin;
	uint32_t delay;
};

char* getReason595(uint8_t error);
uint8_t tryInitGPIO(uint8_t pin, bool output);
uint8_t tryFreeGPIO(uint8_t pin);
uint8_t init595(struct Chip74HC595* chip, uint8_t clock, uint8_t data, uint8_t latch, uint8_t chainLength);
uint8_t initSingle595(struct Chip74HC595* chip, uint8_t clock, uint8_t data, uint8_t latch);
uint8_t setResetPin595(struct Chip74HC595* chip, uint8_t resetPin);
uint8_t free595(struct Chip74HC595* chip);
uint8_t setSpeed595(struct Chip74HC595* chip, uint32_t delay);
uint8_t write595(struct Chip74HC595* chip, uint8_t* dat, uint8_t len);
uint8_t writeb595(struct Chip74HC595* chip, uint8_t dat);
uint8_t reset595(struct Chip74HC595* chip);
void latch595(struct Chip74HC595* chip);
void clock595(struct Chip74HC595* chip);

#endif
