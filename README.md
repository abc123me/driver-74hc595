# driver-74hc595

A linux kernel driver for the 74HC595

## Usage

1. Compile with `make`
2. Insert the kernel module into the kernel using `insmod driver.ko <parameters>`
3. Read / Write to `/dev/chip74hc595`

### Module parameters
Name        | Description                                         | Range        | Default value
------------|-----------------------------------------------------|--------------|---------------
clock_pin   | Sets the clock pin                                  | `uint8_t`    | 3
data_pin    | Sets the data pin                                   | `uint8_t`    | 0
latch_pin   | Sets the latch pin                                  | `uint8_t`    | 1
reset_pin   | Sets the reset pin, if the pin < 0 then its ignored | `int16_t`    | -1 
chain_len   | Sets the amount of shift registers chained together | `uint8_t`    | 1
delay       | Sets the clock's delay (in nanoseconds)             | `uint64_t`   | 50
device_name | Sets the device name, alphanumeric, 2-20 chars      | `char*`      | chip74hc595

### IOCTL syscall commands
Name              | Description                         | Command (ioctl_595.h)     | Arguments
------------------|-------------------------------------|---------------------------|-------------
Reset             | Resets the register chain           | `IOCTL_RESET_595`         | `void`
Read chain length | Reads the chain length              | `IOCTL_READ_CHAIN_LENGTH` | `uint8_t*`
Latch             | Manually latches the 595            | `IOCTL_MANUAL_LATCH`      | `void`
Automatic latch   | Enables/Disables automatic latching | `IOCTL_SET_AUTO_LATCH`    | `bool`

## Tested configurations
Hardware         | Kernel   | Dstribution       | Status
-----------------|----------|-------------------|---------------------------------------------------------------------
OrangePi Zero    | 3.4.113  | Armbian Xenial    | ![#00FF00](icons/00FF00_15.png) Well Supported
OrangePi Zero    | 4.14     | Armbian Bionic    | ![#00FF00](icons/00FF00_15.png) Well Supported
Raspberry Pi 2   | 4.14     | Raspbian Stretch  | ![#00FF00](icons/00FF00_15.png) Well Supported
Raspberry Pi 3B+ | 4.19.57  | Raspbian Stretch  | ![#00FF00](icons/00FF00_15.png) Well Supported
BananaPi M2 Zero | 5.15.69  | Armbian Buster    | ![#00FF00](icons/00FF00_15.png) Well Supported
Generic boards   | 3.X      | Any               | ![#00FF00](icons/AAFF00_15.png) Supported, Needs testing
Generic boards   | 4.X      | Any               | ![#00FF00](icons/00FF00_15.png) Well Supported
Generic boards   | 5.X      | Any               | ![#FFFF00](icons/AAFF00_15.png) Supported, Needs testing
Generic boards   | >5.2     | Any               | ![#FF0000](icons/FFFF00_15.png) Supported, Untested
Generic boards   | <3.0     | Any               | ![#FF0000](icons/FF0000_15.png) Unsupported

## Known Issues and concerns
 - Dosen't work with `fwrite` (even though they use the same base system call)
   - Caused by fwrite's buffering when it's not supposed to
   - Buffering is easily disabled by using `setbuf(fp, NULL);`, this fixes the problem
 - No support for output enable pin, this should be tied to ground or another GPIO
 - Please report any issues to the [issues section!](https://github.com/abc123me/driver-74hc595/issues)

