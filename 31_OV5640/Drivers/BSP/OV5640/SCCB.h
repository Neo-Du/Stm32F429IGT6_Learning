/*
 * SCCB.h
 * /31_OV5640/Drivers/BSP/OV5640/SCCB.h
 * 10:15:49 PM
 *  Created on: Aug 24, 2020
 *      Author: Du
 */

#ifndef SCCB_H_
#define SCCB_H_

#include "main.h"

#define SCCB_SDA_OUT(n) 	HAL_GPIO_WritePin (DCMI_SDA_GPIO_Port, DCMI_SDA_Pin, n)
#define SCCB_SCL_OUT(n)		HAL_GPIO_WritePin (DCMI_SCL_GPIO_Port, DCMI_SCL_Pin, n)

#define SCCB_READ_SDA	HAL_GPIO_ReadPin (DCMI_SDA_GPIO_Port, DCMI_SDA_Pin)

#define SCCB_SET_SDA_IN()  {GPIOB->MODER&=~(3<<(3*2));GPIOB->MODER|=0<<3*2;}	//PB3 input
#define SCCB_SET_SDA_OUT() {GPIOB->MODER&=~(3<<(3*2));GPIOB->MODER|=1<<3*2;}	//PB3 output

void SCCB_Start (void);
void SCCB_Stop (void);
void SCCB_No_Ack (void);
uint8_t SCCB_WR_Byte (uint8_t data);
uint8_t SCCB_RD_Byte (void);

#endif /* SCCB_H_ */
