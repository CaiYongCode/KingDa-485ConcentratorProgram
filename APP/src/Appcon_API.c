/*********************************************************************************
//概述：万络设备管理
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "..\inc\Appcon_API.h"
#include "..\..\Projects\inc\SystemDefine.h"
#include "..\..\LIB\FAT\ff.h"
//#include "..\..\SPCP\JingDa\JINGDA.h"
#include "..\..\SPCP\APPCON\APPCON.h"
#include "..\inc\ReadEquipData.h"
/*********************************************************************************
常量定义区
*********************************************************************************/

/*********************************************************************************
公共变量定义区
*********************************************************************************/
/*********************************************************************************
外部变量声明区
*********************************************************************************/

/*********************************************************************************
私有变量定义区
*********************************************************************************/
static AppconModuleStatus_Type   ModuleStatus = FREE_Status; 
static SofTimer_Struct*          ReadingTime = P_NULL;
static unsigned char             ReadingStep = 0;
CLI_Struct                       Present_CLI ={VOID_Format,P_NULL};
static MessageFunction           ActionMessageFunc =P_NULL;

static unsigned short   TotalNumber = 0; 
static unsigned short   RDNODE_Number = 0;
static unsigned char    RDNODE_ID [8];

static KingDaServerSendBuff_Struct* ReadAllAppcon_AckBuff = P_NULL ;
/*********************************************************************************
测试变量定义区
*********************************************************************************/
/*********************************************************************************
内部函数定义区
*********************************************************************************/
static void Appcon_Reading_AckFunc(APPCON_OutPutBuff_Struct *cmd, 
                                   APPCON_AckType AckType, 
                                   DataBuffSpec_Struct* AckData);

static void Appcon_SingCurrentDataRcmd_AckFunc(APPCON_OutPutBuff_Struct *cmd, 
                                                      APPCON_AckType AckType, 
                                                      DataBuffSpec_Struct* AckData);

//static void SetReadingModuleStatus(AppconModuleStatus_Type New_Status);

static void MessageDefaultFunction(APPCON_AckType type,AppconMessage_Struct* Message);

static void DefaulMessagetFunction(APPCON_AckType type,AppconMessage_Struct* Message);

