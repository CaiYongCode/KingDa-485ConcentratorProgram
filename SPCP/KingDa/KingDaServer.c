/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include ".\KingDaServer.h"
#include ".\KingDaV1.h"
#include ".\CRC8.h"
#include "..\..\APP\inc\ReadEquipData.h"
#include "..\..\SPCP\APPCON\APPCON.h"

#include "..\..\APP\inc\GPRS.h"
/*********************************************************************************
常量定义区
*********************************************************************************/

/*********************************************************************************
公共变量定义区
*********************************************************************************/

/*********************************************************************************
外部变量声明区
*********************************************************************************/
//extern TimerData_Struct  SystemTimer;
void MeasureValue_ToKingDaServer(Repeaters_DescrType* repeaters, unsigned char *pbuff);
void AppconMeasureValue_ToKingDaServer1(unsigned char *Number, unsigned char *pbuff);
void AppconMeasureValue_ToKingDaServer2(unsigned char *Number, unsigned char *pbuff);
/*********************************************************************************
私有变量定义区
*********************************************************************************/ 
static KingDaServerSendBuff_Struct  send_buff[KingDaServerSendBuff_Number];
static CommPortNumber_Type InPort_PV;

//static unsigned char  ServerState = 0;

//static unsigned char ReadALLMeasureValue_Repeaters_ID = 1;
//static unsigned char MeasureValue_UpLoad_Repeaters_ID = 1;


/*********************************************************************************
测试变量定义区
*********************************************************************************/

/*********************************************************************************
内部函数定义区
*********************************************************************************/
static void DeInitBuff(void);
static SystemErrName FrameVerify (void *pbuff,unsigned char length, KingDaServer_PackageStruct** data);
static void HandCmd_func(KingDaServer_PackageStruct* pbuff);
static void ReadMeasureValue_func(KingDaServer_PackageStruct* pbuff);
static void TuneUp_RTC_func(KingDaServer_PackageStruct* pbuff);
static void GetRepeatersState_func(KingDaServer_PackageStruct* pbuff);
static void ReadALLMeasureValue_func(KingDaServer_PackageStruct* pbuff);
static void MeasureValue_UpLoad_func(KingDaServer_PackageStruct* pbuff);


static void APPCON_DeleteNode_func(KingDaServer_PackageStruct* pbuff);   //删除节点命令
static void APPCON_BATCH_func(KingDaServer_PackageStruct* pbuff);  //组网并抄表
static void APPCON_STATUS_func(KingDaServer_PackageStruct* pbuff);       //查询组建网络状态
static void APPCON_OpenOV_func(KingDaServer_PackageStruct* pbuff);       //开阀
static void APPCON_HalfOV_func(KingDaServer_PackageStruct* pbuff);       //半开阀门
static void APPCON_MtOV_func(KingDaServer_PackageStruct* pbuff);         //活动阀命令
static void APPCON_CloseOV_func(KingDaServer_PackageStruct* pbuff);      //关阀
static void APPCON_GetOV_func(KingDaServer_PackageStruct* pbuff);        //读取阀门状态



