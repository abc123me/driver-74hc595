#include "linux/module.h"
#include "linux/kernel.h"
#include "linux/types.h"
#include "linux/uaccess.h"
#include "linux/fs.h"

#include "device.h"
#include "595.h"
#include "modinfo.h"
#include "ioctl_595.h"

#define _PARAM_MACRO(type, name, default_val, desc)\
	static type name = default_val;\
	module_param(name, type, 0000);\
	MODULE_PARM_DESC(name, desc);
#define _CHIP_CALL_MACRO(func, err_msg) {\
	uint8_t res = func;\
	if(res) {\
		printk("[mod595 err] %s (%s)!\n", err_msg, getReason595(res));\
		return -1;\
	}\
}
#define DEV_WRITE_BUFLEN 128

int init_module(void);
void cleanup_module(void);
int init_chip(void);

//Hack to get definition for a byte as linux/module.h needs the "byte" type to take 8 bit ints
typedef uint8_t byte;
typedef char* charp;
_PARAM_MACRO(byte, clock_pin, 3, "\tThe clock pin of the 74HC595");
_PARAM_MACRO(byte, latch_pin, 1, "\tThe latch pin of the 74HC595");
_PARAM_MACRO(byte, data_pin,  0, "\tThe data pin of the 74HC595");
_PARAM_MACRO(int, reset_pin,  -1, "\tThe reset pin of the 74HC595, -1 for none");
_PARAM_MACRO(byte, chain_len, 1, "\tThe amount of 74HC595's chained together");
_PARAM_MACRO(ulong, delay, 50, "\t\tThe amount of delay used for the clock line (in ns)");
_PARAM_MACRO(charp, device_name, "chip74hc595", "\tThe device name, defaults to \"chip74hc595\" (appears in /dev)");
//_PARAM_MACRO(bool, clock_invert, 0, "Iverts the clock pin of the 74HC595");


int device_open(struct inode* in, struct file* fp);
int device_close(struct inode* in, struct file* fp);
ssize_t device_read(struct file* fp, char* buf, size_t cnt, loff_t* pos);
ssize_t device_write(struct file* fp, const char* buf, size_t cnt, loff_t* pos);
long device_ioctl(struct file *f, unsigned int cmd, unsigned long arg);
int alphanumeric(char* str);

static struct Chip74HC595 chip;
static struct device_internal dev;
static struct file_operations driver_fops = {
	.owner = THIS_MODULE,
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_close,
	.unlocked_ioctl = device_ioctl
};
static bool device_already_open = false;
static bool latch_on_write	= true;
static char deviceWriteBuf[DEV_WRITE_BUFLEN];

int init_module(){
	int result = 0;
	char* dev_name = device_name;
	if(dev_name == 0) {
		printk("[mod595 err] device_name cannot be null!");
		return -EINVAL;
	}
	int len = strlen(dev_name);
	if(len <= 2 || len >= 20) {
		printk("[mod595 err] device_name must be between 2 and 20 characters");
		return -EINVAL;
	}
	if(!alphanumeric(dev_name)){
		printk("[mod595 err] device_name must be alphanumeric");
		return -EINVAL;
	}
	result = register_device(&dev, dev_name, &driver_fops);
	if(result){
		printk("[mod595 err] register_device() returned a non-zero exit code!\n");
		goto exit;
	}
	result = init_chip();
	if(result){
		printk("[mod595 err] init_chip() returned a non-zero exit code!\n");
		goto exit;
	}
	exit:
	if(result) cleanup_module();
	return result;
}
void cleanup_module(){
	unregister_device(&dev);
	free595(&chip);
	printk("[mod595 info] Driver unloaded!\n");
}

int init_chip(){
	//clock (3), data(0), latch(1)
	uint32_t delay_u32 = delay;
	printk("[mod595 info] Intializing clock=%i, data=%i, latch=%i (%i @ %ins), device_name=%s\n", clock_pin, data_pin, latch_pin, chain_len, delay_u32, device_name);
	_CHIP_CALL_MACRO(init595(&chip, clock_pin, data_pin, latch_pin, chain_len), "Error initializing chip");
	if(reset_pin > 0){
		printk("[mod595 info] Custom reset pin specified (%i)\n", reset_pin);
		_CHIP_CALL_MACRO(setResetPin595(&chip, (uint8_t) reset_pin), "Error setting the reset pin");
	}
	_CHIP_CALL_MACRO(setSpeed595(&chip, delay_u32), "Error setting speed");
	_CHIP_CALL_MACRO(reset595(&chip), "Error resetting chip");
	return 0;
}

int device_close(struct inode* in, struct file* fp){
	device_already_open = false;
	return 0;
}
int device_open(struct inode* in, struct file* fp){
	if(device_already_open) return -1;
	device_already_open = true;
	latch_on_write = true;
	return 0;
}
ssize_t device_read(struct file* fp, char* buf, size_t cnt, loff_t* pos){
//	printk("Read\n");
	return 0;
}
ssize_t device_write(struct file* fp, const char* buf, size_t cnt, loff_t* pos){
//	printk("Wrote [");
	if(cnt <= 0 || cnt > DEV_WRITE_BUFLEN) {
		printk(cnt <= 0 ? "[mod595 warn] Buffer underflow on write" : "[mod595 warn] Buffer overflow on write");
		return 0;
	}
	copy_from_user(deviceWriteBuf, buf, cnt);
	int64_t i;
	for(i = cnt - 1; i >= 0; i--) {
//		printk("%i, ", deviceWriteBuf[i]);
		writeb595(&chip, deviceWriteBuf[i]);
	}
	if(latch_on_write)
		latch595(&chip);
//	printk("%i] to 74HC595\n", deviceWriteBuf[cnt - 1]);
	return cnt;
}
long device_ioctl(struct file *f, unsigned int cmd, unsigned long arg){
	switch(cmd){
		case IOCTL_RESET_595:
			reset595(&chip);
			latch595(&chip);
			break;
		case IOCTL_READ_CHAIN_LENGTH:
			copy_to_user(arg, &chain_len, 1);
			break;
		case IOCTL_MANUAL_LATCH:
			latch595(&chip);
			break;
		case IOCTL_SET_AUTO_LATCH:
			latch_on_write = arg ? 1 : 0;
			break;
		default:
			printk("[mod595 err] Invalid ioctl call to device - %x w/ arg %lx\n", cmd, arg);
			return -1;
	}
	return 0;
}
int alphanumeric(char* str){
	int i = 0, len = strlen(str);
	if(len <= 0) return 0;
	for(i = 0; i < len; i++){
		char c = str[i];
		bool l = c >= 'a' && c <= 'z';
		bool u = c >= 'A' && c <= 'Z';
		bool n = c >= '0' && c <= '9';
		if(!(l || u || n || c == '_'))
			return 0;
	}
	return 1;
}
