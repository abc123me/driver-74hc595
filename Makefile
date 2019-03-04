name = mod595
obj-m += $(name).o
$(name)-objs := driver_main.o 595.o device.o
KVERSION = $(shell uname -r)
used = $(shell lsmod | grep $(name) | awk '{print $$1}' )
args =

all:
	make -C /lib/modules/$(KVERSION)/build M=`pwd` modules
clean:
	make -C /lib/modules/$(KVERSION)/build M=`pwd` clean
remake:
	make clean
	make all
ifeq ($(used), mod595)
load:
	make unload
	make load
else
load:
	sudo insmod $(name).ko clock_pin=15 data_pin=16 latch_pin=14 chain_len=2 delay=1000 $(args)
endif
unload:
	sudo rmmod $(name)
reload:
	make load
info:
	modinfo $(name).ko
