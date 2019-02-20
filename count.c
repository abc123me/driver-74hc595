#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
	int i = 0;
	//FILE* fp = fopen("/dev/driver74hc595", "w");
	int fp = open ("/dev/driver74hc595", O_WRONLY);
	if(!fp){
		puts("Failed to open device");
		return 1;
	}
	printf("Opened file\n");
	while(1){
		//fwrite(&i, 1, sizeof(int), fp);
		write(fp, &i, 1);
		printf("Wrote %i\n", i);
		usleep(1000 * 100);
		i++;
	}
	//fclose(fp);
	close(fp);
	return 0;
}
