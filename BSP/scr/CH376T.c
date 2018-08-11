
/*********************************************************************************
//概述：: Operation for SI446x

//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "..\inc\CH376T.h"  
#include "..\inc\bsp.h"
/*********************************************************************************
常量定义区
*********************************************************************************/

#define CH376T_CSH()     set_SPI3_NSS2pin  
#define CH376T_CSL()	 clr_SPI3_NSS2pin  

#define	EndCH376Cmd( )   CH376T_CSH()  

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
void Launch_CH376T (void)
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
 Return:	//
 Others:        //
*********************************************************************************/
/*unsigned char WriteReadByte_CH376T ( unsigned char TxData)
{		 
 while((SPI2->SR&1<<1)==0)//
 {}			  
 SPI2->DR=TxData;	 	  
 while((SPI2->SR&1<<0)==0) 
 {}	  						    
 return SPI2->DR;        			    
}*/


unsigned char WriteReadByte_CH376T ( unsigned char TxData)
{
  unsigned char itemp = 0;
  unsigned char data = 0;
  GPIOC->BSRR = 0x00000001 << 10;

  while (itemp++ < 8)
  {
   GPIOC->BRR = 0x00000001 << 10;  
   if(TxData & 0x80)
   {GPIOC->BSRR = 0x00000001 << 12;}
   else
   {GPIOC->BRR = 0x00000001 << 12;}
   TxData <<= 1;
   GPIOC->BSRR = 0x00000001 << 10;
   data <<= 1;
   if(GPIOC->IDR &= (0x00000001 << 11))
   {data |= 0x01;}
  }
  GPIOC->BSRR = 0x00000001 << 10;
  GPIOC->BSRR = 0x00000001 << 12;
  return data;
} 
/*********************************************************************************
 Function:      //
 Description:   //  向CH376写命令 
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
void WriteCH376Cmd( UINT8 mCmd )  
{
 CH376T_CSH(); 
 DelayuS(20);
 CH376T_CSL();    
 WriteReadByte_CH376T( mCmd );   //发出命令码 
 DelayuS(50);
}
/*********************************************************************************
 Function:      //
 Description:   // 向CH376写数据
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void WriteCH376Data( UINT8 mData )  
{
 WriteReadByte_CH376T( mData );
}
/*********************************************************************************
 Function:      //
 Description:   //  从CH376读数据
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
UINT8 ReadCH376Data( void ) 
{
 return( WriteReadByte_CH376T( 0xFF ) );
}
/*********************************************************************************
 Function:      //
 Description:   // /初始化CH376 
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
UINT8 Init_CH376Host( void )  
{
  UINT8 res ;
  UINT8 cn = 0; 
  do
  {
   WriteCH376Cmd( CMD11_CHECK_EXIST );  //测试单片机与CH376之间的通讯接口 
   WriteCH376Data( 0x55 );
   res = ReadCH376Data( );
   EndCH376Cmd( );
   if((0xAA) == res )
   {break;}
   else if(cn > 10)
   {return 0;}
   cn ++;
   DelymS (10);
  }while (1);
  
  WriteCH376Cmd( CMD11_SET_USB_MODE );
  WriteCH376Data( 0x06 ); 
  DelayuS( 20 );
  res = ReadCH376Data( );
  EndCH376Cmd( );
  if ( res != CMD_RET_SUCCESS )  
  {return 0; }
  
   WriteCH376Cmd( CMD0H_DISK_CONNECT ); //检查磁盘是否连接
   EndCH376Cmd( );

  return 1;
}
/*********************************************************************************
 Function:      //
 Description:   // 获取中断状态并复位中断请求
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
UINT8 GetIntStatus_CH376( void )
{
  UINT8	s;	
  WriteCH376Cmd( CMD01_GET_STATUS );
  s = ReadCH376Data( );
  EndCH376Cmd( );	
  return( s);
}
