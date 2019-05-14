#include "device.h"

int register_device(struct device_internal* d, char* name, struct file_operations* fops){
	printk("Registering device!\n");
	d->name = name;
	d->fops = fops;
	int major = register_chrdev(0, name, fops);
	if(major <= 0){
		printk("Failed to register character device (errorcode = %i)\n", major);
		return major;
	} else d->major = major;
	printk("Registered device with major number = %i and minor numbers 0...255\n", major);
	dev_t devt = MKDEV(major, 0);
	struct class* cl = class_create(THIS_MODULE, "new");
	if(cl == NULL){
		printk("Failed to create a class");
		return -1;
	} else {
		d->devt = devt;
		d->cl = cl;
	}
    struct device* dev = device_create(cl, NULL, devt, NULL, name);
	if(dev == NULL){
		printk("Failed to create a device");
		return -1;
	} else d->dev = dev;
	return 0;
}
void unregister_device(struct device_internal* d){
	if(d == NULL) return;
	printk("Unregistering device\n");
	if(d->major != 0)
		unregister_chrdev(d->major, d->name);
	if(d->cl == NULL)
		return;
	device_destroy(d->cl, d->devt);
	class_unregister(d->cl);
	class_destroy(d->cl);
	printk("Unregistered device\n");
}
