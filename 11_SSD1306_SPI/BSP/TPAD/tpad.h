/*
 * tpad.h
 *  Created on: Mar 22, 2020
 *      Author: Du
 */

#ifndef TPAD_TPAD_H_
#define TPAD_TPAD_H_

#include "main.h"
#include "stdio.h"

uint8_t tpad_init (void);
uint16_t tpad_get_value (void);
void tpad_Reset (void);
uint16_t TPAD_Get_MaxVal(uint8_t n);
uint8_t TPAD_Scan(uint8_t);



#endif /* TPAD_TPAD_H_ */
