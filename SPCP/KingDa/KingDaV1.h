/*********************************************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************************************/
#ifndef  __APP_PPROTOCOL_H
#define  __APP_PPROTOCOL_H
/*********************************************************************************************************
文件包含区
*********************************************************************************************************/
#include "..\..\APP\inc\MCPX.h"
/*********************************************************************************************************
宏定义区
*********************************************************************************************************/
#define  NodeType_KingDaV1     1 //1:集中器 2：中继器  3. 表计   0.工具设备
#define  PProtocl_Preamble_Down   0xFE     //下行帧头
#define  PProtocl_Preamble_Up     0xDF     //上行帧头
#define  PProtocl_Sync_Down       0xA5     //下行同步字
#define  PProtocl_Sync_Up         0x5A     //下行同步字
#define  PProtoclSendBuff_Number  4

#define PProtoco_PackageTypeA     0x00    //数据包结构类型   00: A型     
#define PProtoco_PackageTypeB     0x01    //数据包结构类型   01: B型

//#define  DomainNameLength_KingDa  64
#ifndef  UP
#define  UP     1
#endif

#ifndef  DOWN 
#define  DOWN    0
#endif


/*********************************************************************************************************
数据类型定义
*********************************************************************************************************/

typedef  enum Package_Dir_Type   PProtocol_DIR_Type;

typedef unsigned char   PProtocolPreamble_type;

typedef struct KindgDaSPCPSendBuff  PProtocolSendBuff_Struct;


//KingDa 设备类型
typedef enum{
  KingDa_WaterMeter  = 0x01,   //
  KingDa_Repeaters   = 0x02,
  KingDa_Concen      = 0x03,
}KingDaEquip_type;


typedef struct{
  KingDaEquip_type Type ;
  unsigned char ID[4];  //出厂编号 
  unsigned char ADDR[3];  //网络地址 
}KingDaEquipInfor_struct;

typedef enum{
  NullCmd_KindgDaSPCP  = 0x00,
// 表计作命令
  ReadValue         = 0xD9,
  ReadData          = 0x9d,
  LVP               = 0xA8,
  ReadDDRS          = 0XE5,
  ReadDDR2          = 0XD6,
  WriteDDR2         = 0XB8,
  WriteDDR1         = 0XC4,
  Open_OV           = 0XF1,          //开阀
  Close_OV          = 0XF4,          //关阀
  Get_OV            = 0XF6,          //读阀状态
  Open_OV_ENF       = 0XF5,          //强制开阀
  Close_OV_ENF      = 0XFA,          //强制关阀
  MT_OV             = 0XF2,          //活动阀命令
  MT_OV_ALL         = 0XF3,          //广播活动阀命令
//集中器操作命令
  ReadId_Concentrator          = 0xB7,    //读集中器号
  WriteId_Concentrator         = 0x7B,    //设置集中器号
  ReadTime_Concentrator        = 0xB9,    //读取当前时间
  WriteTime_Concentrator       = 0x9B,    //集中器时间校准命令
  ReadCycle_Concentrator       = 0x87,    //读取自动发送时间间隔
  WriteCycle_Concentrator      = 0x78,    //设置自动发送时间间隔
  ReadChannel_Concentrator     = 0xE9,    //读取各通道起始结束中继器号
  WriteChannel_Concentrator    = 0x9E,    //设置集中器各通道中继器起始结束号
  ReadIpEnd_Concentrator       = 0xD7,    //读取是否开启域名链接服务器
  WriteIpEnd_Concentrator      = 0x7D,    //设置开启、关闭使用域名链接服务器
  ReadServerInf_Concentrator   = 0xA7,    //读取网络参数
  WriteServerInf_Concentrator  = 0x7A,    //网络参数设置 
  ReadServerI_Concentrator     = 0xC7,    //读取集中器域名信息  
  WriteServerI_Concentrator    = 0x7C,    //设置集中器域名
  ReadServerII_Concentrator    = 0xE7,    //读取备用域名信息
  WriteServerII_Concentrator   = 0x7E,    //设置备用域名
//中继器操作命令  
  ReadID_Repeaters                    = 0x87,   //读中继器号
  WriteID_Repeaters                   = 0x86,   //设置中继器号
  OPen_Repeaters                      = 0x99,   //打开中继器命令
  Close_Repeaters                     = 0x98,   //关闭中继器：即使中继器睡眠
  ReadChannelNodeNumber_Repeaters     = 0x9D,   //读取中继器N下各个通道水表数量
  WriteChannelNodeNumber_Repeaters    = 0x9C,   //设置中继器各通道水表数量

//安美通无线操作命令  
  DeleteNet_APPCON                    = 0x61,   //删除网络命令
  BATCH_APPCON                        = 0x62,   //维护网络并抄表命令
  STATUS_APPCON                       = 0x63,   //组网状态查询命令
  ReadNetId_APPCON                    = 0x64,   //读取网络ID命令
  WriteNetId_APPCON                   = 0x65,   //写网络ID命令
  RDNODE_APPCON                       = 0x66,   //读取网络中无线节点ID的数量
  ReadFreq_APPCON                    = 0x67,   //读频率参数命令
  WriteFreq_APPCON                   = 0x68,   //写频率参数命令
  WriteRate_APPCON                   = 0x69,   //写集中器串口速率命令
  ReadNodeValue_APPCON                = 0x70,   //读取无线表单表数据
  OpenOV_APPCON                       = 0x71,   //开阀
  HalfOV_APPCON                       = 0x72,   //半开阀门
  MtOV_APPCON                         = 0x73,   //活动阀命令
  CloseOV_APPCON                      = 0x74,   //关阀
  GetOV_APPCON                        = 0x75,   //读阀状态
  RDROUTER_APPCON                     = 0x76,   ////读网络存在的无线路由模块的数量
  
}CmdCode_KindgDaSPCP;


