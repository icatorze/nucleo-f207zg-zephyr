#ifndef __BUTTON_H
#define __BUTTON_H

#include <device.h>

struct button_data {
	const struct device *dev;
	const struct device *gpio;
	struct gpio_callback gpio_cb;
};

#endif
