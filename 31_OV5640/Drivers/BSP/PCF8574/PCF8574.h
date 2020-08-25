/*
 * PCF8574.h
 * /20.1_I2C_PCF8574/Drivers/PCF8574/PCF8574.h
 * 上午12:29:30
 *  Created on: 2020年8月26日
 *      Author: Du
 */

#ifndef PCF8574_H_
#define PCF8574_H_

#define PCF8574_ADDR 	0X40	//PCF8574地址(左移了一位)

//PCF8574各个IO的功能
#define BEEP_IO         0	//蜂鸣器控制引脚  	P0
#define AP_INT_IO       1   	//AP3216C中断引脚		P1
#define DCMI_PWDN_IO    2    	//DCMI的电源控制引脚		P2
#define USB_PWR_IO      3    	//USB电源控制引脚		P3
#define EX_IO      	4    	//扩展IO,自定义使用 	P4
#define MPU_INT_IO      5   	//MPU9250中断引脚		P5
#define RS485_RE_IO     6    	//RS485_RE引脚		P6
#define ETH_RESET_IO    7    	//以太网复位引脚	P7


uint8_t PCF8574_Init (void);
uint8_t PCF8574_Write_bit (uint8_t pin,uint8_t status);
uint8_t PCF8574_Read_bit (uint8_t pin);
uint8_t PCF8574_Write_byte (uint8_t byte);
uint8_t PCF8574_Read_byte (void);

#endif /* PCF8574_H_ */
