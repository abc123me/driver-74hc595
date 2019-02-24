#include "stdio.h"
#include "sys/ioctl.h"

int main(){
	FILE* fp = fopen("/dev/chip74hc595", "w");
	if(fp == NULL){
		printf("Failed to open device\n");
		return 1;
	}
	setbuf(fp, 0);
	int res = ioctl(fp->_fileno, 1);
	if(res) printf("ioctl() failed with exit code %i!\n", res);
	fclose(fp);
}
