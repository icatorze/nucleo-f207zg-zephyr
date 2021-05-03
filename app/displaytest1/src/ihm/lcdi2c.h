/*
 * lcdi2c.h
 *
 *  Created on: Apr 3, 2021
 *      Author: jack
 */

#ifndef SRC_DRIVERS_LCDI2C_LCDI2C_H_
#define SRC_DRIVERS_LCDI2C_LCDI2C_H_

#define RS 				0x01
#define RW				0x02
#define CS				0x04
#define BKL				0x08

#define INIT_CODE 		0x3
#define FOURBIT_MODE	0x2

#define CMD_CLEARDISPLAY 0x01

#define LCD_2LINE		0x08
#define LCD_DISPLAYCONTROL 0x08
#define LCD_DISPLAYON	0x04

#define LCD_ENTRYMODESET 0x04
#define LCD_ENTRYLEFT	0x02

#define FUNCTION_SET	0x20|LCD_2LINE //1<<5|1<<4|1<<3|0<<2
#define SCREEN_CLEAR	0x01
#define LEFT2RIGHT		0x04|0x02
//#define INPUT_SET		0x07

#define LINHA_1 		0x80
#define LINHA_2			0xc0

typedef struct _lcdi2c_create {
	int (*init)(void);
//	int (*writeI2C)(uint8_t halfByte, bool isCmd, bool enable);
//	int (*writeNibble)(uint8_t halfByte, bool isCmd);
	int (*print)(const char *);
	int (*line)(int);
	int (*clear)(void);
} lcdi2c;

lcdi2c  *lcdi2cCreate(void);

#endif /* SRC_DRIVERS_LCDI2C_LCDI2C_H_ */