static void KingDa_OpenOV_func(KingDaServer_PackageStruct* pbuff);       //开阀
static void KingDa_HalfOV_func(KingDaServer_PackageStruct* pbuff);       //半开阀门
static void KingDa_MtOV_func(KingDaServer_PackageStruct* pbuff);         //活动阀命令
static void KingDa_CloseOV_func(KingDaServer_PackageStruct* pbuff);      //关阀
static void KingDa_GetOV_func(KingDaServer_PackageStruct* pbuff);        //读取阀门状态
static void  KingDa_OpenOV_ENF_func (KingDaServer_PackageStruct* pbuff);      
static void  KingDa_CloseOV_ENF_func (KingDaServer_PackageStruct* pbuff);  //强制关阀
static void KindgGetOV_AckFunc(PProtocolSendBuff_Struct* SendBuff, void* AckData);
/*********************************************************************************
功能代码定义区
*********************************************************************************/
void KingDaServer_DeInit(void)
{
  InPort_PV = Null_Port;
  DeInitBuff( );
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
SystemErrName SendCmd_KingDaServer(KingDaServer_Struct* Cmd)                         
{
  unsigned char i;
  unsigned char* tc;
  unsigned char* tp;
  KingDaServerSendBuff_Struct*  Buff;
  Buff = Get_KingDaServerBuff ( );
  if(P_NULL  == Buff)
  {return Nothing;}
  Buff->AckFunc = Cmd->AckFunc;
  if(UP == Cmd->CSR.Bit.DIR)
  {Buff->buff.head.Sync= KingDaServerSync_ToServer;}
  else
  {Buff->buff.head.Sync= KingDaServerSync_ToConcentrator;}
  Buff->buff.head.Cmd = Cmd->Cmd;
  Buff->buff.head.Length_L =  Cmd->length;
  Buff->buff.head.Length_H =  Cmd->length  >> 8; 
  tp = (unsigned char*) &(Buff->buff.pram);
  tc = Cmd->Param;
  i =0;
  while(i < Cmd->length)
  {
    tp[i] = tc[i];
    i++;
  }
  KingDaServer_BuffSend (Buff);
  return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        // 1：长度错误
                   2: 帧头同步错误
                   3：同步字错误
                   4：CRC 校验错误 
                   5: 不识别命令
Others:         // 
*********************************************************************************/
SystemErrName KingDaServer_input (void *pbuff, unsigned char length, CommPortNumber_Type port)
{
  SystemErrName err;
  KingDaServer_PackageStruct *package;
  InPort_PV  = port;
  err =  FrameVerify (pbuff, length, &package); 
  
  if(NO_ERR != err)
  {return err;}
  
  if(Rs232_Port == port)
  {
    Concentrator.CSR.Bit.Debug  = 1;
    Concentrator_DebugTimer = 300;
  }
  switch( package->head.Cmd )
  {
  case HandCmd_Server:
    {
      HandCmd_func (package); 
      break;
    }
  case ReadMeasureValue:
  case ReadMeasureValue_OFF:  
    {
      ReadMeasureValue_func (package);
      break;
    }
  case TuneUp_RTC:          
    {
      TuneUp_RTC_func (package);
      break;
    }
  case GetRepeatersState:  
    {
      GetRepeatersState_func (package);
      break;
    }
  case ReadALLMeasureValue:
    {
      ReadALLMeasureValue_func(package);
      break;
    }
  case MeasureValue_UpLoad:   
    {
      MeasureValue_UpLoad_func(package);
	  SofTimerDel(&(MG2618Infr.TimeOut));
      break;
    } 
  case ServerDeleteNode_APPCON :     //= 0x80, 删除节点命令
    {
      if(0x01 == Concentrator.CSR.Bit.Appcon_EN)
      {APPCON_DeleteNode_func(package);}
      break;
    }
  case ServerBATCH_APPCON:             //= 0x81, 组网并抄表
    {
      if(0x01 == Concentrator.CSR.Bit.Appcon_EN)
      {APPCON_BATCH_func(package);}
      break;
    }
  case ServerSTATUS_APPCON:            //= 0x82,  查询组建网络状态
    {
      if(0x01 == Concentrator.CSR.Bit.Appcon_EN)
      {APPCON_STATUS_func(package);}
      break;
    }
  case ServerOpenOV_APPCON:            //= 0x65,  开阀
    {
      if((0x01 == Concentrator.CSR.Bit.Appcon_EN)
         &&(length > sizeof (APPCON_OpenOV_ToKingDaConcentrator_Type)+ sizeof (KindgDaServer_PackageHead )))
      {APPCON_OpenOV_func(package);} 
      else
      {KingDa_OpenOV_func(package);} 
      break;
    }
  case ServerHalfOV_APPCON:           //= 0x74,  半开阀门
    {
      if((0x01 == Concentrator.CSR.Bit.Appcon_EN)
         &&(length > sizeof (APPCON_HalfOV_ToKingDaConcentrator_Type)+ sizeof (KindgDaServer_PackageHead )))
      {APPCON_HalfOV_func(package); }
      else
      {KingDa_HalfOV_func(package);}
      break;
    }
  case ServerMtOV_APPCON:             //= 0x73,  活动阀命令
    {
      if((0x01 == Concentrator.CSR.Bit.Appcon_EN)
         &&(length > sizeof (APPCON_MtOV_ToKingDaConcentrator_Type)+ sizeof (KindgDaServer_PackageHead )))
      {APPCON_MtOV_func(package);}
      else
      {KingDa_MtOV_func(package);}
      break;
    }
  case ServerCloseOV_APPCON:          //= 0x64,  关阀
    {
      if((0x01 == Concentrator.CSR.Bit.Appcon_EN)
         &&(length > sizeof (APPCON_CloseOV_ToKingDaConcentrator_Type)+ sizeof (KindgDaServer_PackageHead )))
      {APPCON_CloseOV_func(package);}
      else
      {KingDa_CloseOV_func(package);}
      break;
    }
  case ServerGetOV_APPCON:            //= 0x61,   读取阀门状态
    {
      if((0x01 == Concentrator.CSR.Bit.Appcon_EN)
         &&(length > sizeof (APPCON_GetOV_ToKingDaConcentrator_Type)+ sizeof (KindgDaServer_PackageHead )))
      {APPCON_GetOV_func(package);}
      else
      {KingDa_GetOV_func(package);}
      break;
    }
  case ServerOpen_OV_ENF:             //强制开阀
    {
      KingDa_OpenOV_ENF_func(package);
      break;
    }
  case ServerClose_OV_ENF:               //强制关阀
    {
      KingDa_CloseOV_ENF_func(package);
      break;
    }
  default:
    {return OperateCmd_err;break;}
  }
  
  return NO_ERR;
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
static void DeInitBuff(void)
{
  unsigned char itemp =0;
  while ( itemp < KingDaServerSendBuff_Number)
  {
   send_buff[itemp].CCR.Byte = 0; 
   send_buff[itemp].CCR.Bit.idle = 1;
   itemp ++;
  }
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
static SystemErrName BuffVerify (KingDaServerSendBuff_Struct* pbuff) 
{
 unsigned char itemp = 0;
 while (itemp < KingDaServerSendBuff_Number)
 {
   if(pbuff == &(send_buff[itemp]))
   {
     return NO_ERR;
   }
   itemp ++;
 }
 return IllegalParam;
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
KingDaServerSendBuff_Struct* Get_KingDaServerBuff (void)
{
  unsigned char itemp =0;
  while ( itemp < KingDaServerSendBuff_Number)
  {
    if(send_buff[itemp].CCR.Bit.idle)
    {
     send_buff[itemp].CCR.Byte = 0; 
     send_buff[itemp].RetransNumber  = RetransNumber_ToServer_Default;
     send_buff[itemp].AckFunc = P_NULL;
     send_buff[itemp].TargetPort = Null_Port;
     return &(send_buff[itemp]);
    }
    itemp ++;
  }
  return  P_NULL;
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
void Free_KingDaServerBuff(KingDaServerSendBuff_Struct* pbuff)
{
  if(NO_ERR == BuffVerify (pbuff))
  {
    FreeOutputBuffForData_MXPC (&(pbuff->buff));
    pbuff->CCR.Byte = 0; 
    pbuff->CCR.Bit.idle = 1;
  }
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        // 1：长度错误
                   2: 帧头同步错误
                   3：同步字错误
                   4：CRC 校验错误 
                   5: 不识别命令
Others:         // 
*********************************************************************************/
static SystemErrName FrameVerify (void *pbuff,unsigned char length, KingDaServer_PackageStruct** data)
{
 union {
   unsigned short* p;
   KindgDaServer_PackageHead  *headp;
 }buff;
 //unsigned char itemp;
 unsigned short Length;
   
 //itemp = 0;
 buff.p = pbuff;
 while (length > 0)
 {
   if((KingDaServerSync_ToServer  == *(buff.p))
      ||(KingDaServerSync_ToConcentrator == *(buff.p)))
   {break;}
  buff.p ++;
  length--;
 }
 if(length < sizeof(KindgDaServer_PackageHead))
 {return Length_err;}
  Length = buff.headp->Length_H;
  Length  <<= 8;
  Length += buff.headp->Length_L;
  Length += sizeof(KindgDaServer_PackageHead);
 if(length < Length + 1)
 {return Length_err;}

 if(((unsigned char*)buff.p)[Length] != CRC8((unsigned char*)buff.p,Length)) 
 {return Verify_err;}
 
 *data = (KingDaServer_PackageStruct*)buff.headp;
 return NO_ERR;
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
static void HandCmd_func(KingDaServer_PackageStruct* pbuff)
{
 unsigned char i;
 
 if(KingDaServerSync_ToConcentrator == pbuff->head.Sync)
 {
  i = 0;
  while (i < KingDaServerSendBuff_Number)
  {
   if((0 == send_buff[i].CCR.Bit.idle)
      && (send_buff[i].buff.head.Cmd == HandCmd_Server) 
      && (send_buff[i].TargetPort == InPort_PV)) 
   {
     if((pbuff->pram.HandsData_Receive.OK.id[0] == Concentrator.id[0]) 
        &&(pbuff->pram.HandsData_Receive.OK.id[1] == Concentrator.id[1])
        &&(pbuff->pram.HandsData_Receive.OK.id[2] == Concentrator.id[2]))
     {
       if(('O' == pbuff->pram.HandsData_Receive.OK.DI[0])
          &&('K' == pbuff->pram.HandsData_Receive.OK.DI[1]))
       {
         break;
       }  
     }
   }
   i ++;
  }
  if(i < KingDaServerSendBuff_Number)
  {
   if(P_NULL != send_buff[i].AckFunc)
   {send_buff[i].AckFunc(&(send_buff[i].buff), pbuff);} // 应答数据处理程序
   //else
   {Free_KingDaServerBuff(&send_buff[i]);}  //释放缓冲区 
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
Others:         // 
*********************************************************************************/
void ReadMeasureValue_AckFunc(PProtocolSendBuff_Struct* SendBuff, void* AckData)
{
 SystemErrName err = NO_ERR; 
 KingDaServerSendBuff_Struct*  buff;  
 KindgDaSPCP_PackageDataStruct* ACK;   //水表数据  应答数据 
 
 if(P_NULL == AckData ){return ;}
 ACK = AckData;
 
 buff =  Get_KingDaServerBuff ( ); 
 if(P_NULL == buff){return ;}
 buff->buff.head.Sync = KingDaServerSync_ToServer;
 buff->buff.head.Cmd = ReadMeasureValue;
 buff->buff.head.Length_L = sizeof(ReadMeasureValue_ToKingDaServer_Type);
 buff->buff.head.Length_H = sizeof(ReadMeasureValue_ToKingDaServer_Type)>> 8;
 buff->buff.pram.ReadMeasureValue_Send.Concentrator_ID[0] = Concentrator.id[0];
 buff->buff.pram.ReadMeasureValue_Send.Concentrator_ID[1] = Concentrator.id[1];
 buff->buff.pram.ReadMeasureValue_Send.Concentrator_ID[2] = Concentrator.id[2];
 buff->buff.pram.ReadMeasureValue_Send.Node_ID[0] = ACK->TypeA.pram.ReadValue_ack.addr[0];
 buff->buff.pram.ReadMeasureValue_Send.Node_ID[1] = ACK->TypeA.pram.ReadValue_ack.addr[1];
 buff->buff.pram.ReadMeasureValue_Send.Node_ID[2] = ACK->TypeA.pram.ReadValue_ack.addr[2];

 buff->buff.pram.ReadMeasureValue_Send.Value =  ACK->TypeA.pram.ReadValue_ack.value;
 buff->TargetPort = SendBuff->SourcePort;
 err = KingDaServer_BuffSend(buff);
 if(NO_ERR != err)
 {Free_KingDaServerBuff(buff);}
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
void ReadMeasureValue_OFF_AckFunc(PProtocolSendBuff_Struct* SendBuff, void* AckData)
{
 if(P_NULL == SendBuff ){return ;}
 //CloseNETChannel_KindDa (SendBuff->buff.pram.ReadValue_ask.addr[0], 
                        // SendBuff->buff.pram.ReadValue_ask.addr[1],
                        // P_NULL);
 changeChannel_Rs485( Null_Port);
 ReadMeasureValue_AckFunc( SendBuff, AckData);
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
static void ReadNodeValue_APPCON_AckFunc (APPCON_OutPutBuff_Struct *cmd, 
                                           APPCON_AckType AckType, 
                                           DataBuffSpec_Struct* AckData)
{
 unsigned long value;
 APPCON_CMD_AckData_Struct* Datas;
 
 SystemErrName err = NO_ERR; 
 KingDaServerSendBuff_Struct*  buff;  

 if((CMD == cmd->Cmd) &&(P_NULL != AckData ))
 {
  buff =  Get_KingDaServerBuff ( ); 
  if(P_NULL != buff)
  {
    buff->buff.head.Sync = KingDaServerSync_ToServer;
    buff->buff.head.Cmd = ReadMeasureValue;
    buff->buff.head.Length_L = sizeof(APPCON_ReadMeasureValue_ToKingDaServer_Type);
    buff->buff.head.Length_H = sizeof(APPCON_ReadMeasureValue_ToKingDaServer_Type)>> 8;
    buff->buff.pram.ReadMeasureValue_Send_APPCON.Concentrator_ID[0] = Concentrator.id[0];
    buff->buff.pram.ReadMeasureValue_Send_APPCON.Concentrator_ID[1] = Concentrator.id[1];
    buff->buff.pram.ReadMeasureValue_Send_APPCON.Concentrator_ID[2] = Concentrator.id[2];
    
    
    if ((Ans_APPCON == AckType)
         && (AckData->length >= sizeof (APPCON_CMD_AckData_Struct)))
     {
       Datas = (APPCON_CMD_AckData_Struct*)AckData->pbuff;
       value = APPCON_AsciiHexToDigit((unsigned char*)Datas->value,12,8);
       buff->buff.pram.ReadMeasureValue_Send_APPCON.Value[0] = value >>24;
       buff->buff.pram.ReadMeasureValue_Send_APPCON.Value[1] = value >>16;
       buff->buff.pram.ReadMeasureValue_Send_APPCON.Value[2] = value >>8;
       buff->buff.pram.ReadMeasureValue_Send_APPCON.Value[3] = value ;
       buff->buff.pram.ReadMeasureValue_Send_APPCON.Bat = APPCON_AsciiHexToDigit(Datas->bat,2,2) ;
     }
    else
    {
       buff->buff.pram.ReadMeasureValue_Send_APPCON.Value[0] = 0xff;
       buff->buff.pram.ReadMeasureValue_Send_APPCON.Value[1] = 0xff;
       buff->buff.pram.ReadMeasureValue_Send_APPCON.Value[2] = 0xff;
       buff->buff.pram.ReadMeasureValue_Send_APPCON.Value[3] = 0xff ;
       buff->buff.pram.ReadMeasureValue_Send_APPCON.Bat = 0xff ;
    }

    buff->buff.pram.ReadMeasureValue_Send_APPCON.Node_ID[0] = cmd->Buff[4];
    buff->buff.pram.ReadMeasureValue_Send_APPCON.Node_ID[1] = cmd->Buff[5];
    buff->buff.pram.ReadMeasureValue_Send_APPCON.Node_ID[2] = cmd->Buff[6];
    buff->buff.pram.ReadMeasureValue_Send_APPCON.Node_ID[3] = cmd->Buff[7];
    buff->buff.pram.ReadMeasureValue_Send_APPCON.Node_ID[4] = cmd->Buff[8];
    buff->buff.pram.ReadMeasureValue_Send_APPCON.Node_ID[5] = cmd->Buff[9];
    buff->buff.pram.ReadMeasureValue_Send_APPCON.Node_ID[6] = cmd->Buff[10];
    buff->buff.pram.ReadMeasureValue_Send_APPCON.Node_ID[7] = cmd->Buff[11];

   buff->TargetPort = cmd->SourcePort;
   err = KingDaServer_BuffSend(buff);
   if(NO_ERR != err)
   {Free_KingDaServerBuff(buff);}
  }
 }
 freeSendBuff_APPCON(cmd);
}
/*********************************************************************************
 Function:      //
 Description:   //删除节点命令
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void APPCON_DeleteNode_AckFunc (APPCON_OutPutBuff_Struct *cmd, 
                                        APPCON_AckType AckType, 
                                        DataBuffSpec_Struct* AckData)
{
  SystemErrName err = NO_ERR; 
  KingDaServerSendBuff_Struct*  buff;  
  if((DELNET == cmd->Cmd) &&(P_NULL != AckData ))
  {
    buff =  Get_KingDaServerBuff ( ); 
    if(P_NULL != buff)
    { 
      buff->buff.head.Sync = KingDaServerSync_ToServer;
      buff->buff.head.Cmd = ServerDeleteNode_APPCON;
    
      buff->buff.head.Length_L = sizeof(APPCON_DeleteNode_ToKingDaServer_Type);
      buff->buff.head.Length_H = sizeof(APPCON_DeleteNode_ToKingDaServer_Type)>> 8;
      
      buff->buff.pram.APPCON_DeleteNode_Send.Concentrator_ID[0] = Concentrator.id[0];
      buff->buff.pram.APPCON_DeleteNode_Send.Concentrator_ID[1] = Concentrator.id[1];
      buff->buff.pram.APPCON_DeleteNode_Send.Concentrator_ID[2] = Concentrator.id[2];
    
      AsciiHexToBCD ( Concentrator.AppconNetID, 4, 
                      buff->buff.pram.APPCON_DeleteNode_Send.Net_ID, 4);
      
      switch(AckType)
      {
      case OK_APPCON:
        {
          buff->buff.pram.APPCON_DeleteNode_Send.Resu[0]= 'O';
          buff->buff.pram.APPCON_DeleteNode_Send.Resu[1]= 'K';
          break;
        }
      default:
        {
          buff->buff.pram.APPCON_DeleteNode_Send.Resu[0]= 'E';
          buff->buff.pram.APPCON_DeleteNode_Send.Resu[1]= 'R';
          break;
        }
      }
      buff->TargetPort = cmd->SourcePort;
      err = KingDaServer_BuffSend(buff);
      if(NO_ERR != err)
      {Free_KingDaServerBuff(buff);}
    }
  }
 freeSendBuff_APPCON(cmd);
}
/*********************************************************************************
 Function:      //
 Description:   //删除节点命令
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void APPCON_DeleteNode_func(KingDaServer_PackageStruct* pbuff)
{
 if(KingDaServerSync_ToConcentrator == pbuff->head.Sync)
 {
  if((pbuff->pram.APPCON_DeleteNode_Receive.Concentrator_ID[0] == Concentrator.id[0]) 
       &&(pbuff->pram.APPCON_DeleteNode_Receive.Concentrator_ID[1] == Concentrator.id[1])
       &&(pbuff->pram.APPCON_DeleteNode_Receive.Concentrator_ID[2] == Concentrator.id[2]))
  {
    APPCON_DELNET_SendFunc (InPort_PV,P_NULL,APPCON_DeleteNode_AckFunc); 
  }
 }
}
/*********************************************************************************
 Function:      //
 Description:   //组网并抄表
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void APPCON_BATCH_AckFunc (APPCON_OutPutBuff_Struct *cmd, 
                                         APPCON_AckType AckType, 
                                         DataBuffSpec_Struct* AckData)
{
  SystemErrName err = NO_ERR; 
  KingDaServerSendBuff_Struct*  buff;  
  if((BATCH == cmd->Cmd) &&(P_NULL != AckData ))
  {
    buff =  Get_KingDaServerBuff ( ); 
    if(P_NULL != buff)
    { 
      buff->buff.head.Sync = KingDaServerSync_ToServer;
      buff->buff.head.Cmd = ServerBATCH_APPCON;
    
      buff->buff.head.Length_L = sizeof(APPCON_BATCH_ToKingDaServer_Type);
      buff->buff.head.Length_H = sizeof(APPCON_BATCH_ToKingDaServer_Type)>> 8;
      
      buff->buff.pram.APPCON_BATCH_Send.Concentrator_ID[0] = Concentrator.id[0];
      buff->buff.pram.APPCON_BATCH_Send.Concentrator_ID[1] = Concentrator.id[1];
      buff->buff.pram.APPCON_BATCH_Send.Concentrator_ID[2] = Concentrator.id[2];
    
      AsciiHexToBCD ( Concentrator.AppconNetID, 4, 
                      buff->buff.pram.APPCON_BATCH_Send.Net_ID, 4);
      
      switch(AckType)
      {
      case OK_APPCON:
        {
          buff->buff.pram.APPCON_BATCH_Send.Resu[0]= 'O';
          buff->buff.pram.APPCON_BATCH_Send.Resu[1]= 'K';
          break;
        }
      default:
        {
          buff->buff.pram.APPCON_BATCH_Send.Resu[0]= 'E';
          buff->buff.pram.APPCON_BATCH_Send.Resu[1]= 'R';
          break;
        }
      }
      buff->TargetPort = cmd->SourcePort;
      err = KingDaServer_BuffSend(buff);
      if(NO_ERR != err)
      {Free_KingDaServerBuff(buff);}
    }
  }
 freeSendBuff_APPCON(cmd);
}
/*********************************************************************************
 Function:      //
 Description:   //组网并抄表
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void APPCON_BATCH_func(KingDaServer_PackageStruct* pbuff)
{
 if(KingDaServerSync_ToConcentrator == pbuff->head.Sync)
 {
  if((pbuff->pram.APPCON_BATCH_Receive.Concentrator_ID[0] == Concentrator.id[0]) 
       &&(pbuff->pram.APPCON_BATCH_Receive.Concentrator_ID[1] == Concentrator.id[1])
       &&(pbuff->pram.APPCON_BATCH_Receive.Concentrator_ID[2] == Concentrator.id[2]))
  {
   APPCON_BATCH_SendFunc (InPort_PV,P_NULL,APPCON_BATCH_AckFunc);
  }
 }
}
/*********************************************************************************
 Function:      //
 Description:   //查询组建网络状态
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void APPCON_STATUS_AckFunc ( APPCON_OutPutBuff_Struct *cmd, 
                                     APPCON_AckType AckType, 
                                     DataBuffSpec_Struct* AckData)
{
 SystemErrName err = NO_ERR; 
 KingDaServerSendBuff_Struct*  buff;  

 if((STATUS == cmd->Cmd) &&(P_NULL != AckData ))
 {
  buff =  Get_KingDaServerBuff ( ); 
  if(P_NULL != buff)
  { 
    buff->buff.head.Sync = KingDaServerSync_ToServer;
    buff->buff.head.Cmd = ServerSTATUS_APPCON;
    
    buff->buff.head.Length_L = sizeof(APPCON_STATUS_ToKingDaServer_Type);
    buff->buff.head.Length_H = sizeof(APPCON_STATUS_ToKingDaServer_Type)>> 8;
    
    buff->buff.pram.APPCON_STATUS_Send.Concentrator_ID[0] = Concentrator.id[0];
    buff->buff.pram.APPCON_STATUS_Send.Concentrator_ID[1] = Concentrator.id[1];
    buff->buff.pram.APPCON_STATUS_Send.Concentrator_ID[2] = Concentrator.id[2];
    
    AsciiHexToBCD ( Concentrator.AppconNetID, 4, 
                    buff->buff.pram.APPCON_STATUS_Send.Net_ID, 4);

    
   switch(AckType)
   {
    case BUSY_APPCON:
      {
       buff->buff.pram.APPCON_STATUS_Send.Resu[0] = 'B';
       buff->buff.pram.APPCON_STATUS_Send.Resu[1] = 'U';
       buff->buff.pram.APPCON_STATUS_Send.Resu[2] = 'S';
       buff->buff.pram.APPCON_STATUS_Send.Resu[3] = 'Y';
       break;
      }
   case FREE_APPCON:
     {
       buff->buff.pram.APPCON_STATUS_Send.Resu[0] = 'F';
       buff->buff.pram.APPCON_STATUS_Send.Resu[1] = 'R';
       buff->buff.pram.APPCON_STATUS_Send.Resu[2] = 'E';
       buff->buff.pram.APPCON_STATUS_Send.Resu[3] = 'E';
       break;
     }
   default:
     {
       buff->buff.pram.APPCON_STATUS_Send.Resu[0] = 'E';
       buff->buff.pram.APPCON_STATUS_Send.Resu[1] = 'R';
       buff->buff.pram.APPCON_STATUS_Send.Resu[2] = 'R';
       buff->buff.pram.APPCON_STATUS_Send.Resu[3] = 'R';
       break;
     }
   }
   buff->TargetPort = cmd->SourcePort;
   err = KingDaServer_BuffSend(buff);
   if(NO_ERR != err)
   {Free_KingDaServerBuff(buff);}
  }
 }
 freeSendBuff_APPCON(cmd);
}
/*********************************************************************************
 Function:      //
 Description:   //查询组建网络状态
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void APPCON_STATUS_func(KingDaServer_PackageStruct* pbuff)
{
 if(KingDaServerSync_ToConcentrator == pbuff->head.Sync)
 {
  if((pbuff->pram.APPCON_STATUS_Receive.Concentrator_ID[0] == Concentrator.id[0]) 
       &&(pbuff->pram.APPCON_STATUS_Receive.Concentrator_ID[1] == Concentrator.id[1])
       &&(pbuff->pram.APPCON_STATUS_Receive.Concentrator_ID[2] == Concentrator.id[2]))
  {
    APPCON_STATUS_SendFunc (InPort_PV,P_NULL,APPCON_STATUS_AckFunc);
  }
 }
}
/*********************************************************************************
 Function:      //
 Description:   // 阀门状态应答
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void APPCON_OVSTATUS_AckFunc (APPCON_OutPutBuff_Struct *cmd, 
                                      APPCON_AckType AckType, 
                                      DataBuffSpec_Struct* AckData)
{
 unsigned char data; 
 SystemErrName err = NO_ERR; 
 KingDaServerSendBuff_Struct*  buff;  

 if((TST == cmd->Cmd) &&(P_NULL != AckData ))
 {
  buff =  Get_KingDaServerBuff ( ); 
  if(P_NULL != buff)
  {
    buff->buff.head.Sync = KingDaServerSync_ToServer;
    buff->buff.head.Cmd = ServerGetOV_APPCON;
    data = cmd->Buff[13] - '0';
    data *= 10;  
    data += cmd->Buff[14] - '0';
    data *= 10; 
    data += cmd->Buff[15] - '0';
    if(GetValveCmd_APPCON == data)           //0XC0, 查询阀门状态
    {buff->buff.head.Cmd = ServerGetOV_APPCON;}
    else if(HalfValveCmd_APPCON == data)    //0XC1, 阀门半开指令
    {buff->buff.head.Cmd = ServerGetOV_APPCON;}
    else if(OpenValveCmd_APPCON == data)    //0XC3, 阀门打开指令
    {buff->buff.head.Cmd = ServerGetOV_APPCON;}
    else if(CloseValveCmd_APPCON == data)   //0XC2, 阀门关闭指令
    {buff->buff.head.Cmd = ServerGetOV_APPCON;}
    else if(MT_ValveCmd_APPCON == data)     //0XC4, 阀门防锈指令
    {buff->buff.head.Cmd = ServerGetOV_APPCON;}
    else 
    {
      freeSendBuff_APPCON(cmd);
      Free_KingDaServerBuff(buff);
      return;
    }
    buff->buff.head.Length_L = sizeof(APPCON_OVSTATUS_ToKingDaServer_Type);
    buff->buff.head.Length_H = sizeof(APPCON_OVSTATUS_ToKingDaServer_Type)>> 8;
    
    buff->buff.pram.APPCON_OVSTATUS_Send.Concentrator_ID[0] = Concentrator.id[0];
    buff->buff.pram.APPCON_OVSTATUS_Send.Concentrator_ID[1] = Concentrator.id[1];
    buff->buff.pram.APPCON_OVSTATUS_Send.Concentrator_ID[2] = Concentrator.id[2];
    
    buff->buff.pram.APPCON_OVSTATUS_Send.Node_ID[0] = cmd->Buff[4];
    buff->buff.pram.APPCON_OVSTATUS_Send.Node_ID[1] = cmd->Buff[5];
    buff->buff.pram.APPCON_OVSTATUS_Send.Node_ID[2] = cmd->Buff[6];
    buff->buff.pram.APPCON_OVSTATUS_Send.Node_ID[3] = cmd->Buff[7];
    buff->buff.pram.APPCON_OVSTATUS_Send.Node_ID[4] = cmd->Buff[8];
    buff->buff.pram.APPCON_OVSTATUS_Send.Node_ID[5] = cmd->Buff[9];
    buff->buff.pram.APPCON_OVSTATUS_Send.Node_ID[6] = cmd->Buff[10];
    buff->buff.pram.APPCON_OVSTATUS_Send.Node_ID[7] = cmd->Buff[11];
 
    if(Ans_APPCON == AckType)
    {
      data = APPCON_AsciiHexToDigit(AckData->pbuff,2,2);
      switch(data)
      {
       case ValveUnState_APPCON:  //阀门不明状态
        {buff->buff.pram.APPCON_OVSTATUS_Send.Status= 0x0450; break;}
       case HalfValveState_APPCON:  //阀门半开状态
        {buff->buff.pram.APPCON_OVSTATUS_Send.Status= 0x0850;break;}
       case CloseValveState_APPCON:  //阀门关闭状态
        {buff->buff.pram.APPCON_OVSTATUS_Send.Status= 0x0650;break;} 
       case OpenValveState_APPCON:  //阀门打开状态
        {buff->buff.pram.APPCON_OVSTATUS_Send.Status= 0x0550;break;} 
      }
    }
    else
    {buff->buff.pram.APPCON_OVSTATUS_Send.Status= 0x0450; }
   buff->TargetPort = cmd->SourcePort;
   err = KingDaServer_BuffSend(buff);
   if(NO_ERR != err)
   {Free_KingDaServerBuff(buff);}
  }
 }
 freeSendBuff_APPCON(cmd);
}
/*********************************************************************************
 Function:      //
 Description:   //开阀
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
/*static void APPCON_OpenOV_AckFunc (APPCON_OutPutBuff_Struct *cmd, 
                                    APPCON_AckType AckType, 
                                    DataBuffSpec_Struct* AckData)
{
}*/
/*********************************************************************************
 Function:      //
 Description:   // 阀门状态应答
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
 void APPCON_OVWeit_AckFunc(void *p_tmr, void *p_arg)
{
  APPCON_OutPutBuff_Struct *cmd;
  if(P_NULL !=p_arg)
  {
   cmd = p_arg;
   APPCON_ValveCmd_SendFunc(cmd->SourcePort,
                             &(cmd->Buff[4]),
                             GetValveCmd_APPCON,
                             P_NULL,
                             APPCON_OVSTATUS_AckFunc); 
   freeSendBuff_APPCON(cmd);
  }

}
/*********************************************************************************
 Function:      //
 Description:   // 阀门状态应答
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void APPCON_OVWeit_Func (APPCON_OutPutBuff_Struct *cmd, 
                                 APPCON_AckType AckType, 
                                 DataBuffSpec_Struct* AckData)
{ 
  unsigned char ack;             
  SofTimer_Struct*   Timer;  
  
  if(Ans_APPCON == AckType)
    {
      ack = APPCON_AsciiHexToDigit(AckData->pbuff,2,2);
      if(ValveUnState_APPCON == ack)
      {
        Timer =  SofTimerCreate ( 18,                 //周期
                                  TimerTB_1S,                    //时基
                                  SofTimer_ONE_SHOT,     //模式
                                  APPCON_OVWeit_AckFunc,
                                 cmd);           //回调参数                      
       if(P_NULL == Timer)
       {APPCON_OVSTATUS_AckFunc (cmd, AckType, AckData);}
       SofTimerStart (Timer);
       return;
      }
    }
  APPCON_OVSTATUS_AckFunc (cmd, AckType, AckData);
}
/*********************************************************************************
 Function:      //
 Description:   //开阀
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void APPCON_OpenOV_func(KingDaServer_PackageStruct* pbuff)
{
 if(KingDaServerSync_ToConcentrator == pbuff->head.Sync)
 {
  if((pbuff->pram.APPCON_OpenOV_Receive.Concentrator_ID[0] == Concentrator.id[0]) 
      &&(pbuff->pram.APPCON_OpenOV_Receive.Concentrator_ID[1] == Concentrator.id[1])
      &&(pbuff->pram.APPCON_OpenOV_Receive.Concentrator_ID[2] == Concentrator.id[2]))
  {
    APPCON_ValveCmd_SendFunc(InPort_PV,
                             pbuff->pram.APPCON_OpenOV_Receive.Node_ID,
                             OpenValveCmd_APPCON,
                             P_NULL,
                             APPCON_OVWeit_Func);
  }
 }
}
/*********************************************************************************
 Function:      //
 Description:   //半开阀门
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void APPCON_HalfOV_func(KingDaServer_PackageStruct* pbuff)
{
 if(KingDaServerSync_ToConcentrator == pbuff->head.Sync)
 {
  if((pbuff->pram.APPCON_HalfOV_Receive.Concentrator_ID[0] == Concentrator.id[0]) 
       &&(pbuff->pram.APPCON_HalfOV_Receive.Concentrator_ID[1] == Concentrator.id[1])
       &&(pbuff->pram.APPCON_HalfOV_Receive.Concentrator_ID[2] == Concentrator.id[2]))
  {
    APPCON_ValveCmd_SendFunc(InPort_PV,
                             pbuff->pram.APPCON_HalfOV_Receive.Node_ID,
                             HalfValveCmd_APPCON,
                             P_NULL,
                             APPCON_OVWeit_Func);
  }
 }
}
/*********************************************************************************
 Function:      //
 Description:   //活动阀命令
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void APPCON_MtOV_func(KingDaServer_PackageStruct* pbuff)
{
  if(KingDaServerSync_ToConcentrator == pbuff->head.Sync)
 {
  if((pbuff->pram.APPCON_MtOV_Receive.Concentrator_ID[0] == Concentrator.id[0]) 
       &&(pbuff->pram.APPCON_MtOV_Receive.Concentrator_ID[1] == Concentrator.id[1])
       &&(pbuff->pram.APPCON_MtOV_Receive.Concentrator_ID[2] == Concentrator.id[2]))
  {
    APPCON_ValveCmd_SendFunc(InPort_PV,
                             pbuff->pram.APPCON_MtOV_Receive.Node_ID,
                             MT_ValveCmd_APPCON,
                             P_NULL,
                             APPCON_OVWeit_Func);
  }
 }
}
/*********************************************************************************
 Function:      //
 Description:   //关阀
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void APPCON_CloseOV_func(KingDaServer_PackageStruct* pbuff)
{
  if(KingDaServerSync_ToConcentrator == pbuff->head.Sync)
 {
  if((pbuff->pram.APPCON_CloseOV_Receive.Concentrator_ID[0] == Concentrator.id[0]) 
       &&(pbuff->pram.APPCON_CloseOV_Receive.Concentrator_ID[1] == Concentrator.id[1])
       &&(pbuff->pram.APPCON_CloseOV_Receive.Concentrator_ID[2] == Concentrator.id[2]))
  {
    APPCON_ValveCmd_SendFunc(InPort_PV,
                             pbuff->pram.APPCON_CloseOV_Receive.Node_ID,
                             CloseValveCmd_APPCON,
                             P_NULL,
                             APPCON_OVWeit_Func);
  }
 }
} 
/*********************************************************************************
 Function:      //
 Description:   // 读取阀门状态
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void APPCON_GetOV_func(KingDaServer_PackageStruct* pbuff)      
{
 if(KingDaServerSync_ToConcentrator == pbuff->head.Sync)
 {
  if((pbuff->pram.APPCON_GetOV_Receive.Concentrator_ID[0] == Concentrator.id[0]) 
       &&(pbuff->pram.APPCON_GetOV_Receive.Concentrator_ID[1] == Concentrator.id[1])
       &&(pbuff->pram.APPCON_GetOV_Receive.Concentrator_ID[2] == Concentrator.id[2]))
  {
    APPCON_ValveCmd_SendFunc(InPort_PV,
                             pbuff->pram.APPCON_GetOV_Receive.Node_ID,
                             GetValveCmd_APPCON,
                             P_NULL,
                             APPCON_OVSTATUS_AckFunc);
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
Others:         // 
*********************************************************************************/
static void ReadMeasureValue_func(KingDaServer_PackageStruct* pbuff)
{
 if(KingDaServerSync_ToConcentrator == pbuff->head.Sync)
 {
  if((pbuff->pram.ReadMeasureValue_Receive.Concentrator_ID[0] == Concentrator.id[0]) 
       &&(pbuff->pram.ReadMeasureValue_Receive.Concentrator_ID[1] == Concentrator.id[1])
       &&(pbuff->pram.ReadMeasureValue_Receive.Concentrator_ID[2] == Concentrator.id[2]))
  {
   if(ReadMeasureValue == pbuff->head.Cmd) 
   {
    ReadNodeValue_KindDa( pbuff->pram.ReadMeasureValue_Receive.Node_ID,
                          AutomatchPort, 
                          InPort_PV,
                          ReadMeasureValue_AckFunc);
   }
   else if(ReadMeasureValue_OFF == pbuff->head.Cmd) 
   {
    if(pbuff->head.Length_L >= sizeof(APPCON_ReadMeasureValue_ToKingDaConcentrator_Type)) 
    {
      APPCON_CMD_SendFunc ( InPort_PV,
                            pbuff->pram.ReadMeasureValue_Receive_APPCON.Node_ID,
                            P_NULL,
                            ReadNodeValue_APPCON_AckFunc); 
    }
    else
    {
     ReadNodeValue_KindDa( pbuff->pram.ReadMeasureValue_Receive.Node_ID,
                          AutomatchPort, 
                         InPort_PV,
                         ReadMeasureValue_OFF_AckFunc); 
    }
   }
  }  
 }
}

/*********************************************************************************
 Function:      //
 Description:   // 阀门状态应答
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void _OVWeit_AckFunc(void *p_tmr, void *p_arg)
{
  KingDaServerSendBuff_Struct *buff;
  SofTimer_Struct* Timer;
  Timer = p_tmr;
  if(P_NULL !=p_arg)
  {
    buff = p_arg;
    if(ServerGetOV_APPCON != buff->buff.head.Cmd)
    {
       SofTimer_ChangePeriod (Timer, 3, 1);
       changeChannel_Rs485(Null_Port);
       buff->buff.head.Cmd = ServerGetOV_APPCON;
    }
    else
    {
      SofTimerDel ( &Timer);
      OperateValve_KindDa(Get_OV,
                        buff->buff.pram.OperateValve_Send.Node_ID,
                        AutomatchPort,
                        buff->TargetPort,                                   
                        KindgGetOV_AckFunc);
      Free_KingDaServerBuff(buff);
    }
  }
  else
  {SofTimerDel ( &Timer);}
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
static void KindgOperateValve_AckFunc(PProtocolSendBuff_Struct* SendBuff, void* AckData)
{
  KingDaServerSendBuff_Struct* buff;
  KindgDaSPCP_PackageDataStruct* ACK;   //水表数据  应答数据             
  SofTimer_Struct*   Timer; 
  unsigned char *itemp;
  unsigned char weitTime = 15;
  KingDaServer_CmdCode  cmd = ServerGetOV_APPCON;
  
  if(P_NULL != AckData )
  {
    ACK = AckData;
    switch( ACK->TypeA.head.Cmd)
    {
    case Open_OV:           //开阀
      {itemp = ACK->TypeA.pram.Open_OV_ack.addr;break;}
    case Close_OV :         //关阀
      {itemp = ACK->TypeA.pram.Close_OV_ack.addr;break;}
    case Open_OV_ENF :      //强制开阀
      {itemp = ACK->TypeA.pram.Open_OV_ENF_ack.addr;break;}
    case Close_OV_ENF:     //强制关阀
      {itemp = ACK->TypeA.pram.Close_OV_ENF_ack.addr;break;}
    case MT_OV  :           //活动阀命令
      {
        itemp = ACK->TypeA.pram.MT_OV_ack.addr;
        cmd = ServerMtOV_APPCON;
        weitTime = 25;
       break;
      }
    default:
      {return;}
    }
    
  }
  else
  {
    switch( SendBuff->buff.Data.TypeA.head.Cmd)
    {
    case Open_OV:           //开阀
      {itemp = SendBuff->buff.Data.TypeA.pram.Open_OV_ask.addr;break;}
    case Close_OV :         //关阀
      {itemp = SendBuff->buff.Data.TypeA.pram.Close_OV_ask.addr;break;}
    case Open_OV_ENF :      //强制开阀
      {itemp = SendBuff->buff.Data.TypeA.pram.Open_OV_ENF_ask.addr;break;}
    case Close_OV_ENF:     //强制关阀
      {itemp = SendBuff->buff.Data.TypeA.pram.Close_OV_ENF_ask.addr;break;}
    case MT_OV  :           //活动阀命令
      {
        itemp = SendBuff->buff.Data.TypeA.pram.MT_OV_ask.addr;
        cmd = ServerMtOV_APPCON;
        weitTime = 25;
        break;
      }
    default:
      {return;}
    }
  }
  buff =  Get_KingDaServerBuff ( ); 
  if(P_NULL == buff)
  {return ;}
  buff->buff.pram.OperateValve_Send.Node_ID[0] = itemp[0];
  buff->buff.pram.OperateValve_Send.Node_ID[1] = itemp[1]; 
  buff->buff.pram.OperateValve_Send.Node_ID[2] = itemp[2]; 
  buff->buff.head.Cmd = cmd;
  buff->TargetPort =  SendBuff->SourcePort;   
  Timer =  SofTimerCreate (weitTime,                    //周期
                           TimerTB_1S,            //时基
                           SofTimer_PERIODIC,     //模式
                           _OVWeit_AckFunc,
                           buff);                //回调参数    
  if(P_NULL == Timer)
  {
    Free_KingDaServerBuff(buff);
    KindgGetOV_AckFunc( SendBuff, P_NULL);
  }
  else
  {SofTimerStart(Timer);}
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
static void KindgGetOV_AckFunc(PProtocolSendBuff_Struct* SendBuff, void* AckData)
{
  SystemErrName err = NO_ERR; 
  KingDaServerSendBuff_Struct*  buff;  
  KindgDaSPCP_PackageDataStruct* ACK;   //水表数据  应答数据 
  
  changeChannel_Rs485(Null_Port);
  if(P_NULL != AckData )
  {
     buff =  Get_KingDaServerBuff ( ); 
    if(P_NULL == buff){return ;}
    buff->buff.head.Sync = KingDaServerSync_ToServer;
    ACK = AckData;
    buff->buff.head.Cmd = ServerGetOV_APPCON;
    if(0x99 == ACK->TypeA.pram.Get_OV_ack.State)  //表示阀关闭状态   
    {buff->buff.pram.OperateValve_Send.state= 0x0650;}  //阀门关闭状态
    else if(0x66 == ACK->TypeA.pram.Get_OV_ack.State)   //表示阀开状态
    {buff->buff.pram.OperateValve_Send.state= 0x0550;}   ///阀门打开状态
    else  //表示控阀失败状态
    {buff->buff.pram.OperateValve_Send.state = 0x0450;} //阀门不明状态
    
    buff->buff.pram.OperateValve_Send.Node_ID[0] = ACK->TypeA.pram.ReadValue_ack.addr[0];
    buff->buff.pram.OperateValve_Send.Node_ID[1] = ACK->TypeA.pram.ReadValue_ack.addr[1];
    buff->buff.pram.OperateValve_Send.Node_ID[2] = ACK->TypeA.pram.ReadValue_ack.addr[2];
    
    buff->buff.head.Cmd = ServerGetOV_APPCON;
    buff->buff.head.Length_L = sizeof(OperateValve_ToKingDaServer_Type);
    buff->buff.head.Length_H = sizeof(OperateValve_ToKingDaServer_Type)>> 8;;
    buff->buff.pram.OperateValve_Send.Concentrator_ID[0] = Concentrator.id[0];
    buff->buff.pram.OperateValve_Send.Concentrator_ID[1] = Concentrator.id[1];
    buff->buff.pram.OperateValve_Send.Concentrator_ID[2] = Concentrator.id[2];
    buff->TargetPort = SendBuff->SourcePort;
    err = KingDaServer_BuffSend(buff);
    if(NO_ERR != err)
    {Free_KingDaServerBuff(buff);}
  }
}
/*********************************************************************************
Function:      //
Description:   //  //开阀
Input:         //
//
Output:        //
Return:        //              
Others:         // 
*********************************************************************************/
static void KingDa_OpenOV_func(KingDaServer_PackageStruct* pbuff)     
{ 
  if(KingDaServerSync_ToConcentrator == pbuff->head.Sync)
  {
    if((pbuff->pram.ReadMeasureValue_Receive.Concentrator_ID[0] == Concentrator.id[0]) 
       &&(pbuff->pram.ReadMeasureValue_Receive.Concentrator_ID[1] == Concentrator.id[1])
         &&(pbuff->pram.ReadMeasureValue_Receive.Concentrator_ID[2] == Concentrator.id[2]))
    {
      if((ServerOpenOV_APPCON == pbuff->head.Cmd)||(ServerCloseOV_APPCON == pbuff->head.Cmd)) 
      {
        OperateValve_KindDa(Open_OV,pbuff->pram.OperateValveReceive.Node_ID,AutomatchPort,InPort_PV,KindgOperateValve_AckFunc);
      }
    }
  }   
}
/*********************************************************************************
Function:      //
Description:   //半开阀门
Input:         //
//
Output:        //
Return:        //              
Others:         // 
*********************************************************************************/
static void KingDa_HalfOV_func(KingDaServer_PackageStruct* pbuff)      //
{
}
/*********************************************************************************
Function:      //
Description:   //活动阀命令
Input:         //
//
Output:        //
Return:        //              
Others:         // 
*********************************************************************************/
static void KingDa_MtOV_func(KingDaServer_PackageStruct* pbuff)         //活动阀命令
{
  if(KingDaServerSync_ToConcentrator == pbuff->head.Sync)
  {
    if((pbuff->pram.ReadMeasureValue_Receive.Concentrator_ID[0] == Concentrator.id[0]) 
       &&(pbuff->pram.ReadMeasureValue_Receive.Concentrator_ID[1] == Concentrator.id[1])
         &&(pbuff->pram.ReadMeasureValue_Receive.Concentrator_ID[2] == Concentrator.id[2]))
    {
      if(ServerMtOV_APPCON == pbuff->head.Cmd) 
      {
        OperateValve_KindDa(MT_OV,pbuff->pram.OperateValveReceive.Node_ID, AutomatchPort, InPort_PV,KindgOperateValve_AckFunc);
      }
    }
  }  
}


/*********************************************************************************
Function:      //
Description:   //关阀
Input:         //
               //
Output:        //
Return:        //              
Others:        // 
*********************************************************************************/
static void KingDa_CloseOV_func(KingDaServer_PackageStruct* pbuff)      //关阀
{ 
  if(KingDaServerSync_ToConcentrator == pbuff->head.Sync)
  {
    if((pbuff->pram.ReadMeasureValue_Receive.Concentrator_ID[0] == Concentrator.id[0]) 
       &&(pbuff->pram.ReadMeasureValue_Receive.Concentrator_ID[1] == Concentrator.id[1])
         &&(pbuff->pram.ReadMeasureValue_Receive.Concentrator_ID[2] == Concentrator.id[2]))
    {
      if(ServerCloseOV_APPCON == pbuff->head.Cmd)
      {
        OperateValve_KindDa(Close_OV,pbuff->pram.OperateValveReceive.Node_ID,AutomatchPort,InPort_PV,  KindgOperateValve_AckFunc);
      }
    }
  } 
}
/*********************************************************************************
Function:      //
Description:   //    //强制开阀
Input:         //
//
Output:        //
Return:        //              
Others:         // 
*********************************************************************************/
static void  KingDa_OpenOV_ENF_func (KingDaServer_PackageStruct* pbuff)      
{
  if(KingDaServerSync_ToConcentrator == pbuff->head.Sync)
  {
    if((pbuff->pram.ReadMeasureValue_Receive.Concentrator_ID[0] == Concentrator.id[0]) 
       &&(pbuff->pram.ReadMeasureValue_Receive.Concentrator_ID[1] == Concentrator.id[1])
         &&(pbuff->pram.ReadMeasureValue_Receive.Concentrator_ID[2] == Concentrator.id[2]))
    {
      if(ServerOpen_OV_ENF == pbuff->head.Cmd)
      {
        OperateValve_KindDa(Open_OV_ENF,pbuff->pram.OperateValveReceive.Node_ID,AutomatchPort,InPort_PV,  KindgOperateValve_AckFunc);
      }
    }
  } 
}
/*********************************************************************************
Function:      //
Description:   //  强制关阀
Input:         //
//
Output:        //
Return:        //              
Others:         // 
*********************************************************************************/
static void  KingDa_CloseOV_ENF_func (KingDaServer_PackageStruct* pbuff)            
{
    if(KingDaServerSync_ToConcentrator == pbuff->head.Sync)
  {
    if((pbuff->pram.ReadMeasureValue_Receive.Concentrator_ID[0] == Concentrator.id[0]) 
       &&(pbuff->pram.ReadMeasureValue_Receive.Concentrator_ID[1] == Concentrator.id[1])
         &&(pbuff->pram.ReadMeasureValue_Receive.Concentrator_ID[2] == Concentrator.id[2]))
    {
      if(ServerClose_OV_ENF == pbuff->head.Cmd)
      {
        OperateValve_KindDa(Close_OV_ENF,pbuff->pram.OperateValveReceive.Node_ID,AutomatchPort,InPort_PV,  KindgOperateValve_AckFunc);
      }
    }
  } 
}
/*********************************************************************************
Function:      //
Description:   //读取阀门状态
Input:         //
//
Output:        //
Return:        //              
Others:         // 
*********************************************************************************/
static void KingDa_GetOV_func(KingDaServer_PackageStruct* pbuff)        
{
  if(KingDaServerSync_ToConcentrator == pbuff->head.Sync)
  {
    if((pbuff->pram.ReadMeasureValue_Receive.Concentrator_ID[0] == Concentrator.id[0]) 
       &&(pbuff->pram.ReadMeasureValue_Receive.Concentrator_ID[1] == Concentrator.id[1])
         &&(pbuff->pram.ReadMeasureValue_Receive.Concentrator_ID[2] == Concentrator.id[2]))
    {
      if(ServerGetOV_APPCON == pbuff->head.Cmd) 
      {
        OperateValve_KindDa(Get_OV,
                            pbuff->pram.OperateValveReceive.Node_ID,
                            AutomatchPort,
                            InPort_PV,                                   
                            KindgGetOV_AckFunc);  
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
Others:         // 
*********************************************************************************/
static void TuneUp_RTC_func(KingDaServer_PackageStruct* pbuff)
{
 SystemErrName err = NO_ERR;  
 TimerData_Struct time;
 KingDaServerSendBuff_Struct*  buff; 
 
 if(KingDaServerSync_ToConcentrator == pbuff->head.Sync)
 {
  if((pbuff->pram.TuneUpRTC_Receive.Concentrator_ID[0] == Concentrator.id[0]) 
       &&(pbuff->pram.TuneUpRTC_Receive.Concentrator_ID[1] == Concentrator.id[1])
       &&(pbuff->pram.TuneUpRTC_Receive.Concentrator_ID[2] == Concentrator.id[2]))
  {
   time.Year   =  pbuff->pram.TuneUpRTC_Receive.Year;
   time.Year  +=  2000;
   time.Month  =  pbuff->pram.TuneUpRTC_Receive.Month;
   time.Day    =  pbuff->pram.TuneUpRTC_Receive.Day;
   time.Hour   =  pbuff->pram.TuneUpRTC_Receive.Hour;
   time.Minute =  pbuff->pram.TuneUpRTC_Receive.Minute;
   time.Second =  pbuff->pram.TuneUpRTC_Receive.Second;
 
   buff =  Get_KingDaServerBuff ( ); 
   if(P_NULL == buff){return;}
   PCF8563_Set_Time(&time);
   buff->buff.head.Sync = KingDaServerSync_ToServer;
   buff->buff.head.Cmd = TuneUp_RTC;
   buff->buff.head.Length_L = sizeof(TuneUpRTC_ToKingDaServer);
   buff->buff.head.Length_H = sizeof(TuneUpRTC_ToKingDaServer)>> 8;
   buff->buff.pram.TuneUpRTC_Send.Concentrator_ID[0] = Concentrator.id[0];
   buff->buff.pram.TuneUpRTC_Send.Concentrator_ID[1] = Concentrator.id[1];
   buff->buff.pram.TuneUpRTC_Send.Concentrator_ID[2] = Concentrator.id[2];
   buff->buff.pram.TuneUpRTC_Send.DIO[0] = 'O';
   buff->buff.pram.TuneUpRTC_Send.DIO[0] = 'K'; 

   buff->TargetPort = InPort_PV;
   err = KingDaServer_BuffSend(buff);
   if(NO_ERR != err)
   {Free_KingDaServerBuff(buff);}
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
Others:         // 
*********************************************************************************/
void GetRepeatersState_AckFunc(PProtocolSendBuff_Struct* SendBuff, void* AckData)
{
 SystemErrName err = NO_ERR; 
 KingDaServerSendBuff_Struct*  buff;  

 if(P_NULL == SendBuff ){return ;}
 buff =  Get_KingDaServerBuff ( ); 
 if(P_NULL == buff){return ;}
 buff->buff.head.Sync = KingDaServerSync_ToServer;
 buff->buff.head.Cmd = GetRepeatersState;
 buff->buff.head.Length_L = sizeof(GetRepeatersState_ToKingDaServer);
 buff->buff.head.Length_H = sizeof(GetRepeatersState_ToKingDaServer)>> 8;
 buff->buff.pram.GetRepeatersState_Send.Concentrator_ID[0] = Concentrator.id[0];
 buff->buff.pram.GetRepeatersState_Send.Concentrator_ID[1] = Concentrator.id[1];
 buff->buff.pram.GetRepeatersState_Send.Concentrator_ID[2] = Concentrator.id[2];
 if(P_NULL == AckData )
 { buff->buff.pram.GetRepeatersState_Send.State = 0;}
 else
 {buff->buff.pram.GetRepeatersState_Send.State = 1;}
 buff->TargetPort = SendBuff->SourcePort;
 err = KingDaServer_BuffSend(buff);
 if(NO_ERR != err)
 {Free_KingDaServerBuff(buff);}
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
static void GetRepeatersState_func(KingDaServer_PackageStruct* pbuff)
{
 if(KingDaServerSync_ToConcentrator == pbuff->head.Sync)
 {
  if((pbuff->pram.GetRepeatersState_Receive.Concentrator_ID[0] == Concentrator.id[0]) 
       &&(pbuff->pram.GetRepeatersState_Receive.Concentrator_ID[1] == Concentrator.id[1])
       &&(pbuff->pram.GetRepeatersState_Receive.Concentrator_ID[2] == Concentrator.id[2]))
  {
   ReadRepeatersNodeNumber_KindDa ( pbuff->pram.GetRepeatersState_Receive.Repeaters_ID,
                                    InPort_PV,
                                    GetRepeatersState_AckFunc);
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
Others:         // 
*********************************************************************************/
static void MeasureValue_UpLoad_func(KingDaServer_PackageStruct* pbuff)
{
 unsigned char length; 
 if(KingDaServerSync_ToConcentrator != pbuff->head.Sync)
 {return;}
 if((pbuff->pram.HandsData_Receive.OK.id[0] == Concentrator.id[0])
     &&(pbuff->pram.HandsData_Receive.OK.id[1] == Concentrator.id[1])
       &&(pbuff->pram.HandsData_Receive.OK.id[2] == Concentrator.id[2]))
 {
   length = 0;
   while (length < KingDaServerSendBuff_Number)
   {
    if((0 == send_buff[length].CCR.Bit.idle)
        &&(send_buff[length].buff.head.Cmd == pbuff->head.Cmd)
          &&(send_buff[length].TargetPort == InPort_PV)) 
    { 
     if(P_NULL != send_buff[length].AckFunc) 
     {send_buff[length].AckFunc(&send_buff[length].buff, pbuff);}
     Free_KingDaServerBuff(&(send_buff[length]));
     break;
    }
    length ++;
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
Others:         // 
*********************************************************************************/
static void ReadALLMeasureValue_func(KingDaServer_PackageStruct* pbuff)
{
 unsigned char length;  
 if(KingDaServerSync_ToConcentrator != pbuff->head.Sync)
 {return;}
 if((pbuff->pram.ReadALLMeasureValue_Receive.OK.Concentrator_ID[0] == Concentrator.id[0])
     &&(pbuff->pram.ReadALLMeasureValue_Receive.OK.Concentrator_ID[1] == Concentrator.id[1])
       &&(pbuff->pram.ReadALLMeasureValue_Receive.OK.Concentrator_ID[2] == Concentrator.id[2]))
 {
  if(0x03 == pbuff->head.Length_L )
  {
   if(ServerState[0].Port == InPort_PV)
   {SendMeasureValue_ToKingDaServer1_ACK(P_NULL, P_NULL); }
   else if(ServerState[1].Port == InPort_PV)
   {SendMeasureValue_ToKingDaServer2_ACK(P_NULL, P_NULL); }
   else
   {return;}
  }
  else 
  {
   length = 0;
   while (length < KingDaServerSendBuff_Number)
   {
    if((0 == send_buff[length].CCR.Bit.idle)
       &&(send_buff[length].buff.head.Cmd == pbuff->head.Cmd)
       &&(send_buff[length].TargetPort == InPort_PV)) 
    { 
     if(P_NULL != send_buff[length].AckFunc) 
     {send_buff[length].AckFunc(&send_buff[length].buff, pbuff);}
     Free_KingDaServerBuff(&(send_buff[length]));
     break;
    }
    length ++;
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
Others:         // 
*********************************************************************************/
/*
static void MeasureValue_UpLoad_func(KingDaServer_PackageStruct* pbuff)
{
 unsigned char length; 
 if(KingDaServerSync_ToConcentrator != pbuff->head.Sync)
 {return;}
 if((ReadALLMeasureValue != pbuff->head.Cmd)   
    &&(MeasureValue_UpLoad != pbuff->head.Cmd))
 {return;}

 length = pbuff->head.Length_H;
 length <<= 8;
 length += pbuff->head.Length_L;

 if((pbuff->pram.HandsData_Receive.OK.id[0] == Concentrator.id[0])
     &&(pbuff->pram.HandsData_Receive.OK.id[1] == Concentrator.id[1])
       &&(pbuff->pram.HandsData_Receive.OK.id[2] == Concentrator.id[2]))
 {
  if(3 == length )
  {
   if(ReadALLMeasureValue  == pbuff->head.Cmd)
   {ReadALLMeasureValue_Repeaters_ID = 1;}
  } 
  else if(5 == length )
  {
   if(('O' != pbuff->pram.HandsData_Receive.OK.DI[0])
       ||('K' != pbuff->pram.HandsData_Receive.OK.DI[1]))
   {return;}
   
   if(ReadALLMeasureValue  == pbuff->head.Cmd)
   {ReadALLMeasureValue_Repeaters_ID ++;}
   else
   {MeasureValue_UpLoad_Repeaters_ID ++;}
   length = 0; 
   while (length < KingDaServerSendBuff_Number)
   {
    if((0 == send_buff[length].CCR.Bit.idle)
        &&(send_buff[length].buff.head.Cmd == pbuff->head.Cmd)
          &&(send_buff[length].TargetPort == InPort_PV)) 
    { FreeBuff(&(send_buff[length])); }
    length ++;
   }
  }
  else{return;}
 }
 else if(5 == length )
 {
  if(('E' != pbuff->pram.ReadALLMeasureValue_Receive.ERR.DIO[0])
     ||('R' != pbuff->pram.ReadALLMeasureValue_Receive.ERR.DIO[1])
       ||('R' != pbuff->pram.ReadALLMeasureValue_Receive.ERR.DIO[2])
         ||('O' != pbuff->pram.ReadALLMeasureValue_Receive.ERR.DIO[3])
           ||('R' != pbuff->pram.ReadALLMeasureValue_Receive.ERR.DIO[4]))
  {return;}
  length = 0; 
  while (length < KingDaServerSendBuff_Number)
  {
   if((0 == send_buff[length].CCR.Bit.idle)
      &&(send_buff[length].buff.head.Cmd == pbuff->head.Cmd)
        &&(send_buff[length].TargetPort == InPort_PV)) 
   { FreeBuff(&(send_buff[length])); }  
   length ++;
  }
 }
 else{return;}
 if(ReadALLMeasureValue  == pbuff->head.Cmd)
 {Send_ALLMeasureValue_ToKingDaServer(ReadALLMeasureValue_Repeaters_ID,pbuff->head.Cmd);}
 else if(MeasureValue_UpLoad  == pbuff->head.Cmd)
 {Send_ALLMeasureValue_ToKingDaServer(MeasureValue_UpLoad_Repeaters_ID,pbuff->head.Cmd);} 
}*/
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        // 
Others:         // 
*********************************************************************************/
static void SendEnd_CallBack(CommPortNumber_Type port, void *pbuff)
{
  unsigned long itemp;
  KingDaServerSendBuff_Struct* p =  (KingDaServerSendBuff_Struct*)0;
  itemp = (unsigned long)pbuff;
  itemp  -= (unsigned long)(&(p->buff));
  Free_KingDaServerBuff((KingDaServerSendBuff_Struct*) itemp);
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
static void TimerOut_CallBack (void *pbuff)
{
  unsigned long itemp;
  KingDaServerSendBuff_Struct* buff =  (KingDaServerSendBuff_Struct*)0;
  itemp = (unsigned long)pbuff;
  itemp  -= (unsigned long)(&(buff->buff));
  buff = (KingDaServerSendBuff_Struct*) itemp;
  if(P_NULL != buff->AckFunc)
  {buff->AckFunc(&(buff->buff),P_NULL);}
  Free_KingDaServerBuff((KingDaServerSendBuff_Struct*) itemp);
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
SystemErrName KingDaServer_BuffSend (KingDaServerSendBuff_Struct *pbuff)
{
 SystemErrName err = NO_ERR; 
 unsigned short length  ;
 unsigned char itemp = 0;
 if((P_NULL == pbuff) || (Null_Port == pbuff->TargetPort))
 {return IllegalParam;}

 length = pbuff->buff.head.Length_H;
 length <<= 8;
 length += pbuff->buff.head.Length_L; 
 length += sizeof (KindgDaServer_PackageHead);

 itemp = CRC8((unsigned char *)&(pbuff->buff),length);
 ((unsigned char *)&(pbuff->buff))[length++] = itemp;  
 
 if(0 != pbuff->CCR.Bit.AckEn)
 {
  err = MXPC_Send_AutoRetrans ( pbuff->TargetPort,                //目的端口
                                &(pbuff->buff),                   //数据缓冲区
                                length,                           //数据长度
                                pbuff->RetransNumber,             //重传次数
                                200,                               //重传周期数
                                TimerTB_100MS ,                   //重传周期
                                P_NULL,                           //发送完成回调
                                TimerOut_CallBack);               //重发完成回调
  }
  else
 {
   err = MXPC_Send_AutoRetrans ( pbuff->TargetPort,               //目的端口
                                 &(pbuff->buff),                  //数据缓冲区
                                 length,                          //数据长度
                                 0,                               //重传次数
                                 0,                               //重传周期数
                                 TimerTB_100MS ,                   //重传周期
                                 SendEnd_CallBack,                //发送完成回调
                                 P_NULL);                         //重发完成回调
  }
 return err; 
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
SystemErrName SendHandCmdToKingDaServer(unsigned char ServerNO, KingDaServer_CmdAckFunc  AckFunc)
{
 SystemErrName err = NO_ERR; 
 KingDaServerSendBuff_Struct*  buff;   
 buff =  Get_KingDaServerBuff ( ); 
 if(P_NULL == buff)
 {return Nothing;}
 buff->CCR.Bit.AckEn = 1;
 buff->AckFunc =  AckFunc;
 buff->buff.head.Sync = KingDaServerSync_ToServer;
 buff->buff.head.Cmd = HandCmd_Server;
 buff->buff.head.Length_L = sizeof(HandsData_ToKingDaServer);
 buff->buff.head.Length_H = sizeof(HandsData_ToKingDaServer)>> 8;
 buff->buff.pram.HandsData_Send.id[0] = Concentrator.id[0];
 buff->buff.pram.HandsData_Send.id[1] = Concentrator.id[1];
 buff->buff.pram.HandsData_Send.id[2] = Concentrator.id[2];
 buff->buff.pram.HandsData_Send.DIO[0] = 'H';
 buff->buff.pram.HandsData_Send.DIO[1] = 'A';
 buff->buff.pram.HandsData_Send.DIO[2] = 'N';
 buff->buff.pram.HandsData_Send.DIO[3] = 'D';
 buff->buff.pram.HandsData_Send.DIO[4] = 'S';
 buff->buff.pram.HandsData_Send.CSQ =  MG2618Infr.ModInf.Rssi;
 buff->TargetPort = getServerPort(ServerNO);
 err = KingDaServer_BuffSend(buff);
 if(NO_ERR != err)
 {Free_KingDaServerBuff(buff);}
 return err;
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
SystemErrName Send_ALLMeasureValue_ToKingDaServer(unsigned char Repeaters_ID,
                                                  KingDaServer_CmdCode Code)
{
 SystemErrName err = NO_ERR; 
 unsigned short length;
 unsigned short itemp ;
 KingDaServerSendBuff_Struct*  buff; 
 
 buff =  Get_KingDaServerBuff ( ); 
 if(P_NULL == buff){return Nothing;}
 
 //buff->CCR.Bit.AckEn = 1;
 //buff->AckFunc =  AckFunc;
 
 buff->buff.head.Sync = KingDaServerSync_ToServer;
 buff->buff.head.Cmd = Code;
   
 buff->buff.pram.ReadALLMeasureValue_Send.Concentrator_ID[0] = Concentrator.id[0];
 buff->buff.pram.ReadALLMeasureValue_Send.Concentrator_ID[1] = Concentrator.id[1];
 buff->buff.pram.ReadALLMeasureValue_Send.Concentrator_ID[2] = Concentrator.id[2];
   
 buff->buff.pram.ReadALLMeasureValue_Send.RepeatersNet_ID[0] = 0;
 buff->buff.pram.ReadALLMeasureValue_Send.RepeatersNet_ID[1] = 0;
 buff->buff.pram.ReadALLMeasureValue_Send.RepeatersNet_ID[2] = 0;
   
 buff->buff.pram.ReadALLMeasureValue_Send.Repeaters_ID = Repeaters_ID;
 buff->buff.pram.ReadALLMeasureValue_Send.RepeatersVoltage = 12;
 buff->buff.pram.ReadALLMeasureValue_Send.DIO[0] = 0xAA;
 buff->buff.pram.ReadALLMeasureValue_Send.DIO[1] = 0xAA;
   
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[0] = 1;
 length = buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[0];
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[1] = 2;
 length += buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[1];
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[2] = 3;
 length += buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[2];
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[3] = 4;
 length += buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[3];

 itemp = 0;
 while(itemp < length)
 {
  buff->buff.pram.ReadALLMeasureValue_Send.Data[itemp] = itemp;
  itemp++;
 }
 length = itemp <<1; 
 //length += (unsigned long)((ReadALLMeasureValue_ToKingDaServer *)0)->Data;
 length += 14;
 buff->buff.head.Length_L = length;
 buff->buff.head.Length_H = length >> 8;

 buff->TargetPort = InPort_PV;
 err = KingDaServer_BuffSend(buff);
 if(NO_ERR != err)
 {Free_KingDaServerBuff(buff);} 
 return err;
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
SystemErrName UpdatMeasureValue_ToKingDaServer(unsigned char Repeaters_NO, 
                                               CommPortNumber_Type Port,
                                               KingDaServer_CmdAckFunc AckFunc)
{
 SystemErrName err = NO_ERR; 
 unsigned short length;
 KingDaServerSendBuff_Struct*  buff; 
 Repeaters_DescrType repeaters;
 
 buff =  Get_KingDaServerBuff ( ); 
 if(P_NULL == buff){return Nothing;}
 
 buff->CCR.Bit.AckEn = 1;
 buff->AckFunc =  AckFunc;
 
 buff->buff.head.Sync = KingDaServerSync_ToServer;
 buff->buff.head.Cmd = MeasureValue_UpLoad;
   
 buff->buff.pram.ReadALLMeasureValue_Send.Concentrator_ID[0] = Concentrator.id[0];
 buff->buff.pram.ReadALLMeasureValue_Send.Concentrator_ID[1] = Concentrator.id[1];
 buff->buff.pram.ReadALLMeasureValue_Send.Concentrator_ID[2] = Concentrator.id[2];
   
 buff->buff.pram.ReadALLMeasureValue_Send.RepeatersNet_ID[0] = 0;
 buff->buff.pram.ReadALLMeasureValue_Send.RepeatersNet_ID[1] = 0;
 buff->buff.pram.ReadALLMeasureValue_Send.RepeatersNet_ID[2] = 0;
   
 buff->buff.pram.ReadALLMeasureValue_Send.Repeaters_ID = Repeaters_NO;
 
 repeaters.ID =  Repeaters_NO;
 MeasureValue_ToKingDaServer(&repeaters, buff->buff.pram.ReadALLMeasureValue_Send.Data);
   
 buff->buff.pram.ReadALLMeasureValue_Send.RepeatersVoltage = repeaters.Voltage;
 buff->buff.pram.ReadALLMeasureValue_Send.DIO[0] = 0xAA;
 buff->buff.pram.ReadALLMeasureValue_Send.DIO[1] = 0xAA;
   
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[0] = repeaters.CH_NO[0];
 length = buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[0];
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[1] = repeaters.CH_NO[1];
 length += buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[1];
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[2] = repeaters.CH_NO[2];
 length += buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[2];
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[3] = repeaters.CH_NO[3];
 length += buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[3];

 length <<= 1; 
 length += 14;
 buff->buff.head.Length_L = length;
 buff->buff.head.Length_H = length >> 8;

 buff->TargetPort = Port;
 err = KingDaServer_BuffSend(buff);
 if(NO_ERR != err)
 {Free_KingDaServerBuff(buff);} 
 return err;
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
SystemErrName UpdatAppconMeasureValue_ToKingDaServer(unsigned char ServerNo,
                                                     KingDaServer_CmdAckFunc AckFunc)
{
 SystemErrName err = NO_ERR; 
 unsigned char Number;
 unsigned short length;
 KingDaServerSendBuff_Struct*  buff;
 TimerData_Struct timer;
 
 buff =  Get_KingDaServerBuff ( ); 
 if(P_NULL == buff){return Nothing;}

 Number = 50;
 if(1 == ServerNo)
 {AppconMeasureValue_ToKingDaServer1(&Number, (unsigned char*)&(buff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Datas));}
 else if(2 == ServerNo)
 {AppconMeasureValue_ToKingDaServer2(&Number, (unsigned char*)&(buff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Datas));}
 else
 {
   Free_KingDaServerBuff(buff);
   return NoData;
 }
 if(0 == Number)
 {
   Free_KingDaServerBuff(buff);
   return NoData;
 }
 
 buff->CCR.Bit.AckEn = 1;
 buff->AckFunc =  AckFunc;
 
 buff->buff.head.Sync = KingDaServerSync_ToServer;
 buff->buff.head.Cmd = MeasureValue_UpLoad;
 get_SystemTimer (&timer);
 if(timer.Year < 2010)
 {timer.Year = 2010;}
  if((timer.Month < 1)||(timer.Month > 12))
 {timer.Month = 1;}
  if((timer.Day < 1)||(timer.Day > 31))
 {timer.Day = 1;}
  if(timer.Hour > 23)
 {timer.Hour = 0;}
  if(timer.Minute > 59)
 {timer.Minute = 0;}
   if(timer.Second > 59)
 {timer.Second = 0;}
 buff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Time.Year = timer.Year- 2000;
 buff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Time.Month = timer.Month;
 buff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Time.Day = timer.Day;
 buff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Time.Hour = timer.Hour;
 buff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Time.Minute = timer.Minute;
 buff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Time.Second = timer.Second;
 buff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Concentrator_ID[0] = Concentrator.id[0];
 buff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Concentrator_ID[1] = Concentrator.id[1];
 buff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Concentrator_ID[2] = Concentrator.id[2];
 buff->buff.pram.ReadALLMeasureValue_Send.RepeatersNet_ID[0] = 0;
 buff->buff.pram.ReadALLMeasureValue_Send.RepeatersNet_ID[1] = 0;
 buff->buff.pram.ReadALLMeasureValue_Send.RepeatersNet_ID[2] = 0;
 buff->buff.pram.ReadALLMeasureValue_Send.Repeaters_ID = 0;
 length = Number;
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[3] = length %10;
 length /= 10;
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[2] = length %10;
 length /= 10;
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[1] = length %10;
 length /= 10;
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[0] = length %10;
 
 length = Number * sizeof (NodeDatas_APPCON_Struct);
 length += 17;
 buff->buff.head.Length_L = length;
 buff->buff.head.Length_H = length >> 8;
 buff->TargetPort = getServerPort(ServerNo); 
 err = KingDaServer_BuffSend(buff);
 if(NO_ERR != err)
 {Free_KingDaServerBuff(buff);} 
 return err;
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
SystemErrName ReadALLMeasureValue_ToKingDaServer_ACK(unsigned char Repeaters_NO, 
                                                     CommPortNumber_Type Port,
                                                     KingDaServer_CmdAckFunc AckFunc)
{
 SystemErrName err = NO_ERR; 
 unsigned short length;
 KingDaServerSendBuff_Struct*  buff; 
 Repeaters_DescrType repeaters;
 
 buff =  Get_KingDaServerBuff ( ); 
 if(P_NULL == buff){return Nothing;}
 
 buff->CCR.Bit.AckEn = 1;
 buff->AckFunc =  AckFunc;
 
 buff->buff.head.Sync = KingDaServerSync_ToServer;
 buff->buff.head.Cmd = ReadALLMeasureValue;
   
 buff->buff.pram.ReadALLMeasureValue_Send.Concentrator_ID[0] = Concentrator.id[0];
 buff->buff.pram.ReadALLMeasureValue_Send.Concentrator_ID[1] = Concentrator.id[1];
 buff->buff.pram.ReadALLMeasureValue_Send.Concentrator_ID[2] = Concentrator.id[2];
   
 buff->buff.pram.ReadALLMeasureValue_Send.RepeatersNet_ID[0] = 0;
 buff->buff.pram.ReadALLMeasureValue_Send.RepeatersNet_ID[1] = 0;
 buff->buff.pram.ReadALLMeasureValue_Send.RepeatersNet_ID[2] = 0;
   
 buff->buff.pram.ReadALLMeasureValue_Send.Repeaters_ID = Repeaters_NO;
 
 repeaters.ID =  Repeaters_NO;
 MeasureValue_ToKingDaServer(&repeaters, buff->buff.pram.ReadALLMeasureValue_Send.Data);
   
 buff->buff.pram.ReadALLMeasureValue_Send.RepeatersVoltage = repeaters.Voltage;
 buff->buff.pram.ReadALLMeasureValue_Send.DIO[0] = 0xAA;
 buff->buff.pram.ReadALLMeasureValue_Send.DIO[1] = 0xAA;
   
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[0] = repeaters.CH_NO[0];
 length = buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[0];
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[1] = repeaters.CH_NO[1];
 length += buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[1];
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[2] = repeaters.CH_NO[2];
 length += buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[2];
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[3] = repeaters.CH_NO[3];
 length += buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[3];

 length <<= 1; 
 length += 14;
 buff->buff.head.Length_L = length;
 buff->buff.head.Length_H = length >> 8;

 buff->TargetPort = Port;
 err = KingDaServer_BuffSend(buff);
 if(NO_ERR != err)
 {Free_KingDaServerBuff(buff);} 
 return err;
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
/*
SystemErrName ReadAllAppconMeasureValue_ToKingDaServer_ACK(unsigned char Repeaters_NO, 
                                                     CommPortNumber_Type Port,
                                                     KingDaServer_CmdAckFunc AckFunc)
{
 SystemErrName err = NO_ERR;
 unsigned char Number = 0; 
 unsigned short length;
 KingDaServerSendBuff_Struct*  buff; 
 TimerData_Struct timer;
 
 buff =  GetIdleBuff ( ); 
 if(P_NULL == buff){return Nothing;}
 Number = 20;
 AppconMeasureValue_ToKingDaServer( &Number, (unsigned char*)&(buff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Datas[0]));

 if(0 == Number)
 {
  FreeBuff(buff); 
  rset_NodeValueRecord_Appcon ( );
  return NoData;
 }
 buff->CCR.Bit.AckEn = 1;
 buff->AckFunc =  AckFunc;
 buff->buff.head.Sync = KingDaServerSync_ToServer;
 buff->buff.head.Cmd = ReadALLMeasureValue;
 
 get_SystemTimer (&timer);
 buff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Time.Year = timer.Year - 2000;
 buff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Time.Month = timer.Month;
 buff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Time.Day = timer.Day;
 buff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Time.Hour = timer.Hour;
 buff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Time.Minute = timer.Minute;
 buff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Time.Second = timer.Second;

 buff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Concentrator_ID[0] = Concentrator.id[0];
 buff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Concentrator_ID[1] = Concentrator.id[1];
 buff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Concentrator_ID[2] = Concentrator.id[2];
   
 buff->buff.pram.ReadALLMeasureValue_Send.RepeatersNet_ID[0] = 0;
 buff->buff.pram.ReadALLMeasureValue_Send.RepeatersNet_ID[1] = 0;
 buff->buff.pram.ReadALLMeasureValue_Send.RepeatersNet_ID[2] = 0;
 buff->buff.pram.ReadALLMeasureValue_Send.Repeaters_ID = 0;
 length = Number;
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[3] = length %10;
 length /= 10;
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[2] = length %10;
 length /= 10;
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[1] = length %10;
 length /= 10;
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[0] = length %10;
 
 length = Number * sizeof (NodeDatas_APPCON_Struct);
 length += 17 ;
 buff->buff.head.Length_L = length;
 buff->buff.head.Length_H = length >> 8;

 buff->TargetPort = Port;
 err = KingDaServer_BuffSend(buff);
 if(NO_ERR != err)
 {FreeBuff(buff);} 
 return err;
}*/
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        // 
Others:         // 
*********************************************************************************/
//unsigned char getServerSate(void)
//{
//  return ServerState ;
//}
       
/******************************************END********************************************************/

