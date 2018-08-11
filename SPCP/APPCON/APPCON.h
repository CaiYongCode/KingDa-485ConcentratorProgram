/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
#ifndef  __SPCP_APPCON_H
#define  __SPCP_APPCON_H 

#ifndef  IDLE 
#define  IDLE  0
#endif

#ifndef  INUSE
#define  INUSE  1
#endif
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "..\..\Projects\inc\SystemDefine.h"
#include "..\..\Projects\inc\Common_API.h"
#include "..\..\APP\inc\MCPX.h"
//#include "..\..\Projects\inc\SofTimer.h"
/*********************************************************************************
宏定义区
*********************************************************************************/


#define  APPCON_OutPutBuff_Number   4
#define  APPCON_OutPutBuffLength    32

#define  CmdAckListNumber   11

//获取无线模块电池信息命令
#define  APPCON_GetBat_SendFunc(id,AckFunc)   APPCON_TST_SendFunc(unsigned char *id,0,AckFunc);
//#define  MG2618_PowerFlg              (GPRS_ENpin)            
//#define  MG2618_PowerON( )            set_GPRS_ENpin         
//#define  MG2618_PowerOFF( )           clr_GPRS_ENpin

//#define  ConfigUsart_MG2616(BaudRate,Parity,StopBits,mod)           Launch_USART2(BaudRate, Parity, StopBits, mod)                           
//#define  GetDataForUsart_MG261( )                                   GetDataForBuff_USART2( )
//#define  FreeUsartInputBuff_MG261(buff)                             FreeBuff_USART2(buff)
//#define  SendDataToMG261_ByIT(buff,length,callBack,callBack_arg)    sendBuffByIT_USART2(buff,length,callBack,callBack_arg)
//#define  SendDataToMG261_ByDMA(buff,length,callBack,callBack_arg)   sendBuffByDMA_USART2(buff, length,callBack,callBack_arg)
//#define  SendStringToMG261_ByIT(String)                             sendStringByIT_USART2(String)
//#define  SendStringToMG261_ByDMA(String)                            sendStringByDMA_USART2(String)
//#define  SendDataToMG2618(buff,length,callBack)                     SendDataToMG261_ByDMA(buff,length,callBack,P_NULL)
//#define  SendStringToMG2618(String)                                 SendStringToMG261_ByDMA(String)    
  

/*********************************************************************************
数据类型定义
*********************************************************************************/

#pragma pack(1)

typedef union {
 unsigned char    Byte;
 struct{
   unsigned char NB0 : 1;
 }Bit;
}APPCON_MeterState_Struct;

typedef struct {
 unsigned char    Value[4];
 APPCON_MeterState_Struct  State;
 unsigned char    Bat;
}APPCON_MeterValue_Struct;


typedef struct {
 unsigned char    link;
 unsigned char    spa1;
 unsigned char    route;
 unsigned char    spa2;
 unsigned char    id[8];
 unsigned char    spa3;
 unsigned char    value[12];
 unsigned char    state[2];
 unsigned char    spa5;
 unsigned char    bat[2];
}APPCON_RDNODE_AckData_Struct;


typedef struct {
 unsigned char    value[4][3];
 unsigned char    state[2];
 unsigned char    spa;
 unsigned char    bat[2];
}APPCON_CMD_AckData_Struct;

typedef struct {
 unsigned char    state[2];
 unsigned char    spa;
 unsigned char    bat[2];
}APPCON_TST_Valve_AckData_Struct;

#pragma pack()

typedef enum{ 
 _1200bps    = 1200,
 _2400bps    = 2400, 
 _4800bps    = 4800, 
 _9600bps    = 9600,
 _19200bps    = 19200, 
 _38400bps    = 38400, 
 _57600bps    = 57600, 
 _115200bps    = 115200, 
}BaudRate_APPCON;

typedef enum{
  ValveUnState_APPCON         = 0XC0,  //状态不明或阀门正在执行动作中
  HalfValveState_APPCON       = 0XC1,  //阀门处于半开状态
  CloseValveState_APPCON      = 0XC2,  //阀门处于关闭状态
  OpenValveState_APPCON       = 0XC3,  //阀门处于打开状态
}ValveState_APPCON;

