
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
#include "bsp.h"
/*********************************************************************************
常量定义区
*********************************************************************************/

//长量定义
#ifndef IIC_TimeOut
#define IIC_TimeOut  255
#endif

/*********************************************************************************
公共变量定义区
*********************************************************************************/

/*********************************************************************************
外部变量声明区
*********************************************************************************/

/*********************************************************************************
私有变量定义区
*********************************************************************************/

/*********************************************************************************
测试变量定义区
*********************************************************************************/
 
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
void Launch_24CXX (void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC , ENABLE);

  GPIO_ResetBits(GPIOA, GPIO_Pin_12);
  GPIO_SetBits(GPIOA, GPIO_Pin_9);			
  
   /* 配置PA4为Flash的片选  */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_12 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
  
  dely_ms (100);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
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
void OFF_Launch_24CXX (void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_SetBits(GPIOA, GPIO_Pin_12);			//禁止Flash的片选
  dely_ms (100);			//禁止Flash的片选
  GPIO_ResetBits(GPIOC, GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12);//禁止Flash的片选
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed =GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  GPIO_ResetBits(GPIOA, GPIO_Pin_9);
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
static void BusyCheck_24CXX(void)
{
 //u8 busy_status = 1;
// while (busy_status != 0)
// {

  //busy_status = WriteReadByte_W25QXX(Dummy) & 0x01;

 //}
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
void ReadEeprom_24CXX(unsigned long address ,unsigned char *readbuff,unsigned int datalen)
{
 //u16 tempcont;
 //Flash_CSL();
 //WriteReadByte_W25QXX(0x0B);
 //WriteReadByte_W25QXX((u8)((address & 0xffffff ) >> 16));
 //WriteReadByte_W25QXX((u8)((address & 0xffff) >> 8));
 //WriteReadByte_W25QXX((u8)(address & 0xff));
 //WriteReadByte_W25QXX(Dummy);	
 //for(tempcont = 0; tempcont < datalen; tempcont++ )
 //{*readbuff++ = WriteReadByte_W25QXX(Dummy);}	
 //Flash_CSH();
 //Flash_Busy_check();
}
/*********************************************************************************
 Function:      //
 Description:   //Flash write ,max datalen == 256byte one time* 
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void WriteEeprom_24CXX(unsigned long address ,unsigned char *writebuff,unsigned short datalen)
{
// u16 tempcont;
// //Flash_CSL();
// WriteReadByte_W25QXX(0x06);//写SR ENABLE
// //Flash_CSH();
// __NOP( );__NOP( );
// ///Flash_Busy_check();
// //Flash_CSL();
// WriteReadByte_W25QXX(0x02);
// WriteReadByte_W25QXX((u8)((address & 0xffffff ) >> 16));
// WriteReadByte_W25QXX((u8)((address & 0xffff) >> 8));
// WriteReadByte_W25QXX((u8)(address & 0xff));	
// for(tempcont = 0; tempcont < datalen; tempcont++)
// {
//  WriteReadByte_W25QXX(*writebuff++);
// }
// //Flash_CSH();
// //Flash_Busy_check();
// ///Flash_CSL();
// WriteReadByte_W25QXX(0x04);//写SR DISABLE
// //Flash_CSH();
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
void PageErase_24CXX(unsigned char CMD ,unsigned long address)
{

//
// //Flash_CSL();
// WriteReadByte_W25QXX(0x06);//写SR ENABLE
// //Flash_CSH();
// __NOP( );	__NOP( );
//
// //Flash_Busy_check();//查忙 
//
//
// //Flash_CSL();
// WriteReadByte_W25QXX(CMD);
// WriteReadByte_W25QXX((u8)((address & 0xffffff ) >> 16));
// WriteReadByte_W25QXX((u8)((address & 0xffff) >> 8));
// WriteReadByte_W25QXX((u8)(address & 0xff));	
// //Flash_CSH();
// //Flash_Busy_check();//查忙
// //Flash_CSL();
// WriteReadByte_W25QXX(0x04);//写SR DISABLE
// //Flash_CSH();
}
/*********************************************************************************
函数名：		   
函数描述：		   
入口参数描述：	   
                   
				  
出口参数描述：
函数返回值描述：
其他说明： 
*********************************************************************************/
static void Delay ( unsigned long temp )
{
 while (temp -- );
}
/*********************************************************************************
函数名：		   
函数描述：	打开写使能	   
入口参数描述：	   
                   
				  
出口参数描述：
函数返回值描述：
其他说明： 
*********************************************************************************/
void WriteEnable_24CXX( void )
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
void WriteDisable_24CXX( void )
{
 
}
/*********************************************************************************
函数名：		   
函数描述：	等待空闲 
入口参数描述：	   
                   			  
出口参数描述：
函数返回值描述：
其他说明： 
*********************************************************************************/
unsigned char WaitBusy_24CXX ( void )
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
void PowerDown_24CXX( void )
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
void ReleasePowerDown_24CXX( void )
{
}
/*********************************************************************************
函数名：		   
函数描述：	整片擦出
入口参数描述：	   
                   
				  
出口参数描述：
函数返回值描述：
其他说明： 
*********************************************************************************/
void ChipErase_24CXX( void )
{
}
/*********************************************************************************
函数名：		   
函数描述：	擦除扇区
入口参数描述：Dst_adr : 扇区号	   
                   
				  
出口参数描述：
函数返回值描述：
其他说明： 
*********************************************************************************/
void SectorErase_24CXX( unsigned long Dst_adr )
{


}
/*********************************************************************************
函数名：		   
函数描述:    在一页内写入指定长度数据 （不多于256个字节）	   
入口参数描述：adr : 写入的目标地址	   
              pbuff ：写入数据数据存储缓冲区
	      NumByte ：要写入数据数据字节数     
				  
出口参数描述：
函数返回值描述：
其他说明： 
*********************************************************************************/
void WritePage_24CXX( unsigned long adr, unsigned char * pbuff, unsigned char NumByte )
{
}
/*********************************************************************************
函数名：		   
函数描述：     在从指定地址开始写入指定长度数据具有自动换页功能	   
入口参数描述： adr : 写入的目标地址	   
               pbuff ：写入数据数据存储缓冲区
	       NumByte ：要写入数据数据字节数     
				  
出口参数描述：
函数返回值描述：
其他说明：
          
*********************************************************************************/
void WriteData_24CXX( unsigned long adr, unsigned char * pbuff, unsigned short NumByte )
{
// unsigned short pagermain ;
// pagermain = 256 - (adr % 256);	 //单页剩余字节数
// if(NumByte <= pagermain)
// {pagermain = NumByte;}
// while(1)
// {
//  //WritePage_W25Q ( adr , pbuff , pagermain);
//  Flash_Write(adr ,pbuff,pagermain);
//
////  ReadData_W25Q(adr , W25Q_buf_tem, 256 );
//  if(pagermain == NumByte)
//  {break;} //写完
//  else
//  {
//   adr +=  pagermain;
//   pbuff += pagermain; 
//   NumByte -= pagermain;
//   if(NumByte > 256)
//   {pagermain = 256;} //剩余字节数大于一页可写整页
//   else
//   {pagermain = NumByte;} 
//   Delay (0xfff);
//  }
// }
}
/***********************************END**********************************************/


