name = mod595
obj-m += $(name).o
$(name)-objs := driver_main.o 595.o device.o
KVERSION = $(shell uname -r)

all:
	make -C /lib/modules/$(KVERSION)/build M=`pwd` modules
clean:
	make -C /lib/modules/$(KVERSION)/build M=`pwd` clean
remake:
	make clean
	make all
load:
	sudo insmod $(name).ko clock_pin=15 data_pin=16 latch_pin=14 chain_len=2
unload:
	sudo rmmod $(name)
reload:
	make unload
	make load
info:
	modinfo $(name).ko
