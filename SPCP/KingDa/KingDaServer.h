/*********************************************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************************************/
#ifndef  __KINGDASERVER_H
#define  __KINGDASERVER_H
/*********************************************************************************************************
文件包含区
*********************************************************************************************************/
#include "..\..\APP\inc\MCPX.h"
#include ".\CRC8.h"
/*********************************************************************************************************
宏定义区
*********************************************************************************************************/

#define  KingDaServerSync_ToServer               0xABAB     //下行同步字
#define  KingDaServerSync_ToConcentrator         0xBABA     //下行同步字


#define  RetransNumber_ToServer_Default           2         //数据包默认重传次数


#define  KingDaServerSendBuff_Number              4         //发送缓冲区长度

#ifndef  UP
#define  UP     1
#endif

#ifndef  DOWN 
#define  DOWN    0
#endif
/*********************************************************************************************************
数据类型定义
*********************************************************************************************************/
typedef struct KingDaServer_SendBuff  KingDaServerSendBuff_Struct;

typedef enum{
// 表计作命令
 ReadMeasureValue     = 0xA1,    //单表数据读取
 ReadMeasureValue_OFF = 0x63,    //单表数据读取  (读完关闭通道) 
//集中器操作命令
 HandCmd_Server       = 0x6B,     //服务器 握手命令
 TuneUp_RTC           = 0x68,     //服务器向集中器校时命令 
 ReadALLMeasureValue  = 0x6D,     //抄表指令   服务器主动读取
 MeasureValue_UpLoad  = 0x6C,     //抄表指令   集中器主动
//中继器操作命令
 GetRepeatersState    = 0x67,     //查询中继器状态  
 
 ServerOpen_OV_ENF       = 0X71,          //强制开阀
 ServerClose_OV_ENF      = 0X72,          //强制关阀

 
 
 
 
 ServerDeleteNode_APPCON      = 0x80,   //删除节点命令
 ServerBATCH_APPCON           = 0x81,   //组网并抄表
 ServerSTATUS_APPCON          = 0x82,   //查询组建网络状态
 ServerOpenOV_APPCON          = 0x65,   //开阀
 ServerHalfOV_APPCON          = 0x74,   //半开阀门
 ServerMtOV_APPCON            = 0x73,   //活动阀命令
 ServerCloseOV_APPCON         = 0x64,   //关阀
 ServerGetOV_APPCON           = 0x61,   //读取阀门状态
 
}KingDaServer_CmdCode;

#pragma pack(1)
//单表数据读取  集中器向服务器发送 服务器返回
typedef struct{
  unsigned char Concentrator_ID [3];
  unsigned char Node_ID[3];
}ReadMeasureValue_ToKingDaConcentrator_Type;   //单表数据读取  服务器向集中器发送 
typedef struct{
  unsigned char  Concentrator_ID[3];
  unsigned char  Node_ID[3];
  unsigned short Value;
}ReadMeasureValue_ToKingDaServer_Type;        //单表数据读取  集中器向服务器发送 

//阀门操作  集中器向服务器发送 服务器返回
typedef struct{
  unsigned char Concentrator_ID [3];
  unsigned char Node_ID[3];
}OperateValve_ToKingDaConcentrator_Type;   //阀门操作 服务器向集中器发送 
typedef struct{
  unsigned char  Concentrator_ID[3];
  unsigned char  Node_ID[3];
  unsigned short  state;
}OperateValve_ToKingDaServer_Type;        //阀门操作  集中器向服务器发送 

//单表数据读取  (读完关闭通道)
typedef ReadMeasureValue_ToKingDaConcentrator_Type   ReadMeasureValue_OFF_ToKingDaConcentrator_Type;  //单表数据读取  服务器向集中器发送  (读完关闭通道)
typedef ReadMeasureValue_ToKingDaServer_Type         ReadMeasureValue_OFF_ToKingDaServer_Type;        //单表数据读取  集中器向服务器发送  (读完关闭通道)

//服务器 握手命令
typedef struct{
  unsigned char id [3];
  unsigned char DIO[5];
  unsigned char CSQ;
}HandsData_ToKingDaServer;               //服务器握手命令 集中器向服务器发送
typedef union{
 struct{
  unsigned char id [3];
  unsigned char DI[2];
 }OK;
 struct{
  unsigned char id [5];
 }err;
}HandsData_ToKingDaConcentrator;   //服务器握手命令 服务器返回

