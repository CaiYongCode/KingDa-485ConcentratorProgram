/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
#ifndef  JWX_SPCP
#define  JWX_SPCP
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "..\..\BSP\inc\bsp.h"
/*********************************************************************************
宏定义区
*********************************************************************************/
# define Preamble_JWX_SPCP     0xFE
# define SYNC_JWX_SPCP         0x68
# define Tailed_JWX_SPCP       0x16



# define DataLengthLimit_JWX_SPCP       0x64
 
# define JWX_SPCP_TxFunc(p,l)   sendBuffByDMA_Rs485(p,l,P_NULL, P_NULL)
/*********************************************************************************
数据类型定义
*********************************************************************************/
typedef enum{
  ColdWaterMeter_JWX  = 0x10,   //冷水表
  HeatWaterMeter_JWX  = 0x11,   //生活热水表
  DrinkWaterMeter_JWX = 0x12,   //直饮水表
  RecycledWaterMeter_JWX = 0x13   //中水表 
}JWX_SPCP_MeterType;

typedef enum{
  JWX_ReadData_ASK     = 0x01,          //读数据 （请求）
  JWX_ReadData_ACK     = 0x81,          //读数据 （应答）
  JWX_ReadData_ERR     = 0xC1,          //读数据 （错误）
  
  JWX_ReadAddress_ASK  = 0x03,          //读地址 （请求）
  JWX_ReadAddress_ACK  = 0x83,          //读地址 （应答）
  JWX_ReadAddress_ERR  = 0xC3,          //读地址 （错误） 

  JWX_WriteAddress_ASK = 0x15,          //写地址 （请求）
  JWX_WriteAddress_ACK = 0x95,          //写地址 （应答）
  JWX_WriteAddress_ERR = 0xD5,          //写地址 （错误）  
  
  JWX_ValveOperation_ASK = 0x2A,          //阀门操作 （请求）
  JWX_ValveOperation_ACK = 0xAA,          //阀门操作 （应答)
    
  JWX_Disable_ASK = 0x24,          //写恢复生产状态命令[取消出厂启用] （请求）
  JWX_Disable_ACK = 0xA4,          //写恢复生产状态命令[取消出厂启用] （应答）
  
  JWX_Enable_ASK = 0x04,          //写出厂启用命令 （请求）
  JWX_Enable_ACK = 0x84,          //写出厂启用命令 （应答）
  
}JWX_SPCP_CMD_Type;

#pragma pack(1)
typedef  struct
{
  unsigned char Sync ;
  unsigned char NodeType;
  unsigned char Addr[7];
  unsigned char CMD;
  unsigned char DataLength;
}JWX_SPCP_PackageHeader_Type;
#pragma pack()

#pragma pack(1)
typedef  struct
{
  unsigned char Preamble1;
  unsigned char Preamble2;
  JWX_SPCP_PackageHeader_Type  Header ;
  unsigned char Data[DataLengthLimit_JWX_SPCP]; 
}JWX_SPCP_Package_Type;
#pragma pack()

#pragma pack(1)
typedef  struct
{
  unsigned char CS ;
  unsigned char Tailed;
}JWX_SPCP_PackageTailed_Type;
#pragma pack()

//typedef enum{
 // NoErr   = 0,
//  Busy = 1,
 // Length_err   = 2,
 // FramePreamble_err = 3,
 // FrameSync_err = 4,
 // Verify_err = 5,
//  OperateCmd_err    = 6,
 // DataRomWrite_err  = 7,
 // FrameInvalid   = 8,        //无效数据帧
 // CounterFault =9 ,      //计数器故障
 // PassWordErr = 10
//}JWX_SPCP_ErrCodeType;

#pragma pack(1)
typedef  struct{
  unsigned char Decimals;
  unsigned char Integer_L;
  unsigned char Integer_M;
  unsigned char Integer_H;  
  unsigned char Unit;  
}JWX_SPCP_QuantityFlow_Format;
#pragma pack()

#pragma pack(1)
typedef  struct{
  unsigned char Second;
  unsigned char Minute;
  unsigned char Hour;
  unsigned char Day;  
  unsigned char Month; 
  unsigned short Year;
}JWX_SPCP_Timer_Format;
#pragma pack()

#pragma pack(1)
typedef  union{
  unsigned short  Word;
  struct{
    unsigned char ValveState : 2;
    unsigned char PVD_Flg : 1;
    unsigned char NC1 : 5;
    
    unsigned char ErrState : 1;
    unsigned char Mag_Flg : 1;
    unsigned char NC2 : 2;
    unsigned char NC3 : 2;
    unsigned char Mode : 1;
    unsigned char NC4 : 1;
  }Bit; 
}JWX_SPCP_Status_Format;
#pragma pack()
  
  
#pragma pack(1)
typedef  struct{
  unsigned char  DI0;
  unsigned char   DI1;
  unsigned char   SER;
  JWX_SPCP_QuantityFlow_Format   CURDATA;
  JWX_SPCP_QuantityFlow_Format   SurplusDATA;
  JWX_SPCP_Timer_Format          Timer;
  JWX_SPCP_Status_Format         ST;
}JWX_SPCP_ReadData_AckData_Struct;
#pragma pack()
  
  
#pragma pack(1)
typedef  struct{
  unsigned char   DI0;
  unsigned char   DI1;
  unsigned char   SER;
  JWX_SPCP_Status_Format   ST;
}JWX_SPCP_ValveOperation_AckData_Struct; 
#pragma pack() 


#pragma pack(1)
typedef  struct{
  unsigned char   DI0;
  unsigned char   DI1;
  unsigned char   SER;
}JWX_SPCP_Disable_Enable_AckData_Struct; 
#pragma pack() 


typedef enum{
 JWX_OpenValve  = 0x55,
 JWX_CloseValve  = 0x99, 
 JWX_AutoValve  = 0x11,  
}JWX_ValveOperationCode_Type;
/*********************************************************************************
外部变量声明区
*********************************************************************************/
extern const unsigned char   BroadcastAddress_JWX_SPCP[];
/*********************************************************************************
函数声明区
*********************************************************************************/
void readVaule_JWX (unsigned char *addr);
void ReadAddress__JWX ( void );
SystemErrName JWX_InPut (void *pbuff, unsigned length);
void ValveOperation_ASK_JWX (unsigned char *addr, JWX_ValveOperationCode_Type  cmd);
/********************************************************************************/

/*********************************************************************************************************/
#endif
