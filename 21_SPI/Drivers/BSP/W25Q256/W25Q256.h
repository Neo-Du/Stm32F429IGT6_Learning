/*
 * W25Q256.h
 * /21_SPI/Drivers/BSP/W25Q256/W25Q256.h
 * 8:40:13 AM
 *  Created on: May 25, 2020
 *      Author: Du
 */

#ifndef BSP_W25Q256_W25Q256_H_
#define BSP_W25Q256_W25Q256_H_

#include "main.h"
#include "spi.h"

#define Enable_W25Q256			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, 0)
#define Disable_W25Q256			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, 1)


#define W25X_WriteEnable		0x06
#define W25X_WriteDisable		0x04
#define W25X_ReadStatusReg1		0x05
#define W25X_ReadStatusReg2		0x35
#define W25X_ReadStatusReg3		0x15
#define W25X_WriteStatusReg1    	0x01
#define W25X_WriteStatusReg2    	0x31
#define W25X_WriteStatusReg3   		0x11
#define W25X_ReadData			0x03
#define W25X_ReadDataWith4ByteAddress	0x13
#define W25X_FastReadData		0x0B
#define W25X_FastReadDual		0x3B
#define W25X_PageProgram		0x02
#define W25X_BlockErase			0xD8
#define W25X_SectorErase		0x20
#define W25X_ChipErase			0xC7
#define W25X_PowerDown			0xB9
#define W25X_ReleasePowerDown		0xAB
#define W25X_DeviceID			0xAB
#define W25X_ManufactDeviceID		0x90
#define W25X_JedecDeviceID		0x9F
#define W25X_Enable4ByteAddr    	0xB7
#define W25X_Exit4ByteAddr      	0xE9



uint8_t W25Q256_Init ();
uint8_t W25Q256_Check_ID ();
void W25Q256_Check_Address_mode ();

void W25Q256_Read (uint32_t addr,uint32_t len,uint8_t*Readdata);
void W25Q256_Write_page (uint32_t addr,uint32_t len,uint8_t*Writedata);

void W25Q256_Write_Enable ();
void W25Q256_Erase_Sector(uint32_t addr);
void W25Q256_Erase_Page (uint32_t addr);
void W25Q256_Erase_Chip();

