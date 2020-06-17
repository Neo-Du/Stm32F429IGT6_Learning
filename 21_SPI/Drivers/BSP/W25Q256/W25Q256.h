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

#define Enable_W25Q256			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, 0)
#define Disable_W25Q256			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, 1)

uint8_t W25Q256_init ();
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