#pragma pack(1)
//水表数据
typedef struct{
  unsigned char addr[3];
  unsigned char NC;
}ReadValue_Node_ASKstruct;    //水表数据  请求数据
typedef struct{
  unsigned char addr[3];
  unsigned short value;
}ReadValue_Node_ACKstruct;   //水表数据  应答数据

//读水表数据,带原始码
typedef struct{
  unsigned char addr[3];
  unsigned char NC;
}ReadData_Node_ASKstruct;   //读水表数据,带原始码  请求数据
typedef struct{
  unsigned char addr[3];
  unsigned short value;
  unsigned char data[4];
}ReadData_Node_ACKstruct;   //读水表数据,带原始码 应答数据


typedef struct {
  unsigned char addr[3];
  unsigned char flg[2];
}LVP_PRAM_ACKstruct;


typedef struct {
  unsigned char NC[2];
}ReadDDRS_PRAM_ASKstruct;

typedef struct {
  unsigned char DDR1[4];
  unsigned char DDR2[3];
}ReadDDRS_PRAM_ACKstruct;


typedef struct{
  unsigned char DDR1[4];
  unsigned char NC;
}ReadDDR2_Node_ASKstruct;

typedef struct{
  unsigned char DDR1[4];
  unsigned char DDR2[3];
}ReadDDR2_Node_ACKstruct;


typedef struct {
  unsigned char DDR1[4];
  unsigned char DDR2[3];
}WriteDDR2_Node_ASKstruct;// Node  WriteDDR2_PRAM_ASKstruct;

typedef struct{
  //unsigned char DDR1[4];
  unsigned char DDR2[3];
  unsigned char NC;
}WriteDDR2_Node_ACKstruct;  //WriteDDR2_PRAM_ACKstruct;

typedef struct{
  unsigned char DDR1[4];
  unsigned char NC;
}WriteDDR1_PRAM_ASKstruct;

typedef struct{
  unsigned char DDR1[4];
  unsigned char NC;
}WriteDDR1_PRAM_ACKstruct;

typedef struct{
  unsigned char addr[3];
  unsigned char NC;
}Open_OV_ASKstruct;         //开阀

typedef struct{
  unsigned char addr[3];
  unsigned char NC;
}Open_OV_ACKstruct;         //开阀

