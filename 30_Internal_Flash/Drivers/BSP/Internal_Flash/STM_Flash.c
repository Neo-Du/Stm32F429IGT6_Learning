/*
 * STM_Flash.c
 * /30_Internal_Flash/Drivers/BSP/Internal_Flash/STM_Flash.c
 * 下午11:17:47
 *  Created on: 2020年8月23日
 *      Author: Du
 */
#include "STM_Flash.h"

uint32_t STMFLASH_ReadWord (uint32_t faddr)
{
    return *(__IO uint32_t*) faddr;
}
uint32_t STMFLASH_GetFlashSector (uint32_t addr)
{
    if (addr < ADDR_FLASH_SECTOR_1)
	return FLASH_SECTOR_0;
    else if (addr < ADDR_FLASH_SECTOR_2)
	return FLASH_SECTOR_1;
    else if (addr < ADDR_FLASH_SECTOR_3)
	return FLASH_SECTOR_2;
    else if (addr < ADDR_FLASH_SECTOR_4)
	return FLASH_SECTOR_3;
    else if (addr < ADDR_FLASH_SECTOR_5)
	return FLASH_SECTOR_4;
    else if (addr < ADDR_FLASH_SECTOR_6)
	return FLASH_SECTOR_5;
    else if (addr < ADDR_FLASH_SECTOR_7)
	return FLASH_SECTOR_6;
    else if (addr < ADDR_FLASH_SECTOR_8)
	return FLASH_SECTOR_7;
    else if (addr < ADDR_FLASH_SECTOR_9)
	return FLASH_SECTOR_8;
    else if (addr < ADDR_FLASH_SECTOR_10)
	return FLASH_SECTOR_9;
    else if (addr < ADDR_FLASH_SECTOR_11)
	return FLASH_SECTOR_10;
    else if (addr < ADDR_FLASH_SECTOR_12)
	return FLASH_SECTOR_11;
    else if (addr < ADDR_FLASH_SECTOR_13)
	return FLASH_SECTOR_12;
    else if (addr < ADDR_FLASH_SECTOR_14)
	return FLASH_SECTOR_13;
    else if (addr < ADDR_FLASH_SECTOR_15)
	return FLASH_SECTOR_14;
    else if (addr < ADDR_FLASH_SECTOR_16)
	return FLASH_SECTOR_15;
    else if (addr < ADDR_FLASH_SECTOR_17)
	return FLASH_SECTOR_16;
    else if (addr < ADDR_FLASH_SECTOR_18)
	return FLASH_SECTOR_17;
    else if (addr < ADDR_FLASH_SECTOR_19)
	return FLASH_SECTOR_18;
    else if (addr < ADDR_FLASH_SECTOR_20)
	return FLASH_SECTOR_19;
    else if (addr < ADDR_FLASH_SECTOR_21)
	return FLASH_SECTOR_20;
    else if (addr < ADDR_FLASH_SECTOR_22)
	return FLASH_SECTOR_21;
    else if (addr < ADDR_FLASH_SECTOR_23)
	return FLASH_SECTOR_22;
    return FLASH_SECTOR_23;
}

void STMFLASH_Write (uint32_t WriteAddr,uint32_t*pBuffer,uint32_t NumToWrite)		//从指定地址开始写入指定长度的数据
{
    FLASH_EraseInitTypeDef FlashEraseInit;
    HAL_StatusTypeDef FlashStatus = HAL_OK;
    uint32_t SectorError = 0;
    uint32_t addrx = 0;
    uint32_t endaddr = 0;
    if (WriteAddr < STM32_FLASH_BASE || WriteAddr % 4)
	return;	//非法地址

    HAL_FLASH_Unlock ();             //解锁
    addrx = WriteAddr;				//写入的起始地址
    endaddr = WriteAddr + NumToWrite * 4;	//写入的结束地址

    if (addrx < 0X1FFF0000)
    {
	while (addrx < endaddr)		//扫清一切障碍.(对非FFFFFFFF的地方,先擦除)
	{
	    if (STMFLASH_ReadWord (addrx) != 0XFFFFFFFF)		//有非0XFFFFFFFF的地方,要擦除这个扇区
	    {
		FlashEraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;       //擦除类型，扇区擦除
		FlashEraseInit.Sector = STMFLASH_GetFlashSector (addrx);   //要擦除的扇区
		FlashEraseInit.NbSectors = 1;                             //一次只擦除一个扇区
		FlashEraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;      //电压范围，VCC=2.7~3.6V之间!!
		if (HAL_FLASHEx_Erase (&FlashEraseInit, &SectorError) != HAL_OK)
		{
		    break;      //发生错误了
		}
	    }
	    else
		addrx += 4;
	    FLASH_WaitForLastOperation (FLASH_WAITETIME);                //等待上次操作完成
	}
    }
    FlashStatus = FLASH_WaitForLastOperation (FLASH_WAITETIME);            //等待上次操作完成
    if (FlashStatus == HAL_OK)
    {
	while (WriteAddr < endaddr)            //写数据
	{
	    if (HAL_FLASH_Program (FLASH_TYPEPROGRAM_WORD, WriteAddr, *pBuffer) != HAL_OK)            //写入数据
	    {
		break;	//写入异常
	    }
	    WriteAddr += 4;
	    pBuffer++;
	}
    }
    HAL_FLASH_Lock ();           //上锁
}
void STMFLASH_Read (uint32_t ReadAddr,uint32_t*pBuffer,uint32_t NumToRead)   		//从指定地址开始读出指定长度的数据
{
    uint32_t i;
    for (i = 0; i < NumToRead; i++)
    {
	pBuffer[i] = STMFLASH_ReadWord (ReadAddr);   		//读取4个字节.
	ReadAddr += 4;   		//偏移4个字节.
    }
}
//测试写入
void Test_Write (uint32_t WriteAddr,uint32_t WriteData)
{
    STMFLASH_Write (WriteAddr, &WriteData, 1);   		//写入一个字
}
