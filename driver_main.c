#include "linux/module.h"
#include "linux/kernel.h"
#include "linux/types.h"
#include "linux/fs.h"
#include "linux/fcntl.h"
#include "linux/uaccess.h"
#include "linux/device.h"
#include "linux/gpio.h"
#include "linux/delay.h"
#include "595.h"

MODULE_LICENSE("GPL");
#define _PARAM_MACRO(type, name, default_val, desc)\
	static type name = default_val;\
	module_param(name, type, 0000);\
	MODULE_PARM_DESC(name, desc);

//Hack to get definition for a byte as linux/module.h needs the "byte" type to take 8 bit ints
typedef uint8_t byte;

int init_module(void);
void cleanup_module(void);
void unregister_device(void);
int register_device(void);
int init_chip(void);
int device_open(struct inode* in, struct file* fp);
int device_close(struct inode* in, struct file* fp);
ssize_t device_read(struct file* fp, char* buf, size_t cnt, loff_t* pos);
ssize_t device_write(struct file* fp, const char* buf, size_t cnt, loff_t* pos);

//Device info
static int device_file_major = 0;
static char* device_file_name = "driver74hc595";
//Device and associated classes
static struct class* cl = NULL;
static struct device* dev = NULL;
static dev_t devt;
static struct file_operations driver_fops = {
	.owner = THIS_MODULE,
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_close
};
//Chip
struct Chip74HC595 chip;
_PARAM_MACRO(byte, clock_pin, 3, "The clock pin of the 74HC595");
_PARAM_MACRO(byte, latch_pin, 1, "The latch pin of the 74HC595");
_PARAM_MACRO(byte, data_pin,  0, "The data pin of the 74HC595");
_PARAM_MACRO(byte, chain_len, 1, "The amount of 74HC595's chained together");
_PARAM_MACRO(ulong, delay, 50, "The amount of delay used for the clock line");
//_PARAM_MACRO(bool, clock_invert, 0, "Iverts the clock pin of the 74HC595");

int init_module(){
	int result = 0;
	result = register_device();
	if(result){
		printk("register_device() returned a non-zero exit code!");
		goto exit;
	}
	result = init_chip();
	if(result){
		printk("init_chip() returned a non-zero exit code!");
		goto exit;
	}
	exit:
	if(result) cleanup_module();
	return result;
}
void cleanup_module(){
	unregister_device();
	free595(&chip);
	printk("Goodbye world 1.\n");
}

int init_chip(){
	//clock (3), data(0), latch(1)
	uint32_t delay_u32 = delay;
	printk("Intializing clock=%i, data=%i, latch=%i (%i @ %ins)\n", clock_pin, data_pin, latch_pin, chain_len, delay_u32);
	uint8_t res = init595(&chip, clock_pin, data_pin, latch_pin, chain_len);
	if(res){
		printk("Error initializing 595 (%s)!\n", getReason595(res));
		return -1;
	}
	setSpeed595(&chip, delay_u32);
	reset595(&chip);
	return 0;
}
int register_device(){
	printk("Registering device!\n");
	device_file_major = register_chrdev(0, device_file_name, &driver_fops);
	if(device_file_major < 0){
		printk("Failed to register character device (errorcode = %i)\n", device_file_major);
		return device_file_major;
	}
	printk("Registered device with major number = %i and minor numbers 0...255\n", device_file_major);
	devt = MKDEV(device_file_major, 0);
        /*int result = register_chrdev_region(devt, 1, device_file_name);
        if(result < 0){
		printk("Failed to register a character device region (errorcode = %i)\n", result);
		return result;
	}*/
	cl = class_create(THIS_MODULE, "new");
	if(cl == NULL){
		printk("Failed to create a class");
		return -1;
	}
        dev = device_create(cl, NULL, devt, NULL, device_file_name);
	if(dev == NULL){
		printk("Failed to create a device");
		return -1;
	}
	return 0;
}
void unregister_device(){
	printk("Unregistering device\n");
	if(device_file_major != 0)
		unregister_chrdev(device_file_major, device_file_name);
	if(cl == NULL)
		return;
	device_destroy(cl, devt);
	class_unregister(cl);
	class_destroy(cl);
	printk("Unregistered device\n");
}
int device_close(struct inode* in, struct file* fp){
	printk("Closed\n");
	return 0;
}
int device_open(struct inode* in, struct file* fp){
	printk("Opened\n");
//	fp->f_flags |= O_NONBLOCK;
	return 0;
}
ssize_t device_read(struct file* fp, char* buf, size_t cnt, loff_t* pos){
	printk("Read\n");
	return 0;
}
ssize_t device_write(struct file* fp, const char* buf, size_t cnt, loff_t* pos){
	printk("Wrote [");
	size_t i;
	for(i = 0; i < cnt; i++){
		printk("%i, ", buf[i]);
		writeb595(&chip, buf[i]);
	}
	latch595(&chip);
	printk("%i] to 74HC595\n", buf[cnt - 1]);
	return cnt;
}