//服务器向集中器校时命令
typedef struct{
  unsigned char Concentrator_ID [3];
  unsigned char DIO[2];
}TuneUpRTC_ToKingDaServer;           //服务器向集中器校时命令 集中器向服务器发送
typedef struct{
  unsigned char Concentrator_ID [3];
  unsigned char Year;
  unsigned char Month;
  unsigned char Day;
  unsigned char Hour;
  unsigned char Minute;
  unsigned char Second;
}TuneUpRTC_ToKingDaConcentrator;    //服务器向集中器校时命令 服务器向集中器发送
        
//查询中继器状态
typedef struct{
  unsigned char Concentrator_ID [3];
  unsigned char State;
}GetRepeatersState_ToKingDaServer;         //查询中继器状态 集中器向服务器发送
typedef struct{
  unsigned char Concentrator_ID [3];
  unsigned char Repeaters_ID;
}GetRepeatersState_ToKingDaConcentrator;   //查询中继器状态 服务器向集中器发送

//服务器读取数据的命令
typedef struct{
  unsigned char ChannelNode_ID [4];    //中继器各通道水表数量
  unsigned char Concentrator_ID [3];    //集中器号
  unsigned char RepeatersNet_ID [3];    //中继器网络号
  unsigned char Repeaters_ID;
  unsigned char RepeatersVoltage;
  unsigned char DIO[2];
  unsigned char Data[400 * 2];
}ReadALLMeasureValue_ToKingDaServer;   //服务器读取数据的命令 集中器向服务器发送



typedef union{
  struct{
  unsigned char Concentrator_ID [3];
  unsigned char DIO[2];
  }OK;        
 struct{
  unsigned char DIO[5];
 }ERR;
}ReadALLMeasureValue_ToKingDaConcentrator;   //服务器读取数据的命令 服务器向集中器发送



//安美通水表数据格式
typedef struct{
 unsigned char id[4];
 unsigned char value[4];
 unsigned char vbat;
 unsigned char link;
 unsigned char route;
}NodeDatas_APPCON_Struct;

//安美通水表数据格式
//typedef struct{
// unsigned long NodeNumber;  //为水表数量
// unsigned char CID[3];         //集中器的编号
// unsigned char NETID[4];      //网络号
// NodeDatas_APPCON_Struct   Nodes[50];
//}ReadNodeDatas_APPCON_ToKingDaServer_Struct;
  

//服务器读取数据的命令
typedef struct{
  unsigned char ChannelNode_ID [4];    //中继器各通道水表数量
  unsigned char Concentrator_ID [3];    //集中器号
  unsigned char RepeatersNet_ID [3];    //中继器网络号
  unsigned char Repeaters_ID;
  struct{
    unsigned char Year;
    unsigned char Month;
    unsigned char Day;
    unsigned char Hour;
    unsigned char Minute;
    unsigned char Second;
  }Time;
  NodeDatas_APPCON_Struct  Datas[52];
  unsigned char crc;
}ReadALLMeasureValue_ToKingDaServer_APPCON;   //服务器读取数据的命令 集中器向服务器发送



 typedef struct{
  unsigned char Concentrator_ID [3];
  unsigned char Node_ID[8];
}APPCON_ReadMeasureValue_ToKingDaConcentrator_Type;   //单表数据读取  服务器向集中器发送 
   
typedef struct{
  unsigned char  Concentrator_ID[3];
  unsigned char  Node_ID[8];
  unsigned char  Value[4];
  unsigned char  Bat;
}APPCON_ReadMeasureValue_ToKingDaServer_Type;        //单表数据读取  集中器向服务器发送



//删除节点命令
  //BA+BA+80+数据长度（两字节）+集中器号（3个字节）+网络号（2个字节）+校验
typedef struct{
 unsigned char  Concentrator_ID[3];
  unsigned short  Net_ID;
}APPCON_DeleteNode_ToKingDaConcentrator_Type; 
  //返回：AB+AB+80+数据长度（1个字节）+集中器号（3个字节）+网络号（2个字节）+OK（ER）+校验
typedef struct{
 unsigned char  Concentrator_ID[3];
 unsigned char  Net_ID[2];
 unsigned char  Resu[2];
 }APPCON_DeleteNode_ToKingDaServer_Type; 

 //组网并抄表
   //BA+BA+81+数据长度（两字节）+集中器号（3个字节）+网络号（2个字节）+校验
