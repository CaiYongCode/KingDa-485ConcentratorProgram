/**********************************************************************************
//概述： 
//作者：大胡子书生      时间： 2011-06-27  16：30   地点：鼎科研发部
//CPU型号：    系统主频：
//版本号：
***********************************************************************************/
/**********************************************************************************
文件包含区
***********************************************************************************/
#include ".\modbus_master.h"
/**********************************************************************************
常量定义区
***********************************************************************************/

/* CRC 高位字节值表 */ 

static const unsigned char  auchCRCHi[] = { 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40 
} ;  
/* CRC低位字节值表*/
static const unsigned char  auchCRCLo[] = { 
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 
0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 
0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 
0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 
0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 
0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 
0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 
0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 
0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 
0x43, 0x83, 0x41, 0x81, 0x80, 0x40 
} ;	 
/**********************************************************************************
公共变量定义去
***********************************************************************************/
unsigned short  message_name = 0;
/**********************************************************************************
私有变量定义区
***********************************************************************************/
static unsigned char  Mastermodbus_receBuf[Mastermodbus_receBuf_Len];             //接受缓冲区
static unsigned char  Mastermodbus_sendBuf[Mastermodbus_sendBuf_Len+1]={0};       //发送缓冲区
static unsigned char  Mastermodbus_sendBuf_sign[Mastermodbus_sendBuf_Len+1]={0};  //发送影子缓冲区
//static unsigned char  receCount = 0;	//接收到的字节个数
static unsigned char  Mastermodbus_RCOM=0;     //主机收到完整数据包标志
static unsigned char  Mastermodbus_sendBuf_flg = EMPTY;
static unsigned char  SendFunction_again_N=0;  //重发次数计时器；
static unsigned char  modbus_receTime=0;
//static unsigned char* TxAddress;	/* UART6 transfer buffer address */
static unsigned char  TxCnt = 0;		/* UART6 transfer number */
static unsigned char  modbus_ACK=TURE;       //命令应答标志   TUER 应答完成 FLASE 尚未收到有效应答
static unsigned short modbus_ACKTime=0;     //应答超时 计时器
 unsigned short        slave_sCan_T = 0;

