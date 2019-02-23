#include "stdio.h"
#include "stdlib.h"
#include "fcntl.h"
#include "unistd.h"

int main(){
	int i = 0;
	puts("Opening device!");
	int fp = open("/dev/driver74hc595", O_WRONLY);
	if(fp <= 0){
		puts("Failed to open device");
		return 1;
	}
	printf("Opened file\n");
	while(1){
		write(fp, &i, 1);
		printf("Wrote %i\n", i);
		usleep(1000 * 100);
		i++;
	}
	close(fp);
	return 0;
}
