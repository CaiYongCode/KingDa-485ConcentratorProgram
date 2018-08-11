/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
#ifndef  __MCPX_H
#define  __MCPX_H 
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "..\..\BSP\inc\bsp.h"
#include "..\..\Projects\inc\SofTimer.h"
/*********************************************************************************
宏定义区
*********************************************************************************/
#define  SPCP_OutputBuff_Number     16

#define  DomainNameLength           32

#define  AutomatchPort              0
#define  CoercePort_1CH             101
#define  CoercePort_2CH             102
#define  CoercePort_3CH             103
#define  CoercePort_4CH             104
#define  MCPX_InputBuffSize         256     
/*********************************************************************************
数据类型定义
*********************************************************************************/

typedef    struct McpxOutputStruct  MCPX_OutputStruct;

typedef void (*MCPX_RetransEndCallBack_Type)(void *pbuff);

//数据包传递的逻辑方向
enum Package_Dir_Type{ASK =0, ACK = ~ASK} ;


//通信接受缓冲区结构定义
typedef struct {
 CommPortNumber_Type Port; 
 unsigned char length;
 unsigned char buff[MCPX_InputBuffSize];
}MCPX_InputStruct;
//通信发送缓冲区结构定义
struct McpxOutputStruct{
  union {
    unsigned char Byte;
    struct {
      unsigned char idle :1; //缓冲区空闲
    }Bit;
  }CSR; //控制状态寄存器
  union {
    unsigned char Byte;
    struct {
      unsigned char Retrans_SV :4; //设定重发次数
      unsigned char Retrans_PV :4; //还剩重发次数
    }Bit;
  }CCR;  //控制寄存器
 CommPortNumber_Type OutPort;
 unsigned short length;
 void* buff;
 SofTimer_Struct*    RetransTimer;                    //重发定时器
 SerialPort_SendEndCallBack_Type  SendEndCallBack;    //发送完成回调函数
 MCPX_RetransEndCallBack_Type     RetransEndCallBack; //重发结束函数
};

//自动发送时间间隔
typedef  struct{
  unsigned char  day;
  unsigned char  hour ;
  unsigned char  minute ; 
}AutoCollectCycleStruct;
//服务器
typedef  struct {
  unsigned char ip[4] ;
  unsigned char DomainName[DomainNameLength];
  unsigned short port;
}DataServerStruct;
//通道中继器描述
typedef  struct {
  unsigned char min ;
  unsigned char max ;
}ChannelConfigStruct;  
//集中器配置参数表结构

typedef  union{
 unsigned char Byte;
 struct {
 unsigned char ip_flg     :1;        //为“1”数据服务器使用IP，为“0”数据服务器使用域名
 unsigned char Updata_flg :1;        //上传数据周期到达标志
 unsigned char Debug      :1;        // 1：调试模式       0：正常运行模式
 unsigned char ReadAll    :1;        // 1：全网超进行中   0：空闲
 unsigned char Appcon_EN  :1;        //是否接安美通模块
 }Bit;
}CSR_ConcentratorParameter; 

typedef  union{
 unsigned char Byte;
 struct {
 unsigned char App_RdVal_end_01  :1;    //数据读完标志
 unsigned char App_UpLod_end_01  :1;   //上传数据结束标志
 
 unsigned char App_RdVal_end_02  :1;    //数据读完标志
 unsigned char App_UpLod_end_02  :1;   //上传数据结束标志
 unsigned char NewData_flg       :1;   //有新数据未上传
 }Bit;
}SR1_ConcentratorParameter; 

typedef  struct{
  CSR_ConcentratorParameter  CSR;             //控制状态寄存器
  SR1_ConcentratorParameter  SR1;
  unsigned char              id[3] ;          //集中器号
  unsigned char              AppconNetID[8];  //报警手机号码
  AutoCollectCycleStruct     cycle;           //集中器 自动发送时间间隔 
  ChannelConfigStruct        channel[4];      //集中器 各个通道中继器配置 
  DataServerStruct           server1;         //主数据服务器
  DataServerStruct           server2;         //备用服务器
  unsigned char             er_tele[12];     //报警手机号码
}ConcentratorParameterStruct;



typedef struct
  {
    union{
      unsigned char Byte;
      struct{
        unsigned char  CN     :1;    //服务器联接标志   
        unsigned char  IF     :1;    //次联接上服务器首标志  
        unsigned char  RC     :1;    //服务器重联工作中 标志  
        unsigned char  ST     :1;    //服务器 活动标志  
        unsigned char  ACK    :1;    //服务器 应答
        unsigned char  GZ     :1;    //重联超过规次数
      }Bit; 
    }SR;
    CommPortNumber_Type    Port;       //服务器端口
    unsigned char        ServerConnect_CNT ;
    SofTimer_Struct        *Time;  //
  }ServerState_Struct;
/*********************************************************************************
外部变量声明区
*********************************************************************************/
extern unsigned short              Concentrator_DebugTimer;  
extern ConcentratorParameterStruct   Concentrator;
extern ServerState_Struct            ServerState[2];    //集中器状态
extern AutoCollectCycleStruct        ConcentratorUpdataTimer;     //集中器 上传时间

/*********************************************************************************
函数声明区
*********************************************************************************/
void MCPX_DeInit(void);
void MXPC_Thread (void);

void DeInitOutputBuff_MXPC(void);
static MCPX_OutputStruct* GetIdleOutputBuff_MXPC (void);
void FreeOutputBuff_MXPC(MCPX_OutputStruct* pbuff);
void FreeOutputBuffByDataACK_MXPC(void* pbuff, void *data, unsigned short length );

SystemErrName 
MXPC_Send_AutoRetrans ( CommPortNumber_Type  outport,     //目的端口
                        void * pbuff,                    //数据缓冲区
                        unsigned short length,          //数据长度
                        unsigned char retrans,          //重传次数
                        unsigned short cycle,           //重传周期数
                        SofTimer_TB tb,                   //重传周期
                        SerialPort_SendEndCallBack_Type SendEnd_callback,  //发送完成回调
                        MCPX_RetransEndCallBack_Type RetransEnd_callback); //重发完成回调                       
void FreeOutputBuffForData_MXPC (void* pbuff);  
MCPX_OutputStruct* GetOutputBuffForData_MXPC (void* pbuff);   

CommPortNumber_Type getServerPort( unsigned char Server_N0);
void ServerConnect_OK(unsigned char ServerNo, CommPortNumber_Type  Port);
void ServerClose_UEV(unsigned char ch);
void  CommPortClose_UEV (CommPortNumber_Type  Port);
void ReconnecServer( unsigned char ServerNo);
void BreakServerAction(unsigned char ServerNo);
/********************************************************************************/
#endif