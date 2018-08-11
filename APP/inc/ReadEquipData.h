/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
#ifndef  __READ_EQUIP_DATA_H
#define  __READ_EQUIP_DATA_H 
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "..\inc\EquipMgmt.h"
/*********************************************************************************
宏定义区
*********************************************************************************/
#define NodeValuelist_Length  2000
#define LaunchReadAllValue_KindgDa( )              ReadValueAll_KindgDa_AckFunc(P_NULL,P_NULL);          
                                                    //Appcon_Reading_Func(VOID_Format,P_NULL,P_NULL)
                                                    //Appcon_ReadNodeDatas_Func(VOID_Format,P_NULL,P_NULL)
                                                    //ReadValueAll_KindgDa_AckFunc(P_NULL,P_NULL)
                                                    //ReadAllAppconMeasureValue_ToKingDaServer_ACK( Rs232_Port,P_NULL)
#define LaunchSendMeasureValue_ToKingDaServer1( )   SendMeasureValue_ToKingDaServer1(P_NULL,P_NULL);
#define LaunchSendMeasureValue_ToKingDaServer2( )   SendMeasureValue_ToKingDaServer2(P_NULL,P_NULL);

#define  KingDaRecord_Type       0x00
#define  AppconDaRecord_Type     0x01
/*********************************************************************************
数据类型定义
*********************************************************************************/
typedef union{
  unsigned char Byte;
  struct{
    unsigned char Channel1 : 1;
    unsigned char Channel2 : 1;
    unsigned char Channel3 : 1;
    unsigned char Channel4 : 1;
  }Bit;
}RepeatersChannelState_Type;


typedef struct{
  enum {
   OPenNETChannel   =  1,
   CloseNETChannel  =  2,
   ReadRepeaters    =  3,  
   ReadNodeValue    =  4,
  }Cmd;
  enum {
   AckOK    = 1,
   TimeOut  = 2,
  }State;
 }CmdAckState_Struct;


#pragma pack(1)
/*
typedef struct 
{
  union{
    unsigned char byte;
    struct {
   unsigned char opt :1; 
   unsigned char FL  :1;    //数据占位标志，存在有效数据
  }bit;
 }CSR;
 unsigned char NodeId[3];
 unsigned short NodeValue;
 struct{
 unsigned char second;
 unsigned char minute;
 unsigned char hour;
 }Timer;
}NodeValue_RecordType;
*/
typedef struct {
  unsigned long id;
  unsigned long value;
  unsigned char vbat;
  unsigned char link;
  unsigned char route;
}AppconRecord_Struct;

typedef struct{
  unsigned char NodeId[3];
  unsigned short NodeValue;
  struct{
    unsigned char second;
    unsigned char minute;
    unsigned char hour;
  }Timer;
}KingDaRecord_Struct;


typedef struct 
{
  union{
    unsigned char byte;
    struct {
   //unsigned char opt :1;
   unsigned char news1 :1;
   unsigned char news2 :1; 
   unsigned char FL  :1;    //数据占位标志，存在有效数据
   unsigned char Type:4;
  }bit;
 }CSR;
 union
 {
  KingDaRecord_Struct  KingDa; 
  AppconRecord_Struct  Appcon;
 }datas;

}NodeValue_RecordType;


typedef struct 
{
 struct{
 unsigned char day;
 unsigned char month;
 unsigned short year;
 }Timer;
}NodeValue_RecordDescType;
#pragma pack(1)


/*********************************************************************************
外部变量声明区
*********************************************************************************/

/*********************************************************************************
函数声明区
*********************************************************************************/
SystemErrName ReadNodeVlue(EquipRecord_struct* equip, KindgDaSPCPCmdCode_AckFunc AckFunc);
void ReadValueAll_KindgDa_AckFunc (PProtocolSendBuff_Struct* SendBuff, void* AckData);

SystemErrName write_NodeValueRecord_KindgDa (unsigned char* adrr,
                                             unsigned long value,
                                             unsigned char adrrWidth);

SystemErrName write_NodeValueRecord_Appcon (APPCON_RDNODE_AckData_Struct* Data);

void rset_NodeValueRecord_Appcon ( void );

SystemErrName read_TodayNodeValueRecord_KindgDa (unsigned char* adrr, NodeValue_RecordType *record);

void SendMeasureValue_ToKingDaServer1(KingDaServer_PackageStruct* SendBuff, void* AckData);
void SendMeasureValue_ToKingDaServer2(KingDaServer_PackageStruct* SendBuff, void* AckData);

void SendMeasureValue_ToKingDaServer1_ACK(KingDaServer_PackageStruct* SendBuff, void* AckData);
void SendMeasureValue_ToKingDaServer2_ACK(KingDaServer_PackageStruct* SendBuff, void* AckData);
/********************************************************************************/
#endif