typedef enum{
  GetValveCmd_APPCON        = 0XC0,  //查询阀门状态
  HalfValveCmd_APPCON       = 0XC1,  //阀门半开指令
  OpenValveCmd_APPCON       = 0XC3,  //阀门打开指令
  CloseValveCmd_APPCON      = 0XC2,  //阀门关闭指令
  MT_ValveCmd_APPCON        = 0XC4,  //阀门防锈指令
}ValveCmd_APPCON;



typedef enum{ 
 DELNET    = 0,   //删除网络命令。删除模块，路由器数据
 CMD       = 1,   //上位机与无线模块模块通讯命令  实时抄表
 TST       = 2,   //测试无线模块命令和控阀命令
 RDNODE    = 3,   //读网络存在的无线表模块ID 的命令
 RDROUTER  = 4,   //读网络存在的无线路由模块ID 的命令
 FREQ      = 5,   //读写频率参数命令
 NETID     = 6,    //读写网络ID 命令 
 BATCH     = 7,   //组网即群表命令  
 STATUS    = 8,   //状态查询命令
 RATE      = 9,  //读写集中器串口速率命令
 MRATE     = 10,  //读写无线模块串口速率参数命令
 VER       = 11,  //集中器软硬件版本命令
 HELP      = 12,  //帮助命令
 TXD       = 13,  //集中器与掌机通讯命令
 SETID     = 14,  //集中器无线设置ID 参数命令
 
 NOP       = 15,
}Command_APPCON;

typedef enum{
  OK_APPCON                = 0x00,
  NoAck_APPCON             = 0x01,   //集中器无应答
  Ans_APPCON               = 0x02,   //节点应答
  NoAns_APPCON             = 0x03,   //节点无应答
  BUSY_APPCON              = 0x04,   //网络处与维护状态
  FREE_APPCON              = 0x05,   //维护结束
  TOTAL_APPCON             = 0x06,   //所有模块的总数
  
  Err1_APPCON              = 0x11,   //错误 1
  Err2_APPCON              = 0x12,   //错误 2
  Err3_APPCON              = 0x13,   //错误 3
  Err4_APPCON              = 0x14,   //错误 4
  Err5_APPCON              = 0x15,   //错误 5
  Err6_APPCON              = 0x16,   //错误 6
  
  AnsDatas_APPCON          = 0x20,   //
  
  
  ReadNodeEnd              =0x60,          
}APPCON_AckType;


typedef   struct APPCON_OutPutBuff_struct   APPCON_OutPutBuff_Struct;

typedef void(*APPCON_AckFunc)(APPCON_OutPutBuff_Struct *cmd, 
                              APPCON_AckType AckType, 
                              DataBuffSpec_Struct* AckData);

struct APPCON_OutPutBuff_struct{
  union{
    unsigned char Byte;
    struct{
      unsigned char idle :1; //缓冲区空闲
      unsigned char ack1 :1; //第一次应答标志
      unsigned char ack2 :1; //第二次应答标志
    }Bit;
  }CSR; //控制状态寄存器
 Command_APPCON       Cmd;    //命令 
 CommPortNumber_Type  SourcePort;
 APPCON_AckFunc       AckFunc;
 void*                AckFuncParam; 
 unsigned char        BuffLength;
 unsigned char        Buff[APPCON_OutPutBuffLength];
};
/*********************************************************************************
外部变量声明区
*********************************************************************************/
/*********************************************************************************
函数声明区
*********************************************************************************/
void APPCON_SPCP_init(void);

void freeSendBuff_APPCON(void* frame);

void APPCON_InPut(void *pbuff, unsigned length,CommPortNumber_Type port);

void Decode_APPCON(DataBuffSpec_Struct *Data);

//删除网络命令。删除模块，路由器数据
SystemErrName APPCON_DELNET_SendFunc(CommPortNumber_Type Source, 
                                     void* AckFuncParam,
                                      APPCON_AckFunc AckFunc);
