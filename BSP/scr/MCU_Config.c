/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "..\inc\MCU_Config.h"
/*********************************************************************************
常量定义区
*********************************************************************************/
/*********************************************************************************
公共变量定义区
*********************************************************************************/

/*********************************************************************************
外部变量声明区
*********************************************************************************/

/*********************************************************************************
私有变量定义区
*********************************************************************************/
extern unsigned short ADC1_DataBuff[8];
/*********************************************************************************
测试变量定义区
*********************************************************************************/

/*********************************************************************************
内部函数定义区
*********************************************************************************/

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
void  RCC_Configuration (void)
{
  ErrorStatus HSEStartUpStatus;
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();
  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);
  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();
  if(HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);   
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 
    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);
    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);
    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){ }
    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08){}
  }
 RCC_HCLKConfig  (RCC_SYSCLK_Div1);  //AHB 72MHZ
 RCC_PCLK1Config (RCC_HCLK_Div2);	 //APB1 36MHZ
 RCC_PCLK2Config (RCC_HCLK_Div1);    //APB2 72MHZ 
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void NVIC_Configuration(void)
{
 NVIC_InitTypeDef NVIC_InitStructure;
#ifdef  VECT_TAB_RAM  
  /* Set the Vector Table base location at 0x20000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE); 
  GPIO_ResetBits(GPIOA, GPIO_Pin_All); 
  GPIO_ResetBits(GPIOB, GPIO_Pin_All); 
  GPIO_ResetBits(GPIOC, GPIO_Pin_All); 
  GPIO_ResetBits(GPIOD, GPIO_Pin_All);
  GPIO_ResetBits(GPIOE, GPIO_Pin_All);   
  GPIO_SetBits(GPIOA, GPIO_Pin_2 | GPIO_Pin_9 | GPIO_Pin_10);
  GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_6 
                      | GPIO_Pin_12);
  GPIO_SetBits(GPIOC, GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_12);
  GPIO_SetBits(GPIOD, GPIO_Pin_8 );
  GPIO_SetBits(GPIOE, GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);  //RS485_CH1_EN,RS485_CH2_EN, RS485_CH3_EN, RS485_CH4_EN,RS485_DIR      
  //GPIOA; 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_8 | GPIO_Pin_11;	//SD_CD		     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;	//SD_CD		     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 
                                | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
  //GPIOB; 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11|GPIO_Pin_14;                       //USART1_RX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |  GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | 
                                GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;	 			     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  //GPIOC;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	 		     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_4;	 		     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_5  | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | 
                                GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;	 		     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  //GPIOD; 
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_6 | GPIO_Pin_9;	// RTC_INT User_K2	     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_7 |
                                GPIO_Pin_8 |  GPIO_Pin_10 |  GPIO_Pin_11 |  GPIO_Pin_12 |  GPIO_Pin_13 |  
                                GPIO_Pin_14 |  GPIO_Pin_15;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  //GPIOE;                      
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void ADC1_Configuration(void)
{
 ADC_InitTypeDef  ADC_InitStruct;
 GPIO_InitTypeDef GPIO_InitStructure;
 NVIC_InitTypeDef NVIC_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC |RCC_APB2Periph_ADC1 ,ENABLE ); //使能 ADC1 通道时钟，各个管脚时钟
 RCC_ADCCLKConfig(RCC_PCLK2_Div6); //72M/6=12,ADC 最大时间不能超过 14M
 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //使能 DMA 传输
 RCC_ADCCLKConfig(RCC_PCLK2_Div6); //72M/6=12,ADC 最大时间不能超过 14M
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //模拟输入引脚
 GPIO_Init(GPIOA, &GPIO_InitStructure);
 //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_4;
// /GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //模拟输入引脚
 //GPIO_Init(GPIOC, &GPIO_InitStructure);
 ADC_DeInit(ADC1);
 ADC_InitStruct.ADC_Mode = ADC_Mode_Independent ; //工作在独立模式
 ADC_InitStruct.ADC_ScanConvMode = ENABLE;   //工作在扫描模式
 ADC_InitStruct.ADC_ContinuousConvMode = DISABLE; //单次模式
 ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //转换由软件而不是外部触发启动
 ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;  //ADC 数据右对齐
 ADC_InitStruct.ADC_NbrOfChannel = 2; //顺序进行规则转换的 ADC 通道的数目  
 ADC_Init(ADC1, &ADC_InitStruct);
 //设置指定 ADC 的规则组通道，设置它们的转化顺序和采样时间
 ADC_RegularChannelConfig(ADC1, ADC_Channel_0,  1,  ADC_SampleTime_239Cycles5 );
 ADC_RegularChannelConfig(ADC1, ADC_Channel_1,  2,  ADC_SampleTime_239Cycles5 ); 
 //ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 3,  ADC_SampleTime_239Cycles5 ); 
 //ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 4,  ADC_SampleTime_239Cycles5 ); 
 //ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 5,  ADC_SampleTime_239Cycles5 ); 
 DMA1_Channel1->CNDTR = 2;
 DMA1_Channel1->CPAR = (u32)(&(ADC1->DR));
 DMA1_Channel1->CMAR = (u32)(ADC1_DataBuff);
 DMA1_Channel1->CCR &= (uint32_t)0xFFFF0000;
 DMA1_Channel1->CCR |= DMA_DIR_PeripheralSRC | DMA_Mode_Circular | DMA_PeripheralInc_Disable | 
                        DMA_MemoryInc_Enable | DMA_PeripheralDataSize_HalfWord |DMA_MemoryDataSize_HalfWord|
                        DMA_Priority_Low | DMA_IT_TE | DMA_IT_TC | DMA_CCR1_EN ;
 ADC_DMACmd(ADC1,ENABLE); //使能或者失能指定的 ADC 的 DMA 请求 
 ADC_Cmd(ADC1,ENABLE);  //使能或者失能指定的 ADC 
 DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
 NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 NVIC_Init(&NVIC_InitStructure); 
 ADC_ResetCalibration(ADC1); //复位指定的ADC1的校准寄存器
 while(ADC_GetResetCalibrationStatus(ADC1)); //获取ADC1复位校准寄存器的状态,设置状态则等待 
 ADC_StartCalibration(ADC1); //开始指定ADC1的校准状态 
 while(ADC_GetCalibrationStatus(ADC1)); //获取指定ADC1的校准程序,设置状态则等待
 ADC_SoftwareStartConvCmd(ADC1, ENABLE);  //使能或者失能指定的 ADC 的软件转换启动功能
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void EXTI_Configuration (void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD , ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;// | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource4);
  EXTI_InitStructure.EXTI_Line = EXTI_Line4;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		   
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        // 72MHz
*********************************************************************************/
void TIM2_Configuration (void)
{
 TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
 TIM_DeInit (TIM2);
 TIM_TimeBaseInitStruct.TIM_Period = 0;
 TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; 
 TIM_TimeBaseInitStruct.TIM_Prescaler = 71;
 TIM_TimeBaseInitStruct.TIM_ClockDivision  = TIM_CKD_DIV1;
 TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0 ;
 TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void TIM3_Configuration (void)
{
 TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
 TIM_DeInit (TIM3);
 TIM_TimeBaseInitStruct.TIM_Period = 0;
 TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; 
 TIM_TimeBaseInitStruct.TIM_Prescaler = 71;
 TIM_TimeBaseInitStruct.TIM_ClockDivision  = TIM_CKD_DIV1;
 TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
 TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void TIM4_Configuration (void)
{
 TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
 TIM_DeInit (TIM4);
 TIM_TimeBaseInitStruct.TIM_Period = 0;
 TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; 
 TIM_TimeBaseInitStruct.TIM_Prescaler = 71;
 TIM_TimeBaseInitStruct.TIM_ClockDivision  = TIM_CKD_DIV1;
 TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
 TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void TIM5_Configuration (void)
{
 TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
 TIM_DeInit (TIM5);
 TIM_TimeBaseInitStruct.TIM_Period = 0;
 TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; 
 TIM_TimeBaseInitStruct.TIM_Prescaler = 71;
 TIM_TimeBaseInitStruct.TIM_ClockDivision  = TIM_CKD_DIV1;
 TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
 TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStruct);
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void SPI1_Configuration(void)
{
 GPIO_InitTypeDef GPIO_InitStructure;
 SPI_InitTypeDef  SPI_InitStruct ;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1  , ENABLE);
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;	//CLK  MOSI
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOA, &GPIO_InitStructure);
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;		 //MISO
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOA, &GPIO_InitStructure);
 GPIOA->BSRR = GPIO_Pin_4;
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	  //NSS
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOA, &GPIO_InitStructure);
 SPI_I2S_DeInit (SPI1);
 SPI_SSOutputCmd(SPI1, ENABLE);
 SPI_InitStruct. SPI_Direction          = SPI_Direction_2Lines_FullDuplex;
 SPI_InitStruct. SPI_Mode               = SPI_Mode_Master;
 SPI_InitStruct. SPI_DataSize           = SPI_DataSize_8b;
 SPI_InitStruct. SPI_CPOL               = SPI_CPOL_Low ;
 SPI_InitStruct. SPI_CPHA               = SPI_CPHA_1Edge;
 SPI_InitStruct. SPI_NSS                = SPI_NSS_Soft;
 SPI_InitStruct. SPI_BaudRatePrescaler  = SPI_BaudRatePrescaler_16;	  //72MHz / 8
 SPI_InitStruct. SPI_FirstBit           = SPI_FirstBit_MSB;	  				  
 SPI_Init (SPI1, &SPI_InitStruct);
 SPI_CalculateCRC(SPI1, DISABLE);
 SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);
 SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
 SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE | SPI_I2S_IT_RXNE | SPI_I2S_IT_ERR, DISABLE);
 SPI_Cmd(SPI1, ENABLE);	
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void SPI2_Configuration(void)
{
 GPIO_InitTypeDef GPIO_InitStructure;
 SPI_InitTypeDef  SPI_InitStruct ;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2  , ENABLE);
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;	//CLK  MOSI
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;		 //MISO
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
 GPIOB->BSRR = GPIO_Pin_12;
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	  //NSS
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
 SPI_I2S_DeInit (SPI2);
 SPI_SSOutputCmd(SPI2, ENABLE);
 SPI_InitStruct. SPI_Direction          = SPI_Direction_2Lines_FullDuplex;
 SPI_InitStruct. SPI_Mode               = SPI_Mode_Master;
 SPI_InitStruct. SPI_DataSize           = SPI_DataSize_8b;
 SPI_InitStruct. SPI_CPOL               = SPI_CPOL_High ;
 SPI_InitStruct. SPI_CPHA               = SPI_CPHA_2Edge;
 SPI_InitStruct. SPI_NSS                = SPI_NSS_Soft;
 SPI_InitStruct. SPI_BaudRatePrescaler  = SPI_BaudRatePrescaler_8;	  //36MHz / 8
 SPI_InitStruct. SPI_FirstBit           = SPI_FirstBit_MSB;	  				  
 SPI_Init (SPI2, &SPI_InitStruct);
 SPI_CalculateCRC(SPI2, DISABLE);
 //SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Rx, ENABLE);
 //SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);
 //SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_TXE | SPI_I2S_IT_RXNE | SPI_I2S_IT_ERR, DISABLE);
 SPI_Cmd(SPI2, ENABLE);
}
/**********************************************
函数名：		  
函数描述：		   
调用列表：		   
被调用列表：
入口参数描述：	   
                   
				  
出口参数描述：
函数返回值描述：
其他说明： 	SPI/I2S2_RX  
***********************************************/
void I2C1_Configuration(void)
{
  I2C_InitTypeDef  I2C_InitStructure; 
  GPIO_InitTypeDef  GPIO_InitStructure; 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIOB->BSRR = GPIO_Pin_8 | GPIO_Pin_9; 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1 , ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);	
  /* I2C configuration */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0xA2;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = 100000;
  /* Apply I2C configuration after enabling it */
  I2C_DeInit( I2C1 );
  I2C_Init(I2C1, &I2C_InitStructure); 
  I2C_Cmd(I2C1, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/**********************************************
函数名：		  
函数描述：		   
调用列表：		   
被调用列表：
入口参数描述：	   
                   
				  
出口参数描述：
函数返回值描述：
其他说明： 	SPI/I2S2_RX  
***********************************************/
void I2C1_Rset(void)
{
  unsigned long  itemp = 0x4fff;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1 , DISABLE);
  while(itemp){itemp --;}
  I2C1_Configuration( );
  
}
/**********************************************
函数名：		  
函数描述：		   
调用列表：		   
被调用列表：
入口参数描述：	   
                   
				  
出口参数描述：
函数返回值描述：
其他说明： 	SPI/I2S2_RX  
***********************************************/
void I2C2_Configuration(void)
{
  I2C_InitTypeDef  I2C_InitStructure; 
  GPIO_InitTypeDef  GPIO_InitStructure; 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIOB->BSRR = GPIO_Pin_11 | GPIO_Pin_10; 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2 , ENABLE);
  /* I2C configuration */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0xA0;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = 100000;
  /* Apply I2C configuration after enabling it */
  I2C_DeInit( I2C2 );
  I2C_Init(I2C2, &I2C_InitStructure); 
  I2C_Cmd(I2C2, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/**********************************************
函数名：		  
函数描述：		   
调用列表：		   
被调用列表：
入口参数描述：	   
                   
				  
出口参数描述：
函数返回值描述：
其他说明： 	SPI/I2S2_RX  
***********************************************/
void I2C2_Rset(void)
{
  unsigned long  itemp = 0x4fff;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2 , DISABLE);
  while(itemp){itemp --;}
  I2C2_Configuration( );
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void IWDG_Configuration(void)                                                 //独立看门狗配置
{
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);                                 //取消写保护
   /* IWDG counter clock: 37KHz(LSI) / 256 = 0.144 KHz */
  IWDG_SetPrescaler(IWDG_Prescaler_256);                                         //写分频值
  IWDG_SetReload(4095);                                                        //自动装载值
  IWDG_ReloadCounter();                                                         //喂狗
  IWDG_Enable();                                                                //启动看门狗
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void MCU_init ( void )
{
 RCC_Configuration ( );
 NVIC_Configuration( );
 GPIO_Configuration( );
 ADC1_Configuration( );
 EXTI_Configuration ( );
 
 //TIM2_Configuration( );
 //TIM3_Configuration( );
 //TIM4_Configuration( );
 //EXTI_Configuration( );
 //SPI1_Configuration( );
 SPI2_Configuration( );
 I2C1_Configuration ( );
 I2C2_Configuration( );
 IWDG_Configuration( );
// DMA1_Configuration( );
}
/**************************************END****************************************/