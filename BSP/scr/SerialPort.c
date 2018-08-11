/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "..\inc\SerialPort.h"
#include "..\inc\PowerConfig.h"
#include <string.h>
/*********************************************************************************
常量定义区
*********************************************************************************/
/*********************************************************************************
公共变量定义区
*********************************************************************************/
PortStatus  Rs232Flg = Idle;
PortStatus  Rs485Flg = Idle;
PortStatus  USART2Flg = Idle;
unsigned short   RS485_IdleTime  = 0x00;
/*********************************************************************************
外部变量声明区
*********************************************************************************/

/*********************************************************************************
私有变量定义区
*********************************************************************************/
static unsigned   char*  Rs232_TX_P = P_NULL;
static unsigned   short  Rs232_TX_CNT =0;
static Rs232_BuffStruct*  Rs232_RX_P = P_NULL;
static unsigned   short  Rs232_RX_CNT =0;
static SerialPort_ReceiveMod   Rs232_ReceiveMod = Polling;
static Rs232_BuffStruct   Rs232_buff[Rs232_Number] ;
static SerialPort_SendEndCallBack_Type  Rs232_SendEnd_CallBack = P_NULL;
static void*  Rs232_SendEnd_CallBack_arg = P_NULL;

static unsigned   char*  Rs485_TX_P = P_NULL;
static unsigned   short  Rs485_TX_CNT =0;
static Rs485_BuffStruct*   Rs485_RX_P = P_NULL;
static unsigned   short  Rs485_RX_CNT =0;  
static SerialPort_ReceiveMod   Rs485_ReceiveMod = Polling;
static Rs485_BuffStruct   Rs485_buff[Rs485_Number] ;
static SerialPort_SendEndCallBack_Type  Rs485_SendEnd_CallBack = P_NULL;
static void* Rs485_SendEnd_CallBack_arg = P_NULL;
static CommPortNumber_Type  Rs485_PortPV =  Null_Port;