static void freePresent_CLI(void);
/*********************************************************************************
功能代码定义区
*********************************************************************************/
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static void Appcon_SingCurrentDataRcmd_AckFunc(APPCON_OutPutBuff_Struct *cmd, 
                                               APPCON_AckType AckType, 
                                               DataBuffSpec_Struct* AckData)
{
 unsigned char id [8];
 unsigned char itmp; 
 AppconMessage_Struct Message;
 if(STATUS == cmd->Cmd)
 {
   if(FREE_APPCON ==  AckType)
   { 
     itmp = 0;  
     if(JINGDA == Present_CLI.Format)
     {
      while (itmp < 8) 
      {
        id[itmp] = (Present_CLI.Data.JingDa_CLI->Frame.Datas.Param.SingCurrentDataRcmd_ask.Meter.Addr.ID[itmp >> 1]) & 0x0f;
        if(id[itmp] < 0x0A)
        {id[itmp] += '0';}
        else
        {id[itmp] += 'A' - 0x0A;}
        itmp ++;
        id[itmp] = (Present_CLI.Data.JingDa_CLI->Frame.Datas.Param.SingCurrentDataRcmd_ask.Meter.Addr.ID[itmp >> 1]) & 0x0f;
        if(id[itmp] < 0x0A)
        {id[itmp] += '0';}
        else
        {id[itmp] += 'A' - 0x0A;}
        itmp ++;
      }
     }
     APPCON_CMD_SendFunc (Null_Port,id,cmd->AckFuncParam,Appcon_SingCurrentDataRcmd_AckFunc);
   }
   else
   {MessageDefaultFunction(AckType,  &Message);}
 }
 if((CMD != cmd->Cmd) || (P_NULL == cmd->AckFuncParam))
 {return ;}
 if(Ans_APPCON == AckType)
 {MessageDefaultFunction(AckType,&Message);}
 else
 { 
  MessageDefaultFunction(AckType,&Message);  
 }
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName  Appcon_SingCurrentDataRcmd_Func(CLIFormat_Type  SPCP_Type,
                                               void *CLI, 
                                               MessageFunction Function)
{
 SystemErrName err ;
 if(FREE_Status == GetReadingModuleStatus( ))
 {
   err = APPCON_STATUS_SendFunc(Null_Port, P_NULL, Appcon_SingCurrentDataRcmd_AckFunc);
   if(NO_ERR == err)
   {
     Present_CLI.Data.Super_CLI = CLI;
     Present_CLI.Format = SPCP_Type;
     ActionMessageFunc = Function;
     SetReadingModuleStatus(CMD_Status);
   }
 }
 {err = Nothing;}
 
 return err;
}

/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static void Appcon_OperateWcmd_AckFunc(APPCON_OutPutBuff_Struct *cmd, 
                                              APPCON_AckType AckType, 
                                              DataBuffSpec_Struct* AckData)
{
  unsigned char id [8];
  unsigned char itmp;
  AppconMessage_Struct  Message;
  if(STATUS == cmd->Cmd)
  {
    if(FREE_APPCON ==  AckType)
    {
     itmp = 0;  
     if(JINGDA == Present_CLI.Format)
     {
      while (itmp < 8) 
      {
        id[itmp] = (Present_CLI.Data.JingDa_CLI->Frame.Datas.Param.SingCurrentDataRcmd_ask.Meter.Addr.ID[itmp >> 1]) & 0x0f;
        if(id[itmp] < 0x0A)
        {id[itmp] += '0';}
        else
        {id[itmp] += 'A' - 0x0A;}
        itmp ++;
        id[itmp] = (Present_CLI.Data.JingDa_CLI->Frame.Datas.Param.SingCurrentDataRcmd_ask.Meter.Addr.ID[itmp >> 1]) & 0x0f;
        if(id[itmp] < 0x0A)
        {id[itmp] += '0';}
        else
        {id[itmp] += 'A' - 0x0A;}
        itmp ++;
      }
      //阀门状态(HEX) 0X99：关阀；0X55：开阀
      if( 0X99 ==Present_CLI.Data.JingDa_CLI->Frame.Datas.Param.OperateWcmd_ask.Operate.MeterState)  
      {APPCON_ValveCmd_SendFunc(Null_Port, id, OpenValveCmd_APPCON,cmd->AckFuncParam,Appcon_OperateWcmd_AckFunc);}
      else if( 0X55 == Present_CLI.Data.JingDa_CLI->Frame.Datas.Param.OperateWcmd_ask.Operate.MeterState)
      {APPCON_ValveCmd_SendFunc(Null_Port, id,CloseValveCmd_APPCON,cmd->AckFuncParam,Appcon_OperateWcmd_AckFunc);}
     }
    }
    else
    {MessageDefaultFunction(AckType,&Message);}
  }
 if((TST != cmd->Cmd) ||(P_NULL == cmd->AckFuncParam))
 {return ;}
 
 if(Ans_APPCON == AckType)
 {
  MessageDefaultFunction(AckType,&Message);  
 }
 else
 { MessageDefaultFunction(AckType,&Message); }
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName Appcon_OperateWcmd_Func(CLIFormat_Type  SPCP_Type,
                                             void *CLI, 
                                             MessageFunction Function)
{
 SystemErrName err ;
 if(FREE_Status == GetReadingModuleStatus( ))
 {
  err = APPCON_STATUS_SendFunc(Null_Port, P_NULL,Appcon_OperateWcmd_AckFunc);
  if(NO_ERR == err)
  {
    Present_CLI.Data.Super_CLI = CLI;
    Present_CLI.Format = SPCP_Type;
    ActionMessageFunc = Function;
    SetReadingModuleStatus(TST_Status);
  }
 }
 {err =  Nothing;}
 return err;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static void QueryAppconStatus(void *p_tmr, void *p_arg)
{
  SystemErrName err;
  err =APPCON_STATUS_SendFunc(Null_Port, P_NULL, (APPCON_AckFunc)p_arg);
  if(NO_ERR != err)
  {
   ((APPCON_AckFunc)p_arg)(P_NULL, 
                           NoAck_APPCON, 
                           P_NULL); 
  }
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static void Appcon_Reading_AckFunc(APPCON_OutPutBuff_Struct *cmd, 
                                   APPCON_AckType AckType, 
                                   DataBuffSpec_Struct* AckData)
{
 SofTimer_Struct*   time;
 AppconMessage_Struct Message;

 Message.AskCmd = cmd->Cmd;
 Message.AckDatas = AckData;
 
 if(NoAck_APPCON == AckType)
 {
   MessageDefaultFunction (AckType,&Message);
   freeSendBuff_APPCON(cmd); 
   return;
 }
 switch (ReadingStep)
 {
 case 0x01:   //查询是否空闲
   {
     if(STATUS == cmd->Cmd)
     {
       if(FREE_APPCON ==  AckType)
       {
        APPCON_BATCH_SendFunc(Null_Port, cmd->AckFuncParam,Appcon_Reading_AckFunc);
        ReadingStep = 0x02;
       }
       else
       {MessageDefaultFunction (AckType,&Message);}
     }
     break;
   }
 case 0x02:   //
   {
     if(BATCH == cmd->Cmd)
     {
       if(OK_APPCON == AckType )
       {
         //MessageDefaultFunction (AckType,&Message);
         APPCON_STATUS_SendFunc(Null_Port, P_NULL, Appcon_Reading_AckFunc);
         ReadingStep = 0x03;
       }
       else
       {APPCON_BATCH_SendFunc(Null_Port, cmd->AckFuncParam,Appcon_Reading_AckFunc);}
     }
     break; 
   }
 case 0x03:   ///
   {
     if(STATUS == cmd->Cmd)
     {
       if(BUSY_APPCON ==  AckType)
       {
         freeSendBuff_APPCON(cmd); 
         time = SofTimerCreate (10,                  //周期
                                TimerTB_1S,          //时基
                                SofTimer_ONE_SHOT,   //模式
                                QueryAppconStatus,   //回调
                                (void*)(Appcon_Reading_AckFunc));             //回调参数 
         SofTimerStart(time);
       }
       else
       {MessageDefaultFunction (AckType,&Message);}
     }
    break; 
   } 
 default :
   {
    ReadingStep = 0;
    break;
   }
 }
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName Appcon_Reading_Func(CLIFormat_Type   SPCP_Type,
                                  void *CLI, 
                                  MessageFunction Function)
{
 SystemErrName err = NO_ERR;
  
 if(FREE_Status == GetReadingModuleStatus( ))
 {
  err = APPCON_STATUS_SendFunc(Null_Port, P_NULL, Appcon_Reading_AckFunc);
  if(NO_ERR == err)
  {
    ReadingStep = 1;
    Present_CLI.Format = SPCP_Type;
    Present_CLI.Data.Super_CLI = CLI;
    ActionMessageFunc = Function;
    SetReadingModuleStatus (BATCH_Status);
  } 
 }
 else
 {err = Nothing;}
 return err; 
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static void Appcon_ReadNodeDatas_AckFunc(APPCON_OutPutBuff_Struct *cmd, 
                                         APPCON_AckType AckType, 
                                         DataBuffSpec_Struct* AckData)
{
 static unsigned short Number = 0;
 static unsigned short Number1 = 0; 
 static unsigned char  id [8];
 static unsigned char  ERR_CNT =0;
 
 unsigned char itmp;
 APPCON_RDNODE_AckData_Struct*  RDNODE_AckData = P_NULL;
 AppconMessage_Struct Message;

 Message.AskCmd = cmd->Cmd;
 Message.AckDatas =  AckData;

 
 if((RDNODE == cmd->Cmd)&&(TOTAL_APPCON == AckType))
 {
  ERR_CNT = 0;
  itmp = 0;
  Number = 0;
  while(itmp < AckData->length)
  {
    if(IsDigit(AckData->pbuff[itmp]))
     {
      Number *= 10;
      Number += AckData->pbuff[itmp] - '0';   
     }
     itmp ++;
  }
   itmp = 0;
   while (itmp < 8)
   {id[itmp++] = '0';}
   APPCON_RDNODE_SendFunc(Null_Port, id,1,P_NULL, Appcon_ReadNodeDatas_AckFunc);  //读取下
   Number1 = 0;
 }
 else if((AnsDatas_APPCON ==  AckType)&&(P_NULL != AckData))
 {
   ERR_CNT = 0;
   while (AckData->length >= sizeof(APPCON_RDNODE_AckData_Struct))
   {
     while (1)
     {
       if(IsLETTER(*(AckData->pbuff)) || Isletter(*(AckData->pbuff))){break;}
       if(0 == --(AckData->length)){break;}
       AckData->pbuff ++;
     }
     if(AckData->length <= sizeof(APPCON_RDNODE_AckData_Struct)){break;}
     Number1 ++;
     RDNODE_AckData = (APPCON_RDNODE_AckData_Struct*)AckData->pbuff;
     itmp = 0;
     while (itmp < 8)
     {id[itmp] = RDNODE_AckData->id[itmp];itmp ++;}
     MessageDefaultFunction (AckType,&Message);
     AckData->pbuff += sizeof(APPCON_RDNODE_AckData_Struct);
     AckData->length -= sizeof(APPCON_RDNODE_AckData_Struct);
   }
   while (1)
   {
     if(IsLETTER(*(AckData->pbuff)) || Isletter(*(AckData->pbuff)))
     {break;}
     if(0 == --(AckData->length)){break;}
     AckData->pbuff ++;
   }
   if((AckData->length) > 3)
   {
     if(('E'== AckData->pbuff[0])
        &&('N'== AckData->pbuff[1])
          &&('D'== AckData->pbuff[2]))
     {
       Number -= Number1;
       Number1 = 0;
       if(0 == Number)
       {
         freeSendBuff_APPCON(cmd);
         MessageDefaultFunction (ReadNodeEnd,&Message);
         SetReadingModuleStatus (FREE_Status); //结
         
       } 
       else
       {
        itmp = 8;
        id[7] += 1;
        while (itmp-- > 0)
        {
         if((id[itmp] >= 'F')||(id[itmp] >= 'f'))
         {id[itmp] = '0';id[itmp - 1] += 1;}
         else if((id[itmp] > '9') && (id[itmp] < 'A'))
         {id[itmp] = 'A';}
        }
        APPCON_RDNODE_SendFunc(Null_Port,id,1,P_NULL, Appcon_ReadNodeDatas_AckFunc);  //读取下 10 块表 
       }   
     }
     else
     {APPCON_RDNODE_SendFunc(Null_Port,id,1,P_NULL, Appcon_ReadNodeDatas_AckFunc);}  //读取下 10 块表  
   }
 }
 else
 {
   if(ERR_CNT ++ < 10)
   {
     if(RDNODE == cmd->Cmd)
     {
       if(cmd->BuffLength >8)
       {APPCON_RDNODE_SendFunc(Null_Port,id,1,P_NULL, Appcon_ReadNodeDatas_AckFunc);}
       else
       {APPCON_RDNODE_SendFunc(Null_Port,P_NULL,0,P_NULL,Appcon_ReadNodeDatas_AckFunc);}
     }
   }   
 }
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName Appcon_ReadNodeDatas_Func(CLIFormat_Type   SPCP_Type,
                                        void *CLI, 
                                        MessageFunction Function)
{
 SystemErrName err = NO_ERR;
  
 if(FREE_Status == GetReadingModuleStatus( ))
 {
  err = APPCON_RDNODE_SendFunc(Null_Port,P_NULL,0,P_NULL,Appcon_ReadNodeDatas_AckFunc);
  if(NO_ERR == err)
  {
    Present_CLI.Format = SPCP_Type;
    Present_CLI.Data.Super_CLI = CLI;
    ActionMessageFunc = Function;
    SetReadingModuleStatus (ReadNode);
  } 
 }
 else
 {err = Nothing;}
 return err; 
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static void ReadingTimeOut(void *p_tmr, void *p_arg)
{
 freePresent_CLI( );
 ModuleStatus = FREE_Status;
 SofTimerDel(&ReadingTime);
 //SetReadingModuleStatus (FREE_Status);
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
AppconModuleStatus_Type GetReadingModuleStatus(void)
{
 return (ModuleStatus);    //0：抄表网络空闲   1：正在抄表
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void SetReadingModuleStatus(AppconModuleStatus_Type New_Status)
{
  if((CMD_Status == New_Status)||(TST_Status == New_Status))
  {
   ModuleStatus = New_Status;  
   ReadingTime = SofTimerCreate (60,                  //周期
                                 TimerTB_1S,          //时基
                                 SofTimer_ONE_SHOT,   //模式
                                 ReadingTimeOut,      //回调
                                 P_NULL);             //回调参数 
   SofTimerStart(ReadingTime);  
  }
  else if(BATCH_Status == New_Status)
  {
   ModuleStatus = BATCH_Status;
   ReadingTime = SofTimerCreate (120,                  //周期
                                 TimerTB_1M,          //时基
                                 SofTimer_ONE_SHOT,   //模式
                                 ReadingTimeOut,      //回调
                                 P_NULL);             //回调参数 
   SofTimerStart(ReadingTime);  
  }
  else if(ReadNode == New_Status)
  {
   ModuleStatus = ReadNode;
   ReadingTime = SofTimerCreate (5,                  //周期
                                 TimerTB_1M,          //时基
                                 SofTimer_ONE_SHOT,   //模式
                                 ReadingTimeOut,      //回调
                                 P_NULL);             //回调参数 
   SofTimerStart(ReadingTime);  
  } 
  else
  {
    ModuleStatus = FREE_Status;
    SofTimerDel(&ReadingTime);
  } 
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static void MessageDefaultFunction(APPCON_AckType type,AppconMessage_Struct* Message)
{
  if(P_NULL != ActionMessageFunc)
  {ActionMessageFunc(type, &Message);}
  else if(VOID_Format == Present_CLI.Format)
  {DefaulMessagetFunction(type,Message);}
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static void StatusTest(APPCON_OutPutBuff_Struct *cmd, 
                        APPCON_AckType AckType, 
                        DataBuffSpec_Struct* AckData)
{
 SofTimer_Struct*   time;
 if((P_NULL != cmd)&&(STATUS == cmd->Cmd))
 {
   if(FREE_APPCON ==  AckType)
   {
     SetReadingModuleStatus(FREE_Status);
     freeSendBuff_APPCON(cmd);
     return;
   } 
 }
 freeSendBuff_APPCON(cmd);
 time = SofTimerCreate (10,                  //周期
                        TimerTB_1S,          //时基
                        SofTimer_ONE_SHOT,   //模式
                        QueryAppconStatus,   //回调
                       (void*)(StatusTest));             //回调参数 
 SofTimerStart(time);
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static void DefaulMessagetFunction(APPCON_AckType type,AppconMessage_Struct* Message)
{
  if(NoAck_APPCON == type)
  {
   if(FREE_Status != GetReadingModuleStatus( ))
   {
    StatusTest(P_NULL,type,Message->AckDatas); 
   }
   if(0 != ServerState[0].SR.Bit.CN)
   {
     Concentrator.CSR.Bit.ReadAll = 0;
     LaunchSendMeasureValue_ToKingDaServer1( );
     Concentrator.SR1.Bit.NewData_flg = 0;
   }
   else
   {
     Concentrator.SR1.Bit.NewData_flg = 1; 
     if(0 != ServerState[1].SR.Bit.CN)
     {
       LaunchSendMeasureValue_ToKingDaServer2( );
     }
   }
  }
 switch (GetReadingModuleStatus( ))
 {
  case FREE_Status :
    {break;}
  case CMD_Status :
    {break;}
  case TST_Status :
    {break;}
  case BATCH_Status : 
    {
     if(FREE_APPCON == type)
     {
       SetReadingModuleStatus(FREE_Status);
       Appcon_ReadNodeDatas_Func(VOID_Format,P_NULL,P_NULL);
     }
     break;
    }
  case ReadNode : 
    {
      if(AnsDatas_APPCON == type) //
      {//记录数据
       write_NodeValueRecord_Appcon ((APPCON_RDNODE_AckData_Struct*)Message->AckDatas->pbuff);
      }
      if(ReadNodeEnd == type)
      {
        if(0 != ServerState[0].SR.Bit.CN)
        {
          Concentrator.CSR.Bit.ReadAll = 0;
          LaunchSendMeasureValue_ToKingDaServer1( );
          Concentrator.SR1.Bit.NewData_flg = 0;
        }
        else
        {
          Concentrator.SR1.Bit.NewData_flg = 1; 
          if(0 != ServerState[1].SR.Bit.CN)
          {LaunchSendMeasureValue_ToKingDaServer2( );
          }
        }
      }
    }
  default:
    {break;}
 }
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static void freePresent_CLI(void)
{
  ActionMessageFunc = P_NULL;
 if(P_NULL == Present_CLI.Data.Super_CLI)
 {return;}
 if(JINGDA == Present_CLI.Format)
 {freeJingDaSendBuff (Present_CLI.Data.Super_CLI);}
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static void ReadAllAppconMeasureValue_ToKingDaServer_AckSendFunc(unsigned char Number)
{
  SystemErrName err;
  unsigned short length;
  TimerData_Struct timer;
  
 if(P_NULL == ReadAllAppcon_AckBuff)
 {return ;}
  


 ReadAllAppcon_AckBuff->buff.head.Sync = KingDaServerSync_ToServer;
 ReadAllAppcon_AckBuff->buff.head.Cmd = ReadALLMeasureValue;
 
 get_SystemTimer (&timer);

 ReadAllAppcon_AckBuff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Time.Year = timer.Year - 2000;
 if(ReadAllAppcon_AckBuff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Time.Year < 10)
 {ReadAllAppcon_AckBuff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Time.Year  = 10;}
 
 if((timer.Month < 1)||(timer.Month > 12))
 {timer.Month = 1;}
 ReadAllAppcon_AckBuff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Time.Month = timer.Month;
 if((timer.Day < 1)||(timer.Day > 31))
 {timer.Day = 1;}
 ReadAllAppcon_AckBuff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Time.Day = timer.Day;
 if(timer.Hour >23)
 {timer.Hour = 1;}
 ReadAllAppcon_AckBuff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Time.Hour = timer.Hour;
  if(timer.Minute >59)
 {timer.Minute = 1;}
 ReadAllAppcon_AckBuff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Time.Minute = timer.Minute;
 if(timer.Second >59)
 {timer.Second = 1;}
 ReadAllAppcon_AckBuff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Time.Second = timer.Second;

 ReadAllAppcon_AckBuff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Concentrator_ID[0] = Concentrator.id[0];
 ReadAllAppcon_AckBuff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Concentrator_ID[1] = Concentrator.id[1];
 ReadAllAppcon_AckBuff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Concentrator_ID[2] = Concentrator.id[2];
   
 ReadAllAppcon_AckBuff->buff.pram.ReadALLMeasureValue_Send.RepeatersNet_ID[0] = Concentrator.AppconNetID[0];
 ReadAllAppcon_AckBuff->buff.pram.ReadALLMeasureValue_Send.RepeatersNet_ID[1] = 0;
 ReadAllAppcon_AckBuff->buff.pram.ReadALLMeasureValue_Send.RepeatersNet_ID[2] = 0;
 ReadAllAppcon_AckBuff->buff.pram.ReadALLMeasureValue_Send.Repeaters_ID = 0;
 length = Number;
 ReadAllAppcon_AckBuff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[3] = length %10;
 length /= 10;
 ReadAllAppcon_AckBuff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[2] = length %10;
 length /= 10;
 ReadAllAppcon_AckBuff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[1] = length %10;
 length /= 10;
 ReadAllAppcon_AckBuff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[0] = length %10;
 
 length = Number * sizeof (NodeDatas_APPCON_Struct);
 length += 17 ;
 ReadAllAppcon_AckBuff->buff.head.Length_L = length;
 ReadAllAppcon_AckBuff->buff.head.Length_H = length >> 8;

 err = KingDaServer_BuffSend(ReadAllAppcon_AckBuff);
 if(NO_ERR != err)
 {Free_KingDaServerBuff(ReadAllAppcon_AckBuff);}
 ReadAllAppcon_AckBuff = P_NULL; 
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static void ReadAllAppconMeasureValue_ToKingDaServer_AckFunc( APPCON_OutPutBuff_Struct *cmd, 
                                                              APPCON_AckType AckType, 
                                                              DataBuffSpec_Struct* AckData)
{
 unsigned char itmp;
 unsigned long itemp32;
 APPCON_RDNODE_AckData_Struct*  AckDatas = P_NULL;

 if((RDNODE == cmd->Cmd)&&(TOTAL_APPCON == AckType))
 {
   itmp = 0;
   while(itmp < AckData->length)
   {
     if(IsDigit(AckData->pbuff[itmp]))
     {
      TotalNumber *= 10;
      TotalNumber += AckData->pbuff[itmp] - '0';   
     }
     itmp ++;
   }
   APPCON_RDNODE_SendFunc( Null_Port, 
                           RDNODE_ID,
                           1,
                           P_NULL,
                           ReadAllAppconMeasureValue_ToKingDaServer_AckFunc);  //读取下
 }
 else if((AnsDatas_APPCON ==  AckType)&&(P_NULL != AckData))
 {
   while (AckData->length >= sizeof(APPCON_RDNODE_AckData_Struct))
   {
     while (1)
     {
       if(IsLETTER(*(AckData->pbuff)) || Isletter(*(AckData->pbuff))){break;}
       if(0 == --(AckData->length)){break;}
       AckData->pbuff ++;
     }
     if(AckData->length <= sizeof(APPCON_RDNODE_AckData_Struct))
     {break;}
     AckDatas = (APPCON_RDNODE_AckData_Struct*)(AckData->pbuff);
     itmp = 0;
     while (itmp < 8)
     {RDNODE_ID[itmp] = AckDatas->id[itmp];itmp ++;}
     if(P_NULL != ReadAllAppcon_AckBuff)
     {
      itemp32 = APPCON_AsciiHexToDigit(AckDatas->id,8,8); 
      ReadAllAppcon_AckBuff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Datas[RDNODE_Number].id[0] = itemp32 >>24;
      ReadAllAppcon_AckBuff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Datas[RDNODE_Number].id[1] = itemp32 >>16;
      ReadAllAppcon_AckBuff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Datas[RDNODE_Number].id[2] = itemp32 >>8;
      ReadAllAppcon_AckBuff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Datas[RDNODE_Number].id[3] = itemp32;
      itemp32 = APPCON_AsciiHexToDigit(AckDatas->value,12,8);
      ReadAllAppcon_AckBuff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Datas[RDNODE_Number].value[0] = itemp32 >>24;
      ReadAllAppcon_AckBuff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Datas[RDNODE_Number].value[1] = itemp32 >>16;
      ReadAllAppcon_AckBuff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Datas[RDNODE_Number].value[2] = itemp32 >>8;
      ReadAllAppcon_AckBuff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Datas[RDNODE_Number].value[3] = itemp32;
      ReadAllAppcon_AckBuff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Datas[RDNODE_Number].vbat = APPCON_AsciiHexToDigit(AckDatas->bat,2,2);
      ReadAllAppcon_AckBuff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Datas[RDNODE_Number].route = AckDatas->route;
      ReadAllAppcon_AckBuff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Datas[RDNODE_Number].link = AckDatas->link;
      RDNODE_Number ++;
      TotalNumber --;
      if((RDNODE_Number >= 50)||(0 == TotalNumber)) //完毕
      { 
        freeSendBuff_APPCON(cmd);
        SetReadingModuleStatus (FREE_Status); //结
        if(0 == TotalNumber)
        {
          if(ServerState[0].Port == ReadAllAppcon_AckBuff->TargetPort)  //
          {Concentrator.SR1.Bit.App_RdVal_end_01 = 1;}
          else if(ServerState[1].Port== ReadAllAppcon_AckBuff->TargetPort)
          {Concentrator.SR1.Bit.App_RdVal_end_02 = 1;}
        }
        ReadAllAppconMeasureValue_ToKingDaServer_AckSendFunc(RDNODE_Number);
      }
     }
     AckData->pbuff += sizeof(APPCON_RDNODE_AckData_Struct);
     AckData->length -= sizeof(APPCON_RDNODE_AckData_Struct);
   }
   while (1)
   {
     if(IsLETTER(*(AckData->pbuff)) || Isletter(*(AckData->pbuff)))
     {break;}
     if(0 == --(AckData->length)){break;}
     AckData->pbuff ++;
   }
   if((AckData->length) > 3)
   {
     if(('E'== AckData->pbuff[0])
        &&('N'== AckData->pbuff[1])
          &&('D'== AckData->pbuff[2]))
     {
       itmp = 8;
       RDNODE_ID[7] += 1;
       while (itmp-- > 0)
       {
         if((RDNODE_ID[itmp] >= 'F')||(RDNODE_ID[itmp] >= 'f'))
         {RDNODE_ID[itmp] = '0';RDNODE_ID[itmp - 1] += 1;}
         else if((RDNODE_ID[itmp] > '9') && (RDNODE_ID[itmp] < 'A'))
         {RDNODE_ID[itmp] = 'A';}
        }
        APPCON_RDNODE_SendFunc(Null_Port,RDNODE_ID,1,P_NULL, ReadAllAppconMeasureValue_ToKingDaServer_AckFunc);  //读取下 10 块表 
     }
     else
     {APPCON_RDNODE_SendFunc(Null_Port,RDNODE_ID,1,P_NULL, ReadAllAppconMeasureValue_ToKingDaServer_AckFunc);}  //读取下 10 块表  
   }
 }
 else
 {
   Free_KingDaServerBuff(ReadAllAppcon_AckBuff);
   ReadAllAppcon_AckBuff = P_NULL; 
   TotalNumber = 0;
   Concentrator.SR1.Bit.App_RdVal_end_01 = 1;
   Concentrator.SR1.Bit.App_RdVal_end_02 = 1;
 }
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        // 
Others:         // 
*********************************************************************************/
SystemErrName ReadAllAppconMeasureValue_ToKingDaServer_ACK( CommPortNumber_Type Port,
                                                            KingDaServer_CmdAckFunc AckFunc)
{
 SystemErrName err = NO_ERR;
 unsigned char itmp = 0; 
 if((ServerState[0].Port == Port )&&(0 == Concentrator.SR1.Bit.App_RdVal_end_02))  //现在 有端口在读取
 {return Nothing;}
 if((ServerState[1].Port == Port )&&(0 == Concentrator.SR1.Bit.App_RdVal_end_01))  //现在 有端口在读取
 {return Nothing;}
 //if(P_NULL != ReadAllAppcon_AckBuff)   
 //{return Nothing;}
 ReadAllAppcon_AckBuff =  Get_KingDaServerBuff ( ); 
 if(P_NULL == ReadAllAppcon_AckBuff)
 {return Nothing;}
 
 ReadAllAppcon_AckBuff->TargetPort = Port; 
 ReadAllAppcon_AckBuff->CCR.Bit.AckEn = 1;
 ReadAllAppcon_AckBuff->AckFunc =  AckFunc;
 if(FREE_Status == GetReadingModuleStatus( ))
 {
   if( 0 == TotalNumber)
   {
     itmp = 0;
     while (itmp < 8)
     {RDNODE_ID[itmp++] = '0';} 
     err = APPCON_RDNODE_SendFunc( Null_Port,
                                   P_NULL,
                                   0,
                                   P_NULL,
                                   ReadAllAppconMeasureValue_ToKingDaServer_AckFunc);
     if(NO_ERR == err)
     {
       RDNODE_Number = 0;
       Present_CLI.Format = VOID_Format;
       Present_CLI.Data.Super_CLI = P_NULL;
       ActionMessageFunc = P_NULL;
       SetReadingModuleStatus (ReadNode);
     }
   }
   else
   {
     RDNODE_Number = 0;
     itmp = 8;
     RDNODE_ID[7] += 1;
     while (itmp-- > 0)
     {
       if((RDNODE_ID[itmp] >= 'F')||(RDNODE_ID[itmp] >= 'f'))
       {RDNODE_ID[itmp] = '0';RDNODE_ID[itmp - 1] += 1;}
       else if((RDNODE_ID[itmp] > '9') && (RDNODE_ID[itmp] < 'A'))
       {RDNODE_ID[itmp] = 'A';}
     }
     err = APPCON_RDNODE_SendFunc( Null_Port, 
                                   RDNODE_ID,
                                   1,
                                   P_NULL,
                                   ReadAllAppconMeasureValue_ToKingDaServer_AckFunc);  //读取下
     if(NO_ERR == err)
     {
       Present_CLI.Format = VOID_Format;
       Present_CLI.Data.Super_CLI = P_NULL;
       ActionMessageFunc = P_NULL;
       SetReadingModuleStatus (ReadNode);
     }
   }
 }
 else
 { err = Nothing; }
 if(NO_ERR != err)
 {
   Free_KingDaServerBuff(ReadAllAppcon_AckBuff);
   ReadAllAppcon_AckBuff = P_NULL;
 } 
 return err; 
}
/*********************************************************************************
 Function:      //
 Description:   // 将本地抄表数据上传至服务器
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/

/*********************************************************************************/
/************************************END*****************************************/
/*********************************************************************************/