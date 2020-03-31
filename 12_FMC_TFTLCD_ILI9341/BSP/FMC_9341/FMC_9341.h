/*
 * FMC_9341.h
 * /12_FMC_TFTLCD_ILI9341/BSP/FMC_9341/FMC_9341.h
 * 12:59:51 AM
 *  Created on: Mar 29, 2020
 *      Author: Du
 */

#ifndef FMC_9341_FMC_9341_H_
#define FMC_9341_FMC_9341_H_

#include "main.h"

//LCD重要参数集
typedef struct
{
    uint16_t width;			//LCD 宽度
    uint16_t height;			//LCD 高度
    uint16_t id;				//LCD ID
    uint8_t dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。
    uint16_t wramcmd;		//开始写gram指令
    uint16_t setxcmd;		//设置x坐标指令
    uint16_t setycmd;		//设置y坐标指令
} _lcd_dev;

typedef struct
{
    __IO uint16_t LCD_REG;
    __IO uint16_t LCD_RAM;
} LCD_TypeDef;
//使用NOR/SRAM的 Bank1.sector1,地址位HADDR[27,26]=00 A18作为数据命令区分线
//注意设置时STM32内部会右移一位对其!

#define LCD_BASE        ((uint32_t)(0x60000000 | 0x0007FFFE))
#define LCD             ((LCD_TypeDef *) LCD_BASE)

//扫描方向定义
#define L2R_U2D  0 		//从左到右,从上到下
#define L2R_D2U  1 		//从左到右,从下到上
#define R2L_U2D  2 		//从右到左,从上到下
#define R2L_D2U  3 		//从右到左,从下到上

#define U2D_L2R  4 		//从上到下,从左到右
#define U2D_R2L  5 		//从上到下,从右到左
#define D2U_L2R  6 		//从下到上,从左到右
#define D2U_R2L  7		//从下到上,从右到左

#define DFT_SCAN_DIR  L2R_U2D  //默认的扫描方向

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000
#define BLUE         	 0x001F
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色

#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)


void LCD_WR_REG (__IO uint16_t regval);

uint16_t LCD_RD_DATA (void);

void LCD_WR_DATA (__IO uint16_t data);

void LCD_Init (void);

void LCD_Display_Dir (uint8_t dir);

void LCD_Scan_Dir (uint8_t dir);

void LCD_Clear (uint32_t color);

void LCD_WriteRAM_Prepare (void);

void LCD_SetCursor (uint16_t Xpos,uint16_t Ypos);

void LCD_WriteReg (uint16_t LCD_Reg,uint16_t LCD_RegValue);












#endif /* FMC_9341_FMC_9341_H_ */