typedef struct{
  unsigned char addr[3];
  unsigned char NC;
}Close_OV_ASKstruct;        //关阀

typedef struct{
  unsigned char addr[3];
  unsigned char NC;
}Close_OV_ACKstruct;        //关阀

typedef struct{
  unsigned char addr[3];
  unsigned char NC;
}Get_OV_ASKstruct;          //读阀状态

typedef struct{
  unsigned char addr[3];
  unsigned char State;
}Get_OV_ACKstruct;          //读阀状态

typedef struct{
  unsigned char addr[3];
  unsigned char NC;
}Open_OV_ENF_ASKstruct;     //强制开阀

typedef struct{
  unsigned char addr[3];
  unsigned char NC;
}Open_OV_ENF_ACKstruct;     //强制开阀

typedef struct{
  unsigned char addr[3];
}Close_OV_ENF_ASKstruct;    //强制关阀

typedef struct{
  unsigned char addr[3];
  unsigned char NC;
}Close_OV_ENF_ACKstruct;    //强制关阀

typedef struct{
  unsigned char addr[3];
  unsigned char NC;
}MT_OV_ASKstruct;           //活动阀命令

typedef struct{
  unsigned char addr[3];
  unsigned char NC;
}MT_OV_ACKstruct;           //活动阀命令

typedef struct{
  unsigned char addr[3];
}MT_OV_ALL_ASKstruct;       //广播活动阀命令

//读集中器号
typedef struct{
  unsigned char DI1;
  unsigned char DI2;
}ReadId_Concentrator_ASKstruct;       //读集中器号 请求数据
typedef struct{
  unsigned char DI1;
  unsigned char id[3];
}ReadId_Concentrator_ACKstruct;       //读集中器号 应答数据

//设置集中器号
typedef struct{
  unsigned char DI1;
  unsigned char id[3];
  unsigned char DI2;
}WriteId_Concentrator_ASKstruct;       //设置集中器号 请求数据
typedef struct{
  unsigned char DI1;
  //unsigned char DI2; 
  unsigned char state[2];
}WriteId_Concentrator_ACKstruct;       //设置集中器号 应答数据

//读取当前时间
typedef struct{
  unsigned char DI1;
  unsigned char DI2;
}ReadTime_Concentrator_ASKstruct;       //读取当前时间 请求数据
typedef struct{
   unsigned char DI1;
   //unsigned char DI2;
   unsigned char year;
   unsigned char month;
   unsigned char day;
   unsigned char hour;
   unsigned char minute;
   unsigned char second;
}ReadTime_Concentrator_ACKstruct;       //读取当前时间 应答数据

//集中器时间校准命令
typedef struct{
   unsigned char DI1;
   unsigned char year;
   unsigned char month;
   unsigned char day;
   unsigned char hour;
   unsigned char minute;
   unsigned char second;
}WriteTime_Concentrator_ASKstruct;       //集中器时间校准命令 请求数据
typedef struct{
   unsigned char DI1;
   unsigned char state[2];
}WriteTime_Concentrator_ACKstruct;       //集中器时间校准命令 应答数据

//读取自动发送时间间隔
typedef struct{
   unsigned char DI1;
   unsigned char DI2;
}ReadCycle_Concentrator_ASKstruct;       //读取自动发送时间间隔 请求数据

typedef struct{
   unsigned char DI1;
   //unsigned char DI2;
   unsigned char hour;
   unsigned char minute;
}ReadCycle_Concentrator_ACKstruct_A;       //读取自动发送时间间隔 应答数据
typedef struct{
   unsigned char DI1;
   unsigned char Day;
   unsigned char hour;
   unsigned char minute;
}ReadCycle_Concentrator_ACKstruct_B;       //读取自动发送时间间隔 应答数据

//设置自动发送时间间隔
typedef struct{
   unsigned char DI1;
   unsigned char hour;
   unsigned char minute;
}WriteCycle_Concentrator_ASKstruct_TypeA;       //设置自动发送时间间隔 请求数据
//设置自动发送时间间隔
typedef struct{
   unsigned char DI1;
   unsigned char day;
   unsigned char hour;
   unsigned char minute;
}WriteCycle_Concentrator_ASKstruct_TypeB;       //设置自动发送时间间隔 请求数据

