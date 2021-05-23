#ifndef __BUTTONAPI_H
#define __BUTTONAPI_H

#include <device.h>

typedef int (*buttondrv_init_api)(const struct device *dev, 
		struct k_poll_signal *signal);

struct button_driver_api {
	buttondrv_init_api init;
};

static inline int buttondrv_init(const struct device *dev, 
		struct k_poll_signal *signal)
{
	struct button_driver_api *api =
		(struct button_driver_api *)dev->api;

	return api->init(dev, signal);
}

#endif
