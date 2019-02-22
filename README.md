# driver-74hc595

A linux kernel driver for the 74HC595

## Usage

1. Compile with `make`
2. Verify that `make` was succesful
3. Insert the kernel module into the kernel using `insmod driver.ko <parameters>`
4. Write to `/dev/driver74hc595`
5. Profit!

### Module parameters
Name      | Description                                         | Range
----------|-----------------------------------------------------|--------------
clock_pin | Sets the clock pin                                  | `uint8_t`
data_pin  | Sets the data pin                                   | `uint8_t`
latch_pin | Sets the latch pin                                  | `uint8_t`
chain_len | Sets the amount of shift registers chained together | `uint64_t`
 
## Tested onfigurations
Hardware   | Architecture | Kernel version | Dstribution | State
-----------|--------------|----------------|-------------|------
OrangePi 0 | 3.4 | armhf | Armbian Xenial | ![#00ff00](https://placehold.it/15/00ff00/000000?text=+) Supported
OrangePi 0 | 4.1 | armhf | Armbian Bionic | ![#ff0000](https://placehold.it/15/ff0000/000000?text=+) Unsupported

## Known Issues
 - Dosen't work with `fwrite` but works with `write` (even though they are the same system call)
   - Possibly being caused by fwrite buffering when it's not supposed to
 - No support for reset or output enable pins
 - ***NOT SUPPORTED ON KERNEL VERSION 3.4 AND ABOVE***
