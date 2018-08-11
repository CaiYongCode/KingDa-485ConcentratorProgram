
/*********************************************************************************
//������: Operation for SI446x

//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include "W25QXX.h"  
#include "bsp.h"
/*********************************************************************************
����������
*********************************************************************************/

#define Flash_CSH()       set_SPI3_NSS1pin  //GPIO_SetBits( GPIOA, GPIO_Pin_4)
#define Flash_CSL()	  clr_SPI3_NSS1pin  //GPIO_ResetBits(GPIOA, GPIO_Pin_4)

//��������
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
��������������
*********************************************************************************/

/*********************************************************************************
�ⲿ����������
*********************************************************************************/

/*********************************************************************************
˽�б���������
*********************************************************************************/
//static unsigned char  W25Q_buf[4096]; 
/*********************************************************************************
���Ա���������
*********************************************************************************/
//static unsigned char W25Q_buf_tem[256]; 
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
void Launch_W25QXX (void)
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
 Return:		//
 Others:        //
*********************************************************************************/
void OFF_W25QXX (void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_SetBits(GPIOA, GPIO_Pin_12);			//��ֹFlash��Ƭѡ
  DelymS (100);			//��ֹFlash��Ƭѡ
  GPIO_ResetBits(GPIOC, GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12);//��ֹFlash��Ƭѡ
  
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
  WriteReadByte_W25QXX(0x05);//дSR0
  busy_status = WriteReadByte_W25QXX(Dummy) & 0x01;
  Flash_CSH();
 }
} 
/*********************************************************************************
 Function:      //
 Description:   //����SR, ָ��FLASH��д�Ĺ���  ����SR,memory �Ĺ��� 
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void Flash_SR_Init(void )
{
 Flash_CSL();
 WriteReadByte_W25QXX(0x06);//дSR ENABLE
 Flash_CSH();
 Flash_Busy_check();
 __NOP( );__NOP( );
 Flash_CSL();
 WriteReadByte_W25QXX(0x11);//SR3-->WPS = 0  other default
 WriteReadByte_W25QXX(0x60);
 Flash_CSH();
 __NOP( );	__NOP( );
 Flash_CSL();
 WriteReadByte_W25QXX(0x04);//дSR DISABLE
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
 WriteReadByte_W25QXX(0x06);//дSR ENABLE
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
 WriteReadByte_W25QXX(0x04);//дSR DISABLE
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
 WriteReadByte_W25QXX(0x06);//дSR ENABLE
 Flash_CSH();
 __NOP( );	__NOP( );

 Flash_Busy_check();//��æ 


 Flash_CSL();
 WriteReadByte_W25QXX(CMD);
 WriteReadByte_W25QXX((u8)((address & 0xffffff ) >> 16));
 WriteReadByte_W25QXX((u8)((address & 0xffff) >> 8));
 WriteReadByte_W25QXX((u8)(address & 0xff));	
 Flash_CSH();
 Flash_Busy_check();//��æ
 Flash_CSL();
 WriteReadByte_W25QXX(0x04);//дSR DISABLE
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
 WriteReadByte_W25QXX(0x05);//дSR0
 *readbuff++ = WriteReadByte_W25QXX(Dummy);
 Flash_CSH();
 Flash_CSL();
 WriteReadByte_W25QXX(0x35);//дSR0
 *readbuff++ = WriteReadByte_W25QXX(Dummy);	
 Flash_CSH();
 Flash_CSL();
 WriteReadByte_W25QXX(0x15);//дSR0
 *readbuff++ = WriteReadByte_W25QXX(Dummy);
 Flash_CSH();
}
/**********************************************
��������		   
����������		   
��ڲ���������	   
                   
				  
���ڲ���������
��������ֵ������
����˵���� 
***********************************************/
static void Delay ( unsigned long temp )
{
 while (temp -- );
}
/**********************************************
��������		   
����������	��״̬�Ĵ���	   
��ڲ���������	   
                   
				  
���ڲ���������
��������ֵ������
����˵���� 
***********************************************/
unsigned char  ReadStatusReg_W25Q( void )
{
 unsigned char itemp;
 Flash_CSL();		
  WriteReadByte_W25QXX( 0x05 );   //��������
 itemp =  WriteReadByte_W25QXX( 0xff );		 //��ȡ����
 Flash_CSH();
 return itemp;
}
/**********************************************
��������		   
����������	д״̬�Ĵ���	   
��ڲ���������	   
                   
				  
���ڲ���������
��������ֵ������
����˵���� 
***********************************************/  
void WriteStatusReg_W25Q( unsigned char SR )
{
 Flash_CSL();
 WriteReadByte_W25QXX( 0x01 );   //��������
 WriteReadByte_W25QXX( SR );		   //��ȡ����
 Flash_CSH();
}
/**********************************************
��������		   
����������	��дʹ��	   
��ڲ���������	   
                   
				  
���ڲ���������
��������ֵ������
����˵���� 
***********************************************/
void WriteEnable_W25Q( void )
{
 Flash_CSL();
 WriteReadByte_W25QXX( 0x06 );   //��������
 Flash_CSH();
}
/**********************************************
��������		   
����������	�ر�дʹ��	   
��ڲ���������	   
                   
				  
���ڲ���������
��������ֵ������
����˵���� 
***********************************************/
void WriteDisable_W25Q( void )
{
 Flash_CSL();
 WriteReadByte_W25QXX( 0x04 );   //��������
 Flash_CSH();
}
/**********************************************
��������		   
����������	��ȡоƬID  
��ڲ���������	   
                   
				  
���ڲ���������
��������ֵ������
����˵���� 
***********************************************/
unsigned short ReadDeviceID_W25Q( void )
{
 unsigned short itemp;
 Flash_CSL();
 WriteReadByte_W25QXX( 0x90 );   //��������
 WriteReadByte_W25QXX( 0x00 );   //���͵�ַ
 WriteReadByte_W25QXX( 0x00 );   //���͵�ַ
 WriteReadByte_W25QXX( 0x00 );   //���͵�ַ
 itemp =  WriteReadByte_W25QXX( 0xff );   //
 itemp <<= 8;
 itemp |=  WriteReadByte_W25QXX( 0xff );   //
 Flash_CSH();
 return itemp;
}
/**********************************************
��������		   
����������	�ȴ����� 
��ڲ���������	   
                   
				  
���ڲ���������
��������ֵ������
����˵���� 
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
��������		   
����������	ʹоƬ�������ģʽ
��ڲ���������	   
                   
				  
���ڲ���������
��������ֵ������
����˵���� 
***********************************************/
void PowerDown_W25Q( void )
{
 Flash_CSL();
 WriteReadByte_W25QXX( W25X_PowerDown_CMD );   //��������
 Flash_CSH();
 Delay(0xff);    //�ȴ�TPD
}
/**********************************************
��������		   
����������	����оƬ
��ڲ���������	   
                   
				  
���ڲ���������
��������ֵ������
����˵���� 
***********************************************/
void ReleasePowerDown_W25Q( void )
{
 Flash_CSL();
  WriteReadByte_W25QXX( W25X_ReleasePowerDown_CMD );   //��������
 Flash_CSH();
 Delay(0x4ffff);    //�ȴ�TPD
}
/**********************************************
��������		   
����������	��Ƭ����
��ڲ���������	   
                   
				  
���ڲ���������
��������ֵ������
����˵���� 
***********************************************/
void ChipErase_W25Q( void )
{
 WriteEnable_W25Q( );	  //��дʹ�� WriteEnable_W25Q
 WaitBusy_W25Q ( ); 	  //��æ

 Flash_CSL();;

  WriteReadByte_W25QXX( 0xC7 );   //��������

 Flash_CSH();;
}
/**********************************************
��������		   
����������	��������
��ڲ���������Dst_adr : ������	   
                   
				  
���ڲ���������
��������ֵ������
����˵���� 
***********************************************/
void SectorErase_W25Q( unsigned long Dst_adr )
{
 
 Dst_adr <<= 12;

 WriteEnable_W25Q( );	  //��дʹ��
 WaitBusy_W25Q ( ); 	  //��æ

 Flash_CSL();;

  WriteReadByte_W25QXX( 0x20 );   //��������
  WriteReadByte_W25QXX( Dst_adr >> 16 );   //���͵�ַ
  WriteReadByte_W25QXX( Dst_adr >> 8 );   //���͵�ַ
  WriteReadByte_W25QXX( Dst_adr  );   //���͵�ַ

 Flash_CSH();;

}
/**********************************************
��������		   
����������	��ָ����ַ��ȡָ����������	   
��ڲ���������adr : ��ȡ��Ŀ���ַ	   
              pbuff �����ݴ洢������
			  NumByte ��Ҫ��ȡ���ݵ��ֽ���     
				  
���ڲ���������
��������ֵ������
����˵���� 
***********************************************/
void ReadData_W25Q( unsigned long adr, unsigned char * pbuff, unsigned short NumByte )
{
  Flash_Read(adr ,pbuff,NumByte);

// unsigned short itemp ; 
//
// WaitBusy_W25Q ( ); 	  //��æ
//
// Flash_CSL();;
// WriteReadByte_W25QXX( 0x03 );   //��������
// WriteReadByte_W25QXX( adr >> 16 );   //���͵�ַ
// WriteReadByte_W25QXX( adr >> 8 );   //���͵�ַ
// WriteReadByte_W25QXX( adr  );   //���͵�ַ
// itemp  = 0;
// while ( itemp < NumByte )
// {
//  pbuff[ itemp ++ ] = WriteReadByte_W25QXX( 0xff );
// }
// Flash_CSH();;
}
/**********************************************
��������		   
����������	��������ָ������ָ����������	   
��ڲ���������secpos : Ҫ��ȡ��Ŀ���������	   
   				  
���ڲ���������
��������ֵ�������������ݵ�ָ��
����˵���� 
***********************************************/
//unsigned char * Read_OneSec_W25X( unsigned short  secpos )
//{
// unsigned char * buf;
// ReadData_W25X(secpos << 12 , W25X_buf , 4096 );  //������������
// buf = W25X_buf;
// return buf;
//}
/**********************************************
��������		   
����������	��һҳ��д��ָ���������� ��������256���ֽڣ�	   
��ڲ���������adr : д���Ŀ���ַ	   
              pbuff ��д���������ݴ洢������
			  NumByte ��Ҫд�����������ֽ���     
				  
���ڲ���������
��������ֵ������
����˵���� 
***********************************************/
void WritePage_W25Q( unsigned long adr, unsigned char * pbuff, unsigned char NumByte )
{
 unsigned char itemp; 
 
 WriteEnable_W25Q( );	  //��дʹ��
 WaitBusy_W25Q ( ); 	  //��æ

 Flash_CSL();
 WriteReadByte_W25QXX( 0x02 );   //��������
 WriteReadByte_W25QXX( adr >> 16 );   //���͵�ַ
 WriteReadByte_W25QXX( adr >> 8 );   //���͵�ַ
 WriteReadByte_W25QXX( adr  );   //���͵�ַ
 itemp  = 0;
 while ( itemp < NumByte )
 {
  WriteReadByte_W25QXX( pbuff[ itemp ++ ] );
 }
 Flash_CSH();
}
/**********************************************
��������		   
�����������ڴ�ָ����ַ��ʼд��ָ����������
          �����Զ���ҳ����	   
��ڲ���������adr : д���Ŀ���ַ	   
              pbuff ��д���������ݴ洢������
			  NumByte ��Ҫд�����������ֽ���     
				  
���ڲ���������
��������ֵ������
����˵��������ȷ��ָ����ַ������ȫ����Oxff������
          ��0xff��д��ʧ�� 
***********************************************/
void Writedata_NoCheck_W25Q( unsigned long adr, unsigned char * pbuff, unsigned short NumByte )
{
 unsigned short pagermain ;
 pagermain = 256 - (adr % 256);	 //��ҳʣ���ֽ���
 if(NumByte <= pagermain)
 {pagermain = NumByte;}
 while(1)
 {
  //WritePage_W25Q ( adr , pbuff , pagermain);
  Flash_Write(adr ,pbuff,pagermain);

//  ReadData_W25Q(adr , W25Q_buf_tem, 256 );
  if(pagermain == NumByte)
  {break;} //д��
  else
  {
   adr +=  pagermain;
   pbuff += pagermain; 
   NumByte -= pagermain;
   if(NumByte > 256)
   {pagermain = 256;} //ʣ���ֽ�������һҳ��д��ҳ
   else
   {pagermain = NumByte;} 
   Delay (0xfff);
  }
 }
}
/**********************************************
��������		   
�����������ڴ�ָ����ַ��ʼд��ָ����������
          �����Զ���ҳ����	   
��ڲ���������adr : д���Ŀ���ַ	   
              pbuff ��д���������ݴ洢������
			  NumByte ��Ҫд�����������ֽ���     
				  
���ڲ���������
��������ֵ������
����˵������������	�Ȳ���д
***********************************************/
//void Writedata_Check_Sector_W25Q( unsigned long adr, unsigned char * pbuff, unsigned long NumByte )
//{
// unsigned short itemp;
// unsigned short secpos;     //������ַ
// unsigned short secoff;     //������ƫ��
// unsigned short secremain;  //����ʣ���ֽ���
//
// secpos = adr >> 12;
// secoff = adr % 4096;
// secremain = 4096 - secoff;
// 
// if (NumByte <= secremain)
// {secremain = NumByte;}
// while(1)
// {
//  ReadData_W25Q(secpos << 12 , W25Q_buf , 4096 );  //������������
//  itemp = secoff;
//  while (itemp < (secoff + secremain))
//  {
//   if(0xff != W25Q_buf[itemp ++])
//   {break;}
//  }
//  if( itemp < (secoff + secremain) )  //��Ҫ������������
//  {
//   Page_Erase(SECTOR_ERASE_4K ,secpos << 12);
//   itemp = 0;
//   while (itemp < secremain)  //��������
//   {
//     W25Q_buf[itemp + secoff] = pbuff [itemp];
//	 itemp ++ ;
//   }
//   Writedata_NoCheck_W25Q( secpos << 12, W25Q_buf, 4096 ); //д����������
//  }
//  else //�������
//  { Writedata_NoCheck_W25Q( adr, pbuff, secremain );} 
//  if( NumByte == secremain )
//  {break;} //д�����
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