typedef struct{
  unsigned char DI1;
  unsigned char state[2];
}WriteCycle_Concentrator_ACKstruct;       //设置自动发送时间间隔 应答数据

//读取集中器各通道起始结束中继器号
typedef struct{
   unsigned char DI1;
   unsigned char DI2;
}ReadChannel_Concentrator_ASKstruct;       //读取集中器各通道起始结束中继器号请求数据
typedef struct{
  unsigned char DI1;
  unsigned char channel[3][2];
}ReadChannel_Concentrator_ACKstruct;       //读取集中器各通道起始结束中继器号 应答数据

//设置集中器各通道中继器起始结束号
typedef struct{
  unsigned char DI1;
  unsigned char channel[3][2];
}WriteChannel_Concentrator_ASKstruct;       //设置集中器各通道中继器起始结束号 请求数据
typedef struct{
  unsigned char DI1;
  unsigned char state[2];
}WriteChannel_Concentrator_ACKstruct;       //设置集中器各通道中继器起始结束号 应答数据
 
//读取是否开启域名链接服务器
typedef struct{
   unsigned char DI1;
   unsigned char DI2;
}ReadIpEnd_Concentrator_ASKstruct;       //读取是否开启域名链接服务器 请求数据
typedef struct{
  unsigned char DI1;
  //unsigned char DI2;
  unsigned char state;
}ReadIpEnd_Concentrator_ACKstruct;       //读取是否开启域名链接服务器 应答数据

 //设置开启、关闭使用域名链接服务器
typedef struct{
  unsigned char DI1;
  unsigned char state;
}WriteIpEnd_Concentrator_ASKstruct;       //设置开启、关闭使用域名链接服务器 请求数据
typedef struct{
  unsigned char DI1;
  unsigned char state[2];
}WriteIpEnd_Concentrator_ACKstruct;       //设置开启、关闭使用域名链接服务器 应答数据


//读取网络参数 
typedef struct{
   unsigned char DI1;
   unsigned char DI2;
}ReadServerInf_Concentrator_ASKstruct;       //读取网络参数  请求数据
typedef struct{
  unsigned char DI1;
  //unsigned char DI2;
  unsigned char IP_M[4];
  unsigned short PORT_M;
  unsigned char IP_S[4];
  unsigned short PORT_S;
  unsigned char  TEL[5];
}ReadServerInf_Concentrator_ACKstruct;       //读取网络参数  应答数据

//网络参数设置
typedef struct{
  unsigned char DI1;
  unsigned char IP_M[4];
  unsigned short PORT_M;
  unsigned char IP_S[4];
  unsigned short PORT_S;
  unsigned char  TEL[5];
}WriteServerInf_Concentrator_ASKstruct;       //网络参数设置 请求数据
typedef struct{
  unsigned char DI1;
  unsigned char state[2];
}WriteServerInf_Concentrator_ACKstruct;       //网络参数设置 应答数据

//读取集中器域名信息  
typedef struct{
  unsigned char DI1;
  unsigned char DI2;
}ReadServer_Concentrator_ASKstruct;       //读取集中器域名信息  请求数据
typedef struct{
  unsigned char DI1;
  //unsigned char DI2;
  unsigned char DomainName[DomainNameLength];
  unsigned short PORT;
}ReadServer_Concentrator_ACKstruct;       //读取集中器域名信息  应答数据 

//设置集中器域名
typedef struct{
  unsigned char DI1;
  unsigned char DomainName[DomainNameLength];
  unsigned short PORT;
}WriteServer_Concentrator_ASKstruct;       //设置集中器域名 请求数据
typedef struct{
  unsigned char DI1;
  unsigned char state[2];
}WriteServer_Concentrator_ACKstruct;       //设置集中器域名 应答数据 

