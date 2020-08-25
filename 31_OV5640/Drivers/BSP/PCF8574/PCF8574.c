/*
 * PCF8574.c
 * /20.1_I2C_PCF8574/Drivers/PCF8574/PCF8574.c
 * 上午12:29:22
 *  Created on: 2020年8月26日
 *      Author: Du
 */
#include "main.h"
#include "PCF8574.h"
#include "i2c.h"

extern I2C_HandleTypeDef hi2c2;

uint8_t PCF8574_Init (void)
{
    uint8_t temp = 0XFF;
    return (HAL_I2C_Master_Transmit (&hi2c2, PCF8574_ADDR, &temp, 1, 100));
}

uint8_t PCF8574_Write_bit (uint8_t pin,uint8_t status)
{
    uint8_t temp_buf = 0;
    HAL_I2C_Master_Receive (&hi2c2, PCF8574_ADDR, &temp_buf, 1, 100);
    if (status)
    {
	temp_buf |= 1 << pin;
    }
    else
    {
	temp_buf &= ~(1 << pin);
    }
    HAL_I2C_Master_Transmit (&hi2c2, PCF8574_ADDR, &temp_buf, 1, 100);
}

uint8_t PCF8574_Read_bit (uint8_t pin)
{
    uint8_t read_data = 0, temp_buf = 0;

    HAL_I2C_Master_Receive (&hi2c2, PCF8574_ADDR, &read_data, 1, 100);
    return ((read_data >> pin) & 1);
}

uint8_t PCF8574_Write_byte (uint8_t byte)
{
    return (HAL_I2C_Master_Transmit (&hi2c2, PCF8574_ADDR, &byte, 1, 100));
}

uint8_t PCF8574_Read_byte (void)
{
    uint8_t read_data = 0;
    HAL_I2C_Master_Receive (&hi2c2, PCF8574_ADDR, &read_data, 1, 100);
    return read_data;
}
