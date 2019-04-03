name = mod595
koname = $(name).ko
obj-m += $(name).o
$(name)-objs := driver_main.o 595.o device.o
KVERSION = $(shell uname -r)
used = $(shell lsmod | grep $(name) | awk '{print $$1}' )
load_args =



compile:
	make -C /lib/modules/$(KVERSION)/build M=`pwd` modules
clean:
	make -C /lib/modules/$(KVERSION)/build M=`pwd` clean
unload:
	sudo rmmod $(name)
info:
	modinfo $(koname)
opts = $(shell cat options)
_load:
	sudo insmod $(koname) $(opts) $(load_args)



install_dir = /lib/modules/$(KVERSION)/kernel/drivers/
install_loc = $(install_dir)/$(koname)
modules_conf_loc = /etc/modules-load.d/$(name).conf
modprobe_conf_loc = /etc/modprobe.d/$(name).conf

install:	uninstall
	cp $(koname) $(install_dir)
	chown root:root $(install_loc)
	chmod 644 $(install_loc)
	echo $(name) > $(modules_conf_loc)
	echo "options $(name) $(opts)" > $(modprobe_conf_loc)
	depmod
uninstall:
	rm -f $(install_loc)
	rm -f $(modules_conf_loc)
	rm -f $(modprobe_conf_loc)



ifeq ($(used), mod595)
load:		unload _load
else
load:		_load
endif
recompile: 	clean compile
reload: 	load
all:		compile