//读取信号强度
typedef struct{
  unsigned char DI1;
  unsigned char DI2;
}ReadGprsRssi_ASKstruct;       //设置集中器域名 请求数据
typedef struct{
  unsigned char DI1;
  unsigned char Rssi;
}ReadGprsRssi_ACKstruct;       //设置集中器域名 应答数据 

typedef struct{
 unsigned char  Sync;
 CmdCode_KindgDaSPCP  Cmd;
 unsigned short  Length; 
}PPro_PackageHead_struct2;

typedef struct{
 PPro_PackageHead_struct2  Head;
 ReadGprsRssi_ACKstruct  data;
 unsigned char  crc;  
}ReadGprsRssi_AckPacage_struct;



//读中继器号
typedef struct{
  unsigned char DI1;
  unsigned char DI2;
}ReadID_Repeaters_ASKstruct;       //读取中继器号 请求数据
typedef struct{
  unsigned char DI1;
  unsigned char DI2;
}ReadID_Repeaters_ACKstruct;       //读取中继器号 请求数据


//设置中继器号
typedef struct{
  unsigned char DI1;
  unsigned char DI2;
}WriteID_Repeaters_ASKstruct;       //设置中继器号 请求数据
typedef struct{
  unsigned char DI1;
  unsigned char DI2;
}WriteID_Repeaters_ACKstruct;       //设置中继器号 应答数据 

//打开中继器命令  
typedef struct{
  unsigned char RepeatersId;
  unsigned char ChannelNumber;
}OPen_Repeaters_ASKstruct;       //打开中继器命令 请求数据
typedef struct{
  //unsigned char DI1;
  //unsigned char DI2;
  unsigned char RepeatersId;
  unsigned char ChannelNumber;
}OPen_Repeaters_ACKstruct;       //打开中继器命令 应答数据 

//关闭中继器命令  
typedef struct{
  unsigned char RepeatersId;
  unsigned char Zero;
}Close_Repeaters_ASKstruct;       //关闭中继器命令 请求数据
typedef struct{
  //unsigned char DI1;
  //unsigned char DI2;
  unsigned char RepeatersId;
  unsigned char Zero;
}Close_Repeaters_ACKstruct;       //关闭中继器命令  应答数据 

//读取中继器N下各个通道水表数量  
typedef struct{
  unsigned char id;
  unsigned char NC;
}ReadChannelNodeNumber_Repeaters_ASKstruct;       //读取中继器N下各个通道水表数量 请求数据
typedef struct{
  unsigned char id;
  unsigned char NodeNumber_channel[4];
}ReadChannelNodeNumber_Repeaters_ACKstruct;       //读取中继器N下各个通道水表数量 应答数据 

//设置中继器各通道水表数量
typedef struct{
  unsigned char id;
  unsigned char NodeNumber_channel[4];
}WriteChannelNodeNumber_Repeaters_ASKstruct;       //设置中继器各通道水表数量 请求数据
typedef struct{
  //unsigned char DI1;
  //unsigned char DI2;  
  unsigned char id;
  unsigned char NodeNumber_channel[4];
}WriteChannelNodeNumber_Repeaters_ACKstruct;       //设置中继器各通道水表数量 应答数据 

//服务器 握手命令
typedef struct{
  unsigned char id;
  unsigned char NodeNumber_channel[4];
}HandsData_ToServer;       //设置中继器各通道水表数量 请求数据
typedef struct{
  //unsigned char DI1;
  //unsigned char DI2;  
  unsigned char id;
  unsigned char NodeNumber_channel[4];
}WriteChannelNodeNumber_Repeaters_ToServer;       //设置中继器各通道水表数量 应答数据 



//通信数据包头
typedef struct{
 unsigned char  Sync;
 CmdCode_KindgDaSPCP  Cmd;
 unsigned char  Length; 
}PPro_PackageHead_TypeA;

typedef struct{
 unsigned char  Sync;
 CmdCode_KindgDaSPCP  Cmd;
 unsigned short  Length; 
}PPro_PackageHead_TypeB;
  
