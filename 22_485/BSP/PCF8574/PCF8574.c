/*
 * PCF8574.c
 * /22_485/BSP/PCF8574/PCF8574.c
 * 9:47:54 PM
 *  Created on: Jun 17, 2020
 *      Author: Du
 */
//todo add INT
#include "PCF8574.h"

I2C_HandleTypeDef hi2c2;

uint8_t PCF8574_read_bit (uint8_t bit)
{
    uint8_t receive_byte = 0;
    uint8_t temp_byte = 1 << bit;
    HAL_I2C_Master_Receive (&hi2c2, PCF_ADDRESS, &receive_byte, 1, 10);
    receive_byte &= temp_byte;
    return receive_byte;
}

uint8_t PCF8574_read_word ()
{
    uint8_t receive_byte = 0;
    HAL_I2C_Master_Receive (&hi2c2, PCF_ADDRESS, &receive_byte, 1, 10);
    return receive_byte;
}

void PCF8574_write_bit (uint8_t bit,uint8_t state)
{
    uint8_t send_byte = 0;
    uint8_t receive_byte = 0;
    HAL_I2C_Master_Receive (&hi2c2, PCF_ADDRESS, &receive_byte, 1, 10);
    if (state)
    {
	send_byte = (1 << bit) | receive_byte;
    }
    else
    {
	send_byte = (~(1 << bit)) & receive_byte;
    }

    HAL_I2C_Master_Transmit(&hi2c2, PCF_ADDRESS, &send_byte, 1, 10);
}

void PCF8574_write_word (uint8_t state)
{
    uint8_t send_byte = state;
    HAL_I2C_Master_Transmit (&hi2c2, PCF_ADDRESS, &send_byte, 1, 10);
}

void test ()
{

}