unsigned short slave_err_modbus = 0; 
/**********************************************************************************
外不变量声明区
***********************************************************************************/
/**********************************************************************************
测试变量定义区
***********************************************************************************/
//unsigned short modbus_test = 0xbb;
/**********************************************************************************
功能代码定义区
***********************************************************************************/
/*************************************************
  Function:       // UART6_Init 
  Description:    // 串口6 配置
  Calls:          // 
  Called By:      // 
  Table Accessed: // 
  Table Updated:  // 
  Input:          // 
                  // 
  Output:         // 
  Return:         // 
  Others:         // 数据位 8 奇偶校验 无 
                  // 波特率 19200  编码格式 低位在前 
*************************************************/
void masterModbus_Init( void )
{
}
/******************************************************************
  Function:       // modbus_send
  Description:    // MODBUS 发送函数
  Calls:          //
  Called By:      //
  Table Accessed: //
  Table Updated:  //
  Input:          // 
                  //
  Output:         //
  Return:         //
  Others:         //
*******************************************************************/
unsigned char mastermodbus_send(unsigned char* sendBuf, unsigned char sendLen)
{
 unsigned char itemp = 0;
 //itemp = USART1_SendData(  sendBuf, sendLen );
 return itemp;
}
////if( 0 !=TxCnt)
// { return 0;}
//TxAddress = sendBuf;
//TxCnt = sendLen;
//if( TxCnt >= 1)
//  {
//   Modbus_TXRegister_M = *TxAddress;
//   TxAddress++;
//   TxCnt--; 
//  }
/******************************************************************
  Function:       // UARTSR__interrupt
  Description:    // 串口接受中断
  Calls:          //
  Called By:      //
  Table Accessed: //
  Table Updated:  //
  Input:          // 
                  //
  Output:         //
  Return:         //
  Others:         //用于Modbus通信的串口 
*******************************************************************/
//void masterModbus__SRinterrupt( void )
//{
// modbus_receTime = 0 ;
// if(receCount < Mastermodbus_receBuf_Len)
//  {
//   Mastermodbus_receBuf[receCount] = Modbus_RXRegister_M;
//   receCount++;
//   if(receCount >= 4)
//     {
//      if( receCount == ( Mastermodbus_receBuf[2]+5 ) )//数据包未接收完成返回
//        { Mastermodbus_RCOM = 1; }
//     }
//   }
//}
/******************************************************************
  Function:       // UARTTX__interrupt
  Description:    // 串口发送中断
  Calls:          //
  Called By:      //
  Table Accessed: //
  Table Updated:  //
  Input:          // 
                  //
  Output:         //
  Return:         //
  Others:         //用于Modbus通信的串口 
*******************************************************************/
void masterModbus__TXinterrupt (void)
{
// if( TxCnt > 0 )
//     {
//      Modbus_TXRegister_M  = *TxAddress++;
//      TxCnt--;
//     }
}
/******************************************************************
  Function:       // readmultiRegister_M
  Description:    // 获取来自从机一组寄存器数据
  Calls:          //
  Called By:      //
  Table Accessed: //
  Table Updated:  //
  Input:          // 
                  //
  Output:         //
  Return:         //
  Others:         //用于Modbus通信的串口 
*******************************************************************/
unsigned char readmultiRegister_M( void)
{
// unsigned short addr;
// unsigned short tempdata;
// unsigned char tempp=3;
// unsigned char dataCount;
// unsigned char itemp ;
// addr = Mastermodbus_sendBuf_sign[2];
// addr<<=8;
// addr += Mastermodbus_sendBuf_sign[3];
//
// dataCount = Mastermodbus_receBuf[2] >> 1;
//
//
//
// if(Mastermodbus_receBuf[0]!= Mastermodbus_sendBuf_sign[0])
// {return 0;}
//
// if(dataCount != Mastermodbus_sendBuf_sign[5])
// {return 0;}
//
//
//
//  if(Mastermodbus_sendBuf_sign[0]  == SLAVE_data[0].adrr )
//  {
//    dataCount %= 11;
//	itemp = 0;
//	while (dataCount --)
//	{
//	  tempdata = Mastermodbus_receBuf[tempp ++ ];
//      tempdata <<= 8;
//      tempdata += Mastermodbus_receBuf[tempp ++]; 
//	  SLAVE_data_buff1 [ itemp ++ ] = tempdata;
//	}
//	return 1;
//  }
//  if(Mastermodbus_sendBuf_sign[0]  == SLAVE_data[1].adrr )
//  {
//    dataCount %= 11;
//	itemp = 0;
//	while (dataCount --)
//	{
//	  tempdata = Mastermodbus_receBuf[tempp ++ ];
//      tempdata <<= 8;
//      tempdata += Mastermodbus_receBuf[tempp ++]; 
//	  SLAVE_data_buff2 [ itemp ++ ] = tempdata;
//	}
//	return 1;
//  }
//  if(Mastermodbus_sendBuf_sign[0]  == SLAVE_data[2].adrr )
//  {
//    dataCount %= 11;
//	itemp = 0;
//	while (dataCount --)
//	{
//	  tempdata = Mastermodbus_receBuf[tempp ++ ];
//      tempdata <<= 8;
//      tempdata += Mastermodbus_receBuf[tempp ++]; 
//	  SLAVE_data_buff3 [ itemp ++ ] = tempdata;
//	}
//		return 1;
//  }
// if(Mastermodbus_sendBuf_sign[0]  == SLAVE_data[3].adrr )
//  {
//    dataCount %= 11;
//	itemp = 0;
//	while (dataCount --)
//	{
//	  tempdata = Mastermodbus_receBuf[tempp ++ ];
//      tempdata <<= 8;
//      tempdata += Mastermodbus_receBuf[tempp ++]; 
//	  SLAVE_data_buff4 [ itemp ++ ] = tempdata;
//	}
//	return 1;
//  }
// if(Mastermodbus_sendBuf_sign[0]  == SLAVE_data[4].adrr )
//  {
//    dataCount %= 11;
//	itemp = 0;
//	while (dataCount --)
//	{
//	  tempdata = Mastermodbus_receBuf[tempp ++ ];
//      tempdata <<= 8;
//      tempdata += Mastermodbus_receBuf[tempp ++]; 
//	  SLAVE_data_buff5 [ itemp ++ ] = tempdata;
//	}
//		return 1;
//  }
// if(Mastermodbus_sendBuf_sign[0]  == SLAVE_data[5].adrr )
//  {
//    dataCount %= 11;
//	itemp = 0;
//	while (dataCount --)
//	{
//	  tempdata = Mastermodbus_receBuf[tempp ++ ];
//      tempdata <<= 8;
//      tempdata += Mastermodbus_receBuf[tempp ++]; 
//	  SLAVE_data_buff6 [ itemp ++ ] = tempdata;
//	}
//  return 1;
//  }
//   if(Mastermodbus_sendBuf_sign[0]  == SLAVE_data[6].adrr )
//  {
//    dataCount %= 11;
//	itemp = 0;
//	while (dataCount --)
//	{
//	  tempdata = Mastermodbus_receBuf[tempp ++ ];
//      tempdata <<= 8;
//      tempdata += Mastermodbus_receBuf[tempp ++]; 
//	  SLAVE_data_buff7 [ itemp ++ ] = tempdata;
//	}
//	return 1;
//  }
//   if(Mastermodbus_sendBuf_sign[0]  == SLAVE_data[7].adrr )
//  {
//    dataCount %= 11;
//	itemp = 0;
//	while (dataCount --)
//	{
//	  tempdata = Mastermodbus_receBuf[tempp ++ ];
//      tempdata <<= 8;
//      tempdata += Mastermodbus_receBuf[tempp ++]; 
//	  SLAVE_data_buff8 [ itemp ++ ] = tempdata;
//	}
//		return 1;
//  }
  return 0;
}
/******************************************************************
  Function:       // checkMasterModbus
  Description:    // 设置单个寄存器确认
  Calls:          //
  Called By:      //
  Table Accessed: //
  Table Updated:  //
  Input:          // 
                  //
  Output:         //
  Return:         //
  Others:         //用于Modbus通信的串口 
*******************************************************************/
void presetSingleRegister_ACK()
{
  unsigned char i;
  for(i=0;i<6;i++)
    {
      if(Mastermodbus_receBuf[i]!=Mastermodbus_sendBuf_sign[i])
      {break;}
     }
  if(i<6)
  {}
  else
  {modbus_ACK=TURE;}
}
/******************************************************************
  Function:       // CRC_16
  Description:    // 计算数据序列的16位CRC校验码
  Calls:          //
  Called By:      //
  Table Accessed: //
  Table Updated:  //
  Input:          //crc16:数据序列的首地址 
                  //usDataLen:数据序列的长度
  Output:         //
  Return:         //数据序列的CRC校验码
  Others:         //CRC16=X16+X15+X5+1    
                  //数据序列最长255字节
*******************************************************************/
static unsigned short crc16(unsigned char *puchMsg, unsigned char usDataLen) 
{ 
 unsigned char uchCRCHi = 0xFF ; // 高CRC字节初始化 
 unsigned char uchCRCLo = 0xFF ; // 低CRC 字节初始化  
 unsigned char  uIndex ; // CRC循环中的索引  
 while (usDataLen--) // 传输消息缓冲区  
   { 
    uIndex = uchCRCHi ^ *puchMsg++ ; // 计算CRC  
    uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ; 
    uchCRCLo = auchCRCLo[uIndex] ; 
   } 
 return (uchCRCHi << 8 | uchCRCLo) ; 
}  
/******************************************************************
  Function:       // checkMasterModbus
  Description:    // 主机模式接收服务函数
  Calls:          //
  Called By:      //
  Table Accessed: //
  Table Updated:  //
  Input:          // 
                  //
  Output:         //
  Return:         //
  Others:         //用于Modbus通信的串口 
*******************************************************************/
void checkMasterModbus(void)
{
  unsigned short crcData;
  unsigned char  crclen;
  unsigned short  crc_originalData;
  if(1!=Mastermodbus_RCOM)
   { return;}
  crclen=Mastermodbus_receBuf[2]+3;                //计算需要CRC数据序列的长度
  crcData = crc16(Mastermodbus_receBuf,crclen);    //计算CRC校验码
  //得到数据包自带CRC校验码
  crc_originalData=Mastermodbus_receBuf[crclen];         
  crc_originalData<<=8;
  crc_originalData+=Mastermodbus_receBuf[crclen+1];
 if(crcData!=crc_originalData)
  {
   //rece_crcEorr();
  Mastermodbus_RCOM=0;

  //receCount=0;
  
  }
 else
 { 
  switch(Mastermodbus_sendBuf_sign[1])    //Mastermodbus_receBuf[1]
   {
    case 1://返回的是线圈状态
      { 
       // readCoil();
       break;
      }
    case 3://返回的是寄存器的值
      {
       if( 0 != readmultiRegister_M())            
       {modbus_ACK=TURE; }

       break;
      }
    case 5://强制单个线圈
       break;
    case 6://设置单个寄存器
      {
       presetSingleRegister_ACK();     
       ///receCount = 0;
       break;
      }
    default:
      break;
    }
  if((com_err == message_name)&&(TURE == modbus_ACK))
    {message_name = 0;}
  if((slave_err_modbus == Mastermodbus_sendBuf_sign[0]) && (TURE == modbus_ACK))
    {slave_err_modbus = 0; }
  }     
  Mastermodbus_RCOM=0;
  ///receCount=0;
  
 }