typedef union{
  unsigned char              data[64];
  WriteDDR2_Node_ASKstruct   WriteDDR2_ask;  //写水表地址  请求数据
  WriteDDR2_Node_ACKstruct   WriteDDR2_ack;  //读水表地址  应答数据 

  ReadDDR2_Node_ASKstruct    ReadDDR2_ask;   //读水表地址  请求数据
  ReadDDR2_Node_ACKstruct    ReadDDR2_ack;   //读水表地址  请求数据
  
  ReadValue_Node_ASKstruct   ReadValue_ask;  //水表数据  请求数据
  ReadValue_Node_ACKstruct   ReadValue_ack;  //水表数据  应答数据

  ReadData_Node_ASKstruct    ReadData_ask;   //读水表数据,带原始码  请求数据
  ReadData_Node_ACKstruct    ReadData_ack;   //读水表数据,带原始码  应答数据
  
  Open_OV_ASKstruct          Open_OV_ask;    //开阀
  Open_OV_ACKstruct          Open_OV_ack;    //开阀
  
  Close_OV_ASKstruct         Close_OV_ask;      //关阀
  Close_OV_ACKstruct         Close_OV_ack;      //关阀
  
  Get_OV_ASKstruct           Get_OV_ask;        //读阀状态
  Get_OV_ACKstruct           Get_OV_ack;        //读阀状态

  Open_OV_ENF_ASKstruct      Open_OV_ENF_ask;   //强制开阀
  Open_OV_ENF_ACKstruct      Open_OV_ENF_ack;   //强制开阀

  Close_OV_ENF_ASKstruct     Close_OV_ENF_ask;  //强制关阀
  Close_OV_ENF_ACKstruct     Close_OV_ENF_ack;  //强制关阀
  
  MT_OV_ASKstruct            MT_OV_ask;         //活动阀命令
  MT_OV_ACKstruct            MT_OV_ack;         //活动阀命令
  
  MT_OV_ALL_ASKstruct        MT_OV_ALL_ask;     //广播活动阀命令

  ReadId_Concentrator_ASKstruct          ReadId_ask;            //读集中器号 请求数据
  ReadId_Concentrator_ACKstruct          ReadId_ack;            //读集中器号 应答数据
  WriteId_Concentrator_ASKstruct         WriteId_ask;           //设置集中器号 请求数据
  WriteId_Concentrator_ACKstruct         WriteId_ack;           //设置集中器号 应答数据
  ReadTime_Concentrator_ASKstruct        ReadTime_ask;          //读取当前时间 请求数据
  ReadTime_Concentrator_ACKstruct        ReadTime_ack;          //读取当前时间 应答数据   
  WriteTime_Concentrator_ASKstruct       WriteTime_ask;         //集中器时间校准命令 请求数据
  WriteTime_Concentrator_ACKstruct       WriteTime_ack;         //集中器时间校准命令 应答数据   
  ReadCycle_Concentrator_ASKstruct       ReadCycle_ask;         //读取自动发送时间间隔 请求数据
  ReadCycle_Concentrator_ACKstruct_A     ReadCycle_ack_A;
  ReadCycle_Concentrator_ACKstruct_B     ReadCycle_ack_B;         //读取自动发送时间间隔 应答数据   
  WriteCycle_Concentrator_ASKstruct_TypeA     WriteCycle_ask_TypeA;        //设置自动发送时间间隔 请求数据
  WriteCycle_Concentrator_ASKstruct_TypeB     WriteCycle_ask_TypeB;        //设置自动发送时间间隔 请求数据
  WriteCycle_Concentrator_ACKstruct      WriteCycle_ack;        //设置自动发送时间间隔 应答数据    
  ReadChannel_Concentrator_ASKstruct     ReadChannel_ask;       //读取集中器各通道起始结束中继器号请求数据
  ReadChannel_Concentrator_ACKstruct     ReadChannel_ack;       //读取集中器各通道起始结束中继器号 应答数据   
  WriteChannel_Concentrator_ASKstruct    WriteChannel_ask;      //设置集中器各通道中继器起始结束号 请求数据
  WriteChannel_Concentrator_ACKstruct    WriteChannel_ack;      //设置集中器各通道中继器起始结束号 应答数据   
  ReadIpEnd_Concentrator_ASKstruct       ReadIpEnd_ask;         //读取是否开启域名链接服务器 请求数据
  ReadIpEnd_Concentrator_ACKstruct       ReadIpEnd_ack;         //读取是否开启域名链接服务器 应答数据   
  WriteIpEnd_Concentrator_ASKstruct      WriteIpEnd_ask;        //设置开启、关闭使用域名链接服务器 请求数据
  WriteIpEnd_Concentrator_ACKstruct      WriteIpEnd_ack;        //设置开启、关闭使用域名链接服务器 应答数据     
  ReadServerInf_Concentrator_ASKstruct   ReadServerInf_ask;     //读取网络参数  请求数据
  ReadServerInf_Concentrator_ACKstruct   ReadServerInf_ack;     //读取网络参数  应答数据
  WriteServerInf_Concentrator_ASKstruct  WriteServerInf_ask;    //网络参数设置 请求数据
  WriteServerInf_Concentrator_ACKstruct  WriteServerInf_ack;    //网络参数设置 应答数据   
  ReadServer_Concentrator_ASKstruct     ReadServer_ask;       //读取集中器域名信息  请求数据
  ReadServer_Concentrator_ACKstruct     ReadServer_ack;       //读取集中器域名信息  应答数据 
  WriteServer_Concentrator_ASKstruct    WriteServer_ask;      //设置集中器域名 请求数据
  WriteServer_Concentrator_ACKstruct    WriteServer_ack;      //设置集中器域名 应答数据
  ReadGprsRssi_ASKstruct                ReadGprsRssi_ask;       //设置集中器域名 请求数据
  ReadGprsRssi_ACKstruct                ReadGprsRssi_ack;//设置集中器域名 应答数据
 
  
  ReadChannelNodeNumber_Repeaters_ASKstruct   ReadChannelNodeNumber_Repeaters_ask;       //读取中继器N下各个通道水表数量 请求数据
  ReadChannelNodeNumber_Repeaters_ACKstruct   ReadChannelNodeNumber_Repeaters_ack;       //读取中继器N下各个通道水表数量 应答数据 
  WriteChannelNodeNumber_Repeaters_ASKstruct  WriteChannelNodeNumber_Repeaters_ask;      //设置中继器各通道水表数量 请求数据
  WriteChannelNodeNumber_Repeaters_ACKstruct  WriteChannelNodeNumber_Repeaters_ack;      //设置中继器各通道水表数量 应答数据 
  OPen_Repeaters_ASKstruct                    OPen_Repeaters_ask;                        //打开中继器命令 请求数据
  OPen_Repeaters_ACKstruct                    OPen_Repeaters_ack;                        //打开中继器命令 应答数据 
  Close_Repeaters_ASKstruct                   Close_Repeaters_ask;                       //关闭中继器命令 请求数据
  Close_Repeaters_ACKstruct                   Close_Repeaters_ack;                       //关闭中继器命令  应答数据 
  ReadID_Repeaters_ASKstruct                  ReadID_Repeaters_ask;                      //读取中继器号 请求数据
  ReadID_Repeaters_ACKstruct                  ReadID_Repeaters_ack;                      //读取中继器号 请求数据
  WriteID_Repeaters_ASKstruct                 WriteID_Repeaters_ask;                     //设置中继器号 请求数据
  WriteID_Repeaters_ACKstruct                 WriteID_Repeaters_ack;                     //设置中继器号 应答数据 
}PProtocolPackagePram_type;



