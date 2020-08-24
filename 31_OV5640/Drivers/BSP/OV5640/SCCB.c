/*
 * SCCB.c
 * /31_OV5640/Drivers/BSP/OV5640/SCCB.c
 * 10:15:43 PM
 *  Created on: Aug 24, 2020
 *      Author: Du
 */

#include "SCCB.h"

void SCCB_Delay (void)
{
    uint16_t i = 560; // 50us
    while (i--);
}

void SCCB_Start (void)
{
    SCCB_SDA = 1;     //数据线高电平
    SCCB_SCL = 1;	    //在时钟线高的时候数据线由高至低
    SCCB_Delay ();
    SCCB_SDA = 0;
    SCCB_Delay ();
    SCCB_SCL = 0;	    //数据线恢复低电平，单操作函数必要
}
void SCCB_Stop (void)
{
    SCCB_SDA = 0;
    SCCB_Delay ();
    SCCB_SCL = 1;
    SCCB_Delay ();
    SCCB_SDA = 1;
    SCCB_Delay ();
}
void SCCB_No_Ack (void)
{
    SCCB_Delay ();
    SCCB_SDA = 1;
    SCCB_SCL = 1;
    SCCB_Delay ();
    SCCB_SCL = 0;
    SCCB_Delay ();
    SCCB_SDA = 0;
    SCCB_Delay ();
}
uint8_t SCCB_WR_Byte (uint8_t data)
{
    uint8_t j, res;
    for (j = 0; j < 8; j++) //循环8次发送数据
    {
	if (data & 0x80)
	    SCCB_SDA_OUT(1);
	else
	    SCCB_SDA_OUT(0);
	dat <<= 1;
	SCCB_Delay ();
	SCCB_SCL_OUT(1);
	SCCB_Delay ();
	SCCB_SCL_OUT(0);
    }
    SCCB_SET_SDA_IN();		//设置SDA为输入
    SCCB_Delay ();
    SCCB_SCL_OUT(1);
    SCCB_Delay ();
    if (SCCB_READ_SDA)
	res = 1;  //SDA=1发送失败，返回1
    else
	res = 0;         //SDA=0发送成功，返回0
    SCCB_SCL_OUT(0);
    SCCB_SET_SDA_OUT();		//设置SDA为输出
    return res;
}
uint8_t SCCB_RD_Byte (void)
{
    uint8_t temp = 0, j;
    SCCB_SDA_IN ();		//设置SDA为输入
    for (j = 8; j > 0; j--) 	//循环8次接收数据
    {
	SCCB_Delay ();
	SCCB_SCL = 1;
	temp = temp << 1;
	if (SCCB_READ_SDA)
	    temp++;
	SCCB_Delay ();
	SCCB_SCL = 0;
    }
    SCCB_SDA_OUT();		//设置SDA为输出
    return temp;
}
