/*
 * SSD1306_SPI.h
 * /11_SSD1306_SPI/BSP/SSD1306_SPI/SSD1306_SPI.h
 * 1:23:08 AM
 *  Created on: Mar 23, 2020
 *      Author: Du
 */

#ifndef SSD1306_SPI_SSD1306_SPI_H_
#define SSD1306_SPI_SSD1306_SPI_H_

#include "main.h"
//#include "pic_1.h"

#define OLED_CMD  	0		//写命令
#define OLED_DATA 	1		//写数据

void OLED_WR_Byte (uint8_t dat,uint8_t cmd);
void OLED_Display_On (void);
void OLED_Display_Off (void);
void OLED_Refresh_Gram (void);

void OLED_Init (void);
void OLED_Clear (void);
void OLED_Fill (uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void OLED_Send_pic (void);

#endif /* SSD1306_SPI_SSD1306_SPI_H_ */
