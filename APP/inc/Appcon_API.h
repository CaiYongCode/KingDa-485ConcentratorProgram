/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
#ifndef  __APPCON_API_H
#define  __APPCON_API_H 
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "..\..\BSP\inc\bsp.h"
#include "..\..\Projects\inc\SofTimer.h"
#include "..\..\SPCP\SPCP_Config.h"
/*********************************************************************************
宏定义区
*********************************************************************************/
/*********************************************************************************
数据类型定义
*********************************************************************************/
typedef enum {
  FREE_Status  = 0,
  CMD_Status   = 1,
  TST_Status   = 2,
  BATCH_Status = 3,
  ReadNode     = 4,
}AppconModuleStatus_Type;


typedef enum {
  OK_AppconModule                = OK_APPCON,
  NoAck_AppconModule             = NoAck_APPCON,   //集中器无应答
  Ans_AppconModule               = Ans_APPCON,     //节点应答
  NoAns_AppconModule             = NoAns_APPCON,  //节点无应答
  BUSY_AppconModule              = BUSY_APPCON,   //网络处与维护状态
  FREE_AppconModule              = FREE_APPCON,   //维护结束

  Err1_AppconModule              = Err1_APPCON,   //错误 1
  Err2_AppconModule              = Err2_APPCON,   //错误 2
  Err3_AppconModule              = Err3_APPCON,   //错误 3
  Err4_AppconModule              = Err4_APPCON,   //错误 4
  Err5_AppconModule              = Err5_APPCON,   //错误 5
  Err6_AppconModule              = Err6_APPCON,   //错误 6

  BATCH_AppconModule             = 0x86,   //维护结束
  
}AppconModuleStatusAckType;


//typedef APPCON_AckType   AppconModuleStatusAckType; 
typedef struct{
  Command_APPCON        AskCmd;
  DataBuffSpec_Struct*   AckDatas;
}AppconMessage_Struct;

typedef void (*MessageFunction)(APPCON_AckType type,AppconMessage_Struct** Message); 
/*********************************************************************************
外部变量声明区
*********************************************************************************/

/*********************************************************************************
函数声明区
*********************************************************************************/
//读单表
SystemErrName Appcon_SingCurrentDataRcmd_Func(CLIFormat_Type  SPCP_Type,
                                              void *CLI, 
                                              MessageFunction Function);

SystemErrName Appcon_OperateWcmd_Func(CLIFormat_Type  SPCP_Type,
                                      void *CLI, 
                                      MessageFunction Function);

SystemErrName Appcon_Reading_Func(CLIFormat_Type  SPCP_Type,
                                  void *CLI, 
                                  MessageFunction Function);

SystemErrName Appcon_ReadNodeDatas_Func(CLIFormat_Type   SPCP_Type,
                                        void *CLI, 
                                        MessageFunction Function);

void SetReadingModuleStatus(AppconModuleStatus_Type New_Status);

AppconModuleStatus_Type GetReadingModuleStatus(void);

SystemErrName ReadAllAppconMeasureValue_ToKingDaServer_ACK(CommPortNumber_Type Port,
                                                           KingDaServer_CmdAckFunc AckFunc);
/********************************************************************************/
#endif