typedef struct{
  unsigned char  Concentrator_ID[3]; 
  unsigned short  Net_ID;
 }APPCON_BATCH_ToKingDaConcentrator_Type;      
   //返回：AB+AB+81+数据长度（1个字节）+集中器号（3个字节）+网络号（2个字节）+OK（ER）+校验
 typedef struct{
   unsigned char  Concentrator_ID[3];
   unsigned char  Net_ID[2];
   unsigned char  Resu[2];
 }APPCON_BATCH_ToKingDaServer_Type;          
   
//查询组建网络状态
  //BA+BA+82+数据长度（两字节）+集中器号（3个字节）+网络号（2个字节）+校验
typedef struct{
  unsigned char  Concentrator_ID[3];
  unsigned short  Net_ID;
 }APPCON_STATUS_ToKingDaConcentrator_Type;      
  //返回：AB+AB+81+数据长度（1个字节）+集中器号（3个字节）+网络号（2个字节）+BUSY（FREE、ERRR）+校验   
typedef struct{
  unsigned char  Concentrator_ID[3];
  unsigned char  Net_ID[2];
  unsigned char  Resu[4];
 }APPCON_STATUS_ToKingDaServer_Type;       
 
//开阀
  //BA+BA+65+数据长度（两字节）+集中器号（3个字节）+网络号（2个字节）+校验
typedef struct{
  unsigned char  Concentrator_ID[3];
  unsigned char  Node_ID[8];  
 }APPCON_OpenOV_ToKingDaConcentrator_Type; 
  //返回：AB+AB+65+数据长度（1个字节）+集中器号（3个字节）+错误代码（2个字节）+校验
typedef struct{
  unsigned char  Concentrator_ID[3];
  unsigned char  Node_ID[8];
  unsigned short Status;
 }APPCON_OpenOV_ToKingDaServer_Type;   
 
//半开阀门
  //BA+BA+74+数据长度（两字节）+集中器号（3个字节）+网络号（2个字节）+校验
typedef struct{
  unsigned char  Concentrator_ID[3];
  unsigned char  Node_ID[8];
 }APPCON_HalfOV_ToKingDaConcentrator_Type;
  //返回：AB+AB+74+数据长度（1个字节）+集中器号（3个字节）+错误代码（2个字节）+校验
typedef struct{ 
  unsigned char  Concentrator_ID[3];
  unsigned char  Node_ID[8];
  unsigned short Status;
 }APPCON_HalfOV_ToKingDaServer_Type;
 
//活动阀门
  //BA+BA+73+数据长度（两字节）+集中器号（3个字节）+网络号（2个字节）+校验
typedef struct{
  unsigned char  Concentrator_ID[3]; 
  unsigned char  Node_ID[8];
 }APPCON_MtOV_ToKingDaConcentrator_Type;            
  //返回：AB+AB+73+数据长度（1个字节）+集中器号（3个字节）+错误代码（2个字节）+校验
typedef struct{
  unsigned char  Concentrator_ID[3];
  unsigned char  Node_ID[8];
  unsigned short Status;
 }APPCON_MtOV_ToKingDaServer_Type; 
 
//关阀
  //BA+BA+64+数据长度（两字节）+集中器号（3个字节）+网络号（2个字节）+校验
typedef struct{
  unsigned char  Concentrator_ID[3];
  unsigned char  Node_ID[8];
 }APPCON_CloseOV_ToKingDaConcentrator_Type;   
  //返回：AB+AB+64+数据长度（1个字节）+集中器号（3个字节）+错误代码（2个字节）+校验
typedef struct{
  unsigned char  Concentrator_ID[3];
  unsigned char  Node_ID[8];
  unsigned short Status;
 }APPCON_CloseOV_ToKingDaServer_Type;
 
//10、查询阀门状态
  //BA+BA+61+数据长度（两字节）+集中器号（3个字节）+网络号（2个字节）+校验
typedef struct{
  unsigned char  Concentrator_ID[3];
  unsigned char  Node_ID[8];
 }APPCON_GetOV_ToKingDaConcentrator_Type;
//返回：AB+AB+61+数据长度（1个字节）+集中器号（3个字节）+错误代码（2个字节）+校验
typedef struct{
  unsigned char  Concentrator_ID[3];
  unsigned char  Node_ID[8];
  unsigned short Status;
 }APPCON_GetOV_ToKingDaServer_Type;


