#include <string.h>
#include <device.h>
#include <drivers/gpio.h>
#include <sys/util.h>

#include "button.h"
#include "drivers/buttonapi.h"
#include <logging/log.h>
#define DT_DRV_COMPAT i14_button

#define LOG_MODULE_NAME BUTTON
LOG_MODULE_REGISTER(BUTTON, CONFIG_LOG_DEFAULT_LEVEL);

static struct k_poll_signal *signalbtn;

static void button_gpio_callback(const struct device *dev,
				   struct gpio_callback *cb, uint32_t pins)
{
	struct button_data *driver = dev->data;

	gpio_pin_interrupt_configure(driver->gpio,
					     DT_INST_GPIO_PIN(0, int_gpios),
					     GPIO_INT_DISABLE);

	LOG_INF("Interrupt");
	
	k_poll_signal_raise(signalbtn, 0x1337);


	gpio_pin_interrupt_configure(driver->gpio,
					     DT_INST_GPIO_PIN(0, int_gpios),
						 GPIO_INT_DEBOUNCE | GPIO_INT_EDGE_TO_ACTIVE);
}

static int button_init(const struct device *dev, struct k_poll_signal *signal){
	int ret;
	struct button_data *driver = dev->data;
	ret = gpio_pin_interrupt_configure(driver->gpio,
						     DT_INST_GPIO_PIN(0, int_gpios),
							 GPIO_INT_DEBOUNCE| GPIO_INT_EDGE_TO_ACTIVE);
	if(ret){
		return 1;
	}

	signalbtn = signal;
	return 0;

}

static int button_init_drv(const struct device *dev){
	int ret;
	struct button_data *driver = dev->data;
	
	driver->gpio = device_get_binding(DT_INST_GPIO_LABEL(0, int_gpios));
	if (driver->gpio == NULL) {
			LOG_ERR("Failed to get pointer to %s device.",
					DT_INST_GPIO_LABEL(0, int_gpios));
			return -EINVAL;
	}
	
	ret = gpio_pin_configure(driver->gpio,
				   DT_INST_GPIO_PIN(0, int_gpios),
				   GPIO_INPUT |
				   DT_INST_GPIO_FLAGS(0, int_gpios));
	
	if(ret<0) {
		LOG_ERR("Button GPIO configure error: %d", ret);
		return ret;
	}


	gpio_init_callback(&driver->gpio_cb,
				  button_gpio_callback,
				   BIT(DT_INST_GPIO_PIN(0, int_gpios)));

	if (gpio_add_callback(driver->gpio, &driver->gpio_cb) < 0) {
		LOG_ERR("Failed to set gpio callback.");
		return -EIO;
	}
	
	return 0;
}

static struct button_data button_data;

static struct button_driver_api button_driver_api = {
	.init = button_init,
};

DEVICE_DT_INST_DEFINE(
		0,
		button_init_drv,
		device_pm_control_nop,
		&button_data,
		NULL,
	    POST_KERNEL,
		CONFIG_APPLICATION_INIT_PRIORITY,
		&button_driver_api);
