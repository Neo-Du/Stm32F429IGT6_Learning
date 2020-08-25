/*
 * OV5640.h
 * /31_OV5640/Drivers/BSP/OV5640/OV5640.h
 * 10:00:18 PM
 *  Created on: Aug 24, 2020
 *      Author: Du
 */

#ifndef OV5640_H_
#define OV5640_H_

//#define OV5640_PWDN  	PGout(9)			//POWER DOWN控制信号,由PCF8574_P2控制
#define OV5640_RST  	PAout(15)//复位控制信号
//////////////////////////////////////////////////////////////////////////////////
#define OV5640_ID               0X5640  	//OV5640的芯片ID

#define OV5640_ADDR        		0X78		//OV5640的IIC地址

//OV5640相关寄存器定义
#define OV5640_CHIPIDH          0X300A  	//OV5640芯片ID高字节
#define OV5640_CHIPIDL          0X300B  	//OV5640芯片ID低字节

uint8_t OV5640_WR_Reg (uint16_t reg,uint8_t data);
uint8_t OV5640_RD_Reg (uint16_t reg);
void OV5640_PWDN_Set (uint8_t sta);
uint8_t OV5640_Init (void);
void OV5640_JPEG_Mode (void);
void OV5640_RGB565_Mode (void);
void OV5640_Exposure (uint8_t exposure);
void OV5640_Light_Mode (uint8_t mode);
void OV5640_Color_Saturation (uint8_t sat);
void OV5640_Brightness (uint8_t bright);
void OV5640_Contrast (uint8_t contrast);
void OV5640_Sharpness (uint8_t sharp);
void OV5640_Special_Effects (uint8_t eft);
void OV5640_Test_Pattern (uint8_t mode);
void OV5640_Flash_Ctrl (uint8_t sw);
uint8_t OV5640_OutSize_Set (uint16_t offx,uint16_t offy,uint16_t width,uint16_t height);
uint8_t OV5640_ImageWin_Set (uint16_t offx,uint16_t offy,uint16_t width,uint16_t height);
uint8_t OV5640_Focus_Init (void);
uint8_t OV5640_Focus_Single (void);
uint8_t OV5640_Focus_Constant (void);

#endif /* OV5640_H_ */
