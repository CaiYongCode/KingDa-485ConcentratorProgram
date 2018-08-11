/*********************************************************************************
//概述：: Operation for SI446x

//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "PCF8563.h"  
#include "bsp.h"
/*********************************************************************************
常量定义区
*********************************************************************************/
/*********************************************************************************
公共变量定义区
*********************************************************************************/

/*********************************************************************************
外部变量声明区
*********************************************************************************/
TimerData_Struct  SystemTimer;


/*********************************************************************************
私有变量定义区
**********************************************************************************/ 
/*********************************************************************************
测试变量定义区
*********************************************************************************/
//unsigned  short  ioiiiii =0;
/*********************************************************************************
内部数据类型定义区
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
static void delay_us(unsigned int us)
{while (us--);}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
unsigned char I2C_8563_ByteWrite(unsigned char WriteAddr,unsigned char pBuffer)
{
  unsigned long itemp = 0;
  /* Send STRAT condition */
  I2C_GenerateSTART(I2C1, ENABLE);
  /* Test on EV5 and clear it */
  itemp = 0;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
  {
   itemp ++;
   if(itemp > 0x4ff)		//超时
   {return 0;}
  }  
  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C1, PCF8563ADDR_WRITE, I2C_Direction_Transmitter);
  /* Test on EV6 and clear it */
  itemp = 0;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
   itemp ++;
   if(itemp > 0x4ff)		//超时
   {return 0;}
  }     
  /* Send the EEPROM's internal address to write to */
  I2C_SendData(I2C1, WriteAddr);
  /* Test on EV8 and clear it */
  itemp = 0;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    itemp ++;
    if(itemp > 0x4ff)		//超时
    {return 0;}
  }  
  /* Send the byte to be written */
  I2C_SendData(I2C1, pBuffer); 
  /* Test on EV8 and clear it */
  itemp = 0;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    itemp ++;
    if(itemp > 0x4ff)		//超时
    {return 0;}
  }  
  /* Send STOP condition */
  I2C_GenerateSTOP(I2C1, ENABLE);
  return 1;
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
unsigned char I2C_8563_byteRead(unsigned char ReadAddr)
{ 

  unsigned long itemp = 0; 
  unsigned char readbyte = 0;
   /* While the bus is busy */
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
  {itemp ++;if(itemp > 0x4ff){I2C1_Rset( ); break;}}		//超时
  I2C_GenerateSTART(I2C1, ENABLE);
  itemp = 0;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
  {itemp ++;if(itemp > 0x4ff){I2C1_Rset( ); break;}} 
  I2C_Send7bitAddress(I2C1, PCF8563ADDR_WRITE, I2C_Direction_Transmitter);
  itemp = 0;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {itemp ++;if(itemp > 0x4ff){I2C1_Rset( );  break;} }
  /* Clear EV6 by setting again the PE bit */
  I2C_Cmd(I2C1, ENABLE);
  /* Send the EEPROM's internal address to write to */
  I2C_SendData(I2C1, ReadAddr);  
  /* Test on EV8 and clear it */
  itemp = 0;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {itemp ++;if(itemp > 0x4ff){I2C1_Rset( ); break;}}		
  /* Send STRAT condition a second time */  
  I2C_GenerateSTART(I2C1, ENABLE);
  /* Test on EV5 and clear it */
  itemp = 0;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
  {itemp ++;if(itemp > 0x4ff){I2C1_Rset( ); break;}}
  /* Send EEPROM address for read */
  I2C_Send7bitAddress(I2C1, PCF8563ADDR_READ, I2C_Direction_Receiver);
  /* Test on EV6 and clear it */
  itemp = 0;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
  {itemp ++;if(itemp > 0x4ff){I2C1_Rset( ); break;}}
  /* Test on EV7 and clear it */
  itemp = 0;		
  /* Disable Acknowledgement */
  I2C_AcknowledgeConfig(I2C1, DISABLE);	
  /* Send STOP Condition */
  I2C_GenerateSTOP(I2C1, ENABLE);			
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
  {itemp ++;if(itemp > 0x4ff){I2C1_Rset( ); break;}}
  readbyte = I2C_ReceiveData(I2C1);
  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(I2C1, ENABLE);
  return readbyte;
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
void PCF8563_Set_Time(TimerData_Struct* timer)
{
 unsigned char itemp1;
 unsigned char itemp2;
 if(P_NULL == timer){return;}
 if(timer->Year > 1999)
 {
  if(timer->Year < 2099){itemp1 = timer->Year - 2000;}
  else{return;}
 }
 else if(timer->Year > 1900){itemp1 = timer->Year - 1900;}
 else{return;}
  
 if((timer->Month < 1)||(timer->Month > 12)){return;}
 if((timer->Day < 1)||( timer->Day > 31 )){return;}
 if(timer->Hour > 23){return;}
 if(timer->Minute > 59){return;} 
 if(timer->Second > 59){return;}  

 itemp2 = itemp1 / 10;
 itemp2 <<=4 ;
 itemp2 |=itemp1 % 10;
 I2C_8563_ByteWrite(YEAR, itemp2);

 itemp1 = timer->Month;
 itemp2 = itemp1 / 10;
 itemp2 <<=4 ;
 itemp2 |=itemp1 % 10;
 if(timer->Year < 2000){itemp2|= 0x80;}
 I2C_8563_ByteWrite(MONTH, itemp2); 
 
 itemp1 = timer->Day;
 itemp2 = itemp1 / 10;
 itemp2 <<=4 ;
 itemp2 |=itemp1 % 10; 
 I2C_8563_ByteWrite(DAY, itemp2);//
 
 I2C_8563_ByteWrite(WEEK, timer->WeekDay);//

 itemp1 = timer->Hour;
 itemp2 = itemp1 / 10;
 itemp2 <<=4 ;
 itemp2 |=itemp1 % 10; 
 I2C_8563_ByteWrite(HOUR, itemp2);//

 itemp1 = timer->Minute;
 itemp2 = itemp1 / 10;
 itemp2 <<=4 ;
 itemp2 |=itemp1 % 10; 
 I2C_8563_ByteWrite(MINUTE, itemp2);//

 itemp1 = timer->Second;
 itemp2 = itemp1 / 10;
 itemp2 <<=4 ;
 itemp2 |=itemp1 % 10;  
 I2C_8563_ByteWrite(SECOND, itemp2);//
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
void PCF8563_Read_time(TimerData_Struct* timer)
{
  unsigned char itemp1;
  unsigned char itemp2;
  unsigned char flg;
  
  itemp1 = I2C_8563_byteRead(MONTH);//
  flg = itemp1 & 0x80;
  itemp1 &= 0x1F; 
  itemp2 = itemp1 & 0x0F;
  itemp1 >>= 4;
  itemp2 += itemp1 * 10;
  timer->Month = itemp2 ;
  
  itemp1 =  I2C_8563_byteRead(YEAR);  ////
  itemp2 = itemp1 & 0x0F;
  itemp1 >>= 4;
  itemp2 += itemp1 * 10;
  if(flg){timer->Year = itemp2 + 1900;}
  else{timer->Year = itemp2 + 2000;}
  
  itemp1 = (I2C_8563_byteRead(DAY) & 0x3F);//
  itemp2 = itemp1 & 0x0F;
  itemp1 >>= 4;
  itemp2 += itemp1 * 10;
  timer->Day = itemp2 ;
  
  timer->WeekDay = (I2C_8563_byteRead(WEEK) & 0x03);//
  
  itemp1 = (I2C_8563_byteRead(HOUR) & 0x3F);//
  itemp2 = itemp1 & 0x0F;
  itemp1 >>= 4;
  itemp2 += itemp1 * 10;
  timer->Hour = itemp2 ;
  
  itemp1 = (I2C_8563_byteRead(MINUTE)& 0x7F);//
  itemp2 = itemp1 & 0x0F;
  itemp1 >>= 4;
  itemp2 += itemp1 * 10;
  timer->Minute = itemp2 ;
  
  delay_us(20);
  itemp1 = (I2C_8563_byteRead(SECOND)& 0x7F);//
  itemp2 = itemp1 & 0x0F;
  itemp1 >>= 4;
  itemp2 += itemp1 * 10;
  timer->Second = itemp2 ;
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
void F8563_woring_mode(unsigned char mod)
{
 mod &= WARING_DEFINE_TIME; 
 mod |= 0x80;   //脉冲中断
 I2C_8563_ByteWrite(CONTROL2,mod);  //
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
void F8563_worning_init(u8 hour,u8 minute)  ///   
{
 I2C_8563_ByteWrite(WAR_MINUTE,minute);
 I2C_8563_ByteWrite(WAR_HOUR,hour);      
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
void F8563_back_init(unsigned char time, unsigned char TD)  ///  
{
  TD &= 0x03;
  TD |= 0x80; //倒计时有效
  I2C_8563_ByteWrite(DTIME_CONTROL,TD);   //0x80  频率设置 1/60Hz  单位为分钟
  I2C_8563_ByteWrite(TIME_CONT,time);   		// 	
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
void F8563_Interrupt(void)  // 
{
  unsigned char flg;
  flg = I2C_8563_byteRead( CONTROL2 );	//
  I2C_8563_ByteWrite(CONTROL2,flg & 0x03); //清除中断标志
  if(flg & 0x04)  //定时中断
  {PCF8563_Read_time(&SystemTimer);}
  if(flg & 0x08)  //报警中断
  { flg = 0;}
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
void get_SystemTimer(TimerData_Struct* timer)
{
  uint32_t tmp = 0;
  tmp = (uint32_t)EXTI_BASE;
   *(__IO uint32_t *) tmp &= EXTI_Line4;   //关闭外部中断 
   timer->Second = SystemTimer.Second;
   timer->Minute = SystemTimer.Minute;
   timer->Hour = SystemTimer.Hour;
   timer->Day = SystemTimer.Day;
   timer->Month = SystemTimer.Month;
   timer->Year = SystemTimer.Year;                  
   *(__IO uint32_t *) tmp |= EXTI_Line4;   //打开外部中断
}
