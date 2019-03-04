#include "stdio.h"
#include "stdlib.h"
#include "fcntl.h"
#include "unistd.h"
#include "stdint.h"
#include "sys/ioctl.h"

#include "../ioctl_595.h"

uint8_t getChainLength(int fd);
int initFileDesc();
int resetChain(int fd);

int main(int argc, char** argv){
	uint16_t mdelay = 1000;
	if(argc > 1){
		mdelay = atoi(argv[1]);
		printf("Delay set to %ims\n", mdelay);
	}

	int fd = initFileDesc();
	if(fd < 0) return fd;
	uint8_t chain_len = getChainLength(fd);
	if(chain_len == 0) {
		puts("Chain length is 0, nothing to do!");
		return 1;
	}

	uint16_t max = chain_len * 8;
	uint64_t val = 1;
	uint64_t udelay = 1000 * mdelay;
	while(1){
		for(uint8_t i = 0; i < max; i++){
			val <<= 1;
			write(fd, &val, chain_len);
			usleep(udelay);
		}
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
