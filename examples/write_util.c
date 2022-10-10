#include "stdio.h"
#include "stdlib.h"
#include "fcntl.h"
#include "errno.h"
#include "unistd.h"
#include "stdint.h"
#include "sys/ioctl.h"

// Part of the driver's code, contains the ioctl calls needed to use the driver
// https://github.com/abc123me/driver-74hc595/blob/master/ioctl_595.h
#include "../ioctl_595.h"

/*
	This example utilizes a chain of 595's to march a bit along
	a row of LEDs, each LED is connected to the outputs of the
	shift registers sequentially, if wired correctly the first led will light up
	and march up and down the row of LEDs
*/

// Opens a file descriptor for the device
int initFileDesc();
// Returns the chain length via the ioctl
uint8_t getChainLength(int fd);

// For IOCTL information read here:
// https://github.com/abc123me/driver-74hc595/blob/master/ioctl_595.h

int main(int argc, char** argv) {
	// Open and initialize the 595 device
	int fd = initFileDesc();
	if(fd < 0) return fd;
	uint8_t chain_len = getChainLength(fd);
	if(chain_len == 0) {
		puts("Chain length is 0, WTF?!");
		return 1;
	}

	// Check for --help, -h, or -?
	uint8_t help = 0;
	for(uint16_t i = 1; i < argc; i++) {
		if(strncmp("--help", argv[i]) == 0 || strncmp("-h", argv[i]) == 0 || strncmp("-?", argv[i]) == 0) {
			help = 1;
			break;
		}
	}

	// Verify argument length
	if(argc < chain_len + 1 || help) {
usage_err:
		printf("Usage: %s", argv[0]);
		uint8_t n = chain_len < 3 ? chain_len : 3;
		for(uint16_t i = 0; i < n; i++)
			printf(" <chip %d>", i + 1);
		if(chain_len > 3) printf(" <...>");
		printf(" [flags] \n");
		puts("Flags (can be placed anywhere in arguments):");
		puts(" -h / -? / --help: Shows this help dialog");
		puts(" -d / --dry-run: Dry run (doesn't write anything)");
		puts(" -i / --invert: Inverts all written bytes");
		close(fd);
		return 0;
	}

	// Process arguments
	errno = 0;
	uint8_t flags = 0; // 1 - Dry run, 2 - Invert
	uint16_t bpos = 0;
	uint8_t buf[chain_len];
	for(uint16_t i = 1; i < argc; i++) {
		char* arg = argv[i];
		if(strcmp("--dry-run", arg) == 0 || strcmp("-d", arg) == 0) {
			flags |= 1;
			printf("Performing a dry run!\n");
		} else if(strcmp("--invert", arg) == 0 || strcmp("-i", arg) == 0) {
			flags |= 2;
			printf("Inverting buffer data!\n");
		} else {
			if(bpos >= chain_len) goto usage_err;
			buf[bpos] = (uint8_t) atoi(arg);
			if(flags & 2) buf[bpos] = ~buf[bpos];
			if(errno) goto usage_err;
			bpos++;
		}
	}

	// Check to make sure buffer length is the same as the chain length
	if(bpos != chain_len) goto usage_err;

	// Check dry-run flag
	if(flags & 1) {
		printf("Write %d bytes: [", chain_len);
		for(uint8_t i = 0; i < chain_len; i++)
			printf(i >= chain_len - 1 ? "%02X" : "%02X, ", buf[i]);
		printf("]\n");
	} else write(fd, buf, chain_len);
	close(fd);
	return 0;
}
int initFileDesc(){
	int fd = open("/dev/chip74hc595", O_WRONLY);
	if(fd < 0){
		puts("Failed to open device!");
		return -1;
	}
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
