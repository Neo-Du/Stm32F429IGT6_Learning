/*
 * W25Q256.c
 * /21_SPI/Drivers/BSP/W25Q256/W25Q256.c
 * 8:40:04 AM
 *  Created on: May 25, 2020
 *      Author: Du
 */

#include "W25Q256.h"

uint8_t W25Q256_Init ()
{
    //Send random data:
    Enable_W25Q256;
    uint8_t data_send = 0xff;
    HAL_SPI_Transmit (&hspi5, &data_send, 1, 100);
    Disable_W25Q256;

    //Read Chip ID
    if (!W25Q256_Check_ID ())
    {
	while (1);
    }

    //Read Address mode
    W25Q256_Check_Address_mode ();
}

void W25Q256_Read (uint32_t addr,uint32_t len,uint8_t*Readdata)
{
    uint8_t data_send = 0;

    Enable_W25Q256;
    data_send = W25X_ReadDataWith4ByteAddress;
    HAL_SPI_Transmit (&hspi5, &data_send, 1, 100);

    data_send = addr >> 24;
    HAL_SPI_Transmit (&hspi5, &data_send, 1, 100);
    data_send = addr >> 16;
    HAL_SPI_Transmit (&hspi5, &data_send, 1, 100);
    data_send = addr >> 8;
    HAL_SPI_Transmit (&hspi5, &data_send, 1, 100);
    data_send = addr;
    HAL_SPI_Transmit (&hspi5, &data_send, 1, 100);

    HAL_SPI_Receive (&hspi5, Readdata, len, 1000);
    Disable_W25Q256;
}

void W25Q256_Write_page (uint32_t addr,uint32_t len,uint8_t*Writedata)
{
    W25Q256_Write_Enable ();

    uint8_t data_send = W25X_PageProgram;
    Enable_W25Q256;
    HAL_SPI_Transmit (&hspi5, &data_send, 1, 100);
    //send address
    data_send = addr >> 24;
    HAL_SPI_Transmit (&hspi5, &data_send, 1, 100);
    data_send = addr >> 16;
    HAL_SPI_Transmit (&hspi5, &data_send, 1, 100);
    data_send = addr >> 8;
    HAL_SPI_Transmit (&hspi5, &data_send, 1, 100);
    data_send = addr;
    HAL_SPI_Transmit (&hspi5, &data_send, 1, 100);

    for (int i = 0; i < len; i++)
    {
	HAL_SPI_Transmit (&hspi5, &Writedata[i], 1, 100);
    }

    Disable_W25Q256;
    W25Q256_Wait_Busy ();
}

void W25Q256_Wait_Busy ()
{
    uint8_t data_send = W25X_ReadStatusReg1;
    uint8_t data_receive = 1;
    while (data_receive & 0x01 == 1)
    {
	Enable_W25Q256;
	HAL_SPI_Transmit (&hspi5, &data_send, 1, 100);
	HAL_SPI_Receive (&hspi5, &data_receive, 1, 100);
	Disable_W25Q256;
    }
}

void W25Q256_Write_Enable ()
{
    uint8_t data_send = W25X_WriteEnable;
    Enable_W25Q256;
    HAL_SPI_Transmit (&hspi5, &data_send, 1, 100);
    Disable_W25Q256;
}

void W25Q256_Erase_Sector (uint32_t addr)
{
    uint8_t data_send = W25X_SectorErase;
    Enable_W25Q256;
    HAL_SPI_Transmit (&hspi5, &data_send, 1, 100);

    data_send = addr >> 24;
    HAL_SPI_Transmit (&hspi5, &data_send, 1, 100);
    data_send = addr >> 16;
    HAL_SPI_Transmit (&hspi5, &data_send, 1, 100);
    data_send = addr >> 8;
    HAL_SPI_Transmit (&hspi5, &data_send, 1, 100);
    data_send = addr;
    HAL_SPI_Transmit (&hspi5, &data_send, 1, 100);
    Disable_W25Q256;
    W25Q256_Wait_Busy ();
}

void W25Q256_Erase_Page (uint32_t addr)
{
    uint8_t data_send = W25X_BlockErase;
    Enable_W25Q256;
    HAL_SPI_Transmit (&hspi5, &data_send, 1, 100);

    data_send = addr >> 24;
    HAL_SPI_Transmit (&hspi5, &data_send, 1, 100);
    data_send = addr >> 16;
    HAL_SPI_Transmit (&hspi5, &data_send, 1, 100);
    data_send = addr >> 8;
    HAL_SPI_Transmit (&hspi5, &data_send, 1, 100);
    data_send = addr;
    HAL_SPI_Transmit (&hspi5, &data_send, 1, 100);
    Disable_W25Q256;

    W25Q256_Wait_Busy ();
}
// erase IC , takes around 70s
void W25Q256_Erase_Chip()
{
    W25Q256_Write_Enable ();
    W25Q256_Wait_Busy ();
    uint8_t data_send1 = 0xc7;
    Enable_W25Q256;
    HAL_SPI_Transmit (&hspi5, &data_send1, 1, 100);
    Disable_W25Q256;
    W25Q256_Wait_Busy ();
}

uint8_t W25Q256_Check_ID ()
{
    uint8_t data_send = 0;
    uint16_t data_receive = 0;
    Enable_W25Q256;
    data_send = W25X_ManufactDeviceID;
    HAL_SPI_Transmit (&hspi5, &data_send, 1, 100);
    data_send = 0x00;
    HAL_SPI_Transmit (&hspi5, &data_send, 1, 100);
    HAL_SPI_Transmit (&hspi5, &data_send, 1, 100);
    HAL_SPI_Transmit (&hspi5, &data_send, 1, 100);
    HAL_SPI_Receive (&hspi5, &data_receive, 2, 100);
    Disable_W25Q256;
    return (data_receive == 0x18EF) ? 1 : 0;
}

void W25Q256_Check_Address_mode ()
{
    uint8_t data_send = 0;
    uint8_t data_receive = 0;

    Enable_W25Q256;
    data_send = W25X_ReadStatusReg3;
    HAL_SPI_Transmit (&hspi5, &data_send, 1, 100);
    HAL_SPI_Receive (&hspi5, &data_receive, 1, 100);
    Disable_W25Q256;

    if (data_receive & 0x01)
	return;
    Enable_W25Q256;
    data_send = W25X_Enable4ByteAddr;
    HAL_SPI_Transmit (&hspi5, &data_send, 1, 100);
    Disable_W25Q256;
}