typedef struct{
  unsigned char  Concentrator_ID[3];
  unsigned char  Node_ID[8];
  unsigned short Status;
 }APPCON_OVSTATUS_ToKingDaServer_Type;


//通信数据包头
typedef struct{
 //unsigned char  Sync1;
 //unsigned char  Sync2;
 unsigned short  Sync;
 KingDaServer_CmdCode Cmd;
 unsigned char   Length_H; 
 unsigned char   Length_L;  
}KindgDaServer_PackageHead;
  
typedef union{
 unsigned char                                    data[64];
 HandsData_ToKingDaServer                          HandsData_Send;              //服务器握手命令  集中器向服务器发送
 HandsData_ToKingDaConcentrator                    HandsData_Receive;           //服务器握手命令 服务器返回
 
 ReadMeasureValue_ToKingDaServer_Type              ReadMeasureValue_Send;       //单表数据读取  集中器向服务器发送 
 ReadMeasureValue_ToKingDaConcentrator_Type        ReadMeasureValue_Receive;    //单表数据读取  服务器向集中器发送 
 
 OperateValve_ToKingDaServer_Type                  OperateValve_Send;           //阀门操作  集中器向服务器发送
 OperateValve_ToKingDaConcentrator_Type            OperateValveReceive;         //阀门操作 服务器向集中器发送  
 
 ReadMeasureValue_OFF_ToKingDaServer_Type          ReadMeasureValue_OFF_Send;   //单表数据读取  集中器向服务器发送  (读完关闭通道)
 ReadMeasureValue_OFF_ToKingDaConcentrator_Type    ReadMeasureValue_OFF_Receive;//单表数据读取  服务器向集中器发送  (读完关闭通道)
 TuneUpRTC_ToKingDaServer                          TuneUpRTC_Send;              //服务器向集中器校时命令 集中器向服务器发送
 TuneUpRTC_ToKingDaConcentrator                    TuneUpRTC_Receive;           //服务器向集中器校时命令 服务器向集中器发送       
 GetRepeatersState_ToKingDaServer                  GetRepeatersState_Send;      //查询中继器状态 集中器向服务器发送
 GetRepeatersState_ToKingDaConcentrator            GetRepeatersState_Receive;   //查询中继器状态 服务器向集中器发送
 ReadALLMeasureValue_ToKingDaServer                ReadALLMeasureValue_Send;    //服务器读取数据的命令 集中器向服务器发送
 ReadALLMeasureValue_ToKingDaConcentrator          ReadALLMeasureValue_Receive; //服务器读取数据的命令 服务器向集中器发送
 //ReadNodeDatas_APPCON_ToKingDaServer_Struct        NodeDatas_APPCON;
 ReadALLMeasureValue_ToKingDaServer_APPCON          NodeDatas_APPCON_ToKingDaServer;
 APPCON_ReadMeasureValue_ToKingDaConcentrator_Type  ReadMeasureValue_Receive_APPCON;   //单表数据读取  服务器向集中器发送 
 APPCON_ReadMeasureValue_ToKingDaServer_Type        ReadMeasureValue_Send_APPCON;

 //删除节点命令
 APPCON_DeleteNode_ToKingDaConcentrator_Type        APPCON_DeleteNode_Receive; 
 APPCON_DeleteNode_ToKingDaServer_Type              APPCON_DeleteNode_Send;
 //组网并抄表
 APPCON_BATCH_ToKingDaConcentrator_Type             APPCON_BATCH_Receive; 
 APPCON_BATCH_ToKingDaServer_Type                   APPCON_BATCH_Send;
 //查询组建网络状态 
 APPCON_STATUS_ToKingDaConcentrator_Type            APPCON_STATUS_Receive; 
 APPCON_STATUS_ToKingDaServer_Type                  APPCON_STATUS_Send;
 //开阀
 APPCON_OpenOV_ToKingDaConcentrator_Type            APPCON_OpenOV_Receive; 
 APPCON_OpenOV_ToKingDaServer_Type                  APPCON_OpenOV_Send;
 //半开阀门
 APPCON_HalfOV_ToKingDaConcentrator_Type            APPCON_HalfOV_Receive; 
 APPCON_HalfOV_ToKingDaServer_Type                  APPCON_HalfOV_Send;
 //活动阀门
 APPCON_MtOV_ToKingDaConcentrator_Type              APPCON_MtOV_Receive;    
 APPCON_MtOV_ToKingDaServer_Type                    APPCON_MtOV_Send;
 //关阀
 APPCON_CloseOV_ToKingDaConcentrator_Type           APPCON_CloseOV_Receive; 
 APPCON_CloseOV_ToKingDaServer_Type                 APPCON_CloseOV_Send;
 //查询阀门状态
 APPCON_GetOV_ToKingDaConcentrator_Type             APPCON_GetOV_Receive; 
 APPCON_GetOV_ToKingDaServer_Type                   APPCON_GetOV_Send;
 
 APPCON_OVSTATUS_ToKingDaServer_Type                APPCON_OVSTATUS_Send;  
}KindgDaServer_PackagePram_type;

