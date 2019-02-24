#include "linux/module.h"
#include "linux/kernel.h"
#include "linux/types.h"
#include "linux/uaccess.h"

#include "device.h"
#include "595.h"
#include "modinfo.h"

#define _PARAM_MACRO(type, name, default_val, desc)\
	static type name = default_val;\
	module_param(name, type, 0000);\
	MODULE_PARM_DESC(name, desc);
#define _CHIP_CALL_MACRO(func, err_msg) {\
	uint8_t res = func;\
	if(res) {\
		printk("%s (%s)!\n", err_msg, getReason595(res));\
		return -1;\
	}\
}
#define IOCTL_RESET_595_CMD 1
#define IOCTL_GET_CHAIN_LENGTH 2

int init_module(void);
void cleanup_module(void);
int init_chip(void);

//Hack to get definition for a byte as linux/module.h needs the "byte" type to take 8 bit ints
typedef uint8_t byte;
_PARAM_MACRO(byte, clock_pin, 3, "\tThe clock pin of the 74HC595");
_PARAM_MACRO(byte, latch_pin, 1, "\tThe latch pin of the 74HC595");
_PARAM_MACRO(byte, data_pin,  0, "\tThe data pin of the 74HC595");
_PARAM_MACRO(int, reset_pin,  -1, "\tThe reset pin of the 74HC595, -1 for none");
_PARAM_MACRO(byte, chain_len, 1, "\tThe amount of 74HC595's chained together");
_PARAM_MACRO(ulong, delay, 50, "\t\tThe amount of delay used for the clock line (in ns)");
//_PARAM_MACRO(bool, clock_invert, 0, "Iverts the clock pin of the 74HC595");

static struct Chip74HC595 chip;
static struct device_internal dev;

int device_open(struct inode* in, struct file* fp);
int device_close(struct inode* in, struct file* fp);
ssize_t device_read(struct file* fp, char* buf, size_t cnt, loff_t* pos);
ssize_t device_write(struct file* fp, const char* buf, size_t cnt, loff_t* pos);
long device_ioctl(struct file *f, unsigned int cmd, unsigned long arg);
static struct file_operations driver_fops = {
	.owner = THIS_MODULE,
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_close,
	.unlocked_ioctl = device_ioctl
};

int init_module(){
	int result = 0;
	result = register_device(&dev, "chip74hc595", &driver_fops);
	if(result){
		printk("register_device() returned a non-zero exit code!\n");
		goto exit;
	}
	result = init_chip();
	if(result){
		printk("init_chip() returned a non-zero exit code!\n");
		goto exit;
	}
	exit:
	if(result) cleanup_module();
	return result;
}
void cleanup_module(){
	unregister_device(&dev);
	free595(&chip);
	printk("Goodbye world 1.\n");
}

int init_chip(){
	//clock (3), data(0), latch(1)
	uint32_t delay_u32 = delay;
	printk("Intializing clock=%i, data=%i, latch=%i (%i @ %ins)\n", clock_pin, data_pin, latch_pin, chain_len, delay_u32);
	_CHIP_CALL_MACRO(init595(&chip, clock_pin, data_pin, latch_pin, chain_len), "Error initializing chip");
	if(reset_pin > 0){
		printk("Custom reset pin specified (%i)\n", reset_pin);
		_CHIP_CALL_MACRO(setResetPin595(&chip, (uint8_t) reset_pin), "Error setting the reset pin");
	}
	_CHIP_CALL_MACRO(setSpeed595(&chip, delay_u32), "Error setting speed");
	_CHIP_CALL_MACRO(reset595(&chip), "Error resetting chip");
	return 0;
}

static bool device_already_open = false;
int device_close(struct inode* in, struct file* fp){
	device_already_open = false;
	return 0;
}
int device_open(struct inode* in, struct file* fp){
	if(device_already_open) return -1;
	device_already_open = true;
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
long device_ioctl(struct file *f, unsigned int cmd, unsigned long arg){
	switch(cmd){
		case IOCTL_RESET_595_CMD:
			reset595(&chip);
			break;
		case IOCTL_GET_CHAIN_LENGTH:
			copy_to_user(arg, &chain_len, 1);
			break;
		default:
			printk("Invalid ioctl call to device - %x w/ arg %lx\n", cmd, arg);
			return -1;
	}
	return 0;
}
