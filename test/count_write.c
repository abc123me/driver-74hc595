#include "stdio.h"
#include "stdlib.h"
#include "fcntl.h"
#include "unistd.h"

int main(int argc, char** argv){
	int i = 0;
	puts("Opening device!");
	int fp = open("/dev/chip74hc595", O_WRONLY);
	if(fp <= 0){
		puts("Failed to open device");
		return 1;
	}
	printf("Opened file\n");
	while(1){
		write(fp, &i, 2);
		printf("Wrote %i\n", i);
		usleep(1000 * 1);
		i++;
	}
	close(fp);
	return 0;
}