/******************************************************************
  Function:       //  SendFunction
  Description:    // 主机模式发送命令服务函数
  Calls:          //
  Called By:      //
  Table Accessed: //
  Table Updated:  //
  Input:          // 
                  //
  Output:         //
  Return:         //
  Others:         //用于Modbus通信的串口 
*******************************************************************/
unsigned char masterSendFunction( unsigned char addr,  unsigned char  function,
                            unsigned char data1, unsigned char  data2,
                            unsigned char data3, unsigned char  data4    )
{
//  unsigned char byte_cnt=0;
//  unsigned short crcData; 
// if( EMPTY == Mastermodbus_sendBuf_flg )
//   {
//    Mastermodbus_sendBuf[0]=addr;
//    Mastermodbus_sendBuf[1]=function;
//    switch(function)
//    {
//     case 1:
//     case 2:
//     case 3:
//     case 4:
//     case 5:
//     case 6:
//      {
//       Mastermodbus_sendBuf[2]=data1;
//       Mastermodbus_sendBuf[3]=data2;
//       Mastermodbus_sendBuf[4]=data3;
//       Mastermodbus_sendBuf[5]=data4;
//       byte_cnt=6;
//       break;
//      }
//    default:
//       break;  
//     
//    }
//   crcData = crc16(Mastermodbus_sendBuf,6);
//   Mastermodbus_sendBuf[byte_cnt++]=crcData>>8;
//   Mastermodbus_sendBuf[byte_cnt++]=crcData&0x0ff;
//   Mastermodbus_sendBuf[Mastermodbus_sendBuf_Len]=byte_cnt;
//   Mastermodbus_sendBuf_flg=FULL;
//   return  OK;
//   }
// else
//  { return  NG;}
  return  1;
}
/******************************************************************
  Function:       //  
  Description:    // 
  Calls:          //
  Called By:      //
  Table Accessed: //
  Table Updated:  //
  Input:          // 
                  //
  Output:         //
  Return:         //
  Others:         // 
*******************************************************************/
void modbusmaster_send(void)
{
  unsigned char i ,byte_cnt;
  byte_cnt= Mastermodbus_sendBuf[Mastermodbus_sendBuf_Len];
  if((0==TxCnt)&&(modbus_ACK==TURE))
    {
     /*将发送数据送入影子缓冲区*/
     for (i=0; i<=byte_cnt; i++)
     {Mastermodbus_sendBuf_sign[i]=Mastermodbus_sendBuf[i];}
     Mastermodbus_sendBuf_sign[Mastermodbus_sendBuf_Len]=byte_cnt;
     Mastermodbus_sendBuf_flg = EMPTY;
     modbus_ACK=FLASE;
     //UART6_SendData( Mastermodbus_sendBuf_sign, byte_cnt );
     mastermodbus_send(Mastermodbus_sendBuf_sign, byte_cnt);
    }
}
/******************************************************************
  Function:       //  LinkFailure
  Description:    //  modebus 连接失败处理函数
  Calls:          //
  Called By:      //
  Table Accessed: //
  Table Updated:  //
  Input:          // 
                  //
  Output:         //
  Return:         //
  Others:         // 
*******************************************************************/
void LinkFailure(void)
{
  message_name = com_err;

  modbus_ACK = TURE;

  slave_err_modbus =  Mastermodbus_sendBuf_sign[0];
}
/******************************************************************
  Function:       //  modbus_10ms
  Description:    //  modebus 10ms中断服务程序
  Calls:          //
  Called By:      //
  Table Accessed: //
  Table Updated:  //
  Input:          // 
                  //
  Output:         //
  Return:         //
  Others:         // 
*******************************************************************/
//void mastermodbus_send(unsigned char* sendBuf, unsigned char sendLen)
//{
  //UART6_SendData( sendBuf, sendLen );
