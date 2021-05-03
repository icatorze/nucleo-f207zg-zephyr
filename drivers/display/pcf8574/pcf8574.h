#ifndef __PCF8574_H_
#define __PCF8574_H_

#include <device.h>

typedef int (*pcf8574_write_api)(const struct device *dev,
		uint8_t *data,
		uint32_t num_bytes);

struct pcf8574_driver_api {
	pcf8574_write_api write;
};

struct pcf8574_data {
	const struct device *reset;
	const struct device *bus;
};

struct pcf8574_dev_config {
	const char *i2c_port;
	uint16_t i2c_addr;
	uint32_t i2c_cfg;
};

static inline int pcf8574_write(const struct device *dev,
		uint8_t *data,
		uint32_t num_bytes)
{
	struct pcf8574_driver_api *api =
		(struct pcf8574_driver_api *)dev->api;

	return api->write(dev, data, num_bytes);
}

#endif