//上位机与无线模块模块通讯命令
SystemErrName  APPCON_CMD_SendFunc (CommPortNumber_Type Source,
                                    void* id,
                                    void* AckFuncParam,
                                    APPCON_AckFunc  AckFunc);
//测试无线模块命令和控阀命令
SystemErrName  APPCON_TST_SendFunc(CommPortNumber_Type Source,
                                   unsigned char *id,
                                   unsigned char AnsNumber,
                                   void* AckFuncParam,
                                   APPCON_AckFunc  AckFunc);
//无线模块控阀命令 
SystemErrName  APPCON_ValveCmd_SendFunc(CommPortNumber_Type Source,
                                        void *id,
                                        ValveCmd_APPCON  cmd,
                                        void* AckFuncParam,
                                        APPCON_AckFunc  AckFunc);

//读网络存在的无线表模块ID 的命令
SystemErrName  APPCON_RDNODE_SendFunc(CommPortNumber_Type Source,
                                      void* id,
                                      unsigned char Number,
                                      void* AckFuncParam,
                                      APPCON_AckFunc AckFunc);
//读网络存在的无线路由模块ID 的命令
SystemErrName  APPCON_RDROUTER_SendFunc(CommPortNumber_Type Source,
                                        //unsigned char *id,
                                        unsigned char Number,
                                        void* AckFuncParam,
                                        APPCON_AckFunc AckFunc);
//读写频率参数命令
SystemErrName  APPCON_FREQ_SendFunc (CommPortNumber_Type Source,
                                     unsigned char* freq,
                                     void* AckFuncParam,
                                     APPCON_AckFunc AckFunc);
//读写网络ID 命令
SystemErrName  APPCON_NETID_SendFunc (CommPortNumber_Type Source,
                                      unsigned char* id,
                                      void* AckFuncParam,
                                      APPCON_AckFunc AckFunc);
//组网并抄表命令 
SystemErrName  APPCON_BATCH_SendFunc (CommPortNumber_Type Source,
                                      void* AckFuncParam,
                                      APPCON_AckFunc AckFunc);
//状态查询命令
SystemErrName  APPCON_STATUS_SendFunc (CommPortNumber_Type Source,
                                       void* AckFuncParam,
                                       APPCON_AckFunc AckFunc);
//读写集中器串口速率命令
SystemErrName  APPCON_RATE_SendFunc (CommPortNumber_Type Source,
                                     BaudRate_APPCON rate,
                                     void* AckFuncParam,
                                     APPCON_AckFunc AckFunc); 
//读写无线模块串口速率参数命令
SystemErrName  APPCON_MRATE_SendFunc (CommPortNumber_Type Source,
                                      unsigned char dir, 
                                      BaudRate_APPCON rate,
                                      void* AckFuncParam,
                                      APPCON_AckFunc AckFunc); 
//集中器软硬件版本命令
SystemErrName  APPCON_VER_SendFunc (CommPortNumber_Type Source,
                                    void* AckFuncParam,
                                    APPCON_AckFunc AckFunc );
//帮助命令
SystemErrName  APPCON_HELP_SendFunc (CommPortNumber_Type Source,
                                     void* AckFuncParam,
                                     APPCON_AckFunc AckFunc);
 //集中器与掌机通讯命令
SystemErrName APPCON_TXD_SendFunc (CommPortNumber_Type Source,
                                   void *buff,
                                   unsigned char length,
                                   void* AckFuncParam,
                                   APPCON_AckFunc AckFunc); 
//集中器无线设置ID 参数命令
SystemErrName  APPCON_SETID_SendFunc (unsigned long command ,//口令  
                                      unsigned long f,
                                      unsigned short id,
                                      void* AckFuncParam,
                                      APPCON_AckFunc AckFunc) ;  
//ASCII码的16进制变换为实数
unsigned long   APPCON_AsciiHexToDigit (unsigned char* hex, 
                                       unsigned char length, 
                                       unsigned char width ) ;
/********************************************************************************/
#endif
