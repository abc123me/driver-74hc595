#ifndef _IOCTL_H
#define _IOCTL_H

#include "linux/ioctl.h"

#ifndef uint8_t
	typedef unsigned char uint8_t;
#endif

#define IOCTL_MAGIC 'k'
#define IOCTL_RESET_595_CMD _IO(IOCTL_MAGIC, 1)
#define IOCTL_CHAIN_LEN_CMD _IOR(IOCTL_MAGIC, 1, uint8_t)

#endif
