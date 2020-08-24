/*
 * LTDC.c
 * /31_OV5640/Drivers/BSP/LTDC/LTDC.c
 * 9:48:52 PM
 *  Created on: Aug 24, 2020
 *      Author: Du
 */
#include "LTDC_LCD.h"
#include "ltdc.h"

extern LTDC_HandleTypeDef hltdc;


void LCD_Init (void)
{
    HAL_LTDC_SetWindowSize (&hltdc, 1024, 600, 0);
}
