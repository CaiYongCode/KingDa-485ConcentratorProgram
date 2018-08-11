/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
#ifndef  BSP_PRESENT
#define  BSP_PRESENT
/*********************************************************************************
文件包含区
*********************************************************************************/
#include  <stdarg.h>
#include  <stdio.h>

#include ".\MCU_Config.h"
#include ".\PowerConfig.h"
#include ".\ENC28J60.h"
//#include ".\SIM800E.h"
#include ".\MG2618.h"
#include ".\SerialPort.h"
#include ".\PCF8563.h"
#include ".\W25QXX.h"  
#include ".\IICEeprom_24CXX.h"  
#include ".\DriverPcb.h"
//#include ".\CH376T.h"
/*********************************************************************************
宏定义区
*********************************************************************************/
#define  BSP_INT_ID_WWDG                                   0    /* Window WatchDog Interrupt                            */
#define  BSP_INT_ID_PVD                                    1    /* PVD through EXTI Line detection Interrupt            */
#define  BSP_INT_ID_TAMPER                                 2    /* Tamper Interrupt                                     */
#define  BSP_INT_ID_RTC                                    3    /* RTC global Interrupt                                 */
#define  BSP_INT_ID_FLASH                                  4    /* FLASH global Interrupt                               */
#define  BSP_INT_ID_RCC                                    5    /* RCC global Interrupt                                 */
#define  BSP_INT_ID_EXTI0                                  6    /* EXTI Line0 Interrupt                                 */
#define  BSP_INT_ID_EXTI1                                  7    /* EXTI Line1 Interrupt                                 */
#define  BSP_INT_ID_EXTI2                                  8    /* EXTI Line2 Interrupt                                 */
#define  BSP_INT_ID_EXTI3                                  9    /* EXTI Line3 Interrupt                                 */
#define  BSP_INT_ID_EXTI4                                 10    /* EXTI Line4 Interrupt                                 */
#define  BSP_INT_ID_DMA1_CH1                              11    /* DMA1 Channel 1 global Interrupt                      */
#define  BSP_INT_ID_DMA1_CH2                              12    /* DMA1 Channel 2 global Interrupt                      */
#define  BSP_INT_ID_DMA1_CH3                              13    /* DMA1 Channel 3 global Interrupt                      */
#define  BSP_INT_ID_DMA1_CH4                              14    /* DMA1 Channel 4 global Interrupt                      */
#define  BSP_INT_ID_DMA1_CH5                              15    /* DMA1 Channel 5 global Interrupt                      */
#define  BSP_INT_ID_DMA1_CH6                              16    /* DMA1 Channel 6 global Interrupt                      */
#define  BSP_INT_ID_DMA1_CH7                              17    /* DMA1 Channel 7 global Interrupt                      */
#define  BSP_INT_ID_ADC1_2                                18    /* ADC1 et ADC2 global Interrupt                        */
#define  BSP_INT_ID_CAN1_TX                               19    /* CAN1 TX Interrupts                                   */
#define  BSP_INT_ID_CAN1_RX0                              20    /* CAN1 RX0 Interrupts                                  */
#define  BSP_INT_ID_CAN1_RX1                              21    /* CAN1 RX1 Interrupt                                   */
#define  BSP_INT_ID_CAN1_SCE                              22    /* CAN1 SCE Interrupt                                   */
#define  BSP_INT_ID_EXTI9_5                               23    /* External Line[9:5] Interrupts                        */
#define  BSP_INT_ID_TIM1_BRK                              24    /* TIM1 Break Interrupt                                 */
#define  BSP_INT_ID_TIM1_UP                               25    /* TIM1 Update Interrupt                                */
#define  BSP_INT_ID_TIM1_TRG_COM                          26    /* TIM1 Trigger and Commutation Interrupt               */
#define  BSP_INT_ID_TIM1_CC                               27    /* TIM1 Capture Compare Interrupt                       */
#define  BSP_INT_ID_TIM2                                  28    /* TIM2 global Interrupt                                */
#define  BSP_INT_ID_TIM3                                  29    /* TIM3 global Interrupt                                */
#define  BSP_INT_ID_TIM4                                  30    /* TIM4 global Interrupt                                */
#define  BSP_INT_ID_I2C1_EV                               31    /* I2C1 Event Interrupt                                 */
#define  BSP_INT_ID_I2C1_ER                               32    /* I2C1 Error Interrupt                                 */
#define  BSP_INT_ID_I2C2_EV                               33    /* I2C2 Event Interrupt                                 */
#define  BSP_INT_ID_I2C2_ER                               34    /* I2C2 Error Interrupt                                 */
#define  BSP_INT_ID_SPI1                                  35    /* SPI1 global Interrupt                                */
#define  BSP_INT_ID_SPI2                                  36    /* SPI2 global Interrupt                                */
#define  BSP_INT_ID_USART1                                37    /* USART1 global Interrupt                              */
#define  BSP_INT_ID_USART2                                38    /* USART2 global Interrupt                              */
#define  BSP_INT_ID_USART3                                39    /* USART3 global Interrupt                              */
#define  BSP_INT_ID_EXTI15_10                             40    /* External Line[15:10] Interrupts                      */
#define  BSP_INT_ID_RTC_ALARM                             41    /* RTC Alarm through EXTI Line Interrupt                */
#define  BSP_INT_ID_OTG_FS_WKUP                           42    /* USB WakeUp from suspend through EXTI Line Interrupt  */

