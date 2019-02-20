obj-m += driver.o
driver-objs := driver_main.o 595.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	rm -fv *.o
	rm -fv *.ko
	rm -fv *.gch
load:
	sudo insmod driver.ko clock_pin=15 data_pin=16 latch_pin=14 delay=1000000000
#	sudo chmod 777 /dev/driver74HC595
unload:
	sudo rmmod driver
