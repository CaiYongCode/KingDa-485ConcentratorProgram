
/*********************************************************************************
//概述：: Operation for SI446x

//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "W25QXX.h"  
#include "bsp.h"
/*********************************************************************************
常量定义区
*********************************************************************************/

#define Flash_CSH()       set_SPI3_NSS1pin  //GPIO_SetBits( GPIOA, GPIO_Pin_4)
#define Flash_CSL()	  clr_SPI3_NSS1pin  //GPIO_ResetBits(GPIOA, GPIO_Pin_4)

//长量定义
#ifndef W25QXX_OK
#define W25QXX_OK  1
#endif

#ifndef W25QXX_NG
#define W25QXX_NG  0
#endif

#ifndef W25QXX_TimeOut
#define W25QXX_TimeOut  255
#endif

#ifndef W25QXX_Free
#define W25QXX_Free  0
#endif

#ifndef W25QXX_Busy
#define W25QXX_Busy  1
#endif

const unsigned char Dummy = 0;
/*********************************************************************************
公共变量定义区
*********************************************************************************/

/*********************************************************************************
外部变量声明区
*********************************************************************************/

/*********************************************************************************
私有变量定义区
*********************************************************************************/
//static unsigned char  W25Q_buf[4096]; 
/*********************************************************************************
测试变量定义区
*********************************************************************************/
//static unsigned char W25Q_buf_tem[256]; 
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
void Launch_W25QXX (void)
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
  
  DelymS (100);
  
  GPIO_SetBits(GPIOC, GPIO_Pin_10 | GPIO_Pin_12);
  
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
void OFF_W25QXX (void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_SetBits(GPIOA, GPIO_Pin_12);			//禁止Flash的片选
  DelymS (100);			//禁止Flash的片选
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
 unsigned char WriteReadByte_W25QXX ( unsigned char dt)
{
  unsigned char itemp = 0;
  unsigned char data = 0;
  GPIOC->BRR = 0x00000001 << 10;
  while (itemp++ < 8)
  {
    if(dt & 0x80)
    { GPIOC->BSRR = 0x00000001 << 12;}
    else
    {GPIOC->BRR = 0x00000001 << 12;}
    dt <<= 1;
     GPIOC->BSRR = 0x00000001 << 10;
    data <<= 1;
    if(GPIOC->IDR &= (0x00000001 << 11))
    {data |= 0x01;} 
    GPIOC->BRR = 0x00000001 << 10;
  }
  GPIOC->BRR = 0x00000001 << 10;
  return data;
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
static void Flash_Busy_check(void)
{
 u8 busy_status = 1;
 while (busy_status != 0)
 {
  Flash_CSL();
  WriteReadByte_W25QXX(0x05);//写SR0
  busy_status = WriteReadByte_W25QXX(Dummy) & 0x01;
  Flash_CSH();
 }
} 
/*********************************************************************************
 Function:      //
 Description:   //设置SR, 指定FLASH读写的规则  保护SR,memory 的规则 
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void Flash_SR_Init(void )
{
 Flash_CSL();
 WriteReadByte_W25QXX(0x06);//写SR ENABLE
 Flash_CSH();
 Flash_Busy_check();
 __NOP( );__NOP( );
 Flash_CSL();
 WriteReadByte_W25QXX(0x11);//SR3-->WPS = 0  other default
 WriteReadByte_W25QXX(0x60);
 Flash_CSH();
 __NOP( );	__NOP( );
 Flash_CSL();
 WriteReadByte_W25QXX(0x04);//写SR DISABLE
 Flash_CSH();
 Flash_Busy_check();
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
void Flash_Read(unsigned long address ,unsigned char *readbuff,unsigned int datalen)
{
 u16 tempcont;
 Flash_CSL();
 WriteReadByte_W25QXX(0x0B);
 WriteReadByte_W25QXX((u8)((address & 0xffffff ) >> 16));
 WriteReadByte_W25QXX((u8)((address & 0xffff) >> 8));
 WriteReadByte_W25QXX((u8)(address & 0xff));
 WriteReadByte_W25QXX(Dummy);	
 for(tempcont = 0; tempcont < datalen; tempcont++ )
 {*readbuff++ = WriteReadByte_W25QXX(Dummy);}	
 Flash_CSH();
 Flash_Busy_check();
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
void Flash_Write(unsigned long address ,unsigned char *writebuff,unsigned short datalen)
{
 u16 tempcont;
 Flash_CSL();
 WriteReadByte_W25QXX(0x06);//写SR ENABLE
 Flash_CSH();
 __NOP( );__NOP( );
 Flash_Busy_check();
 Flash_CSL();
 WriteReadByte_W25QXX(0x02);
 WriteReadByte_W25QXX((u8)((address & 0xffffff ) >> 16));
 WriteReadByte_W25QXX((u8)((address & 0xffff) >> 8));
 WriteReadByte_W25QXX((u8)(address & 0xff));	
 for(tempcont = 0; tempcont < datalen; tempcont++)
 {
  WriteReadByte_W25QXX(*writebuff++);
 }
 /////Flash_CSH();
 Flash_Busy_check();
 Flash_CSL();
 WriteReadByte_W25QXX(0x04);//写SR DISABLE
 Flash_CSH();
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
void Page_Erase(unsigned char CMD ,unsigned long address)
{


 Flash_CSL();
 WriteReadByte_W25QXX(0x06);//写SR ENABLE
 Flash_CSH();
 __NOP( );	__NOP( );

 Flash_Busy_check();//查忙 


 Flash_CSL();
 WriteReadByte_W25QXX(CMD);
 WriteReadByte_W25QXX((u8)((address & 0xffffff ) >> 16));
 WriteReadByte_W25QXX((u8)((address & 0xffff) >> 8));
 WriteReadByte_W25QXX((u8)(address & 0xff));	
 Flash_CSH();
 Flash_Busy_check();//查忙
 Flash_CSL();
 WriteReadByte_W25QXX(0x04);//写SR DISABLE
 Flash_CSH();
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
void Flash_ReadID(unsigned char *readbuff)
{
 Flash_CSL();
 WriteReadByte_W25QXX(0x90);
 WriteReadByte_W25QXX(0);
 WriteReadByte_W25QXX(0);
 WriteReadByte_W25QXX(0);
 *readbuff++ = WriteReadByte_W25QXX(Dummy);
 *readbuff++ = WriteReadByte_W25QXX(Dummy);
 Flash_CSH();	
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
void Flash_readSR(unsigned char *readbuff)
{
 Flash_CSL();
 WriteReadByte_W25QXX(0x05);//写SR0
 *readbuff++ = WriteReadByte_W25QXX(Dummy);
 Flash_CSH();
 Flash_CSL();
 WriteReadByte_W25QXX(0x35);//写SR0
 *readbuff++ = WriteReadByte_W25QXX(Dummy);	
 Flash_CSH();
 Flash_CSL();
 WriteReadByte_W25QXX(0x15);//写SR0
 *readbuff++ = WriteReadByte_W25QXX(Dummy);
 Flash_CSH();
}
/**********************************************
函数名：		   
函数描述：		   
入口参数描述：	   
                   
				  
出口参数描述：
函数返回值描述：
其他说明： 
***********************************************/
static void Delay ( unsigned long temp )
{
 while (temp -- );
}
/**********************************************
函数名：		   
函数描述：	读状态寄存器	   
入口参数描述：	   
                   
				  
出口参数描述：
函数返回值描述：
其他说明： 
***********************************************/
unsigned char  ReadStatusReg_W25Q( void )
{
 unsigned char itemp;
 Flash_CSL();		
  WriteReadByte_W25QXX( 0x05 );   //发送命令
 itemp =  WriteReadByte_W25QXX( 0xff );		 //读取数据
 Flash_CSH();
 return itemp;
}
/**********************************************
函数名：		   
函数描述：	写状态寄存器	   
入口参数描述：	   
                   
				  
出口参数描述：
函数返回值描述：
其他说明： 
***********************************************/  
void WriteStatusReg_W25Q( unsigned char SR )
{
 Flash_CSL();
 WriteReadByte_W25QXX( 0x01 );   //发送命令
 WriteReadByte_W25QXX( SR );		   //读取数据
 Flash_CSH();
}
/**********************************************
函数名：		   
函数描述：	打开写使能	   
入口参数描述：	   
                   
				  
出口参数描述：
函数返回值描述：
其他说明： 
***********************************************/
void WriteEnable_W25Q( void )
{
 Flash_CSL();
 WriteReadByte_W25QXX( 0x06 );   //发送命令
 Flash_CSH();
}
/**********************************************
函数名：		   
函数描述：	关闭写使能	   
入口参数描述：	   
                   
				  
出口参数描述：
函数返回值描述：
其他说明： 
***********************************************/
void WriteDisable_W25Q( void )
{
 Flash_CSL();
 WriteReadByte_W25QXX( 0x04 );   //发送命令
 Flash_CSH();
}
/**********************************************
函数名：		   
函数描述：	读取芯片ID  
入口参数描述：	   
                   
				  
出口参数描述：
函数返回值描述：
其他说明： 
***********************************************/
unsigned short ReadDeviceID_W25Q( void )
{
 unsigned short itemp;
 Flash_CSL();
 WriteReadByte_W25QXX( 0x90 );   //发送命令
 WriteReadByte_W25QXX( 0x00 );   //发送地址
 WriteReadByte_W25QXX( 0x00 );   //发送地址
 WriteReadByte_W25QXX( 0x00 );   //发送地址
 itemp =  WriteReadByte_W25QXX( 0xff );   //
 itemp <<= 8;
 itemp |=  WriteReadByte_W25QXX( 0xff );   //
 Flash_CSH();
 return itemp;
}
/**********************************************
函数名：		   
函数描述：	等待空闲 
入口参数描述：	   
                   
				  
出口参数描述：
函数返回值描述：
其他说明： 
***********************************************/
unsigned char WaitBusy_W25Q ( void )
{
 unsigned long itemp = 0;
 unsigned char itemp2 ;
 itemp2 =  ReadStatusReg_W25Q( );
 itemp2 &= 0x01;
 while( 0x00 != itemp2)
 {
  itemp2 =  ReadStatusReg_W25Q( );
  itemp2 &= 0x01;
  Delay ( 0xfff);
  itemp ++ ;
  if(itemp > 0xffff)
  {return W25QXX_TimeOut;}
 }
 return W25QXX_OK;
}
/**********************************************
函数名：		   
函数描述：	使芯片进入掉电模式
入口参数描述：	   
                   
				  
出口参数描述：
函数返回值描述：
其他说明： 
***********************************************/
void PowerDown_W25Q( void )
{
 Flash_CSL();
 WriteReadByte_W25QXX( W25X_PowerDown_CMD );   //发送命令
 Flash_CSH();
 Delay(0xff);    //等待TPD
}
/**********************************************
函数名：		   
函数描述：	唤醒芯片
入口参数描述：	   
                   
				  
出口参数描述：
函数返回值描述：
其他说明： 
***********************************************/
void ReleasePowerDown_W25Q( void )
{
 Flash_CSL();
  WriteReadByte_W25QXX( W25X_ReleasePowerDown_CMD );   //发送命令
 Flash_CSH();
 Delay(0x4ffff);    //等待TPD
}
/**********************************************
函数名：		   
函数描述：	整片擦出
入口参数描述：	   
                   
				  
出口参数描述：
函数返回值描述：
其他说明： 
***********************************************/
void ChipErase_W25Q( void )
{
 WriteEnable_W25Q( );	  //打开写使能 WriteEnable_W25Q
 WaitBusy_W25Q ( ); 	  //查忙

 Flash_CSL();;

  WriteReadByte_W25QXX( 0xC7 );   //发送命令

 Flash_CSH();;
}
/**********************************************
函数名：		   
函数描述：	擦除扇区
入口参数描述：Dst_adr : 扇区号	   
                   
				  
出口参数描述：
函数返回值描述：
其他说明： 
***********************************************/
void SectorErase_W25Q( unsigned long Dst_adr )
{
 
 Dst_adr <<= 12;

 WriteEnable_W25Q( );	  //打开写使能
 WaitBusy_W25Q ( ); 	  //查忙

 Flash_CSL();;

  WriteReadByte_W25QXX( 0x20 );   //发送命令
  WriteReadByte_W25QXX( Dst_adr >> 16 );   //发送地址
  WriteReadByte_W25QXX( Dst_adr >> 8 );   //发送地址
  WriteReadByte_W25QXX( Dst_adr  );   //发送地址

 Flash_CSH();;

}
/**********************************************
函数名：		   
函数描述：	在指定地址读取指定长度数据	   
入口参数描述：adr : 读取的目标地址	   
              pbuff ：数据存储缓冲区
			  NumByte ：要读取数据的字节数     
				  
出口参数描述：
函数返回值描述：
其他说明： 
***********************************************/
void ReadData_W25Q( unsigned long adr, unsigned char * pbuff, unsigned short NumByte )
{
  Flash_Read(adr ,pbuff,NumByte);

// unsigned short itemp ; 
//
// WaitBusy_W25Q ( ); 	  //查忙
//
// Flash_CSL();;
// WriteReadByte_W25QXX( 0x03 );   //发送命令
// WriteReadByte_W25QXX( adr >> 16 );   //发送地址
// WriteReadByte_W25QXX( adr >> 8 );   //发送地址
// WriteReadByte_W25QXX( adr  );   //发送地址
// itemp  = 0;
// while ( itemp < NumByte )
// {
//  pbuff[ itemp ++ ] = WriteReadByte_W25QXX( 0xff );
// }
// Flash_CSH();;
}
/**********************************************
函数名：		   
函数描述：	读出整个指定扇区指定扇区数据	   
入口参数描述：secpos : 要读取的目标扇区编号	   
   				  
出口参数描述：
函数返回值描述：读出数据的指针
其他说明： 
***********************************************/
//unsigned char * Read_OneSec_W25X( unsigned short  secpos )
//{
// unsigned char * buf;
// ReadData_W25X(secpos << 12 , W25X_buf , 4096 );  //读出整个扇区
// buf = W25X_buf;
// return buf;
//}
/**********************************************
函数名：		   
函数描述：	在一页内写入指定长度数据 （不多于256个字节）	   
入口参数描述：adr : 写入的目标地址	   
              pbuff ：写入数据数据存储缓冲区
			  NumByte ：要写入数据数据字节数     
				  
出口参数描述：
函数返回值描述：
其他说明： 
***********************************************/
void WritePage_W25Q( unsigned long adr, unsigned char * pbuff, unsigned char NumByte )
{
 unsigned char itemp; 
 
 WriteEnable_W25Q( );	  //打开写使能
 WaitBusy_W25Q ( ); 	  //查忙

 Flash_CSL();
 WriteReadByte_W25QXX( 0x02 );   //发送命令
 WriteReadByte_W25QXX( adr >> 16 );   //发送地址
 WriteReadByte_W25QXX( adr >> 8 );   //发送地址
 WriteReadByte_W25QXX( adr  );   //发送地址
 itemp  = 0;
 while ( itemp < NumByte )
 {
  WriteReadByte_W25QXX( pbuff[ itemp ++ ] );
 }
 Flash_CSH();
}
/**********************************************
函数名：		   
函数描述：在从指定地址开始写入指定长度数据
          具有自动换页功能	   
入口参数描述：adr : 写入的目标地址	   
              pbuff ：写入数据数据存储缓冲区
			  NumByte ：要写入数据数据字节数     
				  
出口参数描述：
函数返回值描述：
其他说明：必须确保指定地址的数据全部是Oxff否则在
          非0xff处写入失败 
***********************************************/
void Writedata_NoCheck_W25Q( unsigned long adr, unsigned char * pbuff, unsigned short NumByte )
{
 unsigned short pagermain ;
 pagermain = 256 - (adr % 256);	 //单页剩余字节数
 if(NumByte <= pagermain)
 {pagermain = NumByte;}
 while(1)
 {
  //WritePage_W25Q ( adr , pbuff , pagermain);
  Flash_Write(adr ,pbuff,pagermain);

//  ReadData_W25Q(adr , W25Q_buf_tem, 256 );
  if(pagermain == NumByte)
  {break;} //写完
  else
  {
   adr +=  pagermain;
   pbuff += pagermain; 
   NumByte -= pagermain;
   if(NumByte > 256)
   {pagermain = 256;} //剩余字节数大于一页可写整页
   else
   {pagermain = NumByte;} 
   Delay (0xfff);
  }
 }
}
/**********************************************
函数名：		   
函数描述：在从指定地址开始写入指定长度数据
          具有自动换页功能	   
入口参数描述：adr : 写入的目标地址	   
              pbuff ：写入数据数据存储缓冲区
			  NumByte ：要写入数据数据字节数     
				  
出口参数描述：
函数返回值描述：
其他说明：基于扇区	先擦再写
***********************************************/
//void Writedata_Check_Sector_W25Q( unsigned long adr, unsigned char * pbuff, unsigned long NumByte )
//{
// unsigned short itemp;
// unsigned short secpos;     //扇区地址
// unsigned short secoff;     //扇区内偏移
// unsigned short secremain;  //扇区剩余字节数
//
// secpos = adr >> 12;
// secoff = adr % 4096;
// secremain = 4096 - secoff;
// 
// if (NumByte <= secremain)
// {secremain = NumByte;}
// while(1)
// {
//  ReadData_W25Q(secpos << 12 , W25Q_buf , 4096 );  //读出整个扇区
//  itemp = secoff;
//  while (itemp < (secoff + secremain))
//  {
//   if(0xff != W25Q_buf[itemp ++])
//   {break;}
//  }
//  if( itemp < (secoff + secremain) )  //需要擦出整个扇区
//  {
//   Page_Erase(SECTOR_ERASE_4K ,secpos << 12);
//   itemp = 0;
//   while (itemp < secremain)  //复制数据
//   {
//     W25Q_buf[itemp + secoff] = pbuff [itemp];
//	 itemp ++ ;
//   }
//   Writedata_NoCheck_W25Q( secpos << 12, W25Q_buf, 4096 ); //写入整个扇区
//  }
//  else //无需擦除
//  { Writedata_NoCheck_W25Q( adr, pbuff, secremain );} 
//  if( NumByte == secremain )
//  {break;} //写入完毕
//
//  pbuff += secremain;
//  NumByte -= secremain;
//  adr += secremain;
//
//  secpos++;
//  secoff = 0;
//  if(NumByte >4096 )
//  {secremain  = 4096;}
//  else
//  {secremain  = NumByte;}
// }
//}
//
//
//
//
