#include <zephyr.h>
#include <kernel.h>
#include <logging/log.h>
#include <sys/printk.h>
#include <sys/util.h>
#include "../../../drivers/display/pcf8574/pcf8574.h"
#include "ihm/lcdi2c.h"
#include <stdio.h>
#include "drivers/buttonapi.h"

LOG_MODULE_REGISTER(main);

#if DT_NODE_HAS_STATUS(DT_INST(0, i14_button), okay)
#define BUTTON_DEV_NAME DT_LABEL(DT_INST(0, i14_button))
#endif

lcdi2c *Lcd;
struct device *button_dev;

static struct k_poll_signal signalbtn;

void main(void){
	int contador = 0;
	char buffer[16] = {'\0'};
	Lcd = lcdi2cCreate();

	button_dev = device_get_binding(BUTTON_DEV_NAME);

	k_poll_signal_init(&signalbtn);

	struct k_poll_event event = K_POLL_EVENT_INITIALIZER(K_POLL_TYPE_SIGNAL,
	                                 K_POLL_MODE_NOTIFY_ONLY,
	                                 &signalbtn);


	buttondrv_init(button_dev, &signalbtn);

	Lcd->init();
	k_sleep(K_MSEC(2));
	Lcd->clear();
	Lcd->print("LCD I2C PCF8574");
	k_sleep(K_MSEC(5000));

	Lcd->clear();
	k_sleep(K_MSEC(1000));

	Lcd->line(LINHA_1);
	Lcd->print("Linha 1:");
	Lcd->line(LINHA_2);
	Lcd->print("Linha 2:");

	k_sleep(K_MSEC(2000));
	k_poll_signal_reset(&signalbtn);

	event.state = K_POLL_STATE_NOT_READY;

	for (;;){
		k_poll(&event, 1, K_FOREVER);
		event.signal->signaled = 0;
		event.state = K_POLL_STATE_NOT_READY;
		contador++;
		Lcd->clear();
		sprintf(buffer,"%d", contador);
		Lcd->line(LINHA_1);
		Lcd->print(buffer);

		sprintf(buffer,"%d", contador*contador);
		Lcd->line(LINHA_2);
		Lcd->print(buffer);

		//k_sleep(K_MSEC(1000));




		//LOG_INF("%d", event.signal->result);

	}
}
