#include <string.h>
#include <device.h>
#include <drivers/i2c.h>

#include "pcf8574.h"
#include <logging/log.h>
#define DT_DRV_COMPAT i14_pcf8574

#define LOG_MODULE_NAME pcf8574
#define DEVICE_ADDRESS CONFIG_PCF8574_ADDRESS
LOG_MODULE_REGISTER(PCF8574, CONFIG_LOG_DEFAULT_LEVEL);

static int pcf8574_write_bus(
		const struct device *dev,
		uint8_t *data,
		uint32_t num_bytes)
{
	int ret;
	struct pcf8574_data *driver = dev->data;

	ret = i2c_write(driver->bus,(const uint8_t *)data,
			num_bytes,
			DT_INST_REG_ADDR(0)|DEVICE_ADDRESS);
	if (ret)
		return 1;

	return 0;

}



static int pcf8574_init(const struct device *dev) {


	struct pcf8574_data *driver = dev->data;
	const struct pcf8574_dev_config *cfg = dev->config;

	LOG_DBG("PCF8574 Driver\r\n");

	driver->bus = device_get_binding(cfg->i2c_port);
	if(driver->bus == NULL){
		LOG_ERR("Failed to get pointer to %s device.",
				cfg->i2c_port);
		return -EINVAL;
	}

	if(i2c_configure(driver->bus, cfg->i2c_cfg))
		LOG_ERR("Failed to config to %s device.",
				cfg->i2c_port);

	return 0;
}

static struct pcf8574_data pcf8574_data;

static struct pcf8574_dev_config pcf8574_config = {
		.i2c_port = DT_INST_BUS_LABEL(0),
		.i2c_addr = DT_INST_REG_ADDR(0),
		.i2c_cfg = I2C_SPEED_SET(I2C_SPEED_STANDARD) | I2C_MODE_MASTER,
};

static struct pcf8574_driver_api pcf8574_driver_api = {
		.write = pcf8574_write_bus,
};


DEVICE_DT_INST_DEFINE(
		0,
		pcf8574_init,
		device_pm_control_nop,
		&pcf8574_data,
		&pcf8574_config,
	    POST_KERNEL,
		CONFIG_APPLICATION_INIT_PRIORITY,
		&pcf8574_driver_api);