#define  BSP_INT_ID_TIM8_BRK                              43    /* USB WakeUp from suspend through EXTI Line Interrupt  */
#define  BSP_INT_ID_TIM8_UP                               44    /* USB WakeUp from suspend through EXTI Line Interrupt  */
#define  BSP_INT_ID_TIM8_TRG_COM                          45    /* USB WakeUp from suspend through EXTI Line Interrupt  */
#define  BSP_INT_ID_TIM8_CC_IRQ                           46    /* USB WakeUp from suspend through EXTI Line Interrupt  */
#define  BSP_INT_ID_ADC3                                  47    /* USB WakeUp from suspend through EXTI Line Interrupt  */
#define  BSP_INT_ID_FSMC                                  48    /* USB WakeUp from suspend through EXTI Line Interrupt  */
#define  BSP_INT_ID_SDIO                                  49    /* USB WakeUp from suspend through EXTI Line Interrupt  */

#define  BSP_INT_ID_TIM5                                  50    /* TIM5 global Interrupt                                */
#define  BSP_INT_ID_SPI3                                  51    /* SPI3 global Interrupt                                */
#define  BSP_INT_ID_USART4                                52    /* USART4 global Interrupt                              */
#define  BSP_INT_ID_USART5                                53    /* USRT5 global Interrupt                               */
#define  BSP_INT_ID_TIM6                                  54    /* TIM6 global Interrupt                                */
#define  BSP_INT_ID_TIM7                                  55    /* TIM7 global Interrupt                                */
#define  BSP_INT_ID_DMA2_CH1                              56    /* DMA2 Channel 1 global Interrupt                      */
#define  BSP_INT_ID_DMA2_CH2                              57    /* DMA2 Channel 2 global Interrupt                      */
#define  BSP_INT_ID_DMA2_CH3                              58    /* DMA2 Channel 3 global Interrupt                      */
#define  BSP_INT_ID_DMA2_CH4                              59    /* DMA2 Channel 4 global Interrupt                      */
#define  BSP_INT_ID_DMA2_CH5                              60    /* DMA2 Channel 5 global Interrupt                      */

#define  BSP_INT_ID_ETH                                   61    /* ETH  global Interrupt                                */
#define  BSP_INT_ID_ETH_WKUP                              62    /* ETH  WakeUp from EXTI line interrupt                 */
#define  BSP_INT_ID_CAN2_TX                               63    /* CAN2 TX Interrupts                                   */
#define  BSP_INT_ID_CAN2_RX0                              64    /* CAN2 RX0 Interrupts                                  */
#define  BSP_INT_ID_CAN2_RX1                              65    /* CAN2 RX1 Interrupt                                   */
#define  BSP_INT_ID_CAN2_SCE                              66    /* CAN2 SCE Interrupt                                   */
#define  BSP_INT_ID_OTG_FS                                67    /* OTG global Interrupt                                 */



#define ConcentratorParameter_AddrEeprom        0x10

#define JD_ConfigData_AddrEeprom                0x20   //竟达抄表数据 在EEPROM 的地址
#define JD_GprsData_AddrEeprom                  0x40   //竟达集中器GPRS参数 在EEPROM 的地址

/*********************************************************************************
数据类型定义
*********************************************************************************/

/*********************************************************************************
外部变量声明区
*********************************************************************************/

/*********************************************************************************
函数声明区
*********************************************************************************/
/********************************************************************************/

#endif
