/**********************************************************************************
//概述： 
//作者：大胡子书生      时间： 2011-06-27  16：30   地点：鼎科研发部
//CPU型号：    系统主频：
//版本号：
***********************************************************************************/
/**********************************************************************************
文件包含区
***********************************************************************************/
#include ".\modbus_slave.h"
#include "..\..\BSP\inc\bsp.h"
/**********************************************************************************
常量定义区
***********************************************************************************/
/* CRC 高位字节值表 */ 
const unsigned char  auchCRCHi[] = { 
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
const unsigned char  auchCRCLo[] = { 
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
const unsigned short localAddr = 7;//(unsigned short *)(ModBus_AddrInROM+ RomStartAddr);	//本机地址；
/**********************************************************************************
私有变量定义区
***********************************************************************************/
unsigned char* slave_receBuf;   //[modbusslave_receBuf_Len]={0};
unsigned char slave_sendBufFlg = 0;
unsigned char slave_sendBuf[modbusslave_sendBuf_Len]; 
unsigned short receCount = 0;	//接收到的字节个数    

unsigned char PortON = 0; 

/**********************************************************************************
外不变量声明区
***********************************************************************************/
/**********************************************************************************
测试变量定义区
***********************************************************************************/

float W5100_TSET = 0; 
/**********************************************************************************
功能代码定义区
***********************************************************************************/
void ModBus_SendOK_CallBack( unsigned char port, void *pbuff);
/**********************************************************************************
  Function:       // UART6_Init 
  Description:    // 串口6 配置
  Input:          // 
                  // 
  Output:         // 
  Return:         // 
  Others:         // 数据位 8 奇偶校验 无 
                  // 波特率 19200  编码格式 低位在前 
**********************************************************************************/
void modbusUART_Init( void )
{
}
/*******************************************************************************
  Function:       // CRC_16
  Description:    // 计算数据序列的16位CRC校验码
  Input:          //crc16:数据序列的首地址 
                  //usDataLen:数据序列的长度
  Output:         //
  Return:         //数据序列的CRC校验码
  Others:         //CRC16=X16+X15+X5+1    
                  //数据序列最长255字节
********************************************************************************/
unsigned short crc16(unsigned char *puchMsg, unsigned char usDataLen) 
{ 
 unsigned char uchCRCHi = 0xFF ;  /* 高CRC字节初始化 */ 
 unsigned char uchCRCLo = 0xFF ;  /* 低CRC 字节初始化 */ 
 unsigned char  uIndex ;          /* CRC循环中的索引 */ 
 while (usDataLen--)              /* 传输消息缓冲区 */ 
  { 
   uIndex = uchCRCHi ^ *puchMsg++ ; /* 计算CRC */ 
   uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ; 
   uchCRCLo = auchCRCLo[uIndex] ; 
  } 
  return (uchCRCHi << 8 | uchCRCLo) ; 
}
/********************************************************************************
  Function:       // 
  Description:    // 
  Input:          // 
                  // 
  Output:         //
  Return:         //
  Others:         //  
********************************************************************************/
void rece_crcEorr(void)
{
 receCount=0;
}
/*******************************************************************************
  Function:       // readRegisters
  Description:    // 读取指定地址寄存器的值
  Input:          // addr：读取的寄存器地址
                  // tempData：读取值的存放地址
  Output:         //
  Return:         //
  Others:         //  
********************************************************************************/
void getCoilVal (unsigned short addr,unsigned short *tempData)
{
 switch(addr & 0xff)
 {
  case 0:
  {
   //if(OV.CCR.Bit.CLR)	    
   //{*tempData = 1;}
   //else
   //{*tempData = 0;}
    break;
   } 
  case 1:
  {
    break;
   }
  case 2:
  {
    break;
   }
  case 3:
  {
    break;
   }
  case 4:
  {
    break;
   }
  case 5:
  {
    break;
   } 
  case 6:
  {
    break;
   }
  case 7:
  {
    break;
   } 
  case 8:
  {
    break;
   } 
  case 9:
  {
    break;
   }    
  case 10:
  {
    break;
   }    
  default: 
  {
   *tempData = 0;
   break;
  }
 }
}
/*************************************************************************************
  Function:       // setRegisterVal
  Description:    // 设置指定地址寄存器的值
  Calls:          //
  Called By:      //
  Table Accessed: //
  Table Updated:  //
  Input:          // addr：设置的寄存器地址
                  // Data：设置值
  Output:         //
  Return:         //
  Others:         //  
**************************************************************************************/
void setCoilVal (unsigned short addr,unsigned short Data)
{
 switch(addr & 0xff)
 {
  case 0:
  {
   //if(Data == 0xff00)
   //{
     //OV.CCR.Bit.CLR  = 1;
     //if(0 == OV.CCR.Bit.ENF_Open)
     //{OV_Start_Close_func ( );}
  // }
   //if(Data == 0x0000)
   //{ 
    // OV.CCR.Bit.CLR  = 0;
     //if((0 == OV.CCR.Bit.ENF_Close))
     //{OV_Start_Open_func ( );}
   //}
  // SAVE_OVdata;
   break;
  }
  case 1:
  {
   break;
  }
  case 2:
  {
   break;
  } 
  case 3:
  {
   break;
  } 
  case 4:
  {
   break;
  } 
  case 5:
  {
   break;
  }
 case 6:
  {
   break;
  }
  default: 
  {break;} 		
 }
}
/******************************************************************
  Function:       // readRegisters
  Description:    // 读取指定地址寄存器的值
  Input:          // addr：读取的寄存器地址
                  // tempData：读取值的存放地址
  Output:         //
  Return:         //
  Others:         //  
*******************************************************************/
void getRegisterVal (unsigned short addr,unsigned short *tempData)
{
  switch(addr & 0xff)
  {
   case 0: 
    { 
     //getCounterValue_bin (tempData, PNULL);
      
      break;
    }
     case 1: 
    { 
     //*tempData = *localAddr;
      break;
    }
    case 2: 
    { 
     
      break;
    }
    case 3: 
    { 
    
      break;
    }
    case 4: 
    { 
     
      break;
    }
    case 5: 
    { 
    
      break;
    }
   
  default:
    {*tempData = 0x0000;break;}  		
  }
}
/**********************************************************************************
  Function:       // setRegisterVal
  Description:    // 设置指定地址寄存器的值
  Input:          // addr：设置的寄存器地址
                  // Data：设置值
  Output:         //
  Return:         //
  Others:         //  
***********************************************************************************/
void setRegisterVal (unsigned short addr,unsigned short Data)
{
 switch(addr & 0xff)
 {
  case 0: 
    {  break;}
  case 1:   
    {
      //if(Data < 255)
     // {WriteRom (ModBus_AddrInROM, &Data, 2);}
      break;
    }
   case 2: 
    { 
     break;
    }
    case 3: 
    { 
      break;
    }  
 }
}
/******************************************************************
  Function:       // readCoil
  Description:    // 读线圈状态
  Input:          // 
                  //
  Output:         //
  Return:         //
  Others:         //
*******************************************************************/
void readCoil(void)
{
 unsigned char addr;
 unsigned char tempAddr;
 unsigned char byteCount;
 unsigned char bitCount;
 unsigned short crcData;
 unsigned char position;
 unsigned char i,k;
 unsigned short  tempData;
 unsigned char  exit = 0;
	
 //addr = (receBuf[2]<<8) + receBuf[3];	
 //tempAddr = addr & 0xfff;
 addr =  slave_receBuf[3];
 tempAddr = slave_receBuf[3];
 //bitCount = (receBuf[4]<<8) + receBuf[5];	//读取的位个数
 bitCount =  slave_receBuf[5];
 //字节个数
 byteCount = bitCount / 8;					
 if(bitCount%8 != 0)
 byteCount++;     
 for(k=0;k<byteCount;k++)
 {//字节位置
  position = k + 3;    
  slave_sendBuf[position] = 0; 
  for(i=0;i<8;i++)
  {
   getCoilVal(tempAddr,&tempData);
   slave_sendBuf[position] |= tempData << i;
   tempAddr++;
   if(tempAddr >= addr+bitCount)
   {//读完
	exit = 1;
	break;
   }	
  }
  if(exit == 1)
  break;
 }
//构造应答数据帧
 if(slave_receBuf[0]==0xff)
 {slave_sendBuf[0] = 0xff;}
 else
 { slave_sendBuf[0] = localAddr;}
 slave_sendBuf[1] = 0x01;	
 slave_sendBuf[2] = byteCount;
 byteCount += 3;
 crcData = crc16(slave_sendBuf,byteCount);	
 slave_sendBuf[byteCount] = crcData >> 8;
 byteCount++;
 slave_sendBuf[byteCount] = crcData & 0xff;
 byteCount++;
 UART_SendData(slave_sendBuf, byteCount);
}
/******************************************************************
  Function:       // presetSingleRegister
  Description:    // 设置1线圈命令服务程序
  Input:          // 
                  //
  Output:         //
  Return:         //
  Others:         //支持255内部数据单元 
*******************************************************************/
void presetSingleCoil(void)
{
  unsigned short addr;
  unsigned char  tempAddr;
  unsigned short crcData;
  unsigned short tempData;
  //获取地址
  addr =slave_receBuf[2];
  addr <<=8;
  addr+=slave_receBuf[3];
  tempAddr = addr & 0xff;
  //获取数据
  tempData=slave_receBuf[4];
  tempData<<=8;
  tempData+=slave_receBuf[5];
  //在指定地址写如指定数据
  setCoilVal(tempAddr,tempData);			
 //构造应答数据帧
 if(slave_receBuf[0]==0xff)
 {slave_sendBuf[0] = 0xff;}
 else
 { slave_sendBuf[0] = localAddr;}
  //slave_sendBuf[0] = localAddr;            //写入本机地址
  slave_sendBuf[1] = 5;                    //写入命令符
  slave_sendBuf[2] = addr >> 8;            //写入地址高字节
  slave_sendBuf[3] = addr & 0xff;          //写入地址低字节
  slave_sendBuf[4] = slave_receBuf[4];    //
  slave_sendBuf[5] = slave_receBuf[5];    //
  crcData = crc16(slave_sendBuf,6);               //计算应答帧CRC
  slave_sendBuf[6] = crcData >> 8;
  slave_sendBuf[7] = crcData & 0x00ff;
 UART_SendData(slave_sendBuf, 8);
}
/******************************************************************
  Function:       // presetSingleRegister
  Description:    // 设置1个寄存器命令服务程序
  Input:          // 
                  //
  Output:         //
  Return:         //
  Others:         //支持255内部数据单元 
*******************************************************************/
void presetSingleRegister(void)
{
  unsigned short addr;
  unsigned char  tempAddr;
  unsigned short crcData;
  unsigned short tempData;
  //获取地址
  addr =slave_receBuf[2];
  addr <<=8;
  addr+=slave_receBuf[3];
  tempAddr = addr & 0xff;
  //获取数据
  tempData=slave_receBuf[4];
  tempData<<=8;
  tempData+=slave_receBuf[5];
  //在指定地址写如指定数据
  setRegisterVal(tempAddr,tempData);			
 //构造应答数据帧
  //slave_sendBuf[0] = localAddr;            //写入本机地址
 if(slave_receBuf[0]==0xff)
 {slave_sendBuf[0] = 0xff;}
 else
 { slave_sendBuf[0] = localAddr;}
  slave_sendBuf[1] = 6;                    //写入命令符
  slave_sendBuf[2] = addr >> 8;            //写入地址高字节
  slave_sendBuf[3] = addr & 0xff;          //写入地址低字节
  slave_sendBuf[4] = slave_receBuf[4];    //
  slave_sendBuf[5] = slave_receBuf[5];    //
  crcData = crc16(slave_sendBuf,6);               //计算应答帧CRC
  slave_sendBuf[6] = crcData >> 8;
  slave_sendBuf[7] = crcData & 0x00ff;
 UART_SendData(slave_sendBuf, 8);
}
/******************************************************************
  Function:       // presetSingleRegister
  Description:    // 设置1个寄存器命令服务程序
  Input:          // 
                  //
  Output:         //
  Return:         //
  Others:         //支持255内部数据单元 
*******************************************************************/
void presetMultipleleRegister(void)
{
  unsigned short addr;
  unsigned char  tempAddr;
  unsigned short crcData;
  unsigned short tempData;
  unsigned short NO;

  unsigned char*  Pdata;
  //获取地址
  addr =slave_receBuf[2];
  addr <<=8;
  addr+=slave_receBuf[3];
  tempAddr = addr & 0xff;

  NO =slave_receBuf[4];
  NO  <<= 8;
  NO +=slave_receBuf[5];
  Pdata = slave_receBuf + 7;
  while( NO -- )
  {
   //获取数据
   tempData=*Pdata++;
   tempData<<=8;
   tempData+=*Pdata++;
  //在指定地址写如指定数据
  setRegisterVal(tempAddr ++ ,tempData);
  }
  			
 //构造应答数据帧
  //slave_sendBuf[0] = localAddr;            //写入本机地址
 if(slave_receBuf[0]==0xff)
 {slave_sendBuf[0] = 0xff;}
 else
 { slave_sendBuf[0] = localAddr;}
  
  slave_sendBuf[1] = 16;                    //写入命令符
  slave_sendBuf[2] = slave_receBuf[2];            //写入地址高字节
  slave_sendBuf[3] = slave_receBuf[3];          //写入地址低字节
  slave_sendBuf[4] = slave_receBuf[4];    //
  slave_sendBuf[5] = slave_receBuf[5];    //
  crcData = crc16(slave_sendBuf,6);               //计算应答帧CRC
  slave_sendBuf[6] = crcData >> 8;
  slave_sendBuf[7] = crcData & 0x00ff;
 UART_SendData(slave_sendBuf, 8);
}
/******************************************************************
  Function:       // readRegisters
  Description:    // 读取寄存器命令服务程序
  Input:          //
                  //
  Output:         //
  Return:         //
  Others:         //  
*******************************************************************/
void readRegisters(void)
{
 unsigned short addr;
 unsigned short i;
 unsigned char readCount;  //要读的个数
 unsigned char byteCount;
 unsigned short tempData;
 unsigned short crcData;
 
 readCount=slave_receBuf[5];
 byteCount=readCount<<1;
 
 addr=slave_receBuf[2];
 addr<<=8;
 addr=slave_receBuf[3];
 //tempAddr = addr & 0xfff;	
 //addr = receBuf[3];
 //tempAddr = addr;
 for(i=0;i<byteCount;i+=2)
 {
  getRegisterVal(addr,&tempData);				
  slave_sendBuf[i+3] = tempData >> 8;				   		
  slave_sendBuf[i+4] = tempData & 0x00ff;	
  addr++;
 }
 //构造应答数据帧
 if(slave_receBuf[0]==0xff)
 {slave_sendBuf[0] = 0xff;}
 else
 { slave_sendBuf[0] = localAddr;}
 //slave_sendBuf[0] = localAddr;   //增加本机地址
 slave_sendBuf[1] = 3;           //命令应答
 slave_sendBuf[2] = byteCount;   
 byteCount+= 3;
 crcData = crc16(slave_sendBuf,byteCount);  //计算CRC校验码
 slave_sendBuf[byteCount] = crcData >> 8;
 byteCount++;
 slave_sendBuf[byteCount] = crcData & 0xff;
 byteCount++;
 UART_SendData(slave_sendBuf, byteCount);
}
/******************************************************************
  Function:       // modbus_send
  Description:    // MODBUS 发送函数
  Input:          // 
                  //
  Output:         //
  Return:         //
  Others:         //
*******************************************************************/
void modbus_send(unsigned char* sendBuf, unsigned char sendLen)
{
  //UART6_SendData( sendBuf, sendLen );
}
/******************************************************************
  Function:       // 
  Description:    // 校验接收数据包
  Input:          // 
                  //
  Output:         //
  Return:         //
  Others:         //
*******************************************************************/
unsigned char crc16_Verify (void *pbuff, unsigned short length)
{
 unsigned short crcData;
 unsigned short tempData;
 crcData = crc16(pbuff,(length)-2);    //计算CRC校验码
  //得到数据包自带CRC校验码
 tempData=((unsigned char*)pbuff)[(length) - 2];         
 tempData<<=8;
 tempData+=((unsigned char*)pbuff)[(length) - 1];
 if(crcData!=tempData)
 {return 0; }
 return 1; 
}  
/******************************************************************
  Function:       // readRegisters
  Description:    // 读取寄存器命令服务程序
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
unsigned char checkComm0Modbus(void)
{
 unsigned char flg = 0;
 if((receCount) >=4)
 {
  if((slave_receBuf[0]==(localAddr))||(slave_receBuf[0]==0xff))
  {
   switch(slave_receBuf[1])
   {
    case 1://读取线圈状态(读取点 16位以内)
    { 
     if(receCount>=8)
     {readCoil();flg = 1;}
     break;
    }
    case 3://读取保持寄存器(一个或多个)
    {
     if(receCount >= 8)
     {readRegisters();flg = 1;}
     break;
    }
    case 5://强制单个线圈
    {
     if(receCount>=8)
     { presetSingleCoil();flg = 1;} 
      break;
     }
    case 6://设置单个寄存器
    {
     if(receCount>=8)
     { presetSingleRegister();flg = 1;}
     break;
     }
    case 15://强制多个线圈
	{
	 //if((*receCount)>=8)
     //{ presetSingleCoil();*receCount = 0;} 
	 break;
	}
	case 16://设置单个寄存器
    {
     if(receCount == 9 + slave_receBuf[6])
      { presetMultipleleRegister();flg = 1;}
     break;
     }
    default:
     {  break;}
   }
  }
 }
 return flg;
}
/******************************************************************
  Function:       //
  Description:    // 
  Input:          // 
                  //
  Output:         //
  Return:         //
  Others:         //
*******************************************************************/
SystemErrName Modbus_Input (unsigned char prot, void *pbuff, unsigned char length)
{
 if(0 == crc16_Verify (pbuff,length))
 {return Verify_err;}
 if(0 != slave_sendBufFlg )
 {return PortBusy;}
 PortON = prot;
 slave_receBuf = pbuff;
 receCount = length;
 if(checkComm0Modbus())
 {return NO_ERR;}
 else
 {return OperateCmd_err;}
}
/**********************************************************************************
  Function:       // 
  Description:    //
  Input:          // 
                  // 
  Output:         // 
  Return:         // 
  Others:         // 
                  //  
**********************************************************************************/
unsigned char UART_SendData( unsigned char* txbuf, unsigned char txnum )
{
 unsigned char itemp = 0 ;

 switch (PortON )
 {
  case 0:
  {
    //slave_sendBufFlg =  //Mbus_SEND_CallBack(txbuf, txnum, ModBus_SendOK_CallBack ,txbuf);
   break;
  }
 }
 return itemp ;
}
/**********************************************************************************
  Function:       // 
  Description:    //
  Input:          // 
                  // 
  Output:         // 
  Return:         // 
  Others:         // 
                  //  
**********************************************************************************/
void ModBus_SendOK_CallBack( unsigned char port, void *pbuff)
{
  slave_sendBufFlg = 0;
}

  

