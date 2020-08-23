/*
 * NRF24L01.c
 * /29_NRF24L01/Drivers/BSP/NRF24L01/NRF24L01.c
 * 下午11:01:54
 *  Created on: 2020年8月21日
 *      Author: Du
 */
#include "NRF24L01.h"

extern SPI_HandleTypeDef hspi2;

const uint8_t TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01}; //发送地址
const uint8_t RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01}; //发送地址

void NRF24L01_Init (void) //初始化
{
    CE_LOW;
    CS_HIGH;
}
void NRF24L01_RX_Mode (void) //配置为接收模式
{
    CE_LOW;
    NRF24L01_Write_Buf (NRF_WRITE_REG + RX_ADDR_P0, (uint8_t*) RX_ADDRESS, RX_ADR_WIDTH); //写RX节点地址

    NRF24L01_Write_Reg (NRF_WRITE_REG + EN_AA, 0x01);       //使能通道0的自动应答
    NRF24L01_Write_Reg (NRF_WRITE_REG + EN_RXADDR, 0x01);   //使能通道0的接收地址
    NRF24L01_Write_Reg (NRF_WRITE_REG + RF_CH, 40);	        //设置RF通信频率
    NRF24L01_Write_Reg (NRF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);	        //选择通道0的有效数据宽度
    NRF24L01_Write_Reg (NRF_WRITE_REG + RF_SETUP, 0x0f);    //设置TX发射参数,0db增益,2Mbps,低噪声增益开启
    NRF24L01_Write_Reg (NRF_WRITE_REG + CONFIG, 0x0f);     //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式
    CE_HIGH; //CE为高,进入接收模式
}
void NRF24L01_TX_Mode (void) //配置为发送模式
{
    CE_LOW;
    NRF24L01_Write_Buf (NRF_WRITE_REG + TX_ADDR, (uint8_t*) TX_ADDRESS, TX_ADR_WIDTH); //写TX节点地址
    NRF24L01_Write_Buf (NRF_WRITE_REG + RX_ADDR_P0, (uint8_t*) RX_ADDRESS, RX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK

    NRF24L01_Write_Reg (NRF_WRITE_REG + EN_AA, 0x01);     //使能通道0的自动应答
    NRF24L01_Write_Reg (NRF_WRITE_REG + EN_RXADDR, 0x01); //使能通道0的接收地址
    NRF24L01_Write_Reg (NRF_WRITE_REG + SETUP_RETR, 0x1a); //设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
    NRF24L01_Write_Reg (NRF_WRITE_REG + RF_CH, 40);       //设置RF通道为40
    NRF24L01_Write_Reg (NRF_WRITE_REG + RF_SETUP, 0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启
    NRF24L01_Write_Reg (NRF_WRITE_REG + CONFIG, 0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
    CE_HIGH;    //CE为高,10us后启动发送
}
uint8_t NRF24L01_Write_Buf (uint8_t reg,uint8_t*pBuf,uint8_t len) //写数据区
{
    uint8_t status;
    CS_LOW;
    HAL_SPI_TransmitReceive (&hspi2, &reg, &status, 1, 100);
    HAL_SPI_Transmit (&hspi2, pBuf, len, 100);
    CS_HIGH;
    return status;
}
uint8_t NRF24L01_Read_Buf (uint8_t reg,uint8_t*pBuf,uint8_t len) //读数据区
{
    uint8_t status;
    CS_LOW;
    HAL_SPI_TransmitReceive (&hspi2, &reg, &status, 1, 100);
    HAL_SPI_Receive (&hspi2, pBuf, len, 100);
    CS_HIGH;
    return status;
}
uint8_t NRF24L01_Read_Reg (uint8_t reg)		//读寄存器
{
    uint8_t reg_val = 0;
    CS_LOW;
    HAL_SPI_Transmit (&hspi2, &reg, 1, 100);
    HAL_SPI_Receive (&hspi2, &reg_val, 1, 100);
    CS_HIGH;
    return reg_val;
}
uint8_t NRF24L01_Write_Reg (uint8_t reg,uint8_t value)		//写寄存器
{
    uint8_t status;
    CS_LOW;
    HAL_SPI_TransmitReceive (&hspi2, &reg, &status, 1, 100);
    HAL_SPI_Transmit (&hspi2, &value, 1, 100);
    CS_HIGH;
    return status;
}
uint8_t NRF24L01_Check (void)		//检查24L01是否存在
{
    uint8_t buf[5] = { 0xA5, 0xA5, 0xA5, 0xA5, 0xA5 }, i;
    NRF24L01_Write_Buf (NRF_WRITE_REG + TX_ADDR, buf, 5);
    NRF24L01_Read_Buf (TX_ADDR, buf, 5); //读出写入的地址
    for (i = 0; i < 5; i++)
	if (buf[i] != 0XA5)
	    break;
    if (i != 5)
	return 1;
    return 0;
}
uint8_t NRF24L01_TxPacket (uint8_t*txbuf)		//发送一个包的数据
{
    uint8_t status;
    CE_LOW;
    NRF24L01_Write_Buf (WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH);
    CE_HIGH;
    while (IRQ_READ == 1);
    status = NRF24L01_Read_Reg (STATUS);          //读取状态寄存器的值
    NRF24L01_Write_Reg (NRF_WRITE_REG + STATUS, status); //清除TX_DS或MAX_RT中断标志
    if (status & MAX_TX)                          //达到最大重发次数
    {
	NRF24L01_Write_Reg (FLUSH_TX, 0xff);  //清除TX FIFO寄存器
	return MAX_TX;
    }
    if (status & TX_OK)                           //发送完成
    {
	return TX_OK;
    }
    return 0xff;                           //其他原因发送失败

}
uint8_t NRF24L01_RxPacket (uint8_t*rxbuf)		//接收一个包的数据
{
    uint8_t status;
    status = NRF24L01_Read_Reg (STATUS);          //读取状态寄存器的值
    NRF24L01_Write_Reg (NRF_WRITE_REG + STATUS, status); //清除TX_DS或MAX_RT中断标志
    if (status & RX_OK) //接收到数据
    {
	NRF24L01_Read_Buf (RD_RX_PLOAD, rxbuf, RX_PLOAD_WIDTH); //读取数据
	NRF24L01_Write_Reg (FLUSH_RX, 0xff);  //清除RX FIFO寄存器
	return 0;
    }
    return 1;  //没收到任何数据
}
