/*
 * PCF8574.h
 * /22_485/BSP/PCF8574/PCF8574.h
 * 9:48:04 PM
 *  Created on: Jun 17, 2020
 *      Author: Du
 */
#include "main.h"

#ifndef BSP_PCF8574_PCF8574_H_
#define BSP_PCF8574_PCF8574_H_

#define PCF_ADDRESS 0x20<<1

uint8_t PCF8574_read_bit(uint8_t bit);
uint8_t PCF8574_read_word();
void PCF8574_write_bit(uint8_t bit,uint8_t state);
void PCF8574_write_word(uint8_t state);

void test();

#endif /* BSP_PCF8574_PCF8574_H_ */
