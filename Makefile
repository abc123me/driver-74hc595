obj-m += driver.o
driver-objs := driver_main.o 595.o
KVERSION = $(shell uname -r)

all:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean
load:
	sudo insmod driver.ko clock_pin=15 data_pin=16 latch_pin=14 delay=1000000 chain_len=2 #0000
unload:
	sudo rmmod driver
reload:
	make unload
	make load
