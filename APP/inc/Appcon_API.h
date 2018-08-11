/*********************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
#ifndef  __APPCON_API_H
#define  __APPCON_API_H 
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include "..\..\BSP\inc\bsp.h"
#include "..\..\Projects\inc\SofTimer.h"
#include "..\..\SPCP\SPCP_Config.h"
/*********************************************************************************
�궨����
*********************************************************************************/
/*********************************************************************************
�������Ͷ���
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
  NoAck_AppconModule             = NoAck_APPCON,   //��������Ӧ��
  Ans_AppconModule               = Ans_APPCON,     //�ڵ�Ӧ��
  NoAns_AppconModule             = NoAns_APPCON,  //�ڵ���Ӧ��
  BUSY_AppconModule              = BUSY_APPCON,   //���紦��ά��״̬
  FREE_AppconModule              = FREE_APPCON,   //ά������

  Err1_AppconModule              = Err1_APPCON,   //���� 1
  Err2_AppconModule              = Err2_APPCON,   //���� 2
  Err3_AppconModule              = Err3_APPCON,   //���� 3
  Err4_AppconModule              = Err4_APPCON,   //���� 4
  Err5_AppconModule              = Err5_APPCON,   //���� 5
  Err6_AppconModule              = Err6_APPCON,   //���� 6

  BATCH_AppconModule             = 0x86,   //ά������
  
}AppconModuleStatusAckType;


//typedef APPCON_AckType   AppconModuleStatusAckType; 
typedef struct{
  Command_APPCON        AskCmd;
  DataBuffSpec_Struct*   AckDatas;
}AppconMessage_Struct;

typedef void (*MessageFunction)(APPCON_AckType type,AppconMessage_Struct** Message); 
/*********************************************************************************
�ⲿ����������
*********************************************************************************/

/*********************************************************************************
����������
*********************************************************************************/
//������
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