/*
 * sdRAM.c
 * /31_OV5640/Drivers/BSP/SDRAM/sdRAM.c
 * 9:50:00 PM
 *  Created on: Aug 24, 2020
 *      Author: Du
 */

#include "sdRAM.h"
#include "fmc.h"

extern SDRAM_HandleTypeDef hsdram1;

uint8_t SDRAM_Send_Cmd (uint8_t bankx,uint8_t cmd,uint8_t refresh,uint16_t regval)
{
    uint32_t target_bank = 0;
    FMC_SDRAM_CommandTypeDef Command;

    if (bankx == 0)
	target_bank = FMC_SDRAM_CMD_TARGET_BANK1;
    else if (bankx == 1)
	target_bank = FMC_SDRAM_CMD_TARGET_BANK2;
    Command.CommandMode = cmd;                //命令
    Command.CommandTarget = target_bank;      //目标SDRAM存储区域
    Command.AutoRefreshNumber = refresh;      //自刷新次�????????
    Command.ModeRegisterDefinition = regval;  //要写入模式寄存器的�??
    if (HAL_SDRAM_SendCommand (&hsdram1, &Command, 0X1000) == HAL_OK) //向SDRAM发�?�命�????????
    {
	return 0;
    }
    else
	return 1;
}
void SDRAM_Initialization_Sequence (SDRAM_HandleTypeDef*hsdram)
{
    uint32_t temp = 0;
    //SDRAM控制器初始化完成以后还需要按照如下顺序初始化SDRAM
    SDRAM_Send_Cmd (0, FMC_SDRAM_CMD_CLK_ENABLE, 1, 0); //时钟配置使能
    HAL_Delay (1);                                  //至少延时200us
    SDRAM_Send_Cmd (0, FMC_SDRAM_CMD_PALL, 1, 0);       //对所有存储区预充�????????
    SDRAM_Send_Cmd (0, FMC_SDRAM_CMD_AUTOREFRESH_MODE, 8, 0);       //设置自刷新次�????????
    temp = (uint32_t) SDRAM_MODEREG_BURST_LENGTH_1 |	//设置突发长度:1(可以�????????1/2/4/8)
	    SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL |	//设置突发类型:连续(可以是连�????????/交错)
	    SDRAM_MODEREG_CAS_LATENCY_3 |	//设置CAS�????????:3(可以�????????2/3)
	    SDRAM_MODEREG_OPERATING_MODE_STANDARD |   //设置操作模式:0,标准模式
	    SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;     //设置突发写模�????????:1,单点访问
    SDRAM_Send_Cmd (0, FMC_SDRAM_CMD_LOAD_MODE, 1, temp);   //设置SDRAM的模式寄存器
}
void SDRAM_Init (void)
{
    SDRAM_Initialization_Sequence (&hsdram1);
    HAL_SDRAM_ProgramRefreshRate (&hsdram1, 683);	//设置刷新频率
}
