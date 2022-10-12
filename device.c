#include "device.h"

int register_device(struct device_internal* d, char* name, struct file_operations* fops){
	printk("[mod595 info] Registering device!\n");
	d->name = name;
	d->fops = fops;
	int major = register_chrdev(0, name, fops);
	if(major <= 0){
		printk("[mod595 err] Failed to register character device (errorcode = %i)\n", major);
		return major;
	} else d->major = major;
	printk("[mod595 info] Registered device with major number = %i and minor numbers 0...255\n", major);
	dev_t devt = MKDEV(major, 0);
	struct class* cl = class_create(THIS_MODULE, "new");
	if(cl == NULL){
		printk("[mod595 err] Failed to create a class");
		return -1;
	} else {
		d->devt = devt;
		d->cl = cl;
	}
    struct device* dev = device_create(cl, NULL, devt, NULL, name);
	if(dev == NULL){
		printk("[mod595 err] Failed to create a device");
		return -1;
	} else d->dev = dev;
	return 0;
}
void unregister_device(struct device_internal* d){
	if(d == NULL) return;
	printk("[mod595 info] Unregistering device\n");
	if(d->major != 0)
		unregister_chrdev(d->major, d->name);
	if(d->cl == NULL)
		return;
	device_destroy(d->cl, d->devt);
	class_destroy(d->cl);
	printk("[mod595 info] Unregistered device\n");
}
