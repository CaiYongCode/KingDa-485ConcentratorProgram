
/*********************************************************************************
//������: Operation for SI446x

//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include "..\inc\IICEeprom_24CXX.h"  
#include "bsp.h"
/*********************************************************************************
����������
*********************************************************************************/

//��������
#ifndef IIC_TimeOut
#define IIC_TimeOut  255
#endif

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
void Launch_24CXX (void)
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
  GPIO_SetBits(GPIOA, GPIO_Pin_12);			//��ֹFlash��Ƭѡ
  dely_ms (100);			//��ֹFlash��Ƭѡ
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
// WriteReadByte_W25QXX(0x06);//дSR ENABLE
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
// WriteReadByte_W25QXX(0x04);//дSR DISABLE
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
// WriteReadByte_W25QXX(0x06);//дSR ENABLE
// //Flash_CSH();
// __NOP( );	__NOP( );
//
// //Flash_Busy_check();//��æ 
//
//
// //Flash_CSL();
// WriteReadByte_W25QXX(CMD);
// WriteReadByte_W25QXX((u8)((address & 0xffffff ) >> 16));
// WriteReadByte_W25QXX((u8)((address & 0xffff) >> 8));
// WriteReadByte_W25QXX((u8)(address & 0xff));	
// //Flash_CSH();
// //Flash_Busy_check();//��æ
// //Flash_CSL();
// WriteReadByte_W25QXX(0x04);//дSR DISABLE
// //Flash_CSH();
}
/*********************************************************************************
��������		   
����������		   
��ڲ���������	   
                   
				  
���ڲ���������
��������ֵ������
����˵���� 
*********************************************************************************/
static void Delay ( unsigned long temp )
{
 while (temp -- );
}
/*********************************************************************************
��������		   
����������	��дʹ��	   
��ڲ���������	   
                   
				  
���ڲ���������
��������ֵ������
����˵���� 
*********************************************************************************/
void WriteEnable_24CXX( void )
{
}
/*********************************************************************************
��������		   
����������	�ر�дʹ��	   
��ڲ���������	   
                   
				  
���ڲ���������
��������ֵ������
����˵���� 
*********************************************************************************/
void WriteDisable_24CXX( void )
{
 
}
/*********************************************************************************
��������		   
����������	�ȴ����� 
��ڲ���������	   
                   			  
���ڲ���������
��������ֵ������
����˵���� 
*********************************************************************************/
unsigned char WaitBusy_24CXX ( void )
{
}
/*********************************************************************************
��������		   
����������	ʹоƬ�������ģʽ
��ڲ���������	   
                   
				  
���ڲ���������
��������ֵ������
����˵���� 
*********************************************************************************/
void PowerDown_24CXX( void )
{
}
/*********************************************************************************
��������		   
����������	����оƬ
��ڲ���������	   
                   
				  
���ڲ���������
��������ֵ������
����˵���� 
*********************************************************************************/
void ReleasePowerDown_24CXX( void )
{
}
/*********************************************************************************
��������		   
����������	��Ƭ����
��ڲ���������	   
                   
				  
���ڲ���������
��������ֵ������
����˵���� 
*********************************************************************************/
void ChipErase_24CXX( void )
{
}
/*********************************************************************************
��������		   
����������	��������
��ڲ���������Dst_adr : ������	   
                   
				  
���ڲ���������
��������ֵ������
����˵���� 
*********************************************************************************/
void SectorErase_24CXX( unsigned long Dst_adr )
{


}
/*********************************************************************************
��������		   
��������:    ��һҳ��д��ָ���������� ��������256���ֽڣ�	   
��ڲ���������adr : д���Ŀ���ַ	   
              pbuff ��д���������ݴ洢������
	      NumByte ��Ҫд�����������ֽ���     
				  
���ڲ���������
��������ֵ������
����˵���� 
*********************************************************************************/
void WritePage_24CXX( unsigned long adr, unsigned char * pbuff, unsigned char NumByte )
{
}
/*********************************************************************************
��������		   
����������     �ڴ�ָ����ַ��ʼд��ָ���������ݾ����Զ���ҳ����	   
��ڲ��������� adr : д���Ŀ���ַ	   
               pbuff ��д���������ݴ洢������
	       NumByte ��Ҫд�����������ֽ���     
				  
���ڲ���������
��������ֵ������
����˵����
          
*********************************************************************************/
void WriteData_24CXX( unsigned long adr, unsigned char * pbuff, unsigned short NumByte )
{
// unsigned short pagermain ;
// pagermain = 256 - (adr % 256);	 //��ҳʣ���ֽ���
// if(NumByte <= pagermain)
// {pagermain = NumByte;}
// while(1)
// {
//  //WritePage_W25Q ( adr , pbuff , pagermain);
//  Flash_Write(adr ,pbuff,pagermain);
//
////  ReadData_W25Q(adr , W25Q_buf_tem, 256 );
//  if(pagermain == NumByte)
//  {break;} //д��
//  else
//  {
//   adr +=  pagermain;
//   pbuff += pagermain; 
//   NumByte -= pagermain;
//   if(NumByte > 256)
//   {pagermain = 256;} //ʣ���ֽ�������һҳ��д��ҳ
//   else
//   {pagermain = NumByte;} 
//   Delay (0xfff);
//  }
// }
}
/***********************************END**********************************************/


