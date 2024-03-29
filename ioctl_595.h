#ifndef _IOCTL_H
#define _IOCTL_H

#include "linux/ioctl.h"
#include "linux/types.h"

#define IOCTL_MAGIC 'k'
#define IOCTL_RESET_595		_IO(IOCTL_MAGIC, 1)
#define IOCTL_READ_CHAIN_LENGTH	_IOR(IOCTL_MAGIC, 2, uint8_t)
#define IOCTL_MANUAL_LATCH	_IO(IOCTL_MAGIC, 3)
#define IOCTL_SET_AUTO_LATCH	_IOW(IOCTL_MAGIC, 4, uint8_t)

#endif
