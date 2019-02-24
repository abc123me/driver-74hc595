#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
	int i = 0;
	puts("Opening device!");
	FILE* fp = fopen("/dev/chip74hc595", "w");
	setbuf(fp, NULL);
	if(!fp){
		puts("Failed to open device");
		return 1;
	}
	printf("Opened file\n");
	while(1){
		printf("Wrote %i\n", i);
		fwrite(&i, sizeof(int), 1, fp);
		usleep(1000 * 100);
		i++;
	}
	fclose(fp);
	return 0;
}
