# driver-74hc595

A linux kernel driver for the 74HC595

## Usage

1. Compile with `make`
2. Insert the kernel module into the kernel using `insmod driver.ko <parameters>`
3. Read/Write to `/dev/chip74hc595`

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
OrangePi Zero    | 4.14     | Armbian Bionic    | ![#00ff00](https://placehold.it/15/00ff00/000000?text=+) Supported
OrangePi Zero    | 3.4.113  | Armbian Xenial    | ![#aaff00](https://placehold.it/15/aaff00/000000?text=+) Supported (untested)
Raspberry Pi 2   | 4.14     | Raspbian Stretch  | ![#00ff00](https://placehold.it/15/00ff00/000000?text=+) Supported
Raspberry Pi 3B+ | 4.19.57  | Raspbian Stretch  | ![#00ff00](https://placehold.it/15/00ff00/000000?text=+) Supported

## Known Issues
 - Dosen't work with `fwrite` but works with `write` (even though they are the same system call)
   - Caused by fwrite buffering when it's not supposed to
   - Easily fixed by using `setbuf(fp, NULL);` to disable buffering
   - There is probably a flag or something I can set to fix this pernamently
 - No support for output enable pin

