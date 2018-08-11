/*********************************************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************************************/
#ifndef  __DLT_645_1997_H
#define  __DLT_645_1997_H
/*********************************************************************************************************
文件包含区
*********************************************************************************************************/
#include "..\..\BSP\inc\BSP.h"
/*********************************************************************************************************
宏定义区
*********************************************************************************************************/
#define  MeterAddrLength        6
#define  DLT_PassWordLength     4



#define  DLT_645_Preamble                0xFE
#define  DLT_645_StartCharacter          0x68
#define  DLT_645_Pause                   0x16
    
#define  DLT_CCR_ReadData_ASK       0x01          //读数据 （请求）
#define  DLT_CCR_ReadData_ACK       0x81          //读数据 （应答）

#define  DLT_CCR_WriteAddress_ASK    0x04          //写地址 （请求）
#define  DLT_CCR_WriteAddress_ACK    0x84          //写地址 （应答）

#define DLT_CCR_SetPassWord_ASK       0x0F        //写密码 （请求）
#define DLT_CCR_SetPassWordOk_ACK     0x8F      //写密码 （正常应答）
#define DLT_CCR_SetPassWordErr_ACK    0xCF      //写密码 （异常应答）

#define  DLT_DI0_ReadData            0x10      //请求数据帧
#define  DLT_DI1_ReadData            0x90      //请求数据帧

//#define  DLT_DI0_WriteAddress        0xA018      //写地址
//#define  DLT_DI1_WriteAddress        0xA018      //写地址

#define  DLT_DI0_ReadAddress         0x32     //读地址
#define  DLT_DI1_ReadAddress         0xC0      //读地址

#define  DLT_DI0_ReadPassWord         0x44     //读地址
#define  DLT_DI1_ReadPassWord         0xC0     //读地址    

/*********************************************************************************************************
数据类型定义
*********************************************************************************************************/
typedef unsigned char           DLT_645_SER_Type;
typedef unsigned char           DLT_645_DI_Type;

typedef enum{ASK_DLT_645 = 0, ACK_DLT_645 = 1  }DLT_645_DirType;

typedef struct{
  unsigned char MeterAddr[MeterAddrLength];
}DLT_645_AddresStruct;

typedef union {
   unsigned short Word;
   struct{
     unsigned char  Custom ;
     unsigned char  M     : 1;       //存储器状态 (1:故障,0:正常)    
     unsigned char  VF    : 1;       //阀门状态 (1:故障,0:正常)
     unsigned char  S     : 1;       //信号状态 (1:故障,0:正常)
     unsigned char  Bat   : 1;       //电池状态 (1:故障,0:正常)     
     unsigned char  NC    : 2;       //保留
     unsigned char  Comm  : 1;       //水表通讯状态（ 1:故障,0:正常） 
     unsigned char  V     : 1;       //阀门开关状态 (1:开,0:合)；
    }Bit;
}DLT_645_ST_Stuct;


typedef struct {
   DLT_645_DI_Type          DI0;
   DLT_645_DI_Type          DI1;
} DLT_SuperData_Struct;

typedef struct {
  DLT_645_DI_Type          DI0;
  DLT_645_DI_Type          DI1;
  unsigned char Value[4];
  DLT_645_ST_Stuct   Y;     
} DLT_645_GetValue_ACK_Struct;


typedef struct {
   DLT_645_DI_Type         DI0;
  DLT_645_DI_Type          DI1;
  unsigned char PassWord[DLT_PassWordLength];
  DLT_645_AddresStruct  NewAddr;     
} DLT_645_WriteAddr_ASK_Struct;



typedef struct {
   DLT_645_DI_Type         DI0;
  DLT_645_DI_Type          DI1;
  DLT_645_AddresStruct  Addr;    
} DLT_645_ReadAddr_ACK_Struct;

typedef struct {
  unsigned char CurrentPassWord[DLT_PassWordLength];
  unsigned char NewPassWord[DLT_PassWordLength];
} DLT_645_SetPassWord_ASK_Struct;

typedef struct {
  unsigned char CurrentPassWord[DLT_PassWordLength];
} DLT_645_SetPassWordOk_ACK_Struct;

typedef struct {
  unsigned char ErrCode;
} DLT_645_SetPassWordErr_ACK_Struct;

typedef struct {
  DLT_645_DI_Type         DI0;
  DLT_645_DI_Type         DI1;
  unsigned char CurrentPassWord[DLT_PassWordLength];
} DLT_645_ReadPassWord_ACK_Struct;

typedef struct {
  unsigned char       StartCharacter1;
  DLT_645_AddresStruct Addr;
  unsigned char       StartCharacter2;
  unsigned char       CCR;              
  unsigned char       Length;
}DLT_645_FrameHeadStruct;

//正常帧格式
typedef struct{
  DLT_645_FrameHeadStruct  Head;
  union{
   unsigned char                      Byte[32];
   DLT_SuperData_Struct               Super;
   DLT_645_WriteAddr_ASK_Struct       WriteAddr;
   DLT_645_SetPassWord_ASK_Struct     SetPassWord;
   DLT_645_GetValue_ACK_Struct        GetValue;
   DLT_645_ReadAddr_ACK_Struct        ReadAddr; 
   DLT_645_SetPassWordErr_ACK_Struct  SetPassWordErr;
   DLT_645_SetPassWordOk_ACK_Struct   SetPassWordOk;
   DLT_645_ReadPassWord_ACK_Struct    ReadPassWord;
  }Data;
}DLT_645_Frame_Struct;
/*********************************************************************************************************
外部变量声明区
*********************************************************************************************************/

/*********************************************************************************************************
函数声明区
*********************************************************************************************************/
SystemErrName DLT_645_1997_InPut (void *pbuff, unsigned length);
SystemErrName DLT_645_SendStart (void *pbuff);
void DLT_time ( void );
void BinToBCD_DLT (unsigned short data, unsigned char *pbuff, unsigned char w);
/********************************************************************************************************/
#endif
/******************************************END********************************************************/