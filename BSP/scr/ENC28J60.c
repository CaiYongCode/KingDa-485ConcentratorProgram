/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "ENC28J60.h"
/*********************************************************************************
常量定义区
*********************************************************************************/
const unsigned char MAC_ENC28J60 [6] ={0x9B,0xAC,0x3E,0x8A,0x42,0x69};
/*********************************************************************************
公共变量定义区
*********************************************************************************/
unsigned short enc28j60Init_cnt = 0;
/*********************************************************************************
外部变量声明区
*********************************************************************************/

/*********************************************************************************
私有变量定义区
*********************************************************************************/
static unsigned char txbuff = 0xff;
static unsigned char Enc28j60Bank;
static unsigned int NextPacketPtr;
/*********************************************************************************
测试变量定义区
*********************************************************************************/

/*********************************************************************************
内部函数定义区
*********************************************************************************/
void INIT_Confi_enc28j60 (void);
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
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(SPI1->DR));         //设置  接收外设(0x4001300C) 地址(源地址)
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)buff;                      //设置 SRAM 存储地址(源地址)
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                                //传输方向 内存-外设
  DMA_InitStructure.DMA_BufferSize = length ;                           //设置 SPI1 接收长度
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;                  //外设地址增量(不变)
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                           //内存地址增量(变化)
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;           //外设传输宽度(字节)
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;                   //内存传输宽度(字节)
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                                     //传输方式,一次传输完停止,不重新加载
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                           //中断方式-高(三级)
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                      //内存到内存方式禁止
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
 DMA1_Channel3->CCR   = ( (1ul<< 0)      //1：通道开启
						 |(1ul<< 4));    //1：从存储器读
 DMA1_Channel2->CCR   = 0;
 DMA1_Channel2->CMAR  = (u32)buff;
 DMA1_Channel2->CNDTR = length;
 DMA1_Channel2->CCR   = ( (1ul<< 0)    //1：通道开启
                         |(1ul<< 1)    //允许传输完成中断
						 |(1ul<< 7)	   //1：执行存储器地址增量操作
						 |(2ul<< 12)); //10：通道优先级 高
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
 DMA1_Channel2->CCR   = (1ul<< 0) ;  //1：通道开启
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

 DMA1_Channel4->CCR   = ( (1ul<< 0)    //1：通道开启
//                         |(1ul<< 1)    //允许传输完成中断
						 |(1ul<< 7)	   //1：执行存储器地址增量操作
						 |(3ul<< 12)); //10：通道优先级 高
  DMA1_Channel5->CCR   = ( (1ul<< 0)      //1：通道开启
						 |(1ul<< 4));    //1：从存储器读
 while ( (DMA1 -> ISR & (1ul << 5)) == 0);
 while (SPI2 ->SR & (1ul << 7));
 set_ENC28J60_SCpin ;
}
/****************************************************************************
* 名    称：unsigned char enc28j60ReadOp(unsigned char op, unsigned char address)
* 功    能：ENC28J60读寄存器
* 入口参数：
* 出口参数：无
* 说    明：
* 调用方法：
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
* 名    称：
* 功    能：ENC28J60 寄存器操作函数
* 入口参数：op 	 address  data
* 出口参数：无
* 说    明：
* 调用方法：
****************************************************************************/ 
void enc28j60WriteCR( unsigned char address, unsigned char data)
{
 unsigned char dat = 0;
 clr_ENC28J60_SCpin;	                      //使能ENC28J60 SPI片选  		
 dat = ENC28J60_WRITE_CTRL_REG | (address & ADDR_MASK);	  //OP--3位操作码 (address & ADDR_MASK)--5位参数
 WriteReadByte_enc28j60(dat);				  //SPI1 写
 WriteReadByte_enc28j60(data);				  //SPI1 写操作数据
 set_ENC28J60_SCpin;						  //禁止ENC28J60 SPI片选  完成操作
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
* 名    称：
* 功    能：ENC28J60 寄存器操作函数
* 入口参数：op 	 address  data
* 出口参数：无
* 说    明：
* 调用方法：
****************************************************************************/ 
void enc28j60_BIT_SET( unsigned char address, unsigned char bitflg)
{
 unsigned char dat = 0;
 clr_ENC28J60_SCpin;	                      //使能ENC28J60 SPI片选  		
 dat = ENC28J60_BIT_FIELD_SET | (address & ADDR_MASK);	  //OP--3位操作码 (address & ADDR_MASK)--5位参数
 WriteReadByte_enc28j60(dat);				  //SPI1 写
 WriteReadByte_enc28j60(bitflg);				  //SPI1 写操作数据
 set_ENC28J60_SCpin;						  //禁止ENC28J60 SPI片选  完成操作
}
/****************************************************************************
* 名    称：
* 功    能：ENC28J60 寄存器操作函数
* 入口参数：op 	 address  data
* 出口参数：无
* 说    明：
* 调用方法：
****************************************************************************/ 
void enc28j60_BIT_CLR( unsigned char address, unsigned char bitflg)
{
 unsigned char dat = 0;
 clr_ENC28J60_SCpin;	                      //使能ENC28J60 SPI片选  		
 dat = ENC28J60_BIT_FIELD_CLR | (address & ADDR_MASK);	  //OP--3位操作码 (address & ADDR_MASK)--5位参数
 WriteReadByte_enc28j60(dat);				  //SPI1 写
 WriteReadByte_enc28j60(bitflg);				  //SPI1 写操作数据
 set_ENC28J60_SCpin;						  //禁止ENC28J60 SPI片选  完成操作
}
/****************************************************************************
* 名    称：void enc28j60SetBank(unsigned char address)
* 功    能：ENC28J60 设置寄存器BANK
* 入口参数：
* 出口参数：无
* 说    明：
* 调用方法：
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
* 名    称：unsigned char enc28j60Read(unsigned char address)
* 功    能：读取指定寄存器的数值
* 入口参数：
* 出口参数：无
* 说    明：
* 调用方法：
****************************************************************************/ 
unsigned char enc28j60Read(unsigned char address)
{
 enc28j60SetBank(address); // set the bank
 return enc28j60ReadCR(address);	 	// do the read
}
/****************************************************************************
* 名    称：void enc28j60Write(unsigned char address, unsigned char data)
* 功    能：向指定寄存器写入数值
* 入口参数：
* 出口参数：无
* 说    明：
* 调用方法：
****************************************************************************/ 
void enc28j60Write(unsigned char address, unsigned char data)
{
 enc28j60SetBank(address);           // set the bank
 enc28j60WriteCR(address, data);	 // do the write
}
/****************************************************************************
* 名    称：void enc28j60PhyWrite(unsigned char address, unsigned int data)
* 功    能：向指定PHY寄存器写入数值
* 入口参数：
* 出口参数：无
* 说    明：
* 调用方法：
****************************************************************************/ 
void enc28j60PhyWrite(unsigned char address, unsigned int data)
{
 unsigned long itemp = 0;
 enc28j60Write(MIREGADR, address); 	// set the PHY register address
 // write the PHY data
 enc28j60Write(MIWRL, data);
 enc28j60Write(MIWRH, data>>8);
 while(enc28j60Read(MISTAT) & MISTAT_BUSY)	 // 等待PHY寄存器写入完成
 {
  if(itemp ++ > 0x4fffff){break;}
 }
}
/****************************************************************************
* 名    称：void enc28j60clkout(unsigned char clk)
* 功    能：设置ENC28J60时钟输出频率
* 入口参数：
* 出口参数：无
* 说    明：
* 调用方法：
****************************************************************************/ 
void enc28j60clkout(unsigned char clk)
{
 enc28j60Write(ECOCON, clk & 0x7);
}
/****************************************************************************
* 名    称：void enc28j60Init(unsigned char* macaddr)
* 功    能：ENC28J60初始化 
* 入口参数：*macaddr--6个字节的MAC地址
* 出口参数：无
* 说    明：
* 调用方法：
****************************************************************************/ 
void Launch_enc28j60 (void )
{ 
 unsigned char flg =0;
 unsigned char cntA = 0;
 
 EthernetPowerON_Order ( );
 DelymS(1);
 SPI1_Configuration ( ); 
 clr_ENC28J60_SCpin;
 WriteReadByte_enc28j60(ENC28J60_SOFT_RESET);	  //ENC28J60软件复位 系统命令
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
 enc28j60Write(ERXSTL, RXSTART_INIT&0xFF);	 //接收缓冲区起始地址
 enc28j60Write(ERXSTH, RXSTART_INIT>>8);
 enc28j60Write(ERXNDL, RXSTOP_INIT&0xFF);	 //接收缓冲区结束地址
 enc28j60Write(ERXNDH, RXSTOP_INIT>>8);
 enc28j60Write(ETXSTL, TXSTART_INIT&0xFF);	 //发送缓冲区起始地址
 enc28j60Write(ETXSTH, TXSTART_INIT>>8);	 
 enc28j60Write(ETXNDL, TXSTOP_INIT&0xFF);    //发送缓冲区结束地址
 enc28j60Write(ETXNDH, TXSTOP_INIT>>8);
 enc28j60Write(ERXFCON, ERXFCON_UCEN|ERXFCON_CRCEN|
                        ERXFCON_MCEN|ERXFCON_BCEN |ERXFCON_PMEN);
                                             //过滤器匹配设置 
 enc28j60Write(EPMM0, 0x3f);	 //格式匹配屏蔽字节0
 enc28j60Write(EPMM1, 0x30);	 //格式匹配屏蔽字节1
 enc28j60Write(EPMCSL, 0xf9);	 //格式匹配校验和低字节
 enc28j60Write(EPMCSH, 0xf7);	 //格式匹配校验和高字节

 enc28j60Write(MACON1, MACON1_MARXEN|MACON1_TXPAUS|MACON1_RXPAUS);	 //MAC控制寄存器1
 enc28j60Write(MACON2, 0x00);										 //MAC 控制寄存器2
 enc28j60_BIT_SET(MACON3, MACON3_PADCFG0|MACON3_TXCRCEN|MACON3_FRMLNEN|MACON3_FULDPX);
                                                                     //MAC 控制寄存器3
 enc28j60Write(MAIPGL, 0x12);  //非背对背包间间隔低字节
 enc28j60Write(MAIPGH, 0x0C);  //非背对背包间间隔高字节
 enc28j60Write(MABBIPG, 0x15); //背对背包间间隔

 enc28j60Write(MAMXFLL, MAX_FRAMELEN&0xFF);	//最大帧长度低字节
 enc28j60Write(MAMXFLH, MAX_FRAMELEN>>8);	//最大帧长度高字节

 enc28j60Write(MAADR5, MAC_ENC28J60[0]);	//MAC 地址字节5
 enc28j60Write(MAADR4, MAC_ENC28J60[1]);        //MAC 地址字节4
 enc28j60Write(MAADR3, MAC_ENC28J60[2]);	//MAC 地址字节3 
 enc28j60Write(MAADR2, MAC_ENC28J60[3]);	//MAC 地址字节2
 enc28j60Write(MAADR1, MAC_ENC28J60[4]);        //MAC 地址字节1
 enc28j60Write(MAADR0, MAC_ENC28J60[5]);        //MAC 地址字节0

 enc28j60PhyWrite(PHCON1, PHCON1_PDPXMD);       //PHY 控制寄存器1
 enc28j60PhyWrite(PHCON2, PHCON2_HDLDIS);       //PHY 控制寄存器2	
                                                //配置PHY为全双工  LEDB为拉电流
 enc28j60SetBank(ECON1);		        //以太网控制寄存器1
 enc28j60_BIT_SET(EIE, EIE_INTIE|EIE_PKTIE);    //EIE_RXERIE//以太网中断允许寄存器
 enc28j60_BIT_SET( ECON1, ECON1_RXEN);           //以太网控制寄存器1  

 INIT_Confi_enc28j60 ( ); 
}
/****************************************************************************
* 名    称：unsigned char enc28j60getrev(void)
* 功    能：获取ENC28J60的版本信息
* 入口参数：
* 出口参数：无
* 说    明：
* 调用方法：
****************************************************************************/ 
unsigned char enc28j60getrev(void)
{return(enc28j60Read(EREVID));}
/****************************************************************************
* 名    称：void enc28j60PacketSend(unsigned int len, unsigned char* packet)
* 功    能：通过ENC28J60发送数据
* 入口参数：
* 出口参数：
* 说    明：
* 调用方法：
****************************************************************************/ 
void enc28j60PacketSend(unsigned char* packet, unsigned short len)
{
 // 设置发送缓冲区地址写指针入口 Set the write pointer to start of transmit buffer area
 while((enc28j60Read(ECON1) & ECON1_TXRTS)!=0); //    
 enc28j60Write(EWRPTL, TXSTART_INIT&0xFF);
 enc28j60Write(EWRPTH, TXSTART_INIT>>8);
 //设置TXND指针，以对应给定的数据包大小	   
 enc28j60Write(ETXNDL, (TXSTART_INIT+len)&0xFF);
 enc28j60Write(ETXNDH, (TXSTART_INIT+len)>>8);

 clr_ENC28J60_SCpin ;
 WriteReadByte_enc28j60 (ENC28J60_WRITE_BUF_MEM);
 WriteReadByte_enc28j60  (  0x00 ); 
 set_ENC28J60_SCpin ;


 enc28j601_WBM_buff(packet ,len);  	 // 将数据包复制到发送缓冲区
 enc28j60_BIT_SET (ECON1, ECON1_TXRTS);// 在网络上发送发送缓冲区的内容
 // 复位发送逻辑的问题。参见 Rev. B4 Silicon Errata point 12.
 if( (enc28j60Read(EIR) & EIR_TXERIF) )
 {
  enc28j60SetBank(ECON1);
  enc28j60_BIT_CLR ( ECON1, ECON1_TXRTS);
  enc28j60_BIT_CLR ( ECON1, ECON1_TXRST);
 }
}
/****************************************************************************
* 名    称：void enc28j60PacketSend(unsigned int len, unsigned char* packet)
* 功    能：通过ENC28J60发送数据
* 入口参数：
* 出口参数：
* 说    明：
* 调用方法：
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
 //设置TXND指针，以对应给定的数据包大小	   
 enc28j60Write(ETXNDL, (TXSTART_INIT+len)&0xFF);
 enc28j60Write(ETXNDH, (TXSTART_INIT+len)>>8);

 clr_ENC28J60_SCpin ;
 WriteReadByte_enc28j60 (ENC28J60_WRITE_BUF_MEM);
 WriteReadByte_enc28j60  (  0x00 ); 
 set_ENC28J60_SCpin ;
}
/****************************************************************************
* 名    称：void enc28j60PacketSend(unsigned int len, unsigned char* packet)
* 功    能：通过ENC28J60发送数据
* 入口参数：
* 出口参数：
* 说    明：
* 调用方法：
****************************************************************************/ 
void enc28j60_SendActivate(  void  )
{
  enc28j60_BIT_SET (ECON1, ECON1_TXRTS);// 在网络上发送发送缓冲区的内容
 // 复位发送逻辑的问题。参见 Rev. B4 Silicon Errata point 12.
 if( (enc28j60Read(EIR) & EIR_TXERIF) )
 {
  enc28j60SetBank(ECON1);
  enc28j60_BIT_CLR ( ECON1, ECON1_TXRTS);
  enc28j60_BIT_CLR ( ECON1, ECON1_TXRST);
 }
}
/****************************************************************************
* 名    称：unsigned int enc28j60PacketReceive(unsigned int maxlen, unsigned char* )
* 功    能：																		 
* 入口参数：从网络接收缓冲区获取一包
			maxlen： 检索到的数据包的最大可接受的长度
			packet:  数据包的指针													
* 出口参数: 如果一个数据包收到返回数据包长度，以字节为单位，否则为零。
* 说    明：
* 调用方法：
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
* 名    称：unsigned int enc28j60PacketReceive(unsigned int maxlen, unsigned char* )
* 功    能：																		 
* 入口参数：从网络接收缓冲区获取一包
			maxlen： 检索到的数据包的最大可接受的长度
			packet:  数据包的指针													
* 出口参数: 如果一个数据包收到返回数据包长度，以字节为单位，否则为零。
* 说    明：
* 调用方法：
****************************************************************************/ 
void  free_enc28j60Packet( unsigned char * adrr )
{
   //RX读指针移动到下一个接收到的数据包的开始位置 释放我们刚才读出过的内存
 enc28j60Write(ERXRDPTL, adrr[0]);
 enc28j60Write(ERXRDPTH, adrr[1]);
 NextPacketPtr = adrr[1];
 NextPacketPtr <<= 8;
 NextPacketPtr |= adrr[0];
 //递减数据包计数器标志我们已经得到了这个包 
 enc28j60_BIT_SET(ECON2, ECON2_PKTDEC);
} 



/****************************************************************************
* 名    称：
* 功    能：																		 
* 入口参数：
			
															
* 出口参数: 
* 说    明：
* 调用方法：
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





