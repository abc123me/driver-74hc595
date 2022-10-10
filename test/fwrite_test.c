#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
	puts("\e[1;33m------------------------");
	puts("Basic fwrite test!");
	puts("------------------------\e[0m");

	puts("Opening device!");
	FILE* fp = fopen("/dev/chip74hc595", "w");
	if(!fp){
		puts("Failed to open device");
		puts("\e[1;31mWrite test failed!\e[0m");
		return 1;
	}
	printf("Opened file\n");
	setbuf(fp, NULL);

	puts("Disabling buffering!");
	uint16_t n = 1024;
	ssize_t sz = sizeof(uint16_t);
	for(uint16_t i = 0; i <= n; i++){
		ssize_t nsz = fwrite(&i, 1, sz, fp);
		if(sz == nsz)
			printf("\e[0GWrote %d / %d\e[0m", i, n);
		else {
			printf("\e[1;31m\e[0GFailed at %d / %d: %d (written) != %d (to write) \e[0m", i, n, nsz, sz);
			n = 0; break;
		}
		usleep(1000 * 1);
	}
	fclose(fp);
	if(n > 0) {
		puts("\n\e[1;32mWrite test finished sucessfully!\e[0m");
		return 0;
	} else {
		puts("\n\e[1;31mWrite test failed!\e[0m");
		return 1;
	}
	return 0;
}
