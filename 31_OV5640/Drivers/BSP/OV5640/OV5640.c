/*
 * OV5640.c
 * /31_OV5640/Drivers/BSP/OV5640/OV5640.c
 * 10:00:11 PM
 *  Created on: Aug 24, 2020
 *      Author: Du
 */
#include "main.h"
#include "SCCB.h"
#include "OV5640.h"
#include "OV5640AF.h"
#include "OV5640cfg.h"
#include "PCF8574.h"

#define u8 uint8_t
#define u16 uint16_t

u8 OV5640_WR_Reg (u16 reg,u8 data)
{
    u8 res = 0;
    SCCB_Start (); 					//启动SCCB传输
    if (SCCB_WR_Byte (OV5640_ADDR))
	res = 1;	//写器件ID
    if (SCCB_WR_Byte (reg >> 8))
	res = 1;	//写寄存器高8位地址
    if (SCCB_WR_Byte (reg))
	res = 1;		//写寄存器低8位地址
    if (SCCB_WR_Byte (data))
	res = 1; 	//写数据
    SCCB_Stop ();
    return res;
}

u8 OV5640_RD_Reg (u16 reg)
{
    u8 val = 0;
    SCCB_Start (); 				//启动SCCB传输
    SCCB_WR_Byte (OV5640_ADDR);	//写器件ID
    SCCB_WR_Byte (reg >> 8);	    //写寄存器高8位地址
    SCCB_WR_Byte (reg);			//写寄存器低8位地址
    SCCB_Stop ();
    //设置寄存器地址后，才是读
    SCCB_Start ();
    SCCB_WR_Byte (OV5640_ADDR | 0X01);			//发送读命令
    val = SCCB_RD_Byte ();		 	//读取数据
    SCCB_No_Ack ();
    SCCB_Stop ();
    return val;
}

u8 OV5640_Init (void)
{
    u16 i = 0;
    u16 reg;
    PCF8574_Init ();			//初始化PCF8574
    HAL_GPIO_WritePin (DCMI_RESET_GPIO_Port, DCMI_RESET_Pin, 0);
    HAL_Delay (20);
    PCF8574_Write_bit (DCMI_PWDN_IO, 0);
    HAL_Delay (5);
    HAL_GPIO_WritePin (DCMI_RESET_GPIO_Port, DCMI_RESET_Pin, 1);
    HAL_Delay (20);
    reg = OV5640_RD_Reg (OV5640_CHIPIDH);	//读取ID 高八位
    reg <<= 8;
    reg |= OV5640_RD_Reg (OV5640_CHIPIDL);	//读取ID 低八位
    if (reg != OV5640_ID)
    {
	return 1;
    }
    OV5640_WR_Reg (0x3103, 0X11);	//system clock from pad, bit[1]
    OV5640_WR_Reg (0X3008, 0X82);	//软复位
    HAL_Delay (10);
    //初始化 OV5640,采用SXGA分辨率(1600*1200)
    for (i = 0; i < sizeof(ov5640_uxga_init_reg_tbl) / 4; i++)
    {
	OV5640_WR_Reg (ov5640_uxga_init_reg_tbl[i][0], ov5640_uxga_init_reg_tbl[i][1]);
    }
    //检查闪光灯是否正常
//	OV5640_Flash_Ctrl(1);//打开闪光灯
//	delay_ms(50);
//	OV5640_Flash_Ctrl(0);//关闭闪光灯
    return 0x00; 	//ok
}
