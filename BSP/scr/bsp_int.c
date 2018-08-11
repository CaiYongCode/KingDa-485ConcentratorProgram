/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#define  BSP_INT_MODULE
#include <bsp.h>

/*********************************************************************************
常量定义区
*********************************************************************************/
#define  BSP_INT_SRC_NBR     68
/*********************************************************************************
公共变量定义区
*********************************************************************************/

/*********************************************************************************
外部变量声明区
*********************************************************************************/

/*********************************************************************************
私有变量定义区
*********************************************************************************/
static  CPU_FNCT_VOID  BSP_IntVectTbl[BSP_INT_SRC_NBR];
/*********************************************************************************
测试变量定义区
*********************************************************************************/

/*********************************************************************************
内部函数定义区
*********************************************************************************/
static  void  BSP_IntHandler     (CPU_DATA  int_id);
static  void  BSP_IntHandlerDummy(void);
/*********************************************************************************
功能代码定义区
*********************************************************************************/
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void  BSP_IntClr (CPU_DATA  int_id)
{

}


/*
*********************************************************************************************************
*                                              BSP_IntDis()
*
* Description : Disable interrupt.
*
* Argument(s) : int_id      Interrupt to disable.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_IntDis (CPU_DATA  int_id)
{
    if (int_id < BSP_INT_SRC_NBR) {
        CPU_IntSrcDis(int_id + 16);
    }
}


/*
*********************************************************************************************************
*                                           BSP_IntDisAll()
*
* Description : Disable ALL interrupts.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_IntDisAll (void)
{
    CPU_IntDis();
}


/*
*********************************************************************************************************
*                                               BSP_IntEn()
*
* Description : Enable interrupt.
*
* Argument(s) : int_id      Interrupt to enable.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_IntEn (CPU_DATA  int_id)
{
    if (int_id < BSP_INT_SRC_NBR) {
        CPU_IntSrcEn(int_id + 16);
    }
}


/*
*********************************************************************************************************
*                                            BSP_IntVectSet()
*
* Description : Assign ISR handler.
*
* Argument(s) : int_id      Interrupt for which vector will be set.
*
*               isr         Handler to assign
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_IntVectSet (CPU_DATA       int_id, CPU_FNCT_VOID  isr)
{
    CPU_SR_ALLOC();

    if (int_id < BSP_INT_SRC_NBR) {
        CPU_CRITICAL_ENTER();
        BSP_IntVectTbl[int_id] = isr;
        CPU_CRITICAL_EXIT();
    }
}


/*
*********************************************************************************************************
*                                            BSP_IntPrioSet()
*
* Description : Assign ISR priority.
*
* Argument(s) : int_id      Interrupt for which vector will be set.
*
*               prio        Priority to assign
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_IntPrioSet (CPU_DATA    int_id,
                      CPU_INT08U  prio)
{
    CPU_SR_ALLOC();


    if (int_id < BSP_INT_SRC_NBR) {
        CPU_CRITICAL_ENTER();
        CPU_IntSrcPrioSet(int_id + 16, prio);
        CPU_CRITICAL_EXIT();
    }
}


/*
*********************************************************************************************************
*********************************************************************************************************
*                                           INTERNAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                              BSP_IntInit()
*
* Description : Initialize interrupts:
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_IntInit (void)
{
    CPU_DATA  int_id;


    for (int_id = 0; int_id < BSP_INT_SRC_NBR; int_id++) {
        BSP_IntVectSet(int_id, BSP_IntHandlerDummy);
    }
}


/*
*********************************************************************************************************
*                                        BSP_IntHandler####()
*
* Description : Handle an interrupt.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_IntHandlerWWDG          (void)  { BSP_IntHandler(BSP_INT_ID_WWDG);            }
void  BSP_IntHandlerPVD           (void)  { BSP_IntHandler(BSP_INT_ID_PVD);             }
void  BSP_IntHandlerTAMPER        (void)  { BSP_IntHandler(BSP_INT_ID_TAMPER);          }
void  BSP_IntHandlerRTC           (void)  { BSP_IntHandler(BSP_INT_ID_RTC);             }
void  BSP_IntHandlerFLASH         (void)  { BSP_IntHandler(BSP_INT_ID_FLASH);           }
void  BSP_IntHandlerRCC           (void)  { BSP_IntHandler(BSP_INT_ID_RCC);             }
void  BSP_IntHandlerEXTI0         (void)  { BSP_IntHandler(BSP_INT_ID_EXTI0);           }
void  BSP_IntHandlerEXTI1         (void)  { BSP_IntHandler(BSP_INT_ID_EXTI1);           }
void  BSP_IntHandlerEXTI2         (void)  { BSP_IntHandler(BSP_INT_ID_EXTI2);           }
void  BSP_IntHandlerEXTI3         (void)  { BSP_IntHandler(BSP_INT_ID_EXTI3);           }
void  BSP_IntHandlerEXTI4         (void)  { BSP_IntHandler(BSP_INT_ID_EXTI4);           }
void  BSP_IntHandlerDMA1_CH1      (void)  { BSP_IntHandler(BSP_INT_ID_DMA1_CH1);        }
void  BSP_IntHandlerDMA1_CH2      (void)  { BSP_IntHandler(BSP_INT_ID_DMA1_CH2);        }
void  BSP_IntHandlerDMA1_CH3      (void)  { BSP_IntHandler(BSP_INT_ID_DMA1_CH3);        }
void  BSP_IntHandlerDMA1_CH4      (void)  { BSP_IntHandler(BSP_INT_ID_DMA1_CH4);        }
void  BSP_IntHandlerDMA1_CH5      (void)  { BSP_IntHandler(BSP_INT_ID_DMA1_CH5);        }
void  BSP_IntHandlerDMA1_CH6      (void)  { BSP_IntHandler(BSP_INT_ID_DMA1_CH6);        }
void  BSP_IntHandlerDMA1_CH7      (void)  { BSP_IntHandler(BSP_INT_ID_DMA1_CH7);        }
void  BSP_IntHandlerADC1_2        (void)  { BSP_IntHandler(BSP_INT_ID_ADC1_2);          }
void  BSP_IntHandlerCAN1_TX       (void)  { BSP_IntHandler(BSP_INT_ID_CAN1_TX);         }
void  BSP_IntHandlerCAN1_RX0      (void)  { BSP_IntHandler(BSP_INT_ID_CAN1_RX0);        }
void  BSP_IntHandlerCAN1_RX1      (void)  { BSP_IntHandler(BSP_INT_ID_CAN1_RX1);        }
void  BSP_IntHandlerCAN1_SCE      (void)  { BSP_IntHandler(BSP_INT_ID_CAN1_SCE);        }
void  BSP_IntHandlerEXTI9_5       (void)  { BSP_IntHandler(BSP_INT_ID_EXTI9_5);         }
void  BSP_IntHandlerTIM1_BRK      (void)  { BSP_IntHandler(BSP_INT_ID_TIM1_BRK);        }
void  BSP_IntHandlerTIM1_UP       (void)  { BSP_IntHandler(BSP_INT_ID_TIM1_UP);         }
void  BSP_IntHandlerTIM1_TRG_COM  (void)  { BSP_IntHandler(BSP_INT_ID_TIM1_TRG_COM);    }
void  BSP_IntHandlerTIM1_CC       (void)  { BSP_IntHandler(BSP_INT_ID_TIM1_CC);         }
void  BSP_IntHandlerTIM2          (void)  { BSP_IntHandler(BSP_INT_ID_TIM2);            }
void  BSP_IntHandlerTIM3          (void)  { BSP_IntHandler(BSP_INT_ID_TIM3);            }
void  BSP_IntHandlerTIM4          (void)  { BSP_IntHandler(BSP_INT_ID_TIM4);            }
void  BSP_IntHandlerI2C1_EV       (void)  { BSP_IntHandler(BSP_INT_ID_I2C1_EV);         }
void  BSP_IntHandlerI2C1_ER       (void)  { BSP_IntHandler(BSP_INT_ID_I2C1_ER);         }
void  BSP_IntHandlerI2C2_EV       (void)  { BSP_IntHandler(BSP_INT_ID_I2C2_EV);         }
void  BSP_IntHandlerI2C2_ER       (void)  { BSP_IntHandler(BSP_INT_ID_I2C2_ER);         }
void  BSP_IntHandlerSPI1          (void)  { BSP_IntHandler(BSP_INT_ID_SPI1);            }
void  BSP_IntHandlerSPI2          (void)  { BSP_IntHandler(BSP_INT_ID_SPI2);            }
void  BSP_IntHandlerUSART1        (void)  { BSP_IntHandler(BSP_INT_ID_USART1);          }
void  BSP_IntHandlerUSART2        (void)  { BSP_IntHandler(BSP_INT_ID_USART2);          }
void  BSP_IntHandlerUSART3        (void)  { BSP_IntHandler(BSP_INT_ID_USART3);          }
void  BSP_IntHandlerEXTI15_10     (void)  { BSP_IntHandler(BSP_INT_ID_EXTI15_10);       }
void  BSP_IntHandlerRTCAlarm      (void)  { BSP_IntHandler(BSP_INT_ID_RTC_ALARM);       }
void  BSP_IntHandlerUSBWakeUp     (void)  { BSP_IntHandler(BSP_INT_ID_OTG_FS_WKUP);     }

void  BSP_IntHandlerTIM8_BRK      (void)  { BSP_IntHandler(BSP_INT_ID_TIM8_BRK);        }
void  BSP_IntHandlerTIM8_UP       (void)  { BSP_IntHandler(BSP_INT_ID_TIM8_UP);         }
void  BSP_IntHandlerTIM8_TRG_COM  (void)  { BSP_IntHandler(BSP_INT_ID_TIM8_TRG_COM);    }
void  BSP_IntHandlerTIM8_CC_IRQ   (void)  { BSP_IntHandler(BSP_INT_ID_TIM8_CC_IRQ);     }
void  BSP_IntHandlerADC3          (void)  { BSP_IntHandler(BSP_INT_ID_ADC3);            }
void  BSP_IntHandlerFSMC          (void)  { BSP_IntHandler(BSP_INT_ID_FSMC);            }
void  BSP_IntHandlerSDIO          (void)  { BSP_IntHandler(BSP_INT_ID_SDIO);            }


void  BSP_IntHandlerTIM5          (void)  { BSP_IntHandler(BSP_INT_ID_TIM5);            }
void  BSP_IntHandlerSPI3          (void)  { BSP_IntHandler(BSP_INT_ID_SPI3);            }
void  BSP_IntHandlerUSART4        (void)  { BSP_IntHandler(BSP_INT_ID_USART4);          }
void  BSP_IntHandlerUSART5        (void)  { BSP_IntHandler(BSP_INT_ID_USART5);          }
void  BSP_IntHandlerTIM6          (void)  { BSP_IntHandler(BSP_INT_ID_TIM6);            }
void  BSP_IntHandlerTIM7          (void)  { BSP_IntHandler(BSP_INT_ID_TIM7);            }
void  BSP_IntHandlerDMA2_CH1      (void)  { BSP_IntHandler(BSP_INT_ID_DMA2_CH1);        }
void  BSP_IntHandlerDMA2_CH2      (void)  { BSP_IntHandler(BSP_INT_ID_DMA2_CH2);        }
void  BSP_IntHandlerDMA2_CH3      (void)  { BSP_IntHandler(BSP_INT_ID_DMA2_CH3);        }
void  BSP_IntHandlerDMA2_CH4      (void)  { BSP_IntHandler(BSP_INT_ID_DMA2_CH4);        }
void  BSP_IntHandlerDMA2_CH5      (void)  { BSP_IntHandler(BSP_INT_ID_DMA2_CH5);        }
void  BSP_IntHandlerETH           (void)  { BSP_IntHandler(BSP_INT_ID_ETH);             }
void  BSP_IntHandlerETHWakeup     (void)  { BSP_IntHandler(BSP_INT_ID_ETH_WKUP);        }
void  BSP_IntHandlerCAN2_TX       (void)  { BSP_IntHandler(BSP_INT_ID_CAN2_TX);         }
void  BSP_IntHandlerCAN2_RX0      (void)  { BSP_IntHandler(BSP_INT_ID_CAN2_RX0);        }
void  BSP_IntHandlerCAN2_RX1      (void)  { BSP_IntHandler(BSP_INT_ID_CAN2_RX1);        }
void  BSP_IntHandlerCAN2_SCE      (void)  { BSP_IntHandler(BSP_INT_ID_CAN2_SCE);        }
void  BSP_IntHandlerOTG           (void)  { BSP_IntHandler(BSP_INT_ID_OTG_FS);          }


/*
*********************************************************************************************************
*********************************************************************************************************
*                                           LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                          BSP_IntHandler()
*
* Description : Central interrupt handler.
*
* Argument(s) : int_id          Interrupt that will be handled.
*
* Return(s)   : none.
*
* Caller(s)   : ISR handlers.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  BSP_IntHandler (CPU_DATA  int_id)
{
    CPU_FNCT_VOID  isr;

    CPU_SR_ALLOC();


    CPU_CRITICAL_ENTER();                                       /* Tell the OS that we are starting an ISR            */

    OSIntEnter();

    CPU_CRITICAL_EXIT();

    if (int_id < BSP_INT_SRC_NBR) {
        isr = BSP_IntVectTbl[int_id];
        if (isr != (CPU_FNCT_VOID)0) {
            isr();
        }
    }

    OSIntExit();                                                /* Tell the OS that we are leaving the ISR            */
}


/*
*********************************************************************************************************
*                                        BSP_IntHandlerDummy()
*
* Description : Dummy interrupt handler.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_IntHandler().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  BSP_IntHandlerDummy (void)
{

}


