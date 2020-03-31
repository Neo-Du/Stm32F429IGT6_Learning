/*
 * FMC_9341.c
 * /12_FMC_TFTLCD_ILI9341/BSP/FMC_9341/FMC_9341.c
 * 12:59:37 AM
 *  Created on: Mar 29, 2020
 *      Author: Du
 */
#include "FMC_9341.h"

_lcd_dev lcddev;
uint32_t POINT_COLOR = 0xFF000000;		//画笔颜色
uint32_t BACK_COLOR = 0xFFFFFFFF;  	//背景色

void LCD_WR_REG (__IO uint16_t regval)
{
    regval = regval;		//使用-O2优化的时候,必须插入的延时
    LCD->LCD_REG = regval;		//写入要写的寄存器序号
}

uint16_t LCD_RD_DATA (void)
{
    __IO uint16_t ram;			//防止被优化
    ram = LCD->LCD_RAM;
    return ram;
}

void LCD_WR_DATA (__IO uint16_t data)
{
    data = data;			//使用-O2优化的时候,必须插入的延时
    LCD->LCD_RAM = data;
}

void LCD_Init (void)
{
    LCD_WR_REG (0XD3);
    lcddev.id = LCD_RD_DATA ();
    lcddev.id = LCD_RD_DATA ();
    lcddev.id = LCD_RD_DATA ();
    lcddev.id <<= 8;
    lcddev.id |= LCD_RD_DATA ();
    if (lcddev.id == 0X9341)
    {
	printf ("Start init\r\n");
	LCD_WR_REG (0xCF);
	LCD_WR_DATA (0x00);
	LCD_WR_DATA (0xC1);
	LCD_WR_DATA (0X30);
	LCD_WR_REG (0xED);
	LCD_WR_DATA (0x64);
	LCD_WR_DATA (0x03);
	LCD_WR_DATA (0X12);
	LCD_WR_DATA (0X81);
	LCD_WR_REG (0xE8);
	LCD_WR_DATA (0x85);
	LCD_WR_DATA (0x10);
	LCD_WR_DATA (0x7A);
	LCD_WR_REG (0xCB);
	LCD_WR_DATA (0x39);
	LCD_WR_DATA (0x2C);
	LCD_WR_DATA (0x00);
	LCD_WR_DATA (0x34);
	LCD_WR_DATA (0x02);
	LCD_WR_REG (0xF7);
	LCD_WR_DATA (0x20);
	LCD_WR_REG (0xEA);
	LCD_WR_DATA (0x00);
	LCD_WR_DATA (0x00);
	LCD_WR_REG (0xC0);    //Power control
	LCD_WR_DATA (0x1B);   //VRH[5:0]
	LCD_WR_REG (0xC1);    //Power control
	LCD_WR_DATA (0x01);   //SAP[2:0];BT[3:0]
	LCD_WR_REG (0xC5);    //VCM control
	LCD_WR_DATA (0x30); 	 //3F
	LCD_WR_DATA (0x30); 	 //3C
	LCD_WR_REG (0xC7);    //VCM control2
	LCD_WR_DATA (0XB7);
	LCD_WR_REG (0x36);    // Memory Access Control
	LCD_WR_DATA (0x48);
	LCD_WR_REG (0x3A);
	LCD_WR_DATA (0x55);
	LCD_WR_REG (0xB1);
	LCD_WR_DATA (0x00);
	LCD_WR_DATA (0x1A);
	LCD_WR_REG (0xB6);    // Display Function Control
	LCD_WR_DATA (0x0A);
	LCD_WR_DATA (0xA2);
	LCD_WR_REG (0xF2);    // 3Gamma Function Disable
	LCD_WR_DATA (0x00);
	LCD_WR_REG (0x26);    //Gamma curve selected
	LCD_WR_DATA (0x01);
	LCD_WR_REG (0xE0);    //Set Gamma
	LCD_WR_DATA (0x0F);
	LCD_WR_DATA (0x2A);
	LCD_WR_DATA (0x28);
	LCD_WR_DATA (0x08);
	LCD_WR_DATA (0x0E);
	LCD_WR_DATA (0x08);
	LCD_WR_DATA (0x54);
	LCD_WR_DATA (0XA9);
	LCD_WR_DATA (0x43);
	LCD_WR_DATA (0x0A);
	LCD_WR_DATA (0x0F);
	LCD_WR_DATA (0x00);
	LCD_WR_DATA (0x00);
	LCD_WR_DATA (0x00);
	LCD_WR_DATA (0x00);
	LCD_WR_REG (0XE1);    //Set Gamma
	LCD_WR_DATA (0x00);
	LCD_WR_DATA (0x15);
	LCD_WR_DATA (0x17);
	LCD_WR_DATA (0x07);
	LCD_WR_DATA (0x11);
	LCD_WR_DATA (0x06);
	LCD_WR_DATA (0x2B);
	LCD_WR_DATA (0x56);
	LCD_WR_DATA (0x3C);
	LCD_WR_DATA (0x05);
	LCD_WR_DATA (0x10);
	LCD_WR_DATA (0x0F);
	LCD_WR_DATA (0x3F);
	LCD_WR_DATA (0x3F);
	LCD_WR_DATA (0x0F);
	LCD_WR_REG (0x2B);
	LCD_WR_DATA (0x00);
	LCD_WR_DATA (0x00);
	LCD_WR_DATA (0x01);
	LCD_WR_DATA (0x3f);
	LCD_WR_REG (0x2A);
	LCD_WR_DATA (0x00);
	LCD_WR_DATA (0x00);
	LCD_WR_DATA (0x00);
	LCD_WR_DATA (0xef);
	LCD_WR_REG (0x11); //Exit Sleep
	HAL_Delay (120);
	LCD_WR_REG (0x29); //display on

	FMC_Bank1E->BWTR[0] &= ~(0XF << 0);	//地址建立时间(ADDSET)清零
	FMC_Bank1E->BWTR[0] &= ~(0XF << 8);	//数据保存时间清零
	FMC_Bank1E->BWTR[0] |= 4 << 0;	 	//地址建立时间(ADDSET)为4个HCLK =21ns
	FMC_Bank1E->BWTR[0] |= 4 << 8; 	//数据保存时间(DATAST)为5.2ns*4个HCLK=21ns
	printf ("Init done\r\n");
    }

    LCD_Display_Dir (0);		//默认为竖屏
    //点亮背光
    HAL_GPIO_WritePin (LCD_BL_GPIO_Port, LCD_BL_Pin, 1);
    LCD_Clear (WHITE);
}

