/*********************************************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************************************/
#ifndef  __CJT188_2004_H
#define  __CJT188_2004_H
/*********************************************************************************************************
文件包含区
*********************************************************************************************************/
#include "..\..\BSP\inc\bsp.h"
#include "..\..\Projects\inc\SystemDefine.h"
#include "..\..\APP\inc\MCPX.h"
/*********************************************************************************************************
宏定义区
*********************************************************************************************************/
#define  CJT188_OutPutBuff_Number      4

#define  Locall_MeterType     ColdWaterMeter
#define  Locall_FactoryID     ('K' *10) + 'D'

#define  Locall_Type                 0  //0 ：数据采集器   1：数据传感器

#define  PreambleNumber              2 

#define  CJT188_Preamble             0xFE
#define  CJT188_StartCharacter       0x68
#define  CJT188_Pause                0x16



#define  CJT188_ColdWaterMeter       0x10     //冷水表
#define  CJT188_HeatWaterMeter       0x11     //生活热水表
#define  CJT188_DrinkWaterMeter      0x12     //直饮水表
#define  CJT188_RecycledWaterMeter   0x13     //中水表   


#define  CJT188_ReadData_CTR         0x01        //读数据 （请求）
#define  CJT188_ReadVerNo_CTR        0x09        //读秘钥版本号 （请求）
#define  CJT188_ReadAddress_CTR      0x03        //读地址 （请求）
#define  CJT188_WriteData_CTR        0x04        //写数据 （请求）
#define  CJT188_WriteAddress_CTR     0x15        //写地址 （请求）
#define  CJT188_WriteOffset_CTR      0x16        //写机电同步数据（请求）

#define  CJT188_ReadData_DI          0x1F90      //计量数据
#define  CJT188_ReadHistData_1_DI    0x20D1      //历史计量数据1
#define  CJT188_ReadHistData_2_DI    0x21D1      //历史计量数据2
#define  CJT188_ReadHistData_3_DI    0x22D1      //历史计量数据3
#define  CJT188_ReadHistData_4_DI    0x23D1      //历史计量数据4
#define  CJT188_ReadHistData_5_DI    0x24D1      //历史计量数据5
#define  CJT188_ReadHistData_6_DI    0x25D1      //历史计量数据6
#define  CJT188_ReadHistData_7_DI    0x26D1      //历史计量数据7
#define  CJT188_ReadHistData_8_DI    0x27D1      //历史计量数据8
#define  CJT188_ReadHistData_9_DI    0x28D1      //历史计量数据9
#define  CJT188_ReadHistData_10_DI   0x29D1      //历史计量数据10
#define  CJT188_ReadHistData_11_DI   0x2AD1      //历史计量数据11
#define  CJT188_ReadHistData_12_DI   0x2BD1      //历史计量数据12
#define  CJT188_ReadPriceList_DI     0x0281      //读价格表
#define  CJT188_ReadSelDate_DI       0x0381      //读结算日
#define  CJT188_ReadRecordDate_DI    0x0481      //读抄表日
#define  CJT188_ReadRMoney_DI        0x0581      //读购入金额
#define  CJT188_ReadVerNo_DI         0x0681      //读秘钥版本号
#define  CJT188_ReadAddress_DI       0x0A81      //读地址

#define  CJT188_WritePriceList_DI    0x10A0      //写价格表
#define  CJT188_WriteSelDate_DI      0x11A0      //写结算日期
#define  CJT188_WriteRecordDate_DI   0x12A0      //写抄表日
#define  CJT188_WriteRMoney_DI       0x13A0      //读购入金额
#define  CJT188_WriteSecretKey_DI    0x14A0      //写秘钥版本号
#define  CJT188_WriteTime_DI         0x15A0      //写标注时间
#define  CJT188_WriteValveState_DI   0x17A0      //写阀门控制
#define  CJT188_Lock_DI              0x19A0      //出厂启用
#define  CJT188_WriteAddress_DI      0x18A0      //写地址
#define  CJT188_WriteOffset_DI       0x16A0      //写地址
/*********************************************************************************************************
数据类型定义
*********************************************************************************************************/
#pragma pack(1)
typedef unsigned char           CJT188_SER_Type;
typedef unsigned short          CJT188_DI_Type;

typedef enum{ASK_CJT188 = 0, ACK_CJT188 = 1  }CJT188_DirType;

typedef enum{
  Wh        = 0x02,   //冷水表
  KWh       = 0x05,   //生活热水表
  MWh       = 0x08,
  MWh_100   = 0x0a,
  J         = 0x01,
  KJ        = 0x0B,
  MJ        = 0x0E, 
  GJ        = 0x11,
  GJ_100    = 0x13,
  W         = 0x14,
  KW        = 0x17,
  MW        = 0x1A,
  L         = 0x29,
  m3        = 0x2C,
  L_h       = 0x32,
  m3_h      = 0x35,
}CJT188_UnitType;

typedef struct{
  unsigned char MeterID[5];
  unsigned short FactoryID;
}CJT188_AddresStruct;

typedef union{
  unsigned char Byte;
  struct{
    unsigned char Infr : 6;
    unsigned char CommFlg : 1; //0 — 通讯正常; 1— 通 讯 异 常 
    unsigned char DIR : 1;     //0 — 由主站发出的控制帧; 1— 由从站发出的应答帧。
  }Bit;
}CJT188_CTRStruct;

