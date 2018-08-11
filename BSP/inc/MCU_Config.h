/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
#ifndef  MCU_CONFIG_H
#define  MCU_CONFIG_H 


/*********************************************************************************
文件包含区
*********************************************************************************/
//#include "..\..\LIB\STM32F10x_StdPeriph_Driver\inc\stm32f10x_lib.h"

#include "..\..\LIB\CMSIS\Device\STM32F10x\Include\stm32f10x.h"
#include "..\..\LIB\CMSIS\Device\STM32F10x\Include\system_stm32f10x.h"
#include "..\..\LIB\FLT\filter.h"
/*********************************************************************************
宏定义区
*********************************************************************************/
#define  ErthNet_ENpin           GPIOB->IDR  & GPIO_Pin_0
#define  set_ErthNet_ENpin       GPIOB->BSRR = GPIO_Pin_0
#define  clr_ErthNet_ENpin       GPIOB->BRR  = GPIO_Pin_0

#define  SPI1_NSSpin             GPIOA->IDR  & GPIO_Pin_4
#define  set_SPI1_NSSpin         GPIOA->BSRR = GPIO_Pin_4
#define  clr_SPI1_NSSpin         GPIOA->BRR  = GPIO_Pin_4

#define  SPI3_NSS1pin            GPIOA->IDR  & GPIO_Pin_9
#define  set_SPI3_NSS1pin        GPIOA->BSRR = GPIO_Pin_9
#define  clr_SPI3_NSS1pin        GPIOA->BRR  = GPIO_Pin_9

#define  SPI3_NSS2pin            GPIOA->IDR  & GPIO_Pin_10
#define  set_SPI3_NSS2pin        GPIOA->BSRR = GPIO_Pin_10
#define  clr_SPI3_NSS2pin        GPIOA->BRR  = GPIO_Pin_10


#define  SD_CDpin                GPIOA->IDR  & GPIO_Pin_8
#define  set_SD_CDpin            GPIOA->BSRR = GPIO_Pin_8
#define  clr_SD_CDpin            GPIOA->BRR  = GPIO_Pin_8


#define  GPRS_ONOFFpin           GPIOD->IDR  & GPIO_Pin_11
#define  set_GPRS_ONOFFpin       GPIOD->BSRR = GPIO_Pin_11
#define  clr_GPRS_ONOFFpin       GPIOD->BRR  = GPIO_Pin_11

#define  GPRS_ENpin              GPIOC->IDR  & GPIO_Pin_6
#define  set_GPRS_ENpin          GPIOC->BSRR = GPIO_Pin_6
#define  clr_GPRS_ENpin          GPIOC->BRR  = GPIO_Pin_6

#define  GPRS_RESTpin            GPIOD->IDR  & GPIO_Pin_12
#define  set_GPRS_RESTpin        GPIOD->BSRR = GPIO_Pin_12
#define  clr_GPRS_RESTpin        GPIOD->BRR  = GPIO_Pin_12


#define  RS485_DIRpin            GPIOE->IDR  & GPIO_Pin_7
#define  set_RS485_DIRpin        GPIOE->BSRR = GPIO_Pin_7
#define  clr_RS485_DIRpin        GPIOE->BRR  = GPIO_Pin_7

#define  Rs485_CH1_ENpin         GPIOE->IDR  & GPIO_Pin_1
#define  set_Rs485_CH1_ENpin     GPIOE->BSRR = GPIO_Pin_1
#define  clr_Rs485_CH1_ENpin     GPIOE->BRR  = GPIO_Pin_1

#define  Rs485_CH2_ENpin         GPIOE->IDR  & GPIO_Pin_3
#define  set_Rs485_CH2_ENpin     GPIOE->BSRR = GPIO_Pin_3
#define  clr_Rs485_CH2_ENpin     GPIOE->BRR  = GPIO_Pin_3

#define  Rs485_CH3_ENpin         GPIOE->IDR  & GPIO_Pin_5
#define  set_Rs485_CH3_ENpin     GPIOE->BSRR = GPIO_Pin_5
#define  clr_Rs485_CH3_ENpin     GPIOE->BRR  = GPIO_Pin_5