//}

/******************************************************************
  Function:       //  
  Description:    //  
  Calls:          //
  Called By:      //
  Table Accessed: //
  Table Updated:  //
  Input:          // 
                  //
  Output:         //
  Return:         //
  Others:         // 
*******************************************************************/
void SendFunction_again(void)
{
 mastermodbus_send(Mastermodbus_sendBuf_sign, Mastermodbus_sendBuf_sign[Mastermodbus_sendBuf_Len]);
 //mastermodbus_send(Mastermodbus_sendBuf_sign, byte_cnt);
}
/******************************************************************
  Function:       //  modbus_10ms
  Description:    //  modebus 10ms中断服务程序
  Calls:          //
  Called By:      //
  Table Accessed: //
  Table Updated:  //
  Input:          // 
                  //
  Output:         //
  Return:         //
  Others:         // 
*******************************************************************/
void mastermodbus_10ms(void)
{
 modbus_receTime++;
 slave_sCan_T ++;
 //接收流中断超过规定时间,认为数据帧结束
 if(modbus_receTime>10)
 { } //receCount=0; }
 //当主机发送反冲区有数据发送出去
 if(FULL==Mastermodbus_sendBuf_flg)
   {  modbusmaster_send(); }
 //等待应答计时
 if((0==TxCnt)&&(FLASE==modbus_ACK))
   {  modbus_ACKTime++; }
 else
   { modbus_ACKTime=0;  }
 //应答超时重发
 if(modbus_ACKTime>10)
   {
    modbus_ACKTime = 0;
    //连续重发次数超过上限通信报错否则继续重发
    if(SendFunction_again_N>SendFunction_again_N_MAX)
      {
        LinkFailure();
        SendFunction_again_N=0;
       }
     else
       {
        SendFunction_again();
        SendFunction_again_N++;
       }
     } 
}