typedef struct{

 KindgDaServer_PackageHead       head;
 KindgDaServer_PackagePram_type  pram;
}KingDaServer_PackageStruct;

#pragma pack()

typedef void(*KingDaServer_CmdAckFunc)(KingDaServer_PackageStruct* SendBuff, void* AckData);   //AckData  当AckData = P_NULL  则说明数据包应答超时； 

struct KingDaServer_SendBuff{
  union{
    unsigned char Byte;
    struct {
      unsigned char idle :1; //缓冲区空闲
      unsigned char AckEn : 1;  //是否需要应答
      //unsigned char Preamble :1; //序文是否启用  1：启用  0：不启用
    }Bit;
  }CCR;
  unsigned char                  RetransNumber;    //重发次数
  CommPortNumber_Type             TargetPort;
  KingDaServer_CmdAckFunc         AckFunc;  
  KingDaServer_PackageStruct      buff;    
};

typedef struct{
  union{
    unsigned char Byte;
    struct{
      unsigned char DIR :1;
      //unsigned char Preamble :1; //序文是否启用  1：启用  0：不启用
    }Bit;
  }CSR;
  KingDaServer_CmdCode Cmd;            //命令码
  void* Param;                        //命令参数
  unsigned char length;              //命令参数长度
  KingDaServer_CmdAckFunc AckFunc;    //命令应答回调函数
}KingDaServer_Struct;


typedef struct 
{
 unsigned char ID;           //中继器编号
 unsigned char CH_NO[4];     //中继器各通道水表数量
 unsigned char Voltage;
}Repeaters_DescrType;       //中继器描述符
/*********************************************************************************************************
外部变量声明区
*********************************************************************************************************/

/*********************************************************************************************************
函数声明区
*********************************************************************************************************/
void KingDaServer_DeInit(void);

KingDaServerSendBuff_Struct* Get_KingDaServerBuff (void);
void Free_KingDaServerBuff(KingDaServerSendBuff_Struct* pbuff);

//void Free_KingDaServerBuff(KingDaServerSendBuff_Struct* pbuff)

SystemErrName KingDaServer_input (void *pbuff, unsigned char length, CommPortNumber_Type port);

SystemErrName KingDaServer_BuffSend (KingDaServerSendBuff_Struct *pbuff);

SystemErrName SendHandCmdToKingDaServer(unsigned char ServerNO, KingDaServer_CmdAckFunc  AckFunc);

SystemErrName Send_ALLMeasureValue_ToKingDaServer(unsigned char Repeaters_ID, KingDaServer_CmdCode Code);

SystemErrName UpdatMeasureValue_ToKingDaServer(unsigned char Repeaters_NO, 
                                               CommPortNumber_Type Port,
                                               KingDaServer_CmdAckFunc AckFunc);

                                                      
SystemErrName UpdatAppconMeasureValue_ToKingDaServer(unsigned char ServerNo,
                                                     KingDaServer_CmdAckFunc AckFunc);


SystemErrName ReadALLMeasureValue_ToKingDaServer_ACK(unsigned char Repeaters_NO, 
                                               CommPortNumber_Type Port,
                                               KingDaServer_CmdAckFunc AckFunc);

/*SystemErrName ReadAllAppconMeasureValue_ToKingDaServer_ACK(unsigned char Repeaters_NO, 
                                                     CommPortNumber_Type Port,
                                                     KingDaServer_CmdAckFunc AckFunc);*/
/********************************************************************************************************/
#endif

/******************************************END********************************************************/