#define  Rs485_CH4_ENpin         GPIOE->IDR  & GPIO_Pin_6
#define  set_Rs485_CH4_ENpin     GPIOE->BSRR = GPIO_Pin_6
#define  clr_Rs485_CH4_ENpin     GPIOE->BRR  = GPIO_Pin_6

#define  OUT_CH1_ENpin           GPIOE->IDR  & GPIO_Pin_0
#define  set_OUT_CH1_ENpin       GPIOE->BSRR = GPIO_Pin_0
#define  clr_OUT_CH1_ENpin       GPIOE->BRR  = GPIO_Pin_0

#define  OUT_CH2_ENpin           GPIOE->IDR  & GPIO_Pin_2
#define  set_OUT_CH2_ENpin       GPIOE->BSRR = GPIO_Pin_2
#define  clr_OUT_CH2_ENpin       GPIOE->BRR  = GPIO_Pin_2

#define  OUT_CH3_ENpin           GPIOE->IDR  & GPIO_Pin_4
#define  set_OUT_CH3_ENpin       GPIOE->BSRR = GPIO_Pin_4
#define  clr_OUT_CH3_ENpin       GPIOE->BRR  = GPIO_Pin_4

#define  Wifi_ENpin              GPIOC->IDR  & GPIO_Pin_9
#define  set_Wifi_ENpin          GPIOC->BSRR = GPIO_Pin_9
#define  clr_Wifi_ENpin          GPIOC->BRR  = GPIO_Pin_9

#define  IO00pin                 GPIOE->IDR  & GPIO_Pin_10
#define  set_IO00pin             GPIOE->BSRR = GPIO_Pin_10
#define  clr_IO00pin             GPIOE->BRR  = GPIO_Pin_10

#define  IO01pin                 GPIOE->IDR  & GPIO_Pin_11
#define  set_IO01pin             GPIOE->BSRR = GPIO_Pin_11
#define  clr_IO01pin             GPIOE->BRR  = GPIO_Pin_11

#define  IO02pin                 GPIOE->IDR  & GPIO_Pin_8
#define  set_IO02pin             GPIOE->BSRR = GPIO_Pin_8
#define  clr_IO02pin             GPIOE->BRR  = GPIO_Pin_8

#define  IO03pin                 GPIOE->IDR  & GPIO_Pin_9
#define  set_IO03pin             GPIOE->BSRR = GPIO_Pin_9
#define  clr_IO03pin             GPIOE->BRR  = GPIO_Pin_9


#define  LED1pin                 GPIOB->IDR  & GPIO_Pin_2
#define  set_LED1pin             GPIOB->BSRR = GPIO_Pin_2
#define  clr_LED1pin             GPIOB->BRR  = GPIO_Pin_2

#define  LED2pin                 GPIOB->IDR  & GPIO_Pin_1
#define  set_LED2pin             GPIOB->BSRR = GPIO_Pin_1
#define  clr_LED2pin             GPIOB->BRR  = GPIO_Pin_1

#define  User_K2pin              GPIOD->IDR  & GPIO_Pin_6
#define  set_User_K2pin          GPIOD->BSRR = GPIO_Pin_6
#define  clr_User_K2pin          GPIOD->BRR  = GPIO_Pin_6


#define  LED1_Pin   (LED1pin)
#define  LED1_NO    (clr_LED1pin)
#define  LED1_OFF   (set_LED1pin)

#define  LED2_Pin   (LED2pin)
#define  LED2_NO    (clr_LED2pin)
#define  LED2_OFF   (set_LED2pin)

#define LinkLed_OFF( )       LED1_OFF
#define LinkLed_ON( )        LED1_NO 

/*********************************************************************************
数据类型定义
*********************************************************************************/

/*********************************************************************************
外部变量声明区
*********************************************************************************/

/*********************************************************************************
函数声明区
*********************************************************************************/
void RCC_Configuration (void);
void GPIO_Configuration(void);
void I2C1_Rset(void);
void I2C2_Configuration(void);
void I2C2_Rset(void);
void SPI1_Configuration(void);
void MCU_init ( void );
void ADC1_TCIE_IRQ (void);
void I2C1_Configuration(void);
/********************************************************************************/
#endif













