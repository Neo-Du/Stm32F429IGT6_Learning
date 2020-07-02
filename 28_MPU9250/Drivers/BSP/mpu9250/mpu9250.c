/*
 * mpu9250.c
 * /28_MPU9250/Drivers/BSP/mpu9250/mpu9250.c
 * 9:55:41 PM
 *  Created on: Jul 1, 2020
 *      Author: Du
 */

#include "main.h"
#include "i2c.h"
#include "mpu9250.h"

extern I2C_HandleTypeDef hi2c2;

uint8_t MPU_Write_Len (unsigned char slave_addr,unsigned char reg_addr,unsigned char length,unsigned char const*data)
{
    if (HAL_I2C_Mem_Write (&hi2c2, slave_addr << 1, reg_addr, 1, data, length, 10) == HAL_OK)
    {
	return 0;
    }
    return 1;
}

uint8_t MPU_Read_Len (unsigned char slave_addr,unsigned char reg_addr,unsigned char length,unsigned char*data)
{

    if (HAL_I2C_Mem_Read (&hi2c2, slave_addr << 1, reg_addr, 1, data, length, 10) == HAL_OK)
    {
	return 0; // success
    }
    return 1;
}

void MPU_Get_ms (unsigned long*num_ms)
{
    *num_ms = HAL_GetTick ();
}