/******************************************************************
  Function:       //  modbus_10ms
  Description:    //  modebus 10ms中断服务程序
  Calls:          //
  Called By:      //
  Table Accessed: //
  Table Updated:  //
  Input:          // 
                  //
  Output:         //
  Return:         //
  Others:         // 
*******************************************************************/
//void Slave_scan(void)
//{
// static	 unsigned char scan_step = 0;
// unsigned char itemp = 0;
// if(100 > slave_sCan_T)
//  {return;}
// slave_sCan_T = 0;
// itemp = masterSendFunction( 0x01,0x03,0x00,scan_step,0x00,0x01 );
// if( OK == itemp )
// {
//  scan_step ++;
//  scan_step %= 27;
// }
//}
/******************************************************************
  Function:       //  SetSlave_oneRegister
  Description:    //  设置从机指定地址寄存器值
  Input:          //  addr_slave ：从机地址
                  //  addr_data  ；寄存器地址
                  //  data       ； 数据
  Output:         //
  Return:         //
  Others:         // 
*******************************************************************/
void SetSlave_oneRegister ( unsigned short addr_slave, unsigned short addr_reg, unsigned short data )
{
  unsigned char addr_H;
  unsigned char addr_L;
  unsigned char data_H;
  unsigned char data_L;
  
  addr_L = addr_reg & 0x00ff;
  addr_H = addr_reg >> 8;
  data_L = data & 0x00ff;
  data_H = data >> 8;
  //while ( EMPTY != Mastermodbus_sendBuf_flg  )
  // {checkMasterModbus();}
   Mastermodbus_sendBuf_flg = EMPTY;  //缓冲区数据作废
   masterSendFunction( addr_slave,0x06,addr_H, addr_L, data_H,data_L) ; 
}