void W25Q256_Wait_Busy ();
#endif /* BSP_W25Q256_W25Q256_H_ */
//#ifndef __W25QXX_H
//#define __W25QXX_H
//#include "main.h"
//#define Enable_W25Q256			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, 0)
//#define Disable_W25Q256			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, 1)
//
//typedef int32_t  s32;
//typedef int16_t s16;
//typedef int8_t  s8;
//
//typedef const int32_t sc32;
//typedef const int16_t sc16;
//typedef const int8_t sc8;
//
//typedef __IO int32_t  vs32;
//typedef __IO int16_t  vs16;
//typedef __IO int8_t   vs8;
//
//typedef __I int32_t vsc32;
//typedef __I int16_t vsc16;
//typedef __I int8_t vsc8;
//
//typedef uint32_t  u32;
//typedef uint16_t u16;
//typedef uint8_t  u8;
//
//typedef const uint32_t uc32;
//typedef const uint16_t uc16;
//typedef const uint8_t uc8;
//
//typedef __IO uint32_t  vu32;
//typedef __IO uint16_t vu16;
//typedef __IO uint8_t  vu8;
//
//typedef __I uint32_t vuc32;
//typedef __I uint16_t vuc16;
//typedef __I uint8_t vuc8;
//
////位带操作,实现51类似的GPIO控制功能
////具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).M4同M3类似,只是寄存器地址变了.
////IO口操作宏定义
//#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
//#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))
//#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))
////IO口地址映射
//#define GPIOA_ODR_Addr    (GPIOA_BASE+20) //0x40020014
//#define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40020414
//#define GPIOC_ODR_Addr    (GPIOC_BASE+20) //0x40020814
//#define GPIOD_ODR_Addr    (GPIOD_BASE+20) //0x40020C14
//#define GPIOE_ODR_Addr    (GPIOE_BASE+20) //0x40021014
//#define GPIOF_ODR_Addr    (GPIOF_BASE+20) //0x40021414
//#define GPIOG_ODR_Addr    (GPIOG_BASE+20) //0x40021814
//#define GPIOH_ODR_Addr    (GPIOH_BASE+20) //0x40021C14
//#define GPIOI_ODR_Addr    (GPIOI_BASE+20) //0x40022014
//#define GPIOJ_ODR_ADDr    (GPIOJ_BASE+20) //0x40022414
//#define GPIOK_ODR_ADDr    (GPIOK_BASE+20) //0x40022814
//
//#define GPIOA_IDR_Addr    (GPIOA_BASE+16) //0x40020010
//#define GPIOB_IDR_Addr    (GPIOB_BASE+16) //0x40020410
//#define GPIOC_IDR_Addr    (GPIOC_BASE+16) //0x40020810
//#define GPIOD_IDR_Addr    (GPIOD_BASE+16) //0x40020C10
//#define GPIOE_IDR_Addr    (GPIOE_BASE+16) //0x40021010
//#define GPIOF_IDR_Addr    (GPIOF_BASE+16) //0x40021410
//#define GPIOG_IDR_Addr    (GPIOG_BASE+16) //0x40021810
//#define GPIOH_IDR_Addr    (GPIOH_BASE+16) //0x40021C10
//#define GPIOI_IDR_Addr    (GPIOI_BASE+16) //0x40022010
//#define GPIOJ_IDR_Addr    (GPIOJ_BASE+16) //0x40022410
//#define GPIOK_IDR_Addr    (GPIOK_BASE+16) //0x40022810
//
////IO口操作,只对单一的IO口!
////确保n的值小于16!
//#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出
//#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入
//
//#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出
//#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入
//
//#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出
//#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入
//
//#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出
//#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入
//
//#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出
//#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入
//
//#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出
//#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入
//
//#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出
//#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入
//
//#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //输出
//#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //输入
//
//#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //输出
//#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //输入
//
//#define PJout(n)   BIT_ADDR(GPIOJ_ODR_Addr,n)  //输出
//#define PJin(n)    BIT_ADDR(GPIOJ_IDR_Addr,n)  //输入
//
//#define PKout(n)   BIT_ADDR(GPIOK_ODR_Addr,n)  //输出
//#define PKin(n)    BIT_ADDR(GPIOK_IDR_Addr,n)  //输入
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////
////本程序只供学习使用，未经作者许可，不得用于其它任何用途
////ALIENTEK STM32F429开发板
////W25QXX驱动代码
////正点原子@ALIENTEK
////技术论坛:www.openedv.com
////创建日期:2016/1/16
////版本：V1.0
////版权所有，盗版必究。
////Copyright(C) 广州市星翼电子科技有限公司 2014-2024
////All rights reserved
////////////////////////////////////////////////////////////////////////////////////
//
////W25X系列/Q系列芯片列表
////W25Q80  ID  0XEF13
////W25Q16  ID  0XEF14
////W25Q32  ID  0XEF15
////W25Q64  ID  0XEF16
////W25Q128 ID  0XEF17
////W25Q256 ID  0XEF18
//#define W25Q80 	0XEF13
//#define W25Q16 	0XEF14
//#define W25Q32 	0XEF15
//#define W25Q64 	0XEF16
//#define W25Q128	0XEF17
//#define W25Q256 0XEF18
//
//extern u16 W25QXX_TYPE;					//定义W25QXX芯片型号
//
//#define	W25QXX_CS 		PFout(6)  		//W25QXX的片选信号
//
////////////////////////////////////////////////////////////////////////////////////
////指令表
//#define W25X_WriteEnable		0x06
//#define W25X_WriteDisable		0x04
//#define W25X_ReadStatusReg1		0x05
//#define W25X_ReadStatusReg2		0x35
//#define W25X_ReadStatusReg3		0x15
//#define W25X_WriteStatusReg1    0x01
//#define W25X_WriteStatusReg2    0x31
//#define W25X_WriteStatusReg3    0x11
//#define W25X_ReadData			0x03
//#define W25X_FastReadData		0x0B
//#define W25X_FastReadDual		0x3B
//#define W25X_PageProgram		0x02
//#define W25X_BlockErase			0xD8
//#define W25X_SectorErase		0x20
//#define W25X_ChipErase			0xC7
//#define W25X_PowerDown			0xB9
//#define W25X_ReleasePowerDown	0xAB
//#define W25X_DeviceID			0xAB
//#define W25X_ManufactDeviceID	0x90
//#define W25X_JedecDeviceID		0x9F
//#define W25X_Enable4ByteAddr    0xB7
//#define W25X_Exit4ByteAddr      0xE9
//
//void W25QXX_Init(void);
//u16  W25QXX_ReadID(void);  	    		//读取FLASH ID
//u8 W25QXX_ReadSR(u8 regno);             //读取状态寄存器
//void W25QXX_4ByteAddr_Enable(void);     //使能4字节地址模式
//void W25QXX_Write_SR(u8 regno,u8 sr);   //写状态寄存器
//void W25QXX_Write_Enable(void);  		//写使能
//void W25QXX_Write_Disable(void);		//写保护
//void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
//void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //读取flash
//void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//写入flash
//void W25QXX_Erase_Chip(void);    	  	//整片擦除
//void W25QXX_Erase_Sector(u32 Dst_Addr);	//扇区擦除
//void W25QXX_Wait_Busy(void);           	//等待空闲
//void W25QXX_PowerDown(void);        	//进入掉电模式
//void W25QXX_WAKEUP(void);				//唤醒
//
//#endif
