/*
 * lcdi2c.c
 *
 *  Created on: Apr 3, 2021
 *      Author: jack
 */

#include <string.h>
#include <device.h>
#include <logging/log.h>
#include "../../../../drivers/display/pcf8574/pcf8574.h"
#include "lcdi2c.h"


#if DT_NODE_HAS_STATUS(DT_INST(0, i14_pcf8574), okay)
#define DISPLAY_DEV_NAME DT_LABEL(DT_INST(0, i14_pcf8574))
#endif
LOG_MODULE_REGISTER(lcdi2c, CONFIG_LOG_DEFAULT_LEVEL);

struct device *pcf8574_dev;
bool Backlight = 1;

static int writeI2C(uint8_t halfByte, bool isCmd, bool enable){
	int ret;
	uint8_t d = 0;
	d |= halfByte << 4;
	if (!isCmd)
		d |= RS;
	if (enable)
		d |= CS;
	if (Backlight)
		d |= BKL;
	ret = pcf8574_write(pcf8574_dev,&d, 1);

	return ret;
}

static int writeNibble(uint8_t halfByte, bool isCmd){
	int ret;
	ret = writeI2C(halfByte, isCmd, 1);
	k_sleep(K_USEC(10));
	ret |= writeI2C(halfByte, isCmd, 0);
	k_sleep(K_USEC(10));

	return ret;
}


static int writeByte(uint8_t data, bool isCmd){
	int ret;
	ret = writeNibble(data >> 4 & 0x0f, isCmd);
	ret |= writeNibble(data & 0x0f, isCmd);
	return ret;
}

static int writeChar(uint8_t ch){
	int ret;
	ret = writeByte(ch, 0);
	return ret;
}

static int line(int line){
	int ret;
	ret = writeByte(line, 1);
	return ret;
}

static int clear(void){
	int ret;
	ret = writeByte(CMD_CLEARDISPLAY, 1);
	k_sleep(K_MSEC(2));
	return ret;
}

static int print(const char *buffer){
	int ret = 0;
	for (int j=0;j<strlen(buffer);j++){
		ret |= writeChar(buffer[j]);
	}
	return ret;
}

static int init(void){

	LOG_INF("Display sample for %s", DISPLAY_DEV_NAME);

	pcf8574_dev = device_get_binding(DISPLAY_DEV_NAME);

	if (pcf8574_dev == NULL) {
		LOG_ERR("Device %s not found. Aborting sample.",
			DISPLAY_DEV_NAME);
	}

	int ret = 0;

	writeI2C(0x00, 1, false);
	k_sleep(K_MSEC(20));
	ret = writeNibble(INIT_CODE, 1);
	k_sleep(K_MSEC(5));
	ret |= writeNibble(INIT_CODE, 1);
	k_sleep(K_MSEC(1));
	ret |= writeNibble(INIT_CODE, 1);
	k_sleep(K_MSEC(1));
	ret |= writeNibble(FOURBIT_MODE, 1);
	k_sleep(K_MSEC(1));
	ret |= writeByte(FUNCTION_SET, 1);
	k_sleep(K_MSEC(1));
	ret |= writeByte(SCREEN_CLEAR, 1);
	k_sleep(K_MSEC(1));
	ret |= writeByte(LEFT2RIGHT, 1);
	k_sleep(K_MSEC(1));
	ret |= writeByte(LCD_DISPLAYCONTROL|LCD_DISPLAYON, 1);
	k_sleep(K_MSEC(1));
	ret |= writeByte(LCD_ENTRYMODESET|LCD_ENTRYLEFT, 1);
	k_sleep(K_MSEC(1));
	return ret;
}

lcdi2c *lcdi2cCreate(void){
	static lcdi2c const this = {init,print,line,clear};
	return (lcdi2c*)&this;
}
