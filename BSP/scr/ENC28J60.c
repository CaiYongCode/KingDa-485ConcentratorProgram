/*********************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include "ENC28J60.h"
/*********************************************************************************
����������
*********************************************************************************/
const unsigned char MAC_ENC28J60 [6] ={0x9B,0xAC,0x3E,0x8A,0x42,0x69};
/*********************************************************************************
��������������
*********************************************************************************/
unsigned short enc28j60Init_cnt = 0;
/*********************************************************************************
�ⲿ����������
*********************************************************************************/

/*********************************************************************************
˽�б���������
*********************************************************************************/
static unsigned char txbuff = 0xff;
static unsigned char Enc28j60Bank;
static unsigned int NextPacketPtr;
/*********************************************************************************
���Ա���������
*********************************************************************************/

/*********************************************************************************
�ڲ�����������
*********************************************************************************/
void INIT_Confi_enc28j60 (void);
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
**********************************************************************************/
unsigned char WriteReadByte_enc28j60 ( unsigned char dt)
{
 unsigned short i = 0;
 while (!(SPI1 -> SR & (0x0001<< 1)))
 {
  i ++ ;
  if(i > 0x4fff)
  {break;}
 }
 SPI1->DR = dt;//       SPI_I2S_SendData(SPI1,dt);
while (!(SPI1 -> SR & (0x0001<< 0)))
{
 i ++ ;
 if(i > 0x4fff)
 {break;}
} 
 return SPI1->DR;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
**********************************************************************************/
void writeBuff_enc28j60 ( unsigned char* buff, unsigned short length )
{
  DMA_InitTypeDef DMA_InitStructure;
  DMA_DeInit(DMA1_Channel3);  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(SPI1->DR));         //����  ��������(0x4001300C) ��ַ(Դ��ַ)
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)buff;                      //���� SRAM �洢��ַ(Դ��ַ)
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                                //���䷽�� �ڴ�-����
  DMA_InitStructure.DMA_BufferSize = length ;                           //���� SPI1 ���ճ���
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;                  //�����ַ����(����)
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                           //�ڴ��ַ����(�仯)
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;           //���贫����(�ֽ�)
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;                   //�ڴ洫����(�ֽ�)
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                                     //���䷽ʽ,һ�δ�����ֹͣ,�����¼���
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                           //�жϷ�ʽ-��(����)
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                      //�ڴ浽�ڴ淽ʽ��ֹ
  DMA_Init(DMA1_Channel3, &DMA_InitStructure);
  DMA_Cmd(DMA1_Channel3, ENABLE);
  while ( (DMA1 -> ISR & (1ul << 9)) == 0);
  while (SPI1 ->SR & (1ul << 7));
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        // SPI/I2S2_RX 
**********************************************************************************/
void readBuff_enc28j60 ( unsigned char* buff, unsigned short length)
{
 DMA1_Channel3->CCR   = 0 ;
 DMA1_Channel3->CMAR  = (u32)(&txbuff);
 DMA1_Channel3->CNDTR = length ;
 DMA1_Channel3->CCR   = ( (1ul<< 0)      //1��ͨ������
						 |(1ul<< 4));    //1���Ӵ洢����
 DMA1_Channel2->CCR   = 0;
 DMA1_Channel2->CMAR  = (u32)buff;
 DMA1_Channel2->CNDTR = length;
 DMA1_Channel2->CCR   = ( (1ul<< 0)    //1��ͨ������
                         |(1ul<< 1)    //����������ж�
						 |(1ul<< 7)	   //1��ִ�д洢����ַ��������
						 |(2ul<< 12)); //10��ͨ�����ȼ� ��
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
**********************************************************************************/
void writeBuff_enc28j60_finish ( void )
{  
 DMA1->IFCR |= (0x0000000f << 8);
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
**********************************************************************************/
void readBuff_enc28j60_finish ( void )
{ 
 DMA1->IFCR |= (0x0000000f << 4);
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
**********************************************************************************/
void enc28j601_WBM_buff (unsigned char* buff, unsigned short length)
{
 clr_ENC28J60_SCpin ;
 WriteReadByte_enc28j60 (ENC28J60_WRITE_BUF_MEM);
 DMA1_Channel3->CCR   = 0;
 DMA1_Channel3->CPAR  = (u32)(&(SPI1->DR));
 DMA1_Channel3->CMAR  = (u32)buff;
 DMA1_Channel3->CNDTR = length;
 DMA1_Channel2->CCR   = 0;
 DMA1_Channel2->CPAR  = (u32)(&(SPI1->DR));
 DMA1_Channel2->CMAR  = (u32)(&txbuff);
 DMA1_Channel2->CNDTR = length;
 DMA1_Channel2->CCR   = (1ul<< 0) ;  //1��ͨ������
 DMA1_Channel3->CCR   = ((1ul<< 0)|(1ul<< 7)|(1ul<< 4)|(3ul<< 12));

 while ( (DMA1 -> ISR & (1ul << 9)) == 0);
 while (SPI1 ->SR & (1ul << 7))	;
 set_ENC28J60_SCpin ;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
**********************************************************************************/
void enc28j601_RBM_buff (unsigned char* buff, unsigned short length)
{
 txbuff = 0xFF;
 if(length > MAX_FRAMELEN )	{length = MAX_FRAMELEN ;}
 clr_ENC28J60_SCpin ;
 WriteReadByte_enc28j60 ( ENC28J60_READ_BUF_MEM );

 DMA1_Channel5->CCR   = 0 ;
 DMA1_Channel5->CMAR  = (u32)(&txbuff);
 DMA1_Channel5->CNDTR = length ;

 DMA1_Channel4->CCR   = 0;
 DMA1_Channel4->CMAR  = (u32)buff;
 DMA1_Channel4->CNDTR = length;

 DMA1_Channel4->CCR   = ( (1ul<< 0)    //1��ͨ������
//                         |(1ul<< 1)    //����������ж�
						 |(1ul<< 7)	   //1��ִ�д洢����ַ��������
						 |(3ul<< 12)); //10��ͨ�����ȼ� ��
  DMA1_Channel5->CCR   = ( (1ul<< 0)      //1��ͨ������
						 |(1ul<< 4));    //1���Ӵ洢����
 while ( (DMA1 -> ISR & (1ul << 5)) == 0);
 while (SPI2 ->SR & (1ul << 7));
 set_ENC28J60_SCpin ;
}
/****************************************************************************
* ��    �ƣ�unsigned char enc28j60ReadOp(unsigned char op, unsigned char address)
* ��    �ܣ�ENC28J60���Ĵ���
* ��ڲ�����
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/ 
unsigned char enc28j60ReadCR(unsigned char address)
{
 unsigned char dat = 0;
 clr_ENC28J60_SCpin;
 dat = ENC28J60_READ_CTRL_REG | (address & ADDR_MASK);
 WriteReadByte_enc28j60(dat);
 dat = WriteReadByte_enc28j60(0xFF);
 if(address & 0x80)
 {dat = WriteReadByte_enc28j60(0xFF);}
 set_ENC28J60_SCpin;
 return dat;
}
/****************************************************************************
* ��    �ƣ�
* ��    �ܣ�ENC28J60 �Ĵ�����������
* ��ڲ�����op 	 address  data
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/ 
void enc28j60WriteCR( unsigned char address, unsigned char data)
{
 unsigned char dat = 0;
 clr_ENC28J60_SCpin;	                      //ʹ��ENC28J60 SPIƬѡ  		
 dat = ENC28J60_WRITE_CTRL_REG | (address & ADDR_MASK);	  //OP--3λ������ (address & ADDR_MASK)--5λ����
 WriteReadByte_enc28j60(dat);				  //SPI1 д
 WriteReadByte_enc28j60(data);				  //SPI1 д��������
 set_ENC28J60_SCpin;						  //��ֹENC28J60 SPIƬѡ  ��ɲ���
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
**********************************************************************************/
unsigned short enc28j601_RBM_16word ( unsigned short addr )
{
 unsigned short itemp;
 enc28j60WriteCR(ERDPTL, (addr));
 enc28j60WriteCR(ERDPTH, (addr)>>8);

 clr_ENC28J60_SCpin ;
 WriteReadByte_enc28j60 ( ENC28J60_READ_BUF_MEM );
 itemp = WriteReadByte_enc28j60 (0xFF);
 itemp <<= 8;
 itemp = WriteReadByte_enc28j60 (0xFF);
 set_ENC28J60_SCpin ;
 return itemp;
}
/****************************************************************************
* ��    �ƣ�
* ��    �ܣ�ENC28J60 �Ĵ�����������
* ��ڲ�����op 	 address  data
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/ 
void enc28j60_BIT_SET( unsigned char address, unsigned char bitflg)
{
 unsigned char dat = 0;
 clr_ENC28J60_SCpin;	                      //ʹ��ENC28J60 SPIƬѡ  		
 dat = ENC28J60_BIT_FIELD_SET | (address & ADDR_MASK);	  //OP--3λ������ (address & ADDR_MASK)--5λ����
 WriteReadByte_enc28j60(dat);				  //SPI1 д
 WriteReadByte_enc28j60(bitflg);				  //SPI1 д��������
 set_ENC28J60_SCpin;						  //��ֹENC28J60 SPIƬѡ  ��ɲ���
}
/****************************************************************************
* ��    �ƣ�
* ��    �ܣ�ENC28J60 �Ĵ�����������
* ��ڲ�����op 	 address  data
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/ 
void enc28j60_BIT_CLR( unsigned char address, unsigned char bitflg)
{
 unsigned char dat = 0;
 clr_ENC28J60_SCpin;	                      //ʹ��ENC28J60 SPIƬѡ  		
 dat = ENC28J60_BIT_FIELD_CLR | (address & ADDR_MASK);	  //OP--3λ������ (address & ADDR_MASK)--5λ����
 WriteReadByte_enc28j60(dat);				  //SPI1 д
 WriteReadByte_enc28j60(bitflg);				  //SPI1 д��������
 set_ENC28J60_SCpin;						  //��ֹENC28J60 SPIƬѡ  ��ɲ���
}
/****************************************************************************
* ��    �ƣ�void enc28j60SetBank(unsigned char address)
* ��    �ܣ�ENC28J60 ���üĴ���BANK
* ��ڲ�����
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/ 
void enc28j60SetBank(unsigned char address)
{
 if((address & BANK_MASK) != Enc28j60Bank)
 {
  enc28j60_BIT_CLR(ECON1, (ECON1_BSEL1|ECON1_BSEL0));
  enc28j60_BIT_SET(ECON1, (address & BANK_MASK)>>5);
  Enc28j60Bank = (address & BANK_MASK);
 }
}
/****************************************************************************
* ��    �ƣ�unsigned char enc28j60Read(unsigned char address)
* ��    �ܣ���ȡָ���Ĵ�������ֵ
* ��ڲ�����
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/ 
unsigned char enc28j60Read(unsigned char address)
{
 enc28j60SetBank(address); // set the bank
 return enc28j60ReadCR(address);	 	// do the read
}
/****************************************************************************
* ��    �ƣ�void enc28j60Write(unsigned char address, unsigned char data)
* ��    �ܣ���ָ���Ĵ���д����ֵ
* ��ڲ�����
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/ 
void enc28j60Write(unsigned char address, unsigned char data)
{
 enc28j60SetBank(address);           // set the bank
 enc28j60WriteCR(address, data);	 // do the write
}
/****************************************************************************
* ��    �ƣ�void enc28j60PhyWrite(unsigned char address, unsigned int data)
* ��    �ܣ���ָ��PHY�Ĵ���д����ֵ
* ��ڲ�����
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/ 
void enc28j60PhyWrite(unsigned char address, unsigned int data)
{
 unsigned long itemp = 0;
 enc28j60Write(MIREGADR, address); 	// set the PHY register address
 // write the PHY data
 enc28j60Write(MIWRL, data);
 enc28j60Write(MIWRH, data>>8);
 while(enc28j60Read(MISTAT) & MISTAT_BUSY)	 // �ȴ�PHY�Ĵ���д�����
 {
  if(itemp ++ > 0x4fffff){break;}
 }
}
/****************************************************************************
* ��    �ƣ�void enc28j60clkout(unsigned char clk)
* ��    �ܣ�����ENC28J60ʱ�����Ƶ��
* ��ڲ�����
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/ 
void enc28j60clkout(unsigned char clk)
{
 enc28j60Write(ECOCON, clk & 0x7);
}
/****************************************************************************
* ��    �ƣ�void enc28j60Init(unsigned char* macaddr)
* ��    �ܣ�ENC28J60��ʼ�� 
* ��ڲ�����*macaddr--6���ֽڵ�MAC��ַ
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/ 
void Launch_enc28j60 (void )
{ 
 unsigned char flg =0;
 unsigned char cntA = 0;
 
 EthernetPowerON_Order ( );
 DelymS(1);
 SPI1_Configuration ( ); 
 clr_ENC28J60_SCpin;
 WriteReadByte_enc28j60(ENC28J60_SOFT_RESET);	  //ENC28J60�����λ ϵͳ����
 WriteReadByte_enc28j60(ENC28J60_SOFT_RESET);
 set_ENC28J60_SCpin;

 while (1)		// while(!(enc28j60Read(ESTAT) & ESTAT_CLKRDY)){} 
 {
  flg =  enc28j60Read(ESTAT);
  if(flg & ESTAT_CLKRDY)	
  {break;}
  DelymS(10);
  cntA ++;
 }

 NextPacketPtr = RXSTART_INIT;
 enc28j60Write(ERXSTL, RXSTART_INIT&0xFF);	 //���ջ�������ʼ��ַ
 enc28j60Write(ERXSTH, RXSTART_INIT>>8);
 enc28j60Write(ERXNDL, RXSTOP_INIT&0xFF);	 //���ջ�����������ַ
 enc28j60Write(ERXNDH, RXSTOP_INIT>>8);
 enc28j60Write(ETXSTL, TXSTART_INIT&0xFF);	 //���ͻ�������ʼ��ַ
 enc28j60Write(ETXSTH, TXSTART_INIT>>8);	 
 enc28j60Write(ETXNDL, TXSTOP_INIT&0xFF);    //���ͻ�����������ַ
 enc28j60Write(ETXNDH, TXSTOP_INIT>>8);
 enc28j60Write(ERXFCON, ERXFCON_UCEN|ERXFCON_CRCEN|
                        ERXFCON_MCEN|ERXFCON_BCEN |ERXFCON_PMEN);
                                             //������ƥ������ 
 enc28j60Write(EPMM0, 0x3f);	 //��ʽƥ�������ֽ�0
 enc28j60Write(EPMM1, 0x30);	 //��ʽƥ�������ֽ�1
 enc28j60Write(EPMCSL, 0xf9);	 //��ʽƥ��У��͵��ֽ�
 enc28j60Write(EPMCSH, 0xf7);	 //��ʽƥ��У��͸��ֽ�

 enc28j60Write(MACON1, MACON1_MARXEN|MACON1_TXPAUS|MACON1_RXPAUS);	 //MAC���ƼĴ���1
 enc28j60Write(MACON2, 0x00);										 //MAC ���ƼĴ���2
 enc28j60_BIT_SET(MACON3, MACON3_PADCFG0|MACON3_TXCRCEN|MACON3_FRMLNEN|MACON3_FULDPX);
                                                                     //MAC ���ƼĴ���3
 enc28j60Write(MAIPGL, 0x12);  //�Ǳ��Ա����������ֽ�
 enc28j60Write(MAIPGH, 0x0C);  //�Ǳ��Ա����������ֽ�
 enc28j60Write(MABBIPG, 0x15); //���Ա�������

 enc28j60Write(MAMXFLL, MAX_FRAMELEN&0xFF);	//���֡���ȵ��ֽ�
 enc28j60Write(MAMXFLH, MAX_FRAMELEN>>8);	//���֡���ȸ��ֽ�

 enc28j60Write(MAADR5, MAC_ENC28J60[0]);	//MAC ��ַ�ֽ�5
 enc28j60Write(MAADR4, MAC_ENC28J60[1]);        //MAC ��ַ�ֽ�4
 enc28j60Write(MAADR3, MAC_ENC28J60[2]);	//MAC ��ַ�ֽ�3 
 enc28j60Write(MAADR2, MAC_ENC28J60[3]);	//MAC ��ַ�ֽ�2
 enc28j60Write(MAADR1, MAC_ENC28J60[4]);        //MAC ��ַ�ֽ�1
 enc28j60Write(MAADR0, MAC_ENC28J60[5]);        //MAC ��ַ�ֽ�0

 enc28j60PhyWrite(PHCON1, PHCON1_PDPXMD);       //PHY ���ƼĴ���1
 enc28j60PhyWrite(PHCON2, PHCON2_HDLDIS);       //PHY ���ƼĴ���2	
                                                //����PHYΪȫ˫��  LEDBΪ������
 enc28j60SetBank(ECON1);		        //��̫�����ƼĴ���1
 enc28j60_BIT_SET(EIE, EIE_INTIE|EIE_PKTIE);    //EIE_RXERIE//��̫���ж�����Ĵ���
 enc28j60_BIT_SET( ECON1, ECON1_RXEN);           //��̫�����ƼĴ���1  

 INIT_Confi_enc28j60 ( ); 
}
/****************************************************************************
* ��    �ƣ�unsigned char enc28j60getrev(void)
* ��    �ܣ���ȡENC28J60�İ汾��Ϣ
* ��ڲ�����
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/ 
unsigned char enc28j60getrev(void)
{return(enc28j60Read(EREVID));}
/****************************************************************************
* ��    �ƣ�void enc28j60PacketSend(unsigned int len, unsigned char* packet)
* ��    �ܣ�ͨ��ENC28J60��������
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
****************************************************************************/ 
void enc28j60PacketSend(unsigned char* packet, unsigned short len)
{
 // ���÷��ͻ�������ַдָ����� Set the write pointer to start of transmit buffer area
 while((enc28j60Read(ECON1) & ECON1_TXRTS)!=0); //    
 enc28j60Write(EWRPTL, TXSTART_INIT&0xFF);
 enc28j60Write(EWRPTH, TXSTART_INIT>>8);
 //����TXNDָ�룬�Զ�Ӧ���������ݰ���С	   
 enc28j60Write(ETXNDL, (TXSTART_INIT+len)&0xFF);
 enc28j60Write(ETXNDH, (TXSTART_INIT+len)>>8);

 clr_ENC28J60_SCpin ;
 WriteReadByte_enc28j60 (ENC28J60_WRITE_BUF_MEM);
 WriteReadByte_enc28j60  (  0x00 ); 
 set_ENC28J60_SCpin ;


 enc28j601_WBM_buff(packet ,len);  	 // �����ݰ����Ƶ����ͻ�����
 enc28j60_BIT_SET (ECON1, ECON1_TXRTS);// �������Ϸ��ͷ��ͻ�����������
 // ��λ�����߼������⡣�μ� Rev. B4 Silicon Errata point 12.
 if( (enc28j60Read(EIR) & EIR_TXERIF) )
 {
  enc28j60SetBank(ECON1);
  enc28j60_BIT_CLR ( ECON1, ECON1_TXRTS);
  enc28j60_BIT_CLR ( ECON1, ECON1_TXRST);
 }
}
/****************************************************************************
* ��    �ƣ�void enc28j60PacketSend(unsigned int len, unsigned char* packet)
* ��    �ܣ�ͨ��ENC28J60��������
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
****************************************************************************/ 
void enc28j60_ConfigSendBuf(unsigned short len)
{
 unsigned char flg;
 unsigned char cntA = 0;
 unsigned char cntB = 0;
 unsigned char cntC = 0;
 extern struct netif   ENC28J60Netif;

 while (1)
 {
  flg  = enc28j60Read(ECON1);
  if((flg & ECON1_TXRTS) == 0){break;}
  cntA ++;
  if(cntA > 0x0f){/*OSTimeDlyHMSM(0,0,0,OS_OPT_TIME_HMSM_STRICT,100,&err)*/;cntA = 0;cntB ++;}
  if(cntB > 3){/*enc28j60Init(&ENC28J60Netif);*/cntB = 0;cntC ++;}
  if(cntC > 3){}
 }
   
 enc28j60Write(EWRPTL, TXSTART_INIT&0xFF);
 enc28j60Write(EWRPTH, TXSTART_INIT>>8);
 //����TXNDָ�룬�Զ�Ӧ���������ݰ���С	   
 enc28j60Write(ETXNDL, (TXSTART_INIT+len)&0xFF);
 enc28j60Write(ETXNDH, (TXSTART_INIT+len)>>8);

 clr_ENC28J60_SCpin ;
 WriteReadByte_enc28j60 (ENC28J60_WRITE_BUF_MEM);
 WriteReadByte_enc28j60  (  0x00 ); 
 set_ENC28J60_SCpin ;
}
/****************************************************************************
* ��    �ƣ�void enc28j60PacketSend(unsigned int len, unsigned char* packet)
* ��    �ܣ�ͨ��ENC28J60��������
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
****************************************************************************/ 
void enc28j60_SendActivate(  void  )
{
  enc28j60_BIT_SET (ECON1, ECON1_TXRTS);// �������Ϸ��ͷ��ͻ�����������
 // ��λ�����߼������⡣�μ� Rev. B4 Silicon Errata point 12.
 if( (enc28j60Read(EIR) & EIR_TXERIF) )
 {
  enc28j60SetBank(ECON1);
  enc28j60_BIT_CLR ( ECON1, ECON1_TXRTS);
  enc28j60_BIT_CLR ( ECON1, ECON1_TXRST);
 }
}
/****************************************************************************
* ��    �ƣ�unsigned int enc28j60PacketReceive(unsigned int maxlen, unsigned char* )
* ��    �ܣ�																		 
* ��ڲ�������������ջ�������ȡһ��
			maxlen�� �����������ݰ������ɽ��ܵĳ���
			packet:  ���ݰ���ָ��													
* ���ڲ���: ���һ�����ݰ��յ��������ݰ����ȣ����ֽ�Ϊ��λ������Ϊ�㡣
* ˵    ����
* ���÷�����
****************************************************************************/ 
void  get_enc28j60Packet_state( unsigned char * buff )
{
 unsigned char i;
 if((ERDPTL & BANK_MASK) != Enc28j60Bank)
 {
  clr_ENC28J60_SCpin;	                     		
  WriteReadByte_enc28j60(ENC28J60_BIT_FIELD_CLR|(ECON1 & ADDR_MASK));
  WriteReadByte_enc28j60(ECON1_BSEL1|ECON1_BSEL0);			
  set_ENC28J60_SCpin;						 

  clr_ENC28J60_SCpin; 		
  WriteReadByte_enc28j60(ENC28J60_BIT_FIELD_SET | (ECON1 & ADDR_MASK));			
  WriteReadByte_enc28j60((ERDPTL & BANK_MASK)>>5);		
  set_ENC28J60_SCpin;
  Enc28j60Bank = (ERDPTL & BANK_MASK);
 }
 clr_ENC28J60_SCpin;	                   		
 WriteReadByte_enc28j60(ENC28J60_WRITE_CTRL_REG | (ERDPTL & ADDR_MASK));
 WriteReadByte_enc28j60(NextPacketPtr);		
 set_ENC28J60_SCpin;
 
 clr_ENC28J60_SCpin;	                   		
 WriteReadByte_enc28j60(ENC28J60_WRITE_CTRL_REG | (ERDPTH & ADDR_MASK));
 WriteReadByte_enc28j60( (NextPacketPtr) >> 8 );	
 set_ENC28J60_SCpin;						 

 clr_ENC28J60_SCpin ;
 i= 0 ;
 WriteReadByte_enc28j60 ( ENC28J60_READ_BUF_MEM );
 while (i < 6)
 {
  buff [i]= WriteReadByte_enc28j60 (0xFF);
  i ++ ;
 }
 set_ENC28J60_SCpin ;
}
/****************************************************************************
* ��    �ƣ�unsigned int enc28j60PacketReceive(unsigned int maxlen, unsigned char* )
* ��    �ܣ�																		 
* ��ڲ�������������ջ�������ȡһ��
			maxlen�� �����������ݰ������ɽ��ܵĳ���
			packet:  ���ݰ���ָ��													
* ���ڲ���: ���һ�����ݰ��յ��������ݰ����ȣ����ֽ�Ϊ��λ������Ϊ�㡣
* ˵    ����
* ���÷�����
****************************************************************************/ 
void  free_enc28j60Packet( unsigned char * adrr )
{
   //RX��ָ���ƶ�����һ�����յ������ݰ��Ŀ�ʼλ�� �ͷ����ǸղŶ��������ڴ�
 enc28j60Write(ERXRDPTL, adrr[0]);
 enc28j60Write(ERXRDPTH, adrr[1]);
 NextPacketPtr = adrr[1];
 NextPacketPtr <<= 8;
 NextPacketPtr |= adrr[0];
 //�ݼ����ݰ���������־�����Ѿ��õ�������� 
 enc28j60_BIT_SET(ECON2, ECON2_PKTDEC);
} 



/****************************************************************************
* ��    �ƣ�
* ��    �ܣ�																		 
* ��ڲ�����
			
															
* ���ڲ���: 
* ˵    ����
* ���÷�����
****************************************************************************/ 
void INIT_Confi_enc28j60 (void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOC, ENABLE);
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5);

  EXTI_InitStructure.EXTI_Line = EXTI_Line5;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		   
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel =EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
  
}





