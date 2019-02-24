#ifndef _DEVICE_H
#define _DEVICE_H

#include "linux/fs.h"
#include "linux/device.h"
#include "linux/module.h"
#include "linux/kernel.h"

typedef struct device_internal{
	char* name;
	int major;
	struct class* cl;
	struct device* dev;
	struct file_operations* fops;
	dev_t devt;
};

int register_device(struct device_internal* d, char* name, struct file_operations* fops);
void unregister_device(struct device_internal* d);

#endif
