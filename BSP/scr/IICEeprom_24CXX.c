
/*********************************************************************************
//概述：: Operation for SI446x

//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "..\inc\IICEeprom_24CXX.h"  
#include "..\inc\bsp.h"
#include "..\..\Projects\inc\SofTimer.h"
/*********************************************************************************
常量定义区
*********************************************************************************/
DriverState_type  EepromState = OffState;
/*********************************************************************************
公共变量定义区
*********************************************************************************/
 
/*********************************************************************************
外部变量声明区
*********************************************************************************/

/*********************************************************************************
私有变量定义区
*********************************************************************************/
static SofTimer_Struct*   EepromTimer = P_NULL;
static unsigned char     I2C_SlaverAddress = 0;
/*********************************************************************************
测试变量定义区
*********************************************************************************/
 
/*********************************************************************************
内部数据类型定义区
*********************************************************************************/

/*********************************************************************************
内部函数定义区
*********************************************************************************/
static void EepromTimer_CallBack(void *p_tmr, void *p_arg);
static SystemErrName EEPROM_I2C_WaitEepromStandbyState(void); //EEPROM设为待命状态
//void EEPROM_I2C_PageWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite);//写少于一页的数据
/*********************************************************************************
功能代码定义区
*********************************************************************************/
static void Delay ( unsigned long temp )
{
 while (temp -- );
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
SystemErrName  Launch_24CXX (unsigned char DriverAddr)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  I2C_SlaverAddress = DriverAddr;
  EepromTimer =  SofTimerCreate (60,     //周期
                                 TimerTB_1S,              //时基
                                 SofTimer_ONE_SHOT,          //模式
                                 EepromTimer_CallBack, //回调
                                 P_NULL);       //回调参数
  if(P_NULL == EepromTimer)
  {return Nothing;}
  I2C2_Configuration( );
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD , ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIOD->BRR =  GPIO_Pin_10; 
  Delay (0x4fff);
  EepromState = RunState;
 
  SofTimerStart(EepromTimer); 
  
  return NO_ERR;
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
void OFF_24CXX (void)
{
 GPIO_InitTypeDef GPIO_InitStructure; 
  
 EepromState = OffState; 
 SofTimerDel(&EepromTimer);
 EepromTimer = P_NULL;

 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
 GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 GPIO_Init(GPIOB, &GPIO_InitStructure);

 RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2 , DISABLE);
 
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD , ENABLE);
 GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_Init(GPIOD, &GPIO_InitStructure);
 GPIOD->BSRR =  GPIO_Pin_10; 
 
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
void  EepromPort_rset( unsigned char DriverAddr) 
{
  OFF_24CXX ();
  Launch_24CXX (DriverAddr);
}
/*******************************************************************************
* Function Name  : I2C_EE_BufferWrite
* Description    : 将缓冲器的数据写入EEPROM
* Input          : - pBuffer : 指向要写入数据数组的指针 
*                  - WriteAddr : EEPROM中要写入数据的首地址.
*                  - NumByteToWrite : 写入的字节数.
* Output         : None
* Return         : None
pBuffer：
WriteAddr：
NumByteToWrite：写入的字节数
*******************************************************************************/
SystemErrName EEPROM_I2C_BufferWrite(void* Buffer, short WriteAddr, short NumByteToWrite)//将缓冲器的数据写入EEPROM
{
 SystemErrName err = NO_ERR ; 
 unsigned char  NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
 unsigned char* pBuffer; 
 
 Addr = WriteAddr % EEPROM_PageSize;//写入地址是每页的第几位
 count = EEPROM_PageSize - Addr;//在开始的一页要写入的个数
 pBuffer = Buffer;
 
 if(Addr + NumByteToWrite < EEPROM_PageSize )   // 数据不跨页
  { err = EEPROM_I2C_PageWrite(pBuffer, WriteAddr, NumByteToWrite);}//写少于一页的数据
 else//数据不跨页
  {
   NumByteToWrite -= count;
   NumOfPage = NumByteToWrite / EEPROM_PageSize; //计算要写入的页数
   NumOfSingle = NumByteToWrite % EEPROM_PageSize;//重新计算不足一页的个数
   if(count != 0)//
   { 
    err = EEPROM_I2C_PageWrite(pBuffer, WriteAddr, count);//将开始的空间写满一页
    if(NO_ERR != err){return err;}
    WriteAddr += count;
    pBuffer += count;
   } 
   while(NumOfPage--)//要写入的页数
   {
    err = EEPROM_I2C_PageWrite(pBuffer, WriteAddr, EEPROM_PageSize);//写一页的数据
    if(NO_ERR != err){return err;}
    WriteAddr += EEPROM_PageSize;
    pBuffer += EEPROM_PageSize; 
   }
   if(NumOfSingle != 0)//剩余数据小于一页
   {err = EEPROM_I2C_PageWrite(pBuffer, WriteAddr, NumOfSingle);  }//写少于一页的数据
  }
 return err;
}
/*******************************************************************************
* Function Name : I2C_EE_ByteWrite
* Description    : Writes one byte to the I2C EEPROM.
* Input          : - data : 要写入的数据
*                  - WriteAddr : EEPROM中要写入数据的首地址.
* Output         : None
* Return         : None
*******************************************************************************/
SystemErrName EEPROM_I2C_ByteWrite(unsigned char data, unsigned char WriteAddr)//写一个字节到EEPROM
{
 SystemErrName err; 
 unsigned long itemp = 0;
 
 err =  EEPROM_I2C_WaitEepromStandbyState();//EEPROM设为待命状态
 if(NO_ERR!= err){return err;} 

 I2C_GenerateSTART(I2C2, ENABLE);//产生 I2Cx传输 START条件

 while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)) //检查最近一次 I2C事件是否是输入的事件
 {if(itemp++ > 0x4ffff){return Unsuccessful;}}
  itemp = 0; 

 I2C_Send7bitAddress(I2C2, I2C_SlaverAddress, I2C_Direction_Transmitter);//向指定的从 I2C设备传送地址字,选择发送方向

 while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))//检查最近一次 I2C事件是否是输入的事件
 {if(itemp++ > 0x4ffff){return Unsuccessful;}}
  itemp = 0;       

 I2C_SendData(I2C2, WriteAddr);//通过外设 I2Cx发送地址

 while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))//检查最近一次 I2C事件是否是输入的事件
 {if(itemp++ > 0x4ffff){return Unsuccessful;}}
  itemp = 0; 

 I2C_SendData(I2C2, data); //通过外设 I2Cx发送数据 /* Send the byte to be written */
   
 while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))//检查最近一次 I2C事件是否是输入的事件
 {if(itemp++ > 0x4ffff){return Unsuccessful;}}//  itemp = 0; 

 I2C_GenerateSTOP(I2C2, ENABLE);//产生 I2Cx传输 STOP条件
 return NO_ERR;
}
/*******************************************************************************
* Function Name : I2C_EE_PageWrite
* Description    : Writes more than one byte to the EEPROM with a single WRITE
*                  cycle. The number of byte can't exceed the EEPROM page size.
* Input          : - pBuffer : pointer to the buffer containing the data to be 
*                    written to the EEPROM.
*                  - WriteAddr : EEPROM's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the EEPROM.
* Output         : None
* Return         : None
pBuffer：指向要写入数据数组的指针
WriteAddr：24c02中要写入数据的首地址
NumByteToWrite：写入的字节数
*******************************************************************************/
SystemErrName EEPROM_I2C_PageWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite)//写少于一页的数据
{
 SystemErrName err; 
 unsigned long itemp = 0;
 
 err =  EEPROM_I2C_WaitEepromStandbyState();//EEPROM设为待命状态
 if(NO_ERR!= err){return err;} 

 I2C_GenerateSTART(I2C2, ENABLE);//产生 I2Cx传输 START条件
 
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)) //检查最近一次 I2C事件是否是输入的事件
 {if(itemp++ > 0x4ffff){return Unsuccessful;}}
  itemp = 0;  

  I2C_Send7bitAddress(I2C2, I2C_SlaverAddress, I2C_Direction_Transmitter);//向指定的从 I2C设备传送地址字,选择发送方向

  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))//检查最近一次 I2C事件是否是输入的事件
 {if(itemp++ > 0x4ffff){return Unsuccessful;}}
  itemp = 0;

  I2C_SendData(I2C2, WriteAddr); //通过外设 I2Cx发送地址

  while(! I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))//检查最近一次 I2C事件是否是输入的事件
 {if(itemp++ > 0x4ffff){return Unsuccessful;}}
  itemp = 0;  

  while(NumByteToWrite--) 
  {
    I2C_SendData(I2C2, *pBuffer); //通过外设 I2Cx发送数据
    pBuffer++; 
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))//检查最近一次 I2C事件是否是输入的事件
   {if(itemp++ > 0x4ffff){return Unsuccessful;}}
    itemp = 0; 
  }
  I2C_GenerateSTOP(I2C2, ENABLE);//产生 I2Cx传输 STOP条件
  return NO_ERR;
}
/*******************************************************************************
* Function Name : I2C_EE_BufferRead
* Description    : Reads a block of data from the EEPROM.
* Input          : - pBuffer : pointer to the buffer that receives the data read 
*                    from the EEPROM.
*                  - ReadAddr : EEPROM's internal address to read from.
*                  - NumByteToRead : number of bytes to read from the EEPROM.
* Output         : None
* Return         : None
pBuffer：指向要保存读出数据的数组的指针
ReadAddr：24c02中要读出数据的首地址
NumByteToRead：读出的字节数
*******************************************************************************/
SystemErrName EEPROM_I2C_BufferRead(void* Buffer, u8 ReadAddr, u16 NumByteToRead)//将EEPROM的数据读入缓冲器
{ 
 SystemErrName err; 
 unsigned long itemp = 0;
 unsigned char* pBuffer;
 pBuffer = Buffer;
 err =  EEPROM_I2C_WaitEepromStandbyState();//EEPROM设为待命状态
 if(NO_ERR!= err){return err;}

  I2C_GenerateSTART(I2C2, ENABLE);//产生 I2Cx传输 START条件  /* Send START condition */

  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))//检查最近一次 I2C事件是否是输入的事件 /* Test on EV5 and clear it */
  {if(itemp++ > 0x4fff){return Unsuccessful;}}
  itemp = 0;
  if(NumByteToRead==1)  
  {I2C_AcknowledgeConfig(I2C2, DISABLE);}//使能或者失能指定 I2C的应答功能 /* In the case of a single data transfer disable ACK before reading the data */
  
  I2C_Send7bitAddress(I2C2, I2C_SlaverAddress, I2C_Direction_Transmitter);//向指定的从 I2C设备传送地址字,选择发送方向 

  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))//检查最近一次 I2C事件是否是输入的事件  /* Test on EV6 and clear it */
  {if(itemp++ > 0x4fff){return Unsuccessful;}}
  itemp = 0; 
  //I2C_Cmd(I2C2, ENABLE);//使能或者失能 I2C外设  /* Clear EV6 by setting again the PE bit */

  I2C_SendData(I2C2, ReadAddr); //通过外设 I2Cx发送地址  /* Send the EEPROM's internal address to write to */
 
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))//检查最近一次 I2C事件是否是输入的事件/* Test on EV8 and clear it */
  {if(itemp++ > 0x4fff){return Unsuccessful;}}
  itemp = 0;
  I2C_GenerateSTART(I2C2, ENABLE);//产生 I2Cx传输 START条件/* Send STRAT condition a second time */ 

  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))//检查最近一次 I2C事件是否是输入的事件/* Test on EV5 and clear it */
  {if(itemp++ > 0x4fff){return Unsuccessful;}}
  itemp = 0;
  
  I2C_Send7bitAddress(I2C2, I2C_SlaverAddress, I2C_Direction_Receiver);//向指定的从 I2C设备传送地址字,选择接收方向 /* Send EEPROM address for read */

  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))//检查最近一次 I2C事件是否是输入的事件 /* Test on EV6 and clear it */
   {if(itemp++ > 0x4fff){return Unsuccessful;}}
  itemp = 0;
  while(NumByteToRead) 
  {
    if(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED)) //检查最近一次 I2C事件是否是输入的事件  /* Test on EV7 and clear it */
    {
      itemp = 0; 
      if(NumByteToRead == 2)
      {I2C_AcknowledgeConfig(I2C2, DISABLE);}//使能或者失能指定 I2C的应答功能
      if(NumByteToRead == 1)
      {I2C_GenerateSTOP(I2C2, ENABLE);}//产生 I2Cx传输 STOP条件
      *pBuffer = I2C_ReceiveData(I2C2);//返回通过 I2Cx最近接收的数据
      pBuffer++; 
      NumByteToRead--;    
   }
   if(itemp++ > 0x4ffff){return Unsuccessful;}
  }
 I2C_AcknowledgeConfig(I2C2, ENABLE);//使能或者失能指定 I2C的应答功能
 return NO_ERR;
}
/*******************************************************************************
* Function Name : I2C_EE_WaitEepromStandbyState
* Description    : Wait for EEPROM Standby state
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
SystemErrName EEPROM_I2C_WaitEepromStandbyState(void) //EEPROM设为待命状态     
{
 vu16 SR1_Tmp = 0;
 unsigned long itemp = 0 ;
 unsigned long itemp1 = 0 ;
 
 if(RunState != EepromState)
 { 
  SofTimerDel(&EepromTimer);
  Launch_24CXX ( EEPROM_ADDRESS);
 }
 if(NO_ERR != SofTimerRset(EepromTimer))
 {Launch_24CXX ( EEPROM_ADDRESS);}
 while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY))
 {if(itemp++ > 0x4fff){return Unsuccessful;}}
 itemp = 0;
 do
 {
  I2C_GenerateSTART(I2C2, ENABLE);//产生 I2Cx传输 START条件
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)) //检查最近一次 I2C事件是否是输入的事件
  {if(itemp++ > 0x4fff){return Unsuccessful;}}
  itemp = 0;
  I2C_Send7bitAddress(I2C2, I2C_SlaverAddress, I2C_Direction_Transmitter);//向指定的从 I2C设备传送地址字 ,选择发送方向
  Delay ( 0x4ff );
  I2C_ClearFlag(I2C2, I2C_FLAG_AF);//清除 I2Cx的应答错误标志位
  SR1_Tmp = I2C_ReadRegister(I2C2, I2C_Register_SR1);//读取指定的 I2C寄存器 I2C_SR1 并返回其值
  if(itemp1++ > 0x4fff){I2C2_Rset( ); return Unsuccessful;}
 }while(!(I2C_ReadRegister(I2C2, I2C_Register_SR1) & 0x0002));//地址发送结束
 I2C_ClearFlag(I2C2, I2C_FLAG_AF);//清除 I2Cx的应答错误标志位
 
 I2C_GenerateSTOP(I2C2, ENABLE);//产生 I2Cx传输 STOP条件
 while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY))
 { 
  if(itemp++ > 0x4fff)
  { return Unsuccessful;}
 }
 return NO_ERR;
}
/*********************************************************************************
函数名：		   
函数描述：	   
入口参数描述：	   
                   
				  
出口参数描述：
函数返回值描述：
其他说明： 
*********************************************************************************/
SystemErrName EEPROM_I2C_MultipleRead(void* Buffer, unsigned short ReadAddr, unsigned short NumByteToRead)
{
  SystemErrName err = NO_ERR; 
  unsigned char itemp = 0 ;
  while (itemp++ < 3)
  {
   err = EEPROM_I2C_BufferRead(Buffer,  ReadAddr, NumByteToRead);
   if(NO_ERR != err)
   { EepromPort_rset(EEPROM_ADDRESS) ;}
   else
   {break;} 
  }
  return err;
}
/*********************************************************************************
函数名：		   
函数描述：	
入口参数描述：	   
                   
				  
出口参数描述：
函数返回值描述：
其他说明： 
*********************************************************************************/
SystemErrName EEPROM_I2C_MultipleWrite(void* Buffer, unsigned short WriteAddr, unsigned short NumByteToWrite)
{
  SystemErrName err = NO_ERR; 
  unsigned char itemp = 0 ;
  while (itemp++ < 3)
  {
   err = EEPROM_I2C_BufferWrite(Buffer, WriteAddr, NumByteToWrite);//将缓冲器的数据写入EEPROM
   if(NO_ERR != err)
   { EepromPort_rset(EEPROM_ADDRESS) ;}
   else
   {break;} 
  }
  return err;
}
/*********************************************************************************
函数名：		   
函数描述：	打开写使能	   
入口参数描述：	   
                   
				  
出口参数描述：
函数返回值描述：
其他说明： 
*********************************************************************************/
void EEPROM_I2C_EEPROM_WriteEnable( void )
{

}
/*********************************************************************************
函数名：		   
函数描述：	关闭写使能	   
入口参数描述：	   
                   
				  
出口参数描述：
函数返回值描述：
其他说明： 
*********************************************************************************/
void EEPROM_I2C_EEPROM_WriteDisable( void )
{

}
/*********************************************************************************
函数名：		   
函数描述：	使芯片进入掉电模式
入口参数描述：	   
                   
				  
出口参数描述：
函数返回值描述：
其他说明： 
*********************************************************************************/
void EEPROM_I2C_PowerDown( void )
{

}
/*********************************************************************************
函数名：		   
函数描述：	唤醒芯片
入口参数描述：	   
                   
				  
出口参数描述：
函数返回值描述：
其他说明： 
*********************************************************************************/
void EEPROM_I2C_ReleasePowerDown( void )
{

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
void EepromTimer_CallBack(void *p_tmr, void *p_arg)
{
  OFF_24CXX ( );
}
/***********************************END**********************************************/