typedef union{
  struct{
     PPro_PackageHead_TypeA     head;
     PProtocolPackagePram_type  pram;
  }TypeA;
  struct{
     PPro_PackageHead_TypeB     head;
     PProtocolPackagePram_type  pram;
  }TypeB; 
}KindgDaSPCP_PackageDataStruct;


typedef struct{
 PProtocolPreamble_type Prm1;
 PProtocolPreamble_type Prm2;
 KindgDaSPCP_PackageDataStruct Data;  
}KindgDaSPCP_PackageStruct;
#pragma pack()

typedef void(*KindgDaSPCPCmdCode_AckFunc)(PProtocolSendBuff_Struct* SendBuff, void* AckData);   //AckData  当AckData = P_NULL  则说明数据包应答超时； 

struct KindgDaSPCPSendBuff{
  union{
    unsigned char Byte;
    struct {
      unsigned char idle :1; //缓冲区空闲
      unsigned char Preamble :1; //序文是否启用  1：启用  0：不启用
      unsigned char OFF : 1;     //处理完毕后是否关闭发射通道   1：主动关闭  0：不处理
      unsigned char TypeA_B : 2;   //数据包结构类型   00: A型     01 :B型
    }Bit;
  }CCR;
  CommPortNumber_Type             SourcePort;
  CommPortNumber_Type             TargetPort;
  KindgDaSPCPCmdCode_AckFunc      AckFunc;
  KindgDaSPCP_PackageStruct       buff;    
};

