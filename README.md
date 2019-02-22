# driver-74hc595

A linux kernel driver for the 74HC595

## Usage

1. Compile with `make`
2. Verify that `make` was succesful
3. Insert the kernel module into the kernel using `insmod driver.ko <parameters>`
4. Write to `/dev/driver74hc595`
5. Profit!

### Moduel parameters
Name | Description
-----|------------
clock_pin | Sets the clock pin
data_pin | Sets the data pin
latch_pin | Sets the latch pin
chain_len | Sets the amount of shift registers chained together

## Issues

 - Dosen't work with `fwrite` but works with `write` (even though they are the same system call)
   - Possibly being caused by fwrite buffering when it's not supposed to
 - No support for reset or output enable pins
 - ***NOT SUPPORTED ON KERNEL VERSION 3.4 AND ABOVE***
 
## Tested onfigurations
Hardware | Kernel version | Dstribution | State
---------|----------------|-------------|------
OrangePi Zero | Kernel version 3.4 | Armbian Xenial | ![#00ff00](https://placehold.it/15/00ff00/000000?text=+) Supported
OrangePi Zero | Kernel version 4.1 | Armbian Bionic | ![#ff0000](https://placehold.it/15/ff0000/000000?text=+) Unsupported
