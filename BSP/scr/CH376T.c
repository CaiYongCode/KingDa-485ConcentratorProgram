
/*********************************************************************************
//������: Operation for SI446x

//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include "..\inc\CH376T.h"  
#include "..\inc\bsp.h"
/*********************************************************************************
����������
*********************************************************************************/

#define CH376T_CSH()     set_SPI3_NSS2pin  
#define CH376T_CSL()	 clr_SPI3_NSS2pin  

#define	EndCH376Cmd( )   CH376T_CSH()  

/*********************************************************************************
��������������
*********************************************************************************/

/*********************************************************************************
�ⲿ����������
*********************************************************************************/

/*********************************************************************************
˽�б���������
*********************************************************************************/
/*********************************************************************************
���Ա���������
*********************************************************************************/

/*********************************************************************************
�ڲ��������Ͷ�����
*********************************************************************************/
/*********************************************************************************
�ڲ�����������
*********************************************************************************/
/*********************************************************************************
���ܴ��붨����
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
  
   /* ����PA4ΪFlash��Ƭѡ  */
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
 Description:   //  ��CH376д���� 
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
 WriteReadByte_CH376T( mCmd );   //���������� 
 DelayuS(50);
}
/*********************************************************************************
 Function:      //
 Description:   // ��CH376д����
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
 Description:   //  ��CH376������
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
 Description:   // /��ʼ��CH376 
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
   WriteCH376Cmd( CMD11_CHECK_EXIST );  //���Ե�Ƭ����CH376֮���ͨѶ�ӿ� 
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
  
   WriteCH376Cmd( CMD0H_DISK_CONNECT ); //�������Ƿ�����
   EndCH376Cmd( );

  return 1;
}
/*********************************************************************************
 Function:      //
 Description:   // ��ȡ�ж�״̬����λ�ж�����
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
