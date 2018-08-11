#ifndef __18b20_h__
#define __18b20_h__

#include "stm32f10x_gpio.h"
#define uchar unsigned char

#define BUS_H()  GPIO_SetBits ( GPIOC, GPIO_Pin_2)
#define BUS_L()  GPIO_ResetBits ( GPIOC, GPIO_Pin_2)

#define BUS_IN() GPIO_ReadInputDataBit ( GPIOC, GPIO_Pin_2)

static void delay_us(unsigned int us);

static void GPIO_18b20_Out_Config(void );
static void GPIO_18b20_In_Config(void );
unsigned char init_18b20(void );	//18b20复位
static unsigned char read_btye(void);				//读一个字节
static void write_btye(uchar outdata);		   //写一个字节
unsigned int temp_gether(void);						   //读取温度

unsigned int temp_gether(void);						   //读取温度
#endif
