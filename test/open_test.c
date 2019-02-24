#include "stdio.h"

int openCloseReopenTest(){
	FILE* fp;
	int fail = 0;
	puts("\e[1;33m------------------------");
	puts("Open, Close, Reopen test");
	puts("------------------------\e[0m");
	puts("Opening");
	fp = fopen("/dev/chip74hc595", "w");
	if(!fp){
		puts("Failed to open device!");
		fail = 1; goto end;
	}
	puts("Closing");
	if(fclose(fp)){
		puts("Failed to close");
		fail = 1; goto end;
	}
	fp = fopen("/dev/chip74hc595", "w");
	if(!fp){
		puts("Failed to reopen device!");
		fail = 1; goto end;
	}
	puts("Reopening");
	if(fclose(fp)){
		puts("Failed to close, reopening");
		fail = 1; goto end;
	}
	end:
	if(fail) puts("\e[1;31mTest failed\e[0m");
	else puts("\e[1;32mTest was successful!\e[0m");
	return fail;
}
int doubleOpenTest(){
	FILE* fp1;
	FILE* fp2;
	int fail = 0;
	puts("\e[1;33m------------------------");
	puts("Double-open test!");
	puts("------------------------\e[0m");
	fp1 = fopen("/dev/chip74hc595", "w");
	if(!fp1){
		puts("Failed to open device!");
		fail = 1; goto end;
	}
	puts("fp1 opened!");
	fp2 = fopen("/dev/chip74hc595", "w");
	if(fp2){
		puts("fp2 opened!");
		fail = 1; goto end;
	}
	puts("fp2 failed to open!");
	end:
	if(fp1 && !fclose(fp1)) puts("fp1 closed!");
	if(fp2 && !fclose(fp2)) puts("fp2 closed!");
	if(fail) puts("\e[1;31mTest failed\e[0m");
	else puts("\e[1;32mTest was successful!\e[0m");
	return fail;
}
int main(){
	openCloseReopenTest();
	doubleOpenTest();
}
	
