#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "fcntl.h"
#include "unistd.h"

int main(int argc, char** argv){
	puts("\e[1;33m------------------------");
	puts("Basic write test!");
	puts("------------------------\e[0m");
	puts("Opening device!");
	int fp = open("/dev/chip74hc595", O_WRONLY);
	if(fp <= 0){
		puts("Failed to open device");
		puts("\e[1;31mWrite test failed!\e[0m");
		return 1;
	}
	printf("Opened file\n");
	uint16_t n = 1024;
	ssize_t sz = 2;
	for(uint16_t i = 0; i <= n; i++){
		ssize_t nsz = write(fp, &i, sz);
		if(sz == nsz)
			printf("\e[0GWrote %d / %d\e[0m", i, n);
		else {
			printf("\e[1;31m\e[0GFailed at %d / %d: %d (written) != %d (to write)\e[0m", i, n, nsz, sz);
			n = 0; break;
		}
		usleep(1000 * 1);
	}
	close(fp);
	if(n > 0) {
		puts("\n\e[1;32mWrite test finished sucessfully!\e[0m");
		return 0;
	} else {
		puts("\n\e[1;31mWrite test failed!\e[0m");
		return 1;
	}
}