static unsigned   char*  USART2_TX_P = P_NULL;
static unsigned   short  USART2_TX_CNT =0;
static USART2_BuffStruct*  USART2_RX_P = P_NULL;
static unsigned   short  USART2_RX_CNT =0;
static SerialPort_ReceiveMod   USART2_ReceiveMod = Polling;
static USART2_BuffStruct   USART2_buff[USART2_Number] ;
static SerialPort_SendEndCallBack_Type  USART2_SendEnd_CallBack = P_NULL;
static void* USART2_SendEnd_CallBack_arg = P_NULL;
/*********************************************************************************
测试变量定义区
*********************************************************************************/
//static   unsigned char _485_test = 0;
/*********************************************************************************
内部函数定义区
*********************************************************************************/
static SystemErrName activateReceiveByDMA_Rs232 ( void );
static Rs232_BuffStruct* GetIdleBuff_Rs232 (void);
static SystemErrName activateReceiveByDMA_Rs485 ( void );
static Rs485_BuffStruct* GetIdleBuff_Rs485 (void);
static SystemErrName activateReceiveByDMA_USART2 ( void );
static USART2_BuffStruct* GetIdleBuff_USART2 (void);
/*********************************************************************************
功能代码定义区
*********************************************************************************/
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void DeInitBuff_Rs232(void)
{
  unsigned char itemp =0;
  while ( itemp < Rs232_Number)
  {
   Rs232_buff[itemp].CCR.Byte = 0; 
   Rs232_buff[itemp].CCR.Bit.idle = 1;
   itemp ++;
  }
  Rs232_RX_P = GetIdleBuff_Rs232 ( );
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
Rs232_BuffStruct* GetIdleBuff_Rs232 (void)
{
  unsigned char itemp =0;
  while ( itemp < Rs232_Number)
  {
    if(1 == Rs232_buff[itemp].CCR.Bit.idle)
    {
     Rs232_buff[itemp].CCR.Bit.idle = 0;
     return &(Rs232_buff[itemp]);
    }
    itemp ++;
  }
  return  P_NULL;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
Rs232_BuffStruct* GetDataForBuff_Rs232 (void)
{
  unsigned char itemp =0;
  while ( itemp < Rs232_Number)
  {
    if(1 == Rs232_buff[itemp].CCR.Bit.flg)
    { return &(Rs232_buff[itemp]);}
    itemp ++;
  }
  return  P_NULL;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void FreeBuff_Rs232(Rs232_BuffStruct* buff)
{
  if(P_NULL != buff)
  {
    buff->CCR.Byte = 0; 
    buff->CCR.Bit.idle = 1;
  }
  if(P_NULL == Rs232_RX_P)
  {
    Rs232_RX_P = GetIdleBuff_Rs232 ( );
    Rs232_RX_CNT = 0; 
    if(P_NULL != Rs232_RX_P)
    {
      if(ByDMA == Rs232_ReceiveMod)
      {
        DMA1_Channel3->CCR   &= (uint32_t)0xFFFF0000;
        DMA1_Channel3->CNDTR = Rs232_BuffLength;
        DMA1_Channel3->CPAR  = (u32)(&(USART3->DR));
        DMA1_Channel3->CMAR  = (u32)(Rs232_RX_P->data);
        DMA1_Channel3->CCR  |= DMA_DIR_PeripheralSRC | DMA_Mode_Normal | DMA_PeripheralInc_Disable | 
                               DMA_MemoryInc_Enable | DMA_PeripheralDataSize_Byte |DMA_MemoryDataSize_Byte|
                               DMA_Priority_High | DMA_IT_TE | DMA_IT_TC | DMA_CCR1_EN ; 
       USART3->CR1 |= (0x00000001 << 4); //IDLE中断使能能 
       USART3->CR3 |= USART_DMAReq_Rx; 
      }
    }
  }
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void TXE_Interrupt_Rs232 (void)
{
  if(Rs232_TX_CNT > 1)
  {
    USART3->DR = *Rs232_TX_P ++;
    Rs232_TX_CNT --;
  } 
  else if(1 ==Rs232_TX_CNT)
  {
   USART_ITConfig(USART3, USART_IT_TXE, DISABLE); 
   USART_ITConfig(USART3, USART_IT_TC, ENABLE);
   USART3->DR = *Rs232_TX_P ++; 
   Rs232_TX_CNT = 0;
  }
  else
  {
   USART_ITConfig(USART3, USART_IT_TXE, DISABLE); 
   USART_ITConfig(USART3, USART_IT_TC, DISABLE);
   Rs232Flg = Idle;
  }
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void TC_Interrupt_Rs232 (void)
{
  USART_ITConfig(USART3, USART_IT_TC, DISABLE);
  if(P_NULL != Rs232_SendEnd_CallBack)
  {
    Rs232_SendEnd_CallBack(Rs232_Port ,Rs232_SendEnd_CallBack_arg);
    Rs232_SendEnd_CallBack = P_NULL;
  }
  Rs232Flg = Idle;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void RXNE_Interrupt_Rs232 (void)
{
  if(P_NULL != Rs232_RX_P)
  {
   if(Rs232_RX_CNT >= Rs232_BuffLength )
   {Rs232_RX_P->CCR.Bit.org = 1;}
   else
   {Rs232_RX_P->data[Rs232_RX_CNT ++] = USART3->DR;}   
  }
  USART3->SR  &= ~0x00000001 << 4;
  USART3->CR1 |= 0x00000001 << 4; 
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void IDLE_Interrupt_Rs232 (void)
{
  unsigned long  itemp;
  
  itemp = USART3->DR;   //清除空闲中断 
  if(P_NULL != Rs232_RX_P)
  {
    if(ByIT == Rs232_ReceiveMod)
    {
      Rs232_RX_P->length = Rs232_RX_CNT;
      Rs232_RX_P->CCR.Bit.flg = 1;
    }
    else if(ByDMA == Rs232_ReceiveMod)
    {
      itemp = DMA1_Channel3->CNDTR;
      Rs232_RX_P->length = Rs232_BuffLength - itemp;
      Rs232_RX_P->CCR.Bit.flg = 1;
    }
  }
  Rs232_RX_P = GetIdleBuff_Rs232 ( );
  Rs232_RX_CNT = 0;
  if(ByDMA == Rs232_ReceiveMod)
  {
   if(P_NULL != Rs232_RX_P)
   {
    DMA1_Channel3->CCR &= (uint32_t)0xFFFF0000;
    DMA1_Channel3->CNDTR = Rs232_BuffLength;
    DMA1_Channel3->CPAR = (u32)(&(USART3->DR));
    DMA1_Channel3->CMAR = (u32)(Rs232_RX_P->data);
    DMA1_Channel3->CCR |= DMA_DIR_PeripheralSRC | DMA_Mode_Normal | DMA_PeripheralInc_Disable | 
                        DMA_MemoryInc_Enable | DMA_PeripheralDataSize_Byte |DMA_MemoryDataSize_Byte|
                        DMA_Priority_High | DMA_IT_TE | DMA_IT_TC | DMA_CCR1_EN ; 
   USART3->CR1 |= (0x00000001 << 4); //IDLE中断使能能 
   USART3->CR3 |= USART_DMAReq_Rx;
   }
   else
   {
    DMA1_Channel3->CCR &=  ~(DMA_CCR1_EN);
    USART3->CR3 &= ~(USART_DMAReq_Rx); 
   }
  }
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void ERR_Interrupt_Rs232 (void)
{
 FreeBuff_Rs232(Rs232_RX_P);
 Rs232_RX_P = P_NULL;  
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void sendByDMA_TC_Interrupt_Rs232 (void)
{
 if(P_NULL != Rs232_SendEnd_CallBack)
  {
    Rs232_SendEnd_CallBack(Rs232_Port ,Rs232_SendEnd_CallBack_arg);
    Rs232_SendEnd_CallBack = P_NULL;
  }
  Rs232Flg = Idle;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void receiveByDMA_TC_Interrupt_Rs232 (void)
{
 FreeBuff_Rs232(Rs232_RX_P);
 Rs232_RX_P = P_NULL; 
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void Launch_Rs232 ( unsigned long BaudRate, SerialPort_Parity  Parity, 
                    SerialPort_StopBits StopBits, SerialPort_ReceiveMod mod )
{
 GPIO_InitTypeDef        GPIO_InitStructure;
 NVIC_InitTypeDef        NVIC_InitStructure;
 USART_InitTypeDef       USART_InitStruct;
 
 DeInitBuff_Rs232( );
 Rs232Flg = Idle;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
 GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOD, &GPIO_InitStructure);
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 GPIO_Init(GPIOD, &GPIO_InitStructure);
 USART_DeInit (USART3); 
 USART_InitStruct.USART_BaudRate = BaudRate;
 USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
 USART_InitStruct.USART_Mode = USART_Mode_Rx |  USART_Mode_Tx;
 USART_InitStruct.USART_Parity = Parity; 
 USART_InitStruct.USART_StopBits= StopBits;
 if((Parity_Even == Parity)||(Parity_Odd == Parity))
 {USART_InitStruct.USART_WordLength = USART_WordLength_9b;}
 else
 {USART_InitStruct.USART_WordLength = USART_WordLength_8b;}
 USART_Init(USART3, &USART_InitStruct);
 USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
 NVIC_InitStructure.NVIC_IRQChannel =  USART3_IRQn;
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 NVIC_Init(&NVIC_InitStructure);
 Rs232_ReceiveMod = mod;
 if(ByIT ==  Rs232_ReceiveMod)
 {
   USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
   USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
   Rs232_RX_CNT =0;
 }
 else if(ByDMA ==  Rs232_ReceiveMod)
 { activateReceiveByDMA_Rs232 ( ); }
 USART_Cmd(USART3, ENABLE);
 USART_ClearFlag(USART3, USART_FLAG_TC);
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName activateReceiveByDMA_Rs232 ( void )
{
  if(P_NULL == Rs232_RX_P)
  {
    Rs232_RX_P = GetIdleBuff_Rs232 ( );
    if(P_NULL == Rs232_RX_P)
    {return Nothing;}
  }
  RCC->AHBENR |= RCC_AHBPeriph_DMA1;
  DMA1_Channel3->CCR &= (uint32_t)0xFFFF0000;
  DMA1_Channel3->CNDTR = Rs232_BuffLength;
  DMA1_Channel3->CPAR = (u32)(&(USART3->DR));
  DMA1_Channel3->CMAR = (u32)(Rs232_RX_P->data);
  DMA1_Channel3->CCR |= DMA_DIR_PeripheralSRC | DMA_Mode_Normal | DMA_PeripheralInc_Disable | 
                        DMA_MemoryInc_Enable | DMA_PeripheralDataSize_Byte |DMA_MemoryDataSize_Byte|
                        DMA_Priority_High | DMA_IT_TE | DMA_IT_TC | DMA_CCR1_EN ;
  USART_ITConfig(USART3, USART_IT_RXNE, DISABLE); 
  USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
  USART3->CR3 |= USART_DMAReq_Rx;
  Rs232_ReceiveMod = ByDMA;
  return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName sendBuffByIT_Rs232 (void * buff, unsigned short length,
                                   SerialPort_SendEndCallBack_Type callBack,
                                   void *callBack_arg )
{
  if(Busy == Rs232Flg)
  {return PortBusy;}
  Rs232Flg = Busy;
  if(length > 0)
  {
    Rs232_SendEnd_CallBack = callBack;
    Rs232_SendEnd_CallBack_arg = callBack_arg;
    Rs232_TX_P = (unsigned char *)buff;
    Rs232_TX_CNT = length - 1;
    USART3->DR = *Rs232_TX_P ++;
    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
  } 
 return NO_ERR; 
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName sendStringByIT_Rs232 (unsigned char *buff)
{
 unsigned short itmep = 0;
 while('\0'!= buff[itmep])
 {
   itmep ++ ;
   if(itmep >256 )
   {return IllegalParam;}
 }
 return sendBuffByIT_Rs232 (buff, itmep,P_NULL,P_NULL);
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName sendBuffByDMA_Rs232 (void *buff, unsigned short length,
                                   SerialPort_SendEndCallBack_Type callBack,
                                   void *callBack_arg )
{
  if(Busy == Rs232Flg)
  {return PortBusy;}
  Rs232Flg = Busy;
  Rs232_SendEnd_CallBack = callBack;
  Rs232_SendEnd_CallBack_arg = callBack_arg;
  RCC->AHBENR |= RCC_AHBPeriph_DMA1;
  DMA1_Channel2->CCR &= (uint32_t)0xFFFF0000;
  DMA1_Channel2->CNDTR = length;
  DMA1_Channel2->CPAR = (u32)(&(USART3->DR));
  DMA1_Channel2->CMAR = (u32)buff;
  DMA1_Channel2->CCR |= DMA_DIR_PeripheralDST | DMA_Mode_Normal | DMA_PeripheralInc_Disable | 
                        DMA_MemoryInc_Enable | DMA_PeripheralDataSize_Byte |DMA_MemoryDataSize_Byte|
                        DMA_Priority_High | DMA_IT_TE | DMA_IT_TC | DMA_CCR1_EN ;
  USART3->CR3 |= USART_DMAReq_Tx;
  return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName sendStringByDMA_Rs232 (unsigned char *buff )
{
 unsigned short itmep = 0;
 while('\0'!= buff[itmep])
 {
   itmep ++ ;
   if(itmep >2048 )
   {return IllegalParam;}
 }
 return sendBuffByDMA_Rs232 (buff, itmep, P_NULL, P_NULL);
}


/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
CommPortNumber_Type get_RS458ChannelNumber ( void )
{
  if(ENABLE == getPowerOut_State (Rs485_PortPV))
  { return Rs485_PortPV; }
  else
  {return Null_Port;}
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void DeInitBuff_Rs485(void)
{
  unsigned char itemp =0;
  while ( itemp < Rs485_Number)
  {
   Rs485_buff[itemp].CCR.Byte = 0; 
   Rs485_buff[itemp].CCR.Bit.idle = 1;
   itemp ++;
  }
  Rs485_RX_P = GetIdleBuff_Rs485 ( );
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
Rs485_BuffStruct* GetIdleBuff_Rs485 (void)
{
  unsigned char itemp =0;
  while ( itemp < Rs485_Number)
  {
    if(1 == Rs485_buff[itemp].CCR.Bit.idle)
    {
     Rs485_buff[itemp].CCR.Bit.idle = 0;
     Rs485_buff[itemp].port = Rs485_PortPV;
     return &(Rs485_buff[itemp]);
    }
    itemp ++;
  }
  return  P_NULL;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
Rs485_BuffStruct* GetDataForBuff_Rs485 (void)
{
  unsigned char itemp =0;
  while ( itemp < Rs485_Number)
  {
    if(1 == Rs485_buff[itemp].CCR.Bit.flg)
    { return &(Rs485_buff[itemp]);}
    itemp ++;
  }
  return  P_NULL;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void FreeBuff_Rs485(Rs485_BuffStruct* buff)
{
  if(P_NULL != buff)
  {
    buff->CCR.Byte = 0; 
    buff->CCR.Bit.idle = 1;
  }
  if(P_NULL == Rs485_RX_P)
  {
    Rs485_RX_P = GetIdleBuff_Rs485 ( );
    Rs485_RX_CNT = 0;
    if(ByDMA == Rs485_ReceiveMod)
    {
      if(P_NULL != Rs485_RX_P)
      {
        DMA1_Channel5->CCR &= (uint32_t)0xFFFF0000;
        DMA1_Channel5->CNDTR = Rs485_BuffLength;
        DMA1_Channel5->CPAR = (u32)(&(USART1->DR));
        DMA1_Channel5->CMAR = (u32)(Rs485_RX_P->data);
        DMA1_Channel5->CCR |= DMA_DIR_PeripheralSRC | DMA_Mode_Normal | DMA_PeripheralInc_Disable | 
                              DMA_MemoryInc_Enable | DMA_PeripheralDataSize_Byte |DMA_MemoryDataSize_Byte|
                              DMA_Priority_High | DMA_IT_TE | DMA_IT_TC | DMA_CCR1_EN ;  
        USART1->CR1 |= (0x00000001 << 4); //IDLE中断使能能 
        USART1->CR3 |= USART_DMAReq_Rx;
      }
    } 
  }
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void TXE_Interrupt_Rs485 (void)
{
  if(Rs485_TX_CNT > 1)
  {
    USART1->DR = *Rs485_TX_P ++;
    Rs485_TX_CNT --;
  } 
  else if(1 ==Rs485_TX_CNT)
  {
   USART_ITConfig(USART1, USART_IT_TXE, DISABLE); 
   USART_ITConfig(USART1, USART_IT_TC, ENABLE);
   USART1->DR = *Rs485_TX_P ++; 
   Rs485_TX_CNT = 0;
  }
  else
  {
   USART_ITConfig(USART1, USART_IT_TXE, DISABLE); 
   USART_ITConfig(USART1, USART_IT_TC, DISABLE);
   ConfigRs485_RX;
   Rs485Flg = Idle;
  }
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void TC_Interrupt_Rs485 (void)
{
  //_485_test = 0;
  USART_ITConfig(USART1, USART_IT_TC, DISABLE);
  ConfigRs485_RX;
  if(P_NULL != Rs485_SendEnd_CallBack)
  {
    Rs485_SendEnd_CallBack(Rs485_PortPV ,Rs485_SendEnd_CallBack_arg);
    Rs485_SendEnd_CallBack = P_NULL;
  }
  Rs485Flg = Idle;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void RXNE_Interrupt_Rs485 (void)
{
  if(P_NULL != Rs485_RX_P)
  {
   if(Rs485_RX_CNT >= Rs485_BuffLength )
   {Rs485_RX_P->CCR.Bit.org = 1;}
   else
   {Rs485_RX_P->data[Rs485_RX_CNT ++] = USART1->DR;}   
  }
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void IDLE_Interrupt_Rs485 (void)
{
  unsigned long  itemp;
  itemp = USART1->DR;   //清除空闲中断 
  if(P_NULL != Rs485_RX_P)
  {
    if(ByIT == Rs485_ReceiveMod)
    {
      Rs485_RX_P->length = Rs485_RX_CNT;
      Rs485_RX_P->CCR.Bit.flg = 1;
    }
    else if(ByDMA == Rs485_ReceiveMod)
    {
      itemp = DMA1_Channel5->CNDTR;
      Rs485_RX_P->length = Rs485_BuffLength - itemp;
      Rs485_RX_P->CCR.Bit.flg = 1;
    }
  }
  Rs485_RX_P = GetIdleBuff_Rs485 ( );
  Rs485_RX_CNT = 0;
  if(ByDMA == Rs485_ReceiveMod)
  {
   if(P_NULL != Rs485_RX_P)
   {
    DMA1_Channel5->CCR &= (uint32_t)0xFFFF0000;
    DMA1_Channel5->CNDTR = Rs485_BuffLength;
    DMA1_Channel5->CPAR = (u32)(&(USART1->DR));
    DMA1_Channel5->CMAR = (u32)(Rs485_RX_P->data);
    DMA1_Channel5->CCR |= DMA_DIR_PeripheralSRC | DMA_Mode_Normal | DMA_PeripheralInc_Disable | 
                        DMA_MemoryInc_Enable | DMA_PeripheralDataSize_Byte |DMA_MemoryDataSize_Byte|
                        DMA_Priority_High | DMA_IT_TE | DMA_IT_TC | DMA_CCR1_EN ; 
   USART1->CR1 |= (0x00000001 << 4); //IDLE中断使能能 
   USART1->CR3 |= USART_DMAReq_Rx;
   }
  else
  {
    DMA1_Channel5->CCR &=  ~(DMA_CCR1_EN);
    USART1->CR3 &= ~(USART_DMAReq_Rx); 
  }
 }
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void ERR_Interrupt_Rs485 (void)
{
 FreeBuff_Rs485(Rs485_RX_P);
 Rs485_RX_P = P_NULL;  
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void receiveByDMA_TC_Interrupt_Rs485 ( void )
{
 FreeBuff_Rs485(Rs485_RX_P);
 Rs485_RX_P = P_NULL; 
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void sendByDMA_TC_Interrupt_Rs485 (void)
{
  //Rs485Flg = Idle;
  //_485_test = 1;
  
  USART_ITConfig(USART1, USART_IT_TC, DISABLE);
  USART_ITConfig(USART1, USART_IT_TXE, DISABLE); 
  USART_ClearITPendingBit(USART1, USART_IT_TC);
  USART_ITConfig(USART1, USART_IT_TC, ENABLE);
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName Launch_Rs485( CommPortNumber_Type Channel, unsigned long BaudRate,
                            SerialPort_Parity  Parity, SerialPort_StopBits StopBits,
                            SerialPort_ReceiveMod   mod )
{
 SystemErrName err = NO_ERR; 
 GPIO_InitTypeDef        GPIO_InitStructure;
 NVIC_InitTypeDef        NVIC_InitStructure;
 USART_InitTypeDef       USART_InitStruct;
 GPIOB->BSRR =  GPIO_Pin_6;
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
 err = PowerConfig_Rs485 (Channel);
 if(NO_ERR != err)
 {return err;}
 
 Rs485_PortPV = Channel;
 DeInitBuff_Rs485( );
 ConfigRs485_RX;
 Rs485Flg = Idle;
 
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
 GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOB, ENABLE);
 USART_DeInit(USART1);
 USART_InitStruct.USART_BaudRate = BaudRate;
 USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
 USART_InitStruct.USART_Mode = USART_Mode_Rx |  USART_Mode_Tx;
 USART_InitStruct.USART_Parity = Parity; 
 USART_InitStruct.USART_StopBits= StopBits;
 if((Parity_Even == Parity)||(Parity_Odd == Parity))
 {USART_InitStruct.USART_WordLength = USART_WordLength_9b;}
 else
 {USART_InitStruct.USART_WordLength = USART_WordLength_8b;}
 USART_Init(USART1, &USART_InitStruct);	 
 NVIC_InitStructure.NVIC_IRQChannel =  USART1_IRQn;
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 NVIC_Init(&NVIC_InitStructure);
 Rs485_ReceiveMod = mod;
 if(ByIT ==  Rs485_ReceiveMod)
 {
   USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
   USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
   Rs485_RX_CNT =0;
 }
 else if(ByDMA ==  Rs485_ReceiveMod)
 { activateReceiveByDMA_Rs485 ( ); }
 USART_Cmd(USART1, ENABLE);
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
 return err; 
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName activateReceiveByDMA_Rs485 ( void )
{
  if(P_NULL == Rs485_RX_P)
  {
    Rs485_RX_P = GetIdleBuff_Rs485 ( );
    if(P_NULL == Rs485_RX_P)
    {return Nothing;}
  }
  RCC->AHBENR |= RCC_AHBPeriph_DMA1;
  DMA1_Channel5->CCR &= (uint32_t)0xFFFF0000;
  DMA1_Channel5->CNDTR = Rs485_BuffLength;
  DMA1_Channel5->CPAR = (u32)(&(USART1->DR));
  DMA1_Channel5->CMAR = (u32)(Rs485_RX_P->data);
  DMA1_Channel5->CCR |= DMA_DIR_PeripheralSRC | DMA_Mode_Normal | DMA_PeripheralInc_Disable | 
                        DMA_MemoryInc_Enable | DMA_PeripheralDataSize_Byte |DMA_MemoryDataSize_Byte|
                        DMA_Priority_High | DMA_IT_TE | DMA_IT_TC | DMA_CCR1_EN ;
  USART_ITConfig(USART1, USART_IT_RXNE, DISABLE); 
  USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
  USART1->CR3 |= USART_DMAReq_Rx;
  Rs485_ReceiveMod = ByDMA;
  return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName PowerConfig_Rs485 (CommPortNumber_Type Channel)
{
  
 switch (Channel)
 {
  case Rs485_PortA : 
    {
      GPIOE->BSRR = GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_6;
      GPIOE->BRR = GPIO_Pin_5;
      break;
    }
  case Rs485_PortB : 
    {
      GPIOE->BSRR = GPIO_Pin_1 | GPIO_Pin_5 | GPIO_Pin_6 ;
      GPIOE->BRR = GPIO_Pin_3;
      break;
    }
  case Rs485_PortC : 
    {
      GPIOE->BSRR = GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_6;
      GPIOE->BRR = GPIO_Pin_1;
      break;
    }
  case Rs485_PortD : 
    {
      GPIOE->BSRR = GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_5;
      GPIOE->BRR = GPIO_Pin_6;
      break;
    } 
  case Null_Port :  
    {
      GPIOE->BSRR = GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_6;
      Rs485_PortPV = Null_Port;
      USART_Cmd(USART1, DISABLE);
      Rs485Flg = Idle;
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, DISABLE);
      break;
    }
  default :
    {return IllegalParam;}
 }
 return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
FunctionalState getPowerRs485_State (CommPortNumber_Type channel)
{
  FunctionalState state;
  switch( channel)
  {
   case Rs485_PortA : 
    {
     if(Rs485_CH3_ENpin)  
     {state = DISABLE ;}
     else
     {state = ENABLE ;}
     break;
    }
   case Rs485_PortB:
    {
     if(Rs485_CH2_ENpin) 
     {state = DISABLE;}
     else
     {state = ENABLE;}
     break;
    }
   case Rs485_PortC: 
    {
     if(Rs485_CH1_ENpin)   
     {state = DISABLE;}
     else
     {state = ENABLE;}
     break;
    }
   case Rs485_PortD :    
    {
     if(Rs485_CH4_ENpin)   
     {state = DISABLE;}
     else
     {state = ENABLE;}
      break;
    }
   case Rs485_ALL :    
    {
     if((Rs485_CH4_ENpin)&&(Rs485_CH2_ENpin)&&(Rs485_PortC)&&(Rs485_PortD))   
     {state = DISABLE;}
     else
     {state = ENABLE;}
      break;
    }
   default:
   {
     state = DISABLE;
     break;
   }
  }
  return state;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName changeChannel_Rs485( CommPortNumber_Type Channel)
{
 SystemErrName err = NO_ERR;
 if(Null_Port == Channel)
 {
  PowerOut_Cmd (Rs485_ALL, DISABLE);   
  err = PowerConfig_Rs485 (Channel);
  if(NO_ERR == err)
  {Rs485_PortPV = Channel;}
 }
 else
 {
  PowerOut_Cmd (Rs485_ALL, DISABLE);    
  PowerOut_Cmd (Channel, ENABLE);  
  if(Null_Port == Rs485_PortPV)
  {
    if(Rs485_PortD == Channel )
    {err = Launch_Rs485(Channel, 115200 , Parity_No, StopBits_1, ByDMA);}
    else
    {err = Launch_Rs485(Channel, 2400, Parity_No, StopBits_1, ByDMA);}
  }
  else if( Rs485_PortPV != Channel)
  {
   if((Rs485_PortD == Channel) 
      &&(Rs485_PortD != Rs485_PortPV))
   {err = Launch_Rs485(Channel, 115200 , Parity_No, StopBits_1, ByDMA);}
   else if(Rs485_PortD == Rs485_PortPV) 
   { err = Launch_Rs485(Channel, 2400, Parity_No, StopBits_1, ByDMA);}
   err = PowerConfig_Rs485 (Channel);
   if(NO_ERR == err)
   {Rs485_PortPV = Channel;}
  } 
 }
 return err;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
CommPortNumber_Type  getChannel_Rs485( void )
{
  return Rs485_PortPV;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName sendBuffByIT_Rs485 (void * buff, unsigned short length,
                                  SerialPort_SendEndCallBack_Type callBack,
                                   void *callBack_arg )
{
  if(Busy == Rs485Flg)
  {return PortBusy;}
  ConfigRs485_TX;
  Rs485Flg = Busy;
  RS485_IdleTime = 0;
  if(length > 0)
  {
    Rs485_SendEnd_CallBack = callBack;
    Rs485_SendEnd_CallBack_arg = callBack_arg;
    Rs485_TX_P = (unsigned char *)buff;
    Rs485_TX_CNT = length - 1;
    USART1->DR = *Rs485_TX_P ++;
    USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
  } 
 return NO_ERR; 
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName sendStringByIT_Rs485 (unsigned char *buff)
{
 unsigned short itmep = 0;
 while('\0'!= buff[itmep])
 {
   itmep ++ ;
   if(itmep >256 )
   {return IllegalParam;}
 }
 return sendBuffByIT_Rs485 (buff, itmep,P_NULL,P_NULL);
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName sendBuffByDMA_Rs485 (void *buff, unsigned short length,
                                   SerialPort_SendEndCallBack_Type callBack,
                                   void *callBack_arg )
{
  if(Busy == Rs485Flg)
  {return PortBusy;}
  else
  {Rs485Flg = Busy;}
  RS485_IdleTime = 0;
  ConfigRs485_TX;
  Rs485_SendEnd_CallBack = callBack;
  Rs485_SendEnd_CallBack_arg = callBack_arg;
  RCC->AHBENR |= RCC_AHBPeriph_DMA1;
  DMA1->IFCR |= 0x0000000f << 12;
  USART1->CR3 &= ~(USART_DMAReq_Tx);
  
  USART_ITConfig(USART1, USART_IT_TC, DISABLE);
  USART1->SR &= ~(0x00000001 << 7);
  
  Rs485_TX_CNT = 0;

  DMA1_Channel4->CCR &= (uint32_t)0xFFFF0000;
  DMA1_Channel4->CNDTR = length;
  DMA1_Channel4->CPAR = (u32)(&(USART1->DR));
  DMA1_Channel4->CMAR = (u32)buff;
  DMA1_Channel4->CCR |= DMA_DIR_PeripheralDST | DMA_Mode_Normal | DMA_PeripheralInc_Disable | 
                        DMA_MemoryInc_Enable | DMA_PeripheralDataSize_Byte |DMA_MemoryDataSize_Byte|
                        DMA_Priority_High | DMA_IT_TE | DMA_IT_TC | DMA_CCR1_EN ;
  USART1->CR3 |= USART_DMAReq_Tx;
  return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName sendStringByDMA_Rs485 (unsigned char *buff )
{
 unsigned short itmep = 0;
 while('\0'!= buff[itmep])
 {
   itmep ++ ;
   if(itmep >2048 )
   {return IllegalParam;}
 }
 return sendBuffByDMA_Rs485 (buff, itmep, P_NULL, P_NULL);
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void DeInitBuff_USART2(void)
{
  unsigned char itemp =0;
  while ( itemp < USART2_Number)
  {
   USART2_buff[itemp].CCR.Byte = 0; 
   USART2_buff[itemp].CCR.Bit.idle = 1;
   itemp ++;
  }
  USART2_RX_P = GetIdleBuff_USART2 ( );
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
USART2_BuffStruct* GetIdleBuff_USART2 (void)
{
  unsigned char itemp =0;
  while ( itemp < USART2_Number)
  {
    if(1 == USART2_buff[itemp].CCR.Bit.idle)
    {
     USART2_buff[itemp].CCR.Bit.idle = 0;
     return &(USART2_buff[itemp]);
    }
    itemp ++;
  }
  return  P_NULL;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
USART2_BuffStruct* GetDataForBuff_USART2 (void)
{
  unsigned char itemp =0;
  while ( itemp < USART2_Number)
  {
     if(1 == USART2_buff[itemp].CCR.Bit.flg)
    { 
      if(0 == USART2_buff[itemp].length)
      {FreeBuff_USART2(&(USART2_buff[itemp]));}
      else
      {return &(USART2_buff[itemp]);}
    }
    itemp ++;
  }
  return  P_NULL;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void FreeBuff_USART2(USART2_BuffStruct* buff)
{
  if(P_NULL != buff)
  {
    buff->CCR.Byte = 0; 
    buff->CCR.Bit.idle = 1;
    buff->length = 0;
  }
  if(P_NULL == USART2_RX_P)
  {
    USART2_RX_P = GetIdleBuff_USART2 ( );
    USART2_RX_CNT = 0;
    if(ByDMA == USART2_ReceiveMod)
    {
      if(P_NULL != USART2_RX_P)
      {
        DMA1_Channel6->CCR &= (uint32_t)0xFFFF0000;
        DMA1_Channel6->CNDTR = USART2_BuffLength;
        DMA1_Channel6->CPAR = (u32)(&(USART2->DR));
        DMA1_Channel6->CMAR = (u32)(USART2_RX_P->data);
        DMA1_Channel6->CCR |= DMA_DIR_PeripheralSRC | DMA_Mode_Normal | DMA_PeripheralInc_Disable | 
                                DMA_MemoryInc_Enable | DMA_PeripheralDataSize_Byte |DMA_MemoryDataSize_Byte|
                                DMA_Priority_High | DMA_IT_TE | DMA_IT_TC | DMA_CCR1_EN ;
        USART2->CR1 |= (0x00000001 << 4); //IDLE中断使能能 
        USART2->CR3 |= USART_DMAReq_Rx;
      }
    } 
  }
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void TXE_Interrupt_USART2 (void)
{
  if(USART2_TX_CNT > 1)
  {
    USART2->DR = *USART2_TX_P ++;
    USART2_TX_CNT --;
  } 
  else if(1 ==USART2_TX_CNT)
  {
   USART_ITConfig(USART2, USART_IT_TXE, DISABLE); 
   USART_ITConfig(USART2, USART_IT_TC, ENABLE);
   USART2->DR = *USART2_TX_P ++; 
   USART2_TX_CNT = 0;
  }
  else
  {
   USART_ITConfig(USART2, USART_IT_TXE, DISABLE); 
   USART_ITConfig(USART2, USART_IT_TC, DISABLE);
   USART2Flg = Idle;
  }
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void TC_Interrupt_USART2 (void)
{
  USART_ITConfig(USART2, USART_IT_TC, DISABLE);
  if(P_NULL != USART2_SendEnd_CallBack)
  {
    USART2_SendEnd_CallBack (USART2_Port,USART2_SendEnd_CallBack_arg);
    USART2_SendEnd_CallBack = P_NULL;
  }
  USART2Flg = Idle;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void RXNE_Interrupt_USART2 (void)
{
  if(P_NULL != USART2_RX_P)
  {
   if(USART2_RX_CNT >= USART2_BuffLength )
   {USART2_RX_P->CCR.Bit.org = 1;}
   else
   {USART2_RX_P->data[USART2_RX_CNT ++] = USART2->DR;}   
  }
  USART2->SR  &= ~0x00000001 << 4;
  USART2->CR1 |= 0x00000001 << 4; 
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void IDLE_Interrupt_USART2 (void)
{
  unsigned long  itemp;
  
  itemp = USART2->DR;   //清除空闲中断 
  if(P_NULL != USART2_RX_P)
  {
    if(ByIT == USART2_ReceiveMod)
    {
      USART2_RX_P->length = USART2_RX_CNT;
      USART2_RX_P->CCR.Bit.flg = 1;
    }
    else if(ByDMA == USART2_ReceiveMod)
    {
      itemp = DMA1_Channel6->CNDTR;
      USART2_RX_P->length = USART2_BuffLength - itemp;
      USART2_RX_P->CCR.Bit.flg = 1;
    }
  }
  USART2_RX_P = GetIdleBuff_USART2 ( );
  USART2_RX_CNT = 0;
  if(ByDMA == USART2_ReceiveMod)
  {
   if(P_NULL != USART2_RX_P)
   {
    DMA1_Channel6->CCR &= (uint32_t)0xFFFF0000;
    DMA1_Channel6->CNDTR = USART2_BuffLength;
    DMA1_Channel6->CPAR = (u32)(&(USART2->DR));
    DMA1_Channel6->CMAR = (u32)(USART2_RX_P->data);
    DMA1_Channel6->CCR |= DMA_DIR_PeripheralSRC | DMA_Mode_Normal | DMA_PeripheralInc_Disable | 
                        DMA_MemoryInc_Enable | DMA_PeripheralDataSize_Byte |DMA_MemoryDataSize_Byte|
                        DMA_Priority_High | DMA_IT_TE | DMA_IT_TC | DMA_CCR1_EN ; 
   USART2->CR1 |= (0x00000001 << 4); //IDLE中断使能能 
   USART2->CR3 |= USART_DMAReq_Rx;
   }
   else
   {
    DMA1_Channel6->CCR &=  ~(DMA_CCR1_EN);
    USART2->CR3 &= ~(USART_DMAReq_Rx); 
   }
  }
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void ERR_Interrupt_USART2 (void)
{
 FreeBuff_USART2(USART2_RX_P);
 USART2_RX_P = P_NULL;  
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void receiveByDMA_TC_Interrupt_USART2 (void)
{
  FreeBuff_USART2(USART2_RX_P);
  USART2_RX_P = P_NULL;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void sendByDMA_TC_Interrupt_USART2 (void)
{
  if(P_NULL != USART2_SendEnd_CallBack)
  {
    USART2_SendEnd_CallBack (USART2_Port,USART2_SendEnd_CallBack_arg);
    USART2_SendEnd_CallBack = P_NULL;
  }
  USART2Flg = Idle;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void Launch_USART2 (unsigned long BaudRate, SerialPort_Parity  Parity, 
                     SerialPort_StopBits StopBits, SerialPort_ReceiveMod mod )
{
 GPIO_InitTypeDef        GPIO_InitStructure;
 NVIC_InitTypeDef        NVIC_InitStructure;
 USART_InitTypeDef       USART_InitStruct;
 DeInitBuff_USART2( );
 USART2Flg = Idle ;
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOA, &GPIO_InitStructure);
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 GPIO_Init(GPIOA, &GPIO_InitStructure);
 USART_DeInit (USART2); 
 USART_InitStruct.USART_BaudRate = BaudRate;
 USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
 USART_InitStruct.USART_Mode = USART_Mode_Rx |  USART_Mode_Tx;
 USART_InitStruct.USART_Parity = Parity; 
 USART_InitStruct.USART_StopBits= StopBits;
 if((Parity_Even == Parity)||(Parity_Odd == Parity))
 {USART_InitStruct.USART_WordLength = USART_WordLength_9b;}
 else
 {USART_InitStruct.USART_WordLength = USART_WordLength_8b;}
 USART_Init(USART2, &USART_InitStruct);
 USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
 NVIC_InitStructure.NVIC_IRQChannel =  USART2_IRQn;
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 NVIC_Init(&NVIC_InitStructure);
 USART2_ReceiveMod = mod;
 if(ByIT ==  USART2_ReceiveMod)
 {
   USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
   USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
   USART2_RX_CNT =0;
 }
 else if(ByDMA ==  USART2_ReceiveMod)
 { activateReceiveByDMA_USART2 ( ); }
 USART_Cmd(USART2, ENABLE);
 USART_ClearFlag(USART2, USART_FLAG_TC);
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName activateReceiveByDMA_USART2 ( void )
{
  if(P_NULL == USART2_RX_P)
  {
    USART2_RX_P = GetIdleBuff_USART2 ( );
    if(P_NULL == USART2_RX_P)
    {return Nothing;}
  }
  RCC->AHBENR |= RCC_AHBPeriph_DMA1;
  DMA1_Channel6->CCR &= (uint32_t)0xFFFF0000;
  DMA1_Channel6->CNDTR = USART2_BuffLength;
  DMA1_Channel6->CPAR = (u32)(&(USART2->DR));
  DMA1_Channel6->CMAR = (u32)(USART2_RX_P->data);
  DMA1_Channel6->CCR |= DMA_DIR_PeripheralSRC | DMA_Mode_Normal | DMA_PeripheralInc_Disable | 
                        DMA_MemoryInc_Enable | DMA_PeripheralDataSize_Byte |DMA_MemoryDataSize_Byte|
                        DMA_Priority_High | DMA_IT_TE | DMA_IT_TC | DMA_CCR1_EN ;
  USART_ITConfig(USART2, USART_IT_RXNE, DISABLE); 
  USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
  USART2->CR3 |= USART_DMAReq_Rx;
  USART2_ReceiveMod = ByDMA;
  return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName sendBuffByIT_USART2 (void * buff, unsigned short length,
                                   SerialPort_SendEndCallBack_Type callBack,
                                   void *callBack_arg)

{
  if(Busy == USART2Flg)
  {return PortBusy;}
  USART2Flg = Busy;
  if(length > 0)
  {
    USART2_SendEnd_CallBack = callBack;
    USART2_SendEnd_CallBack_arg = callBack_arg;
    USART2_TX_P = (unsigned char *)buff;
    USART2_TX_CNT = length - 1;
    USART2->DR = *USART2_TX_P ++;
    USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
  } 
 return NO_ERR; 
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName sendStringByIT_USART2 (unsigned char *buff)
{
 unsigned short itmep = 0;
 while('\0'!= buff[itmep])
 {
   itmep ++ ;
   if(itmep >256 )
   {return IllegalParam;}
 }
 return sendBuffByIT_USART2 (buff, itmep, P_NULL ,P_NULL);
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName sendBuffByDMA_USART2 (void *buff, unsigned short length,
                                    SerialPort_SendEndCallBack_Type callBack,
                                    void *callBack_arg)
{
  if(Busy == USART2Flg)
  {return PortBusy;}
  USART2Flg = Busy;
  USART2_SendEnd_CallBack = callBack;
  USART2_SendEnd_CallBack_arg = callBack_arg;
  RCC->AHBENR |= RCC_AHBPeriph_DMA1;
  DMA1_Channel7->CCR &= (uint32_t)0xFFFF0000;
  DMA1_Channel7->CNDTR = length;
  DMA1_Channel7->CPAR = (u32)(&(USART2->DR));
  DMA1_Channel7->CMAR = (u32)buff;
  DMA1_Channel7->CCR |= DMA_DIR_PeripheralDST | DMA_Mode_Normal | DMA_PeripheralInc_Disable | 
                        DMA_MemoryInc_Enable | DMA_PeripheralDataSize_Byte |DMA_MemoryDataSize_Byte|
                        DMA_Priority_High | DMA_IT_TE | DMA_IT_TC | DMA_CCR1_EN ;
  USART2->CR3 |= USART_DMAReq_Tx;
  return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName sendStringByDMA_USART2 (unsigned char *buff )
{
 unsigned short itmep = 0;
 while('\0'!= buff[itmep])
 {
   itmep ++ ;
   if(itmep >2048 )
   {return IllegalParam;}
 }
 return sendBuffByDMA_USART2 (buff, itmep, P_NULL, P_NULL);
}
