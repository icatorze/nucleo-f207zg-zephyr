#include <zephyr.h>
#include <kernel.h>
#include <logging/log.h>
#include <sys/printk.h>
#include "../../../drivers/display/pcf8574/pcf8574.h"
#include "ihm/lcdi2c.h"
#include <stdio.h>

LOG_MODULE_REGISTER(main);

lcdi2c *Lcd;

void main(void){
	int contador = 0;
	char buffer[16] = {'\0'};
	Lcd = lcdi2cCreate();

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

	for (;;){
		contador++;
		Lcd->clear();
		sprintf(buffer,"%d", contador);
		Lcd->line(LINHA_1);
		Lcd->print(buffer);

		sprintf(buffer,"%d", contador*contador);
		Lcd->line(LINHA_2);
		Lcd->print(buffer);

		k_sleep(K_MSEC(1000));

	}
}
