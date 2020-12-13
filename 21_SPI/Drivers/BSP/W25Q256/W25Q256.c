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
    data_send = W25X_ReadData;
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
    W25Q256_Wait_Busy ();
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
    uint8_t data_send2 = 0xff;
    uint8_t data_receive = 0xFF;
    while (data_receive & 0x01 == 1)
    {
	Enable_W25Q256;
	HAL_SPI_Transmit (&hspi5, &data_send, 1, 100);
//	HAL_SPI_Receive (&hspi5, &data_receive, 1, 100);
	HAL_SPI_TransmitReceive(&hspi5, &data_send2, &data_receive, 1, 100);
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
    W25Q256_Write_Enable ();
    W25Q256_Wait_Busy ();
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

void W25Q256_Erase_Block (uint32_t addr)
{
    uint8_t data_send = W25X_BlockErase;

    W25Q256_Write_Enable ();
    W25Q256_Wait_Busy ();

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
void W25Q256_Erase_Chip ()
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

uint8_t W25QXX_BUFFER[4096];
void W25QXX_Write (uint32_t WriteAddr,uint16_t NumByteToWrite,uint8_t*pBuffer)
{
    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i;
    uint8_t *W25QXX_BUF;
    W25QXX_BUF = W25QXX_BUFFER;
    secpos = WriteAddr / 4096; //扇区地址
    secoff = WriteAddr % 4096; //在扇区内的偏移
    secremain = 4096 - secoff; //扇区剩余空间大小

    if (NumByteToWrite <= secremain)
	secremain = NumByteToWrite; //不大于4096个字节
    while (1)
    {
	W25Q256_Read (secpos * 4096, 4096, W25QXX_BUF);
	for (i = 0; i < secremain; i++) //校验数据
	{
	    if (W25QXX_BUF[secoff + i] != 0XFF)
		break; //需要擦除
	}
	if (i < secremain) //需要擦除
	{
	    W25Q256_Erase_Sector (secpos); //擦除这个扇区
	    for (i = 0; i < secremain; i++)	 //复制
	    {
		W25QXX_BUF[i + secoff] = pBuffer[i];
	    }
	    W25QXX_Write_NoCheck (W25QXX_BUF, secpos * 4096, 4096);	 //写入整个扇区

	}
	else
	    W25QXX_Write_NoCheck (pBuffer, WriteAddr, secremain);	 //写已经擦除了的,直接写入扇区剩余区间.
	if (NumByteToWrite == secremain)
	    break;	 //写入结束了
	else	 //写入未结束
	{
	    secpos++;	 //扇区地址增1
	    secoff = 0;	 //偏移位置为0

	    pBuffer += secremain;  //指针偏移
	    WriteAddr += secremain;  //写地址偏移
	    NumByteToWrite -= secremain;				//字节数递减
	    if (NumByteToWrite > 4096)
		secremain = 4096;	//下一个扇区还是写不完
	    else
		secremain = NumByteToWrite;			//下一个扇区可以写完了
	}
    };
}

void W25QXX_Write_NoCheck (uint8_t*pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
    uint16_t pageremain;
    pageremain = 256 - WriteAddr % 256; //单页剩余的字节数
    if (NumByteToWrite <= pageremain)
	pageremain = NumByteToWrite; //不大于256个字节
    while (1)
    {
	W25Q256_Write_page (WriteAddr , pageremain , pBuffer);
	if (NumByteToWrite == pageremain)
	    break; //写入结束了
	else //NumByteToWrite>pageremain
	{
	    pBuffer += pageremain;
	    WriteAddr += pageremain;

	    NumByteToWrite -= pageremain;			  //减去已经写入了的字节数
	    if (NumByteToWrite > 256)
		pageremain = 256; //一次可以写入256个字节
	    else
		pageremain = NumByteToWrite; 	  //不够256个字节了
	}
    };
}