typedef struct {
  unsigned char       Preamble[PreambleNumber]; 
  unsigned char       StartCharacter;
  unsigned char       Type;
  CJT188_AddresStruct Addr;
  CJT188_CTRStruct    CTR;
  unsigned char        Length;
}CJT188_FrameHeadStruct;

typedef struct{
  CJT188_DI_Type   DI;
  CJT188_SER_Type  SER;
}CJT188_DataHeadStruct;

//帧尾格式
typedef struct {
  unsigned char CS;     //校验码,从帧头开始到帧校验标志之前所有字节的累加和低8位	
  unsigned char Pause;  //结束符0x16H
}CJT188_FrameStern_Struct;

//节点状态
typedef union {
   unsigned short Word;
   struct{
      unsigned short F1: 1;          //厂商自定义1
      unsigned short F2: 1;          //厂商自定义2
      unsigned short F3: 1;          //厂商自定义3
      unsigned short CN: 2;          //保留
      unsigned short Bat_Flg: 1;    //电池电压   0：正常  1：欠压
      unsigned short Valve_State : 2;    //阀门状态 00：开， 01 关， 11：异常
      unsigned short Custom :8;
    }Bit;
}CJT188_ST_Stuct;

//实时时钟格式
typedef struct{
  unsigned short  Year;
  unsigned char   Mon;
  unsigned char   Day;
  unsigned char   Hour;
  unsigned char   Min;
  unsigned char   Sec;
}CJT188_Time_Struct;

//读数据 格式
typedef struct{
  unsigned char          Flow_PV[4];              //当前累积流量
  //CJT188_UnitType        Flow_PV_Unit;            //当前累积流量单位
  //unsigned char          Flow_Accounts[4];        //结算日累积流量
  // CJT188_UnitType        Flow_Accounts_Unit;      //结算日累积流量单位
  //CJT188_Time_Struct     Time;                    //实时时间
  CJT188_ST_Stuct        ST;                      //
}CJT188_MeterData_Struct;  

//
typedef enum{
  CJT188_ValveON      = 0x55,   //开阀控制操作:
  CJT188_ValveOFF     = 0x99,   //关阀控制操作:
}CJT188_ValveState_Type;

//正常应帧格式
typedef struct{
  CJT188_FrameHeadStruct  Head;
  CJT188_DataHeadStruct   DataHead;
  unsigned char Data[32];  
}CJT188_OkACK_Struct;

//错误应答帧格式
typedef struct{
  CJT188_FrameHeadStruct    Head;
  CJT188_SER_Type           SER;
  CJT188_ST_Stuct           ST;     //节点状态
  CJT188_FrameStern_Struct  Stern;
}CJT8_188_ErrACK_Struct;

//
typedef union{        
   CJT188_FrameHeadStruct  Head;
   CJT188_OkACK_Struct     Normal;
   CJT8_188_ErrACK_Struct  ERR;
   unsigned char           Data[64]; 
 }CJT188_Frame_Struct; 

#pragma pack()


typedef struct CJT188_SendBuff  CJT188_SendBuff_Struct;

typedef void(*CJT188_AckFunc)(CJT188_SendBuff_Struct* SendData, CJT188_Frame_Struct* AckData);
typedef SerialPort_SendEndCallBack_Type  CJT188_SendOKFunc;

struct CJT188_SendBuff{
 union{
    unsigned char Byte;
    struct{
      unsigned char idle :1; //缓冲区空闲
    }Bit;
  }CSR; //控制状态寄存器
 CJT188_SendOKFunc    SendOK;   //发送完成处理函数
 CJT188_AckFunc       AckFunc;      //应答处理函数
 CommPortNumber_Type  SourcePort;
 CommPortNumber_Type  TargetPort;
 CJT188_Frame_Struct  buff;
};
//CJT188命令描述符
typedef struct{
 CommPortNumber_Type SourcePort; 
 unsigned char       Type;
 CJT188_AddresStruct Addr;
 CJT188_CTRStruct    CTR;
 CJT188_SER_Type     SER;
 CJT188_DI_Type      DataDI;
 unsigned char  ParamLength;
 unsigned char  Param[32];
}CJT188_CmdDeclarator;
/*********************************************************************************************************
外部变量声明区
*********************************************************************************************************/

/*********************************************************************************************************
函数声明区
*********************************************************************************************************/
void CJT188_SPCP_init(void);

SystemErrName CJT188_2004_InPut (void *pbuff, unsigned length, CommPortNumber_Type port);

CJT188_SER_Type getSER (void);

SystemErrName CJT188_SendCmd (CJT188_CmdDeclarator* Cmd,
                              CommPortNumber_Type TargetPort,
                              CJT188_AckFunc AckFunc);

SystemErrName CJT188_ReadNodeValue (unsigned char  MeterType,
                                    CJT188_AddresStruct* Addr,
                                    unsigned short DI,
                                    CommPortNumber_Type SourcePort,
                                    CommPortNumber_Type TargetPort,
                                    CJT188_AckFunc AckFunc);

SystemErrName CJT188_OperateNodeValve(unsigned char  MeterType,
                                      CJT188_AddresStruct* Addr,
                                      unsigned char  ValveCmd,
                                      CommPortNumber_Type SourcePort,
                                      CommPortNumber_Type TargetPort,
                                      CJT188_AckFunc AckFunc);
/********************************************************************************************************/
#endif

/******************************************END********************************************************/