void LCD_Display_Dir (uint8_t dir)
{
    lcddev.dir = dir;		//竖屏/横屏
    if (dir == 0)			//竖屏
    {
	lcddev.width = 240;
	lcddev.height = 320;
    }
    else 				//横屏
    {
	lcddev.width = 320;
	lcddev.height = 240;
    }

    lcddev.wramcmd = 0X2C;	//设置写入GRAM的指令
    lcddev.setxcmd = 0X2A;	//设置写X坐标指令
    lcddev.setycmd = 0X2B;	//设置写Y坐标指令
    LCD_Scan_Dir (DFT_SCAN_DIR);	//默认扫描方向
}

void LCD_Scan_Dir (uint8_t dir)
{
    uint16_t regval = 0;
    uint16_t dirreg = 0;
    uint16_t temp;
    if (lcddev.dir == 1)	//横屏时，对1963不改变扫描方向！竖屏时1963改变方向
    {
	switch (dir)
	    //方向转换
	    {
	    case 0:
		dir = 6;
		break;
	    case 1:
		dir = 7;
		break;
	    case 2:
		dir = 4;
		break;
	    case 3:
		dir = 5;
		break;
	    case 4:
		dir = 1;
		break;
	    case 5:
		dir = 0;
		break;
	    case 6:
		dir = 3;
		break;
	    case 7:
		dir = 2;
		break;
	    }
    }
    switch (dir)
	{
	case L2R_U2D:	//从左到右,从上到下
	    regval |= (0 << 7) | (0 << 6) | (0 << 5);
	    break;
	case L2R_D2U:	//从左到右,从下到上
	    regval |= (1 << 7) | (0 << 6) | (0 << 5);
	    break;
	case R2L_U2D:	//从右到左,从上到下
	    regval |= (0 << 7) | (1 << 6) | (0 << 5);
	    break;
	case R2L_D2U:	//从右到左,从下到上
	    regval |= (1 << 7) | (1 << 6) | (0 << 5);
	    break;
	case U2D_L2R:	//从上到下,从左到右
	    regval |= (0 << 7) | (0 << 6) | (1 << 5);
	    break;
	case U2D_R2L:	//从上到下,从右到左
	    regval |= (0 << 7) | (1 << 6) | (1 << 5);
	    break;
	case D2U_L2R:	//从下到上,从左到右
	    regval |= (1 << 7) | (0 << 6) | (1 << 5);
	    break;
	case D2U_R2L:	//从下到上,从右到左
	    regval |= (1 << 7) | (1 << 6) | (1 << 5);
	    break;
	}
    dirreg = 0X36;

    LCD_WriteReg (dirreg, regval);

    LCD_WR_REG (lcddev.setxcmd);
    LCD_WR_DATA (0);
    LCD_WR_DATA (0);
    LCD_WR_DATA ((lcddev.width - 1) >> 8);
    LCD_WR_DATA ((lcddev.width - 1) & 0XFF);
    LCD_WR_REG (lcddev.setycmd);
    LCD_WR_DATA (0);
    LCD_WR_DATA (0);
    LCD_WR_DATA ((lcddev.height - 1) >> 8);
    LCD_WR_DATA ((lcddev.height - 1) & 0XFF);

}
void LCD_WriteReg (uint16_t LCD_Reg,uint16_t LCD_RegValue)
{
    LCD->LCD_REG = LCD_Reg;		//写入要写的寄存器序号
    LCD->LCD_RAM = LCD_RegValue;		//写入数据
}
void LCD_Clear (uint32_t color)
{
    uint32_t index = 0;
    uint32_t totalpoint = lcddev.width;
    totalpoint *= lcddev.height; 			//得到总点数
    LCD_SetCursor (0x00, 0x00);			//设置光标位置
    LCD_WriteRAM_Prepare ();     		//开始写入GRAM
    for (index = 0; index < totalpoint; index++)
    {
	LCD->LCD_RAM = color;
    }
}

void LCD_WriteRAM_Prepare (void)
{
    LCD->LCD_REG = lcddev.wramcmd;
}

void LCD_SetCursor (uint16_t Xpos,uint16_t Ypos)
{
    LCD_WR_REG (lcddev.setxcmd);
    LCD_WR_DATA (Xpos >> 8);
    LCD_WR_DATA (Xpos & 0XFF);
    LCD_WR_REG (lcddev.setycmd);
    LCD_WR_DATA (Ypos >> 8);
    LCD_WR_DATA (Ypos & 0XFF);
}


