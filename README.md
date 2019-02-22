# driver-74hc595

A linux kernel driver for the 74HC595

## Usage

1. Compile with `make`
2. Verify that `make` was succesful
3. Insert the kernel module into the kernel using `insmod driver.ko <parameters>`
4. Write to `/dev/driver74hc595`
5. Profit!

### Module parameters
Name      | Description                                         | Range        | Default value
----------|-----------------------------------------------------|--------------|---------------
clock_pin | Sets the clock pin                                  | `uint8_t`    | 3
data_pin  | Sets the data pin                                   | `uint8_t`    | 0
latch_pin | Sets the latch pin                                  | `uint8_t`    | 1
chain_len | Sets the amount of shift registers chained together | `uint64_t`   | 1
delay     | Sets the clock's delay (in nanoseconds)             | `uint64_t`   | 50
 
## Tested onfigurations
Hardware      | Architecture | Kernel version | Dstribution | State
--------------|--------------|----------------|-------------|------
OrangePi Zero | armhf | 3.4 | Armbian Xenial | ![#00ff00](https://placehold.it/15/00ff00/000000?text=+) Supported
OrangePi Zero | armhf | 4.1 | Armbian Bionic | ![#ff0000](https://placehold.it/15/ff0000/000000?text=+) Unsupported

## Known Issues
 - Dosen't work with `fwrite` but works with `write` (even though they are the same system call)
   - Possibly being caused by fwrite buffering when it's not supposed to
 - No support for reset or output enable pins
 - ***NOT SUPPORTED ON KERNEL VERSION 3.4 AND ABOVE***
