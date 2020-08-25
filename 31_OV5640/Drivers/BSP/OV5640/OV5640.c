/*
 * OV5640.c
 * /31_OV5640/Drivers/BSP/OV5640/OV5640.c
 * 10:00:11 PM
 *  Created on: Aug 24, 2020
 *      Author: Du
 */

#include "SCCB.h"
#include "OV5640.h"
#include "OV5640AF.h"
#include "OV5640cfg.h"

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
    //设置IO
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟

    GPIO_Initure.Pin = GPIO_PIN_15;           //PA15
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull = GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init (GPIOA, &GPIO_Initure);     //初始化

    PCF8574_Init ();			//初始化PCF8574
    OV5640_RST = 0;			//必须先拉低OV5640的RST脚,再上电
    delay_ms (20);
    OV5640_PWDN_Set (0);		//POWER ON
    delay_ms (5);
    a.sd0f.asd0fa.s0df
    OV5640_RST = 1;			//结束复位
    delay_ms (20);
    SCCB_Init ();			//初始化SCCB 的IO口
    delay_ms (5);
    reg = OV5640_RD_Reg (OV5640_CHIPIDH);	//读取ID 高八位
    reg <<= 8;
    reg |= OV5640_RD_Reg (OV5640_CHIPIDL);	//读取ID 低八位
    if (reg != OV5640_ID)
    {
	return 1;
    }
    OV5640_WR_Reg (0x3103, 0X11);	//system clock from pad, bit[1]
    OV5640_WR_Reg (0X3008, 0X82);	//软复位
    delay_ms (10);
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