typedef struct{
  union{
    unsigned char Byte;
    struct{
      unsigned char DIR :1;
      unsigned char Preamble :1; //序文是否启用  1：启用  0：不启用
    }Bit;
  }CSR;
  CommPortNumber_Type SourcePort;     //原始端口
  CmdCode_KindgDaSPCP Cmd;           //命令码
  unsigned char TargetPort;           //发送的预期地址 
  void* Param;                        //命令参数
  unsigned char length;              //命令参数长度
  KindgDaSPCPCmdCode_AckFunc AckFunc;//命令应答回调函数
}KindgDaCmd_Struct;


typedef struct{
  union{
    unsigned char Byte;
    struct{
      unsigned char Preamble :1; //序文是否启用  1：启用  0：不启用
    }Bit;
  }CSR;
  CommPortNumber_Type SourcePort;     //原始端口
  CommPortNumber_Type TargetPort;     //转发的目标端口
  KindgDaSPCP_PackageDataStruct* pbuff;  //命令参数
}Transpond_KingDaSPCP_struct;

/*********************************************************************************************************
外部变量声明区
*********************************************************************************************************/

/*********************************************************************************************************
函数声明区
*********************************************************************************************************/
void KingDaV1Protocol_DeInit(void);

CommPortNumber_Type getPortForAddr ( unsigned char addr);

SystemErrName SendCmd_KindDaSPCP(KindgDaCmd_Struct* Cmd);

SystemErrName KingDaV1Protocol_input (void *pbuff, 
                                      unsigned char length,
                                      CommPortNumber_Type port);

SystemErrName ReadNodeValue_KindDa(unsigned char* NoderAddr,
                                   unsigned char SendPort,  
                                   CommPortNumber_Type SourcePort,
                                   KindgDaSPCPCmdCode_AckFunc AckFunc);

SystemErrName  OperateValve_KindDa(CmdCode_KindgDaSPCP Operate,
                                   unsigned char* NoderAddr,
                                   unsigned char SendPort,
                                   CommPortNumber_Type SourcePort,                                   
                                   KindgDaSPCPCmdCode_AckFunc AckFunc);

SystemErrName OpenNETChannel_KindDa(unsigned char RepeatersId, 
                                    unsigned char ChannelNumber,
                                    CommPortNumber_Type SourcePort,
                                    KindgDaSPCPCmdCode_AckFunc AckFunc);

SystemErrName CloseNETChannel_KindDa (unsigned char RepeatersId, 
                                      unsigned char ChannelNumber,
                                      CommPortNumber_Type SourcePort,
                                      KindgDaSPCPCmdCode_AckFunc AckFunc);

SystemErrName CloseRepeatersId_KindDa ( unsigned char RepeatersId, 
                                       CommPortNumber_Type SourcePort,
                                       KindgDaSPCPCmdCode_AckFunc AckFunc);

SystemErrName ReadRepeatersNodeNumber_KindDa ( unsigned char RepeatersId,
                                               CommPortNumber_Type SourcePort,
                                               KindgDaSPCPCmdCode_AckFunc AckFunc);
/********************************************************************************************************/
#endif

/******************************************END********************************************************/