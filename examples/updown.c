#include "stdio.h"
#include "stdlib.h"
#include "fcntl.h"
#include "unistd.h"
#include "stdint.h"
#include "sys/ioctl.h"

//Part of the driver's code, contains the ioctl calls needed to use the driver
//https://github.com/abc123me/driver-74hc595/blob/master/ioctl_595.h
#include "../ioctl_595.h"

/*
This example utilizes a chain of 595's to march a bit along
a row of LEDs, each LED is connected to the outputs of the
shift registers sequentially, if wired correctly the first led will light up
and march up and down the row of LEDs
*/

//Opens a file descriptor for the device
int initFileDesc();
//Returns the chain length via the ioctl
uint8_t getChainLength(int fd);
//Resets the entire chain of shift registers via an ioctl
int resetChain(int fd);

//For IOCTL information read here:
//https://github.com/abc123me/driver-74hc595/blob/master/ioctl_595.h

int main(int argc, char** argv){
	//Initialize
	uint16_t mdelay = 1000;
	if(argc > 1){
		mdelay = atoi(argv[1]);
		printf("Delay set to %ims\n", mdelay);
	}
	
	//Open and initialize the 595 device
	int fd = initFileDesc();
	if(fd < 0) return fd;
	uint8_t chain_len = getChainLength(fd);
	if(chain_len == 0) {
		puts("Chain length is 0, nothing to do!");
		return 1;
	}

	//Our animation, remember auto-latching is enabled by default
	//so no manual latch is needed
	uint16_t max = chain_len * 8;
	uint64_t val = 1;
	uint64_t udelay = 1000 * mdelay;
	while(1){
		//Move the bit left for each bit in the chain
		for(uint8_t i = 0; i < max; i++){
			val <<= 1;
			write(fd, &val, chain_len);
			usleep(udelay);
		}
		//Move the bit right for each bit in the chain
		for(uint8_t i = 0; i < max; i++){
			val >>= 1;
			write(fd, &val, chain_len);
			usleep(udelay);
		}
	}
	close(fd);
	return 0;
}
int resetChain(int fd){
	if(ioctl(fd, IOCTL_RESET_595)){
		puts("Failed to reset chain!");
		close(fd);
		return 1;
	}
	return 0;
}
int initFileDesc(){
	int fd = open("/dev/chip74hc595", O_WRONLY);
	if(fd < 0){
		puts("Failed to open device!");
		return -1;
	}
	if(resetChain(fd))
		return -1;
	return fd;
}
uint8_t getChainLength(int fd){
	uint8_t chain_len = 0;
	if(ioctl(fd, IOCTL_READ_CHAIN_LENGTH, &chain_len)){
		puts("Failed to retrieve chain length using ioctl()!");
		return -1;
	}
	printf("Got chain length: %i\n", chain_len);
	return chain_len;
}
