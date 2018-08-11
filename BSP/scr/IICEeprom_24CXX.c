
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
#include "..\inc\bsp.h"
#include "..\..\Projects\inc\SofTimer.h"
/*********************************************************************************
����������
*********************************************************************************/
DriverState_type  EepromState = OffState;
/*********************************************************************************
��������������
*********************************************************************************/
 
/*********************************************************************************
�ⲿ����������
*********************************************************************************/

/*********************************************************************************
˽�б���������
*********************************************************************************/
static SofTimer_Struct*   EepromTimer = P_NULL;
static unsigned char     I2C_SlaverAddress = 0;
/*********************************************************************************
���Ա���������
*********************************************************************************/
 
/*********************************************************************************
�ڲ��������Ͷ�����
*********************************************************************************/

/*********************************************************************************
�ڲ�����������
*********************************************************************************/
static void EepromTimer_CallBack(void *p_tmr, void *p_arg);
static SystemErrName EEPROM_I2C_WaitEepromStandbyState(void); //EEPROM��Ϊ����״̬
//void EEPROM_I2C_PageWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite);//д����һҳ������
/*********************************************************************************
���ܴ��붨����
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
  EepromTimer =  SofTimerCreate (60,     //����
                                 TimerTB_1S,              //ʱ��
                                 SofTimer_ONE_SHOT,          //ģʽ
                                 EepromTimer_CallBack, //�ص�
                                 P_NULL);       //�ص�����
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
* Description    : ��������������д��EEPROM
* Input          : - pBuffer : ָ��Ҫд�����������ָ�� 
*                  - WriteAddr : EEPROM��Ҫд�����ݵ��׵�ַ.
*                  - NumByteToWrite : д����ֽ���.
* Output         : None
* Return         : None
pBuffer��
WriteAddr��
NumByteToWrite��д����ֽ���
*******************************************************************************/
SystemErrName EEPROM_I2C_BufferWrite(void* Buffer, short WriteAddr, short NumByteToWrite)//��������������д��EEPROM
{
 SystemErrName err = NO_ERR ; 
 unsigned char  NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
 unsigned char* pBuffer; 
 
 Addr = WriteAddr % EEPROM_PageSize;//д���ַ��ÿҳ�ĵڼ�λ
 count = EEPROM_PageSize - Addr;//�ڿ�ʼ��һҳҪд��ĸ���
 pBuffer = Buffer;
 
 if(Addr + NumByteToWrite < EEPROM_PageSize )   // ���ݲ���ҳ
  { err = EEPROM_I2C_PageWrite(pBuffer, WriteAddr, NumByteToWrite);}//д����һҳ������
 else//���ݲ���ҳ
  {
   NumByteToWrite -= count;
   NumOfPage = NumByteToWrite / EEPROM_PageSize; //����Ҫд���ҳ��
   NumOfSingle = NumByteToWrite % EEPROM_PageSize;//���¼��㲻��һҳ�ĸ���
   if(count != 0)//
   { 
    err = EEPROM_I2C_PageWrite(pBuffer, WriteAddr, count);//����ʼ�Ŀռ�д��һҳ
    if(NO_ERR != err){return err;}
    WriteAddr += count;
    pBuffer += count;
   } 
   while(NumOfPage--)//Ҫд���ҳ��
   {
    err = EEPROM_I2C_PageWrite(pBuffer, WriteAddr, EEPROM_PageSize);//дһҳ������
    if(NO_ERR != err){return err;}
    WriteAddr += EEPROM_PageSize;
    pBuffer += EEPROM_PageSize; 
   }
   if(NumOfSingle != 0)//ʣ������С��һҳ
   {err = EEPROM_I2C_PageWrite(pBuffer, WriteAddr, NumOfSingle);  }//д����һҳ������
  }
 return err;
}
/*******************************************************************************
* Function Name : I2C_EE_ByteWrite
* Description    : Writes one byte to the I2C EEPROM.
* Input          : - data : Ҫд�������
*                  - WriteAddr : EEPROM��Ҫд�����ݵ��׵�ַ.
* Output         : None
* Return         : None
*******************************************************************************/
SystemErrName EEPROM_I2C_ByteWrite(unsigned char data, unsigned char WriteAddr)//дһ���ֽڵ�EEPROM
{
 SystemErrName err; 
 unsigned long itemp = 0;
 
 err =  EEPROM_I2C_WaitEepromStandbyState();//EEPROM��Ϊ����״̬
 if(NO_ERR!= err){return err;} 

 I2C_GenerateSTART(I2C2, ENABLE);//���� I2Cx���� START����

 while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)) //������һ�� I2C�¼��Ƿ���������¼�
 {if(itemp++ > 0x4ffff){return Unsuccessful;}}
  itemp = 0; 

 I2C_Send7bitAddress(I2C2, I2C_SlaverAddress, I2C_Direction_Transmitter);//��ָ���Ĵ� I2C�豸���͵�ַ��,ѡ���ͷ���

 while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))//������һ�� I2C�¼��Ƿ���������¼�
 {if(itemp++ > 0x4ffff){return Unsuccessful;}}
  itemp = 0;       

 I2C_SendData(I2C2, WriteAddr);//ͨ������ I2Cx���͵�ַ

 while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))//������һ�� I2C�¼��Ƿ���������¼�
 {if(itemp++ > 0x4ffff){return Unsuccessful;}}
  itemp = 0; 

 I2C_SendData(I2C2, data); //ͨ������ I2Cx�������� /* Send the byte to be written */
   
 while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))//������һ�� I2C�¼��Ƿ���������¼�
 {if(itemp++ > 0x4ffff){return Unsuccessful;}}//  itemp = 0; 

 I2C_GenerateSTOP(I2C2, ENABLE);//���� I2Cx���� STOP����
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
pBuffer��ָ��Ҫд�����������ָ��
WriteAddr��24c02��Ҫд�����ݵ��׵�ַ
NumByteToWrite��д����ֽ���
*******************************************************************************/
SystemErrName EEPROM_I2C_PageWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite)//д����һҳ������
{
 SystemErrName err; 
 unsigned long itemp = 0;
 
 err =  EEPROM_I2C_WaitEepromStandbyState();//EEPROM��Ϊ����״̬
 if(NO_ERR!= err){return err;} 

 I2C_GenerateSTART(I2C2, ENABLE);//���� I2Cx���� START����
 
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)) //������һ�� I2C�¼��Ƿ���������¼�
 {if(itemp++ > 0x4ffff){return Unsuccessful;}}
  itemp = 0;  

  I2C_Send7bitAddress(I2C2, I2C_SlaverAddress, I2C_Direction_Transmitter);//��ָ���Ĵ� I2C�豸���͵�ַ��,ѡ���ͷ���

  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))//������һ�� I2C�¼��Ƿ���������¼�
 {if(itemp++ > 0x4ffff){return Unsuccessful;}}
  itemp = 0;

  I2C_SendData(I2C2, WriteAddr); //ͨ������ I2Cx���͵�ַ

  while(! I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))//������һ�� I2C�¼��Ƿ���������¼�
 {if(itemp++ > 0x4ffff){return Unsuccessful;}}
  itemp = 0;  

  while(NumByteToWrite--) 
  {
    I2C_SendData(I2C2, *pBuffer); //ͨ������ I2Cx��������
    pBuffer++; 
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))//������һ�� I2C�¼��Ƿ���������¼�
   {if(itemp++ > 0x4ffff){return Unsuccessful;}}
    itemp = 0; 
  }
  I2C_GenerateSTOP(I2C2, ENABLE);//���� I2Cx���� STOP����
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
pBuffer��ָ��Ҫ����������ݵ������ָ��
ReadAddr��24c02��Ҫ�������ݵ��׵�ַ
NumByteToRead���������ֽ���
*******************************************************************************/
SystemErrName EEPROM_I2C_BufferRead(void* Buffer, u8 ReadAddr, u16 NumByteToRead)//��EEPROM�����ݶ��뻺����
{ 
 SystemErrName err; 
 unsigned long itemp = 0;
 unsigned char* pBuffer;
 pBuffer = Buffer;
 err =  EEPROM_I2C_WaitEepromStandbyState();//EEPROM��Ϊ����״̬
 if(NO_ERR!= err){return err;}

  I2C_GenerateSTART(I2C2, ENABLE);//���� I2Cx���� START����  /* Send START condition */

  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))//������һ�� I2C�¼��Ƿ���������¼� /* Test on EV5 and clear it */
  {if(itemp++ > 0x4fff){return Unsuccessful;}}
  itemp = 0;
  if(NumByteToRead==1)  
  {I2C_AcknowledgeConfig(I2C2, DISABLE);}//ʹ�ܻ���ʧ��ָ�� I2C��Ӧ���� /* In the case of a single data transfer disable ACK before reading the data */
  
  I2C_Send7bitAddress(I2C2, I2C_SlaverAddress, I2C_Direction_Transmitter);//��ָ���Ĵ� I2C�豸���͵�ַ��,ѡ���ͷ��� 

  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))//������һ�� I2C�¼��Ƿ���������¼�  /* Test on EV6 and clear it */
  {if(itemp++ > 0x4fff){return Unsuccessful;}}
  itemp = 0; 
  //I2C_Cmd(I2C2, ENABLE);//ʹ�ܻ���ʧ�� I2C����  /* Clear EV6 by setting again the PE bit */

  I2C_SendData(I2C2, ReadAddr); //ͨ������ I2Cx���͵�ַ  /* Send the EEPROM's internal address to write to */
 
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))//������һ�� I2C�¼��Ƿ���������¼�/* Test on EV8 and clear it */
  {if(itemp++ > 0x4fff){return Unsuccessful;}}
  itemp = 0;
  I2C_GenerateSTART(I2C2, ENABLE);//���� I2Cx���� START����/* Send STRAT condition a second time */ 

  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))//������һ�� I2C�¼��Ƿ���������¼�/* Test on EV5 and clear it */
  {if(itemp++ > 0x4fff){return Unsuccessful;}}
  itemp = 0;
  
  I2C_Send7bitAddress(I2C2, I2C_SlaverAddress, I2C_Direction_Receiver);//��ָ���Ĵ� I2C�豸���͵�ַ��,ѡ����շ��� /* Send EEPROM address for read */

  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))//������һ�� I2C�¼��Ƿ���������¼� /* Test on EV6 and clear it */
   {if(itemp++ > 0x4fff){return Unsuccessful;}}
  itemp = 0;
  while(NumByteToRead) 
  {
    if(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED)) //������һ�� I2C�¼��Ƿ���������¼�  /* Test on EV7 and clear it */
    {
      itemp = 0; 
      if(NumByteToRead == 2)
      {I2C_AcknowledgeConfig(I2C2, DISABLE);}//ʹ�ܻ���ʧ��ָ�� I2C��Ӧ����
      if(NumByteToRead == 1)
      {I2C_GenerateSTOP(I2C2, ENABLE);}//���� I2Cx���� STOP����
      *pBuffer = I2C_ReceiveData(I2C2);//����ͨ�� I2Cx������յ�����
      pBuffer++; 
      NumByteToRead--;    
   }
   if(itemp++ > 0x4ffff){return Unsuccessful;}
  }
 I2C_AcknowledgeConfig(I2C2, ENABLE);//ʹ�ܻ���ʧ��ָ�� I2C��Ӧ����
 return NO_ERR;
}
/*******************************************************************************
* Function Name : I2C_EE_WaitEepromStandbyState
* Description    : Wait for EEPROM Standby state
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
SystemErrName EEPROM_I2C_WaitEepromStandbyState(void) //EEPROM��Ϊ����״̬     
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
  I2C_GenerateSTART(I2C2, ENABLE);//���� I2Cx���� START����
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)) //������һ�� I2C�¼��Ƿ���������¼�
  {if(itemp++ > 0x4fff){return Unsuccessful;}}
  itemp = 0;
  I2C_Send7bitAddress(I2C2, I2C_SlaverAddress, I2C_Direction_Transmitter);//��ָ���Ĵ� I2C�豸���͵�ַ�� ,ѡ���ͷ���
  Delay ( 0x4ff );
  I2C_ClearFlag(I2C2, I2C_FLAG_AF);//��� I2Cx��Ӧ������־λ
  SR1_Tmp = I2C_ReadRegister(I2C2, I2C_Register_SR1);//��ȡָ���� I2C�Ĵ��� I2C_SR1 ��������ֵ
  if(itemp1++ > 0x4fff){I2C2_Rset( ); return Unsuccessful;}
 }while(!(I2C_ReadRegister(I2C2, I2C_Register_SR1) & 0x0002));//��ַ���ͽ���
 I2C_ClearFlag(I2C2, I2C_FLAG_AF);//��� I2Cx��Ӧ������־λ
 
 I2C_GenerateSTOP(I2C2, ENABLE);//���� I2Cx���� STOP����
 while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY))
 { 
  if(itemp++ > 0x4fff)
  { return Unsuccessful;}
 }
 return NO_ERR;
}
/*********************************************************************************
��������		   
����������	   
��ڲ���������	   
                   
				  
���ڲ���������
��������ֵ������
����˵���� 
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
��������		   
����������	
��ڲ���������	   
                   
				  
���ڲ���������
��������ֵ������
����˵���� 
*********************************************************************************/
SystemErrName EEPROM_I2C_MultipleWrite(void* Buffer, unsigned short WriteAddr, unsigned short NumByteToWrite)
{
  SystemErrName err = NO_ERR; 
  unsigned char itemp = 0 ;
  while (itemp++ < 3)
  {
   err = EEPROM_I2C_BufferWrite(Buffer, WriteAddr, NumByteToWrite);//��������������д��EEPROM
   if(NO_ERR != err)
   { EepromPort_rset(EEPROM_ADDRESS) ;}
   else
   {break;} 
  }
  return err;
}
/*********************************************************************************
��������		   
����������	��дʹ��	   
��ڲ���������	   
                   
				  
���ڲ���������
��������ֵ������
����˵���� 
*********************************************************************************/
void EEPROM_I2C_EEPROM_WriteEnable( void )
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
void EEPROM_I2C_EEPROM_WriteDisable( void )
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
void EEPROM_I2C_PowerDown( void )
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


