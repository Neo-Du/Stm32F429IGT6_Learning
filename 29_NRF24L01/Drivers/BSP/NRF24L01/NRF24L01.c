/*
 * NRF24L01.c
 * /29_NRF24L01/Drivers/BSP/NRF24L01/NRF24L01.c
 * 下午11:01:54
 *  Created on: 2020年8月21日
 *      Author: Du
 */
#include "NRF24L01.h"

extern SPI_HandleTypeDef hspi2;

void NRF24L01_Init (void) //初始化
{
    CE_LOW;
    CS_HIGH;
}
void NRF24L01_RX_Mode (void) //配置为接收模式
{

}
void NRF24L01_TX_Mode (void) //配置为发送模式
{

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
    uint8_t buf[5] = { 0xA5, 0xA5, 0xA5, 0xA5, 0xA5 };
    NRF24L01_Write_Buf(NRF_WRITE_REG + TX_ADDR, buf, 5);
    NRF24L01_Read_Buf(TX_ADDR,buf,5); //读出写入的地址
    for(int i=0; i<5; i++)if(buf[i]!=0XA5)break;
    if(i != 5) return 1;
    return 0;
}
uint8_t NRF24L01_TxPacket (uint8_t*txbuf)		//发送一个包的数据
{

}
uint8_t NRF24L01_RxPacket (uint8_t*rxbuf)		//接收一个包的数据
{

}
