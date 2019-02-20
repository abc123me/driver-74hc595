#include "linux/gpio.h"

static inline bool gpio_is_valid(int number)            // check validity of GPIO number (max on BBB is 127)
static inline int  gpio_request(unsigned gpio, const char *label)        // allocate the GPIO number, the label is for sysfs
static inline int  gpio_export(unsigned gpio, bool direction_may_change) // make available via sysfs and decide if it can change from input to output and vice versa
static inline int  gpio_direction_input(unsigned gpio)  // an input line (as usual, return of 0 is success)
static inline int  gpio_get_value(unsigned gpio)        // get the value of the GPIO line
static inline int  gpio_direction_output(unsigned gpio, int value)       // value is the state
static inline int  gpio_set_debounce(unsigned gpio, unsigned debounce)   // set debounce time in ms (platform dependent)
static inline int  gpio_sysfs_set_active_low(unsigned gpio, int value)   // set active low (invert operation states)
static inline void gpio_unexport(unsigned gpio)         // remove from sysfs
static inline void gpio_free(unsigned gpio)             // deallocate the GPIO line
static inline int  gpio_to_irq(unsigned gpio)           // associate with an IRQ
