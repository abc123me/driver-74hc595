#include "stdlib.h"
#include "stdint.h"
#include "stdio.h"
#include "sys/ioctl.h"
#include "../ioctl_595.h"

uint8_t resetTest();
uint8_t chainLengthTest();
uint8_t automaticLatchTest(uint8_t);
uint8_t manualLatchTest();

FILE* fp = NULL;

int main(){
	puts("\e[1;33m------------------------");
	puts("IOCTL tests!");
	puts("------------------------\e[0m");

	fp = fopen("/dev/chip74hc595", "w");
	if(fp == NULL){
		printf("Failed to open device\n");
		printf("\e[1;31mReset test failed!\e[0m\n");
		return 1;
	}
	setbuf(fp, 0);

	if(resetTest()) goto err;
	if(chainLengthTest()) goto err;
	if(automaticLatchTest(0)) goto err;
	if(automaticLatchTest(1)) goto err;
	if(manualLatchTest()) goto err;

err:
	fclose(fp);
	return 0;
}

uint8_t automaticLatchTest(uint8_t en) {
	char* enstr = en ? "enable" : "disable";
	printf("\e[1;33mIOCTL Automatic latch %s test!\e[0m\n", enstr);
	uint8_t latch = 0;
	int res = ioctl(fp->_fileno, IOCTL_SET_AUTO_LATCH, latch);
	if(res) {
		printf("\e[1;31mioctl() failed with exit code %i!\e[0m\n", res);
		printf("\e[1;31mAutomatic latch %s test failed!\e[0m\n", enstr);
		return 1;
	} else printf("\e[1;32mAutomatic latch %s test passed!\e[0m\n", enstr);
	return 0;
}
uint8_t manualLatchTest() {
	puts("\e[1;33mIOCTL Manual latching test!\e[0m");
	int res = ioctl(fp->_fileno, IOCTL_MANUAL_LATCH);
	if(res) {
		printf("\e[1;31mioctl() failed with exit code %i!\e[0m\n", res);
		printf("\e[1;31mManual latch test failed!\e[0m\n");
		return 1;
	} else printf("\e[1;32mManual latch test passed!\e[0m\n");
	return 0;
}
uint8_t resetTest() {
	puts("\e[1;33mIOCTL Reset test!\e[0m");
	int res = ioctl(fp->_fileno, IOCTL_RESET_595);
	if(res) {
		printf("\e[1;31mioctl() failed with exit code %i!\e[0m\n", res);
		printf("\e[1;31mReset test failed!\e[0m\n");
		return 1;
	} else printf("\e[1;32mReset test passed!\e[0m\n");
	return 0;
}
uint8_t chainLengthTest() {
	puts("\e[1;33mIOCTL Chain length test!\e[0m");
	uint8_t clen = 0;
	int res = ioctl(fp->_fileno, IOCTL_READ_CHAIN_LENGTH, &clen);
	if(res) {
		printf("\e[1;31mioctl() failed with exit code %i!\e[0m\n", res);
		printf("\e[1;31mChain length test failed!\e[0m\n");
		return 1;
	} else {
		if(clen) printf("\e[1;32mChain length test passed, chain length = %d!\e[0m\n", clen);
		else {
			printf("\e[1;31mInvalid chain length of 0!\e[0m\n", res);
			printf("\e[1;31mChain length test failed!\e[0m\n");
			return 1;
		}
	}
	return 0;
}
