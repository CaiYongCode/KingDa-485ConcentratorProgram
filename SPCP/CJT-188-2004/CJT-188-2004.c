/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include ".\CJT-188-2004.h"
/*********************************************************************************
常量定义区
*********************************************************************************/

/*********************************************************************************
公共变量定义区
*********************************************************************************/
//const  unsigned char *CJT188_MeterID = (unsigned char *)(CJT188_MeterID_AddrInRom + RomStartAddr);
//const  unsigned char Ctr_List[6] = {CTR_ReadData_ASK, CTR_ReadVerNo_ASK,CTR_ReadAddress_ASK, CTR_WriteData_ASK,
 //                                     CTR_WriteAddress_ASK, CTR_WriteOffset_ASK};
/*********************************************************************************
外部变量声明区
*********************************************************************************/
/*********************************************************************************
私有变量定义区
*********************************************************************************/
static CJT188_SER_Type  SER_PV = 0;    //包序列号 
static CJT188_SendBuff_Struct   SendBuff[CJT188_OutPutBuff_Number]; //协议发送反冲区
//static CommPortNumber_Type  CurrentPort = Null_Port; 
/*********************************************************************************
测试变量定义区
*********************************************************************************/

/*********************************************************************************
内部函数定义区
*********************************************************************************/
static SystemErrName CJT188_2004_frameVerify (void *pbuff, 
                                              unsigned char length, 
                                              CJT188_Frame_Struct** frame);

static SystemErrName CTR_ReadData_ACKFunc(void *pbuff);
static SystemErrName CTR_WriteData_ACKFunc(void *pbuff);

static void AckFrame_func( CJT188_Frame_Struct* input);

static void AckFrame_func(CJT188_Frame_Struct* input);


static void freeSendBuff(void* frame );

static SystemErrName CJT188_Transpond(CJT188_Frame_Struct* InData, 
                                      CommPortNumber_Type  SourcePort, 
                                      CommPortNumber_Type  TargetPort);
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
void CJT188_SPCP_init(void)
{
 unsigned char itemp = 0;
 while (itemp < CJT188_OutPutBuff_Number)
 {
   SendBuff[itemp].CSR.Bit.idle = 1;
   itemp ++;
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
SystemErrName CJT188_2004_InPut (void *pbuff, unsigned length, CommPortNumber_Type port)
{
 SystemErrName err = NO_ERR;
 CJT188_Frame_Struct* frame;
 
 err = CJT188_2004_frameVerify(pbuff,length, &frame);
 if(NO_ERR != err) 
 {return err;}

 
 //CurrentPort = port;
 
#if(0 == Locall_Type)  //数据采集器
 if(0 ==frame->Head.CTR.Bit.DIR) //由主站发出的控制帧 
 {
   //在当前
 }
 else //由从站发出的应答帧。
 {AckFrame_func(frame);}
#else //数据传感器
 if(1==frame->CTR.Bit.DIR) //由从站发出的应答帧。
 {return NO_ERR; }
#endif
 
 switch (frame->Head.CTR.Bit.Infr)
 {
  case CJT188_ReadData_CTR:
    { 
      CTR_ReadData_ACKFunc(frame + 1);
      break;
     }
  case CJT188_ReadVerNo_CTR:
    { 
      //CTR_ReadVerNo_ACKFunc(pbuff);
      break; 
    }
  case CJT188_ReadAddress_CTR:
    { 
      //CTR_ReadVerNo_ACKFunc(pbuff);
      break; 
    }
  case CJT188_WriteData_CTR:
    { 
      CTR_WriteData_ACKFunc(frame + 1);
      break; 
    }
  case CJT188_WriteAddress_CTR:
    { 
      //CTR_WriteAddress_ACKFunc(pbuff);
      break; 
    }
  case CJT188_WriteOffset_CTR:
    {
      //CTR_ReadVerNo_ACKFunc(pbuff);
      break; 
    }
 default:
   {err = OperateCmd_err;}
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
 Others:        //
*********************************************************************************/
static SystemErrName CJT188_2004_frameVerify (void *pbuff, 
                                              unsigned char length, 
                                              CJT188_Frame_Struct** frame)
{
  unsigned char itemp ;
  unsigned char itemp2 ;
  unsigned char sum ;
  //CJT188_Frame_Struct* frame;
  unsigned char* p;
  
  itemp = 0;
  while (CJT188_Preamble ==((unsigned char *)pbuff)[itemp])
  {itemp ++;}
  if(itemp < PreambleNumber)
  {return FramePreamble_err;}
  else
  {itemp -= PreambleNumber;}
  
  *frame =(CJT188_Frame_Struct *)(&((unsigned char *)pbuff)[itemp]);
  if(CJT188_StartCharacter != (*frame)->Head.StartCharacter)
  {return FrameSync_err;}
  
  if((*frame)->Head.CTR.Bit.CommFlg) //1— 通 讯 异 常
  {itemp = sizeof (CJT8_188_ErrACK_Struct);}
  else
  {
    itemp = sizeof(CJT188_FrameHeadStruct) + sizeof(CJT188_FrameStern_Struct);
    itemp += (*frame)->Head.Length; 
  }
  if(length < itemp)
  {return Length_err;} 
  
  itemp2 = sizeof(CJT188_FrameHeadStruct) - PreambleNumber;
  itemp2 += (*frame)->Head.Length ;
  sum = 0;
  itemp = 0;
  p = &((*frame)->Head.StartCharacter);
  while(itemp < itemp2)
  {
    sum += p[itemp];
    itemp ++;
  }
  if(sum != p[itemp])
  {return Verify_err;}
  
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
SystemErrName CTR_ReadData_ACKFunc(void *pbuff)
{
 SystemErrName err = NO_ERR;

 CJT188_DataHeadStruct  *input_p;
 input_p = pbuff;
 switch (input_p->DI)
 {
   case CJT188_ReadData_DI :      //计量数据
     {
       //err =  send_MeterData (input_p->SER,&cjt_188_sendbuff);  
       break;
     }
   case CJT188_ReadHistData_1_DI :       //历史计量数据1
   case CJT188_ReadHistData_2_DI :       //历史计量数据2
   case CJT188_ReadHistData_3_DI  :      //历史计量数据3
   case CJT188_ReadHistData_4_DI  :      //历史计量数据4
   case CJT188_ReadHistData_5_DI  :      //历史计量数据5
   case CJT188_ReadHistData_6_DI  :      //历史计量数据6
   case CJT188_ReadHistData_7_DI  :      //历史计量数据7
   case CJT188_ReadHistData_8_DI  :      //历史计量数据8
   case CJT188_ReadHistData_9_DI  :      //历史计量数据9
   case CJT188_ReadHistData_10_DI  :     //历史计量数据10
   case CJT188_ReadHistData_11_DI  :     //历史计量数据11
   case CJT188_ReadHistData_12_DI  :     //历史计量数据12
     {break;}
   case CJT188_ReadPriceList_DI  :       //读价格表
     {break;}
   case CJT188_ReadSelDate_DI  :         //读结算日
      {break;}
   case CJT188_ReadRecordDate_DI  :      //读抄表日
     {break;}
   case CJT188_ReadRMoney_DI  :          //读购入金额
     {break;}
   case CJT188_ReadVerNo_DI  :           //读秘钥版本号
     {break;}
   default :
     {break;}
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
 Others:        //
*********************************************************************************/
static SystemErrName CTR_WriteData_ACKFunc(void *pbuff)
{
 SystemErrName err = NO_ERR; 
 CJT188_DataHeadStruct  *input_p;
 input_p = pbuff;
 
 switch (input_p->DI)
 {
   case CJT188_WriteValveState_DI  :      //计量数据
     {
       //err =  send_Mete rData (input_p->SER,&cjt_188_sendbuff);  
       break;
     }
     {break;}
   default :
     {break;}
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
 Others:        //
*********************************************************************************/
static void addFrameStern(CJT188_Frame_Struct* Frame)
{
  unsigned char sum = 0;
  unsigned char length;
  unsigned char itemp = 0;
  unsigned char *pin;
  CJT188_FrameStern_Struct* Stern;
  
  pin = &(Frame->Head.StartCharacter);
  if(Frame->Head.CTR.Bit.CommFlg)
  {length = sizeof(CJT8_188_ErrACK_Struct);}
  else
  {length = sizeof(CJT188_FrameHeadStruct) + Frame->Head.Length;} 
  length -= (unsigned long)&(((CJT188_Frame_Struct*)(0))->Head.StartCharacter);
  Stern = (CJT188_FrameStern_Struct*)&(pin[length]);
  while(itemp < length)
  {
    sum += pin[itemp];
    itemp ++;
  }
  Stern->CS = sum;
  Stern->Pause = CJT188_Pause;
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
void CJT188_AckTimeOut(void *pbuff)
{
  CJT188_SendBuff_Struct*  SendData; 
  unsigned long itemp ; 
  if(P_NULL == pbuff)
  {return;}
  SendData = (CJT188_SendBuff_Struct *) 0 ;
  itemp = (unsigned long)&(SendData->buff);
  pbuff =  (((unsigned char*)pbuff) - itemp);
  SendData =  pbuff;
  if(P_NULL != SendData->AckFunc)
  {
    SendData->AckFunc (SendData, P_NULL);
    freeSendBuff(SendData);
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
static SystemErrName CJT188_Send(CJT188_SendBuff_Struct *pbuff)
{
 unsigned char retrans; 
 unsigned short length;
 
 if(pbuff->buff.Head.CTR.Bit.CommFlg)   //1— 通 讯 异 常
 {length = sizeof(CJT8_188_ErrACK_Struct);}
 else
 {
  length = sizeof(CJT188_FrameHeadStruct) + sizeof(CJT188_FrameStern_Struct);
  length += pbuff->buff.Head.Length; 
 }
 if(pbuff->buff.Head.CTR.Bit.DIR)
 {retrans = 0;}
 else
 {retrans = 2;}
  
 MXPC_Send_AutoRetrans (pbuff->TargetPort,      //目的端口
                        &(pbuff->buff),         //数据缓冲区
                        length,                 //数据长度
                        retrans,                //重传次数
                        5,                      //重传周期数
                        TimerTB_100MS,          //重传周期
                        pbuff->SendOK,          //发送完成回调
                        CJT188_AckTimeOut);     //重发完成回调  
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
CJT188_SER_Type getSER (void)
{
  SER_PV ++;
  return SER_PV;
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
static void Frame_init(CJT188_Frame_Struct* frame)
{
 if(P_NULL != frame )
 {
  frame->Head.Preamble[0] = CJT188_Preamble;
  frame->Head.Preamble[1] = CJT188_Preamble;
  frame->Head.StartCharacter = CJT188_StartCharacter;
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
static CJT188_SendBuff_Struct* getSendBuff(void)
{
 unsigned char itemp = 0;
 while (itemp < CJT188_OutPutBuff_Number)
 {
   if(1 == SendBuff[itemp].CSR.Bit.idle)
   {
     SendBuff[itemp].CSR.Bit.idle = 0;
     SendBuff[itemp].AckFunc = P_NULL;
     SendBuff[itemp].SendOK = P_NULL;
     SendBuff[itemp].SourcePort = Null_Port;
     SendBuff[itemp].TargetPort = Null_Port;
     Frame_init(&(SendBuff[itemp].buff));
     return (&(SendBuff[itemp]));
   }
   itemp ++;
 }
 return (P_NULL);
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
static void freeSendBuff(void* frame )
{
 unsigned char itemp = 0;
 while (itemp < CJT188_OutPutBuff_Number)
 {
   if((frame == &(SendBuff[itemp]))
      ||(frame == &(SendBuff[itemp].buff)))
   {
     FreeOutputBuffForData_MXPC (&(SendBuff[itemp].buff));
     SendBuff[itemp].CSR.Bit.idle = 1;
     break;
   }
   itemp ++;
 }
}
#if(0 == Locall_Type)
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName CJT188_SendCmd (CJT188_CmdDeclarator* Cmd,
                              CommPortNumber_Type TargetPort,
                              CJT188_AckFunc AckFunc)
{
  SystemErrName err;
  CJT188_SendBuff_Struct*  Cmdbuff;
  Cmdbuff = getSendBuff( );
  if(P_NULL ==Cmdbuff)
  {return Nothing;}
  
  Cmdbuff->AckFunc = AckFunc;
  Cmdbuff->SourcePort = Null_Port;
  Cmdbuff->TargetPort = TargetPort;
  Cmdbuff->buff.Normal.Head.Type = Cmd->Type;
  CopyBuff (&(Cmdbuff->buff.Normal.Head.Addr), 
            &(Cmd->Addr), 
            sizeof(CJT188_AddresStruct));
  Cmdbuff->buff.Normal.Head.CTR.Byte = Cmd->CTR.Byte;
  Cmdbuff->buff.Normal.Head.Length = sizeof(CJT188_DataHeadStruct)+ Cmd->ParamLength;
  Cmdbuff->buff.Normal.DataHead.DI = Cmd->DataDI;  
  Cmdbuff->buff.Normal.DataHead.SER = Cmd->SER; 
  
  CopyBuff (&(Cmdbuff->buff.Normal.Data), 
            &(Cmd->Param), 
            Cmd->ParamLength);
  addFrameStern(&(Cmdbuff->buff));
  
  err = CJT188_Send(Cmdbuff);
  if(NO_ERR != err)
  {freeSendBuff(Cmdbuff);}
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
SystemErrName CJT188_ReadNodeValue (unsigned char  MeterType,
                                    CJT188_AddresStruct* Addr,
                                    unsigned short DI,
                                    CommPortNumber_Type SourcePort,
                                    CommPortNumber_Type TargetPort,
                                    CJT188_AckFunc AckFunc)
{
  CJT188_CmdDeclarator Cmd;
  Cmd.SourcePort = SourcePort;
  Cmd.Type = MeterType;
  CopyBuff (&(Cmd.Addr), Addr, sizeof(CJT188_AddresStruct));
  Cmd.CTR.Byte = 0;
  Cmd.CTR.Bit.Infr = CJT188_ReadData_CTR;
  Cmd.DataDI = DI;
  Cmd.ParamLength = 0;
  return CJT188_SendCmd (&Cmd,TargetPort, AckFunc);
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
SystemErrName CJT188_OperateNodeValve(unsigned char  MeterType,
                                      CJT188_AddresStruct* Addr,
                                      unsigned char  ValveCmd,
                                      CommPortNumber_Type SourcePort,
                                      CommPortNumber_Type TargetPort,
                                      CJT188_AckFunc AckFunc)
{
  CJT188_CmdDeclarator Cmd;
  CJT188_ValveState_Type* Param;  
  
  Cmd.SourcePort = SourcePort;
  Cmd.Type = MeterType;
  CopyBuff (&(Cmd.Addr), Addr, sizeof(CJT188_AddresStruct));
  Cmd.CTR.Byte = 0;
  Cmd.CTR.Bit.Infr = CJT188_WriteData_CTR;
  Cmd.DataDI = CJT188_WriteValveState_DI ;
  Cmd.ParamLength = sizeof(CJT188_ValveState_Type);
  Param = (CJT188_ValveState_Type*)&Cmd.Param;
  if(ValveCmd)
  {*Param = CJT188_ValveON;}
  else
  {*Param = CJT188_ValveOFF;}
  return CJT188_SendCmd (&Cmd,TargetPort, AckFunc);
}
/*********************************************************************************
 Function:      //
 Description:   //应答包处理 (主机端)
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static void AckFrame_func(CJT188_Frame_Struct* input)
{
 unsigned char itemp;
 unsigned char  j;
 itemp = 0;
 while (itemp < CJT188_OutPutBuff_Number)
 {
  if((0 == SendBuff[itemp].CSR.Bit.idle)
      &&(0== SendBuff[itemp].buff.Head.CTR.Bit.DIR)
      &&(SendBuff[itemp].buff.Head.CTR.Bit.Infr == input->Head.CTR.Bit.Infr))
   { 
    j = 0; 
    if((0 ==input->Head.CTR.Bit.CommFlg)
       &&(SendBuff[itemp].buff.Normal.DataHead.SER = input->Normal.DataHead.SER))
    {j = 1;}
    else
    {
     if(SendBuff[itemp].buff.ERR.SER == input->ERR.SER)
     {j = 1;}
    }
    if(1 == j)
    {
      if((input->Head.Addr.MeterID[0] == SendBuff[itemp].buff.Head.Addr.MeterID[0])
         &&(input->Head.Addr.MeterID[1] == SendBuff[itemp].buff.Head.Addr.MeterID[1])
         &&(input->Head.Addr.MeterID[2] == SendBuff[itemp].buff.Head.Addr.MeterID[2])
         &&(input->Head.Addr.MeterID[3] == SendBuff[itemp].buff.Head.Addr.MeterID[3])
         &&(input->Head.Addr.MeterID[4] == SendBuff[itemp].buff.Head.Addr.MeterID[4])
         &&(input->Head.Addr.FactoryID == SendBuff[itemp].buff.Head.Addr.FactoryID) )
      {break;}
      else if((0xAA == SendBuff[itemp].buff.Head.Addr.MeterID[0])
               &&(0xAA == SendBuff[itemp].buff.Head.Addr.MeterID[1])
               &&(0xAA == SendBuff[itemp].buff.Head.Addr.MeterID[2])
               &&(0xAA == SendBuff[itemp].buff.Head.Addr.MeterID[3])
               &&(0xAA == SendBuff[itemp].buff.Head.Addr.MeterID[4])
               &&(0xAAAA == SendBuff[itemp].buff.Head.Addr.FactoryID))
      {break;}
    }
  }
  itemp ++;
 }
 if(P_NULL != SendBuff[itemp].AckFunc)
 {
  SendBuff[itemp].AckFunc(&(SendBuff[itemp]),input);
  freeSendBuff(&(SendBuff[itemp].buff));
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
static void Transpond_AckFunc(CJT188_SendBuff_Struct* SendData, CJT188_Frame_Struct* AckData)
{
  CJT188_Transpond(AckData, Null_Port, SendData->SourcePort);
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
static void Transpond_SendOKFunc  (CommPortNumber_Type port, void *pbuff)
{
  freeSendBuff(pbuff);
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
static SystemErrName CJT188_Transpond(CJT188_Frame_Struct* InData, 
                                      CommPortNumber_Type  SourcePort, 
                                      CommPortNumber_Type  TargetPort)
{
 SystemErrName err; 
 unsigned char length;
 CJT188_SendBuff_Struct* outbuff;
 
 outbuff = getSendBuff( );
 if(P_NULL == outbuff)
 {return Nothing;}
 
 outbuff->TargetPort = TargetPort;
 outbuff->SourcePort = SourcePort;
 
 if(InData->Head.CTR.Bit.CommFlg)   //1— 通 讯 异 常
 {
  length = sizeof(CJT8_188_ErrACK_Struct); 
 }
 else
 {
  length = sizeof(CJT188_FrameHeadStruct) + sizeof(CJT188_FrameStern_Struct);
  length += InData->Head.Length; 
 }
 CopyBuff (&(outbuff->buff), InData, length);
 if(InData->Head.CTR.Bit.DIR)  //1— 由从站发出的应答帧
 {outbuff->SendOK = Transpond_SendOKFunc;}
 else 
 {outbuff->AckFunc = Transpond_AckFunc;}
 
 err = MXPC_Send_AutoRetrans ( outbuff->TargetPort,    //目的端口
                               &(outbuff->buff),       //数据缓冲区
                               length,                 //数据长度
                               0,                      //重传次数
                               5,                      //重传周期数
                               TimerTB_100MS,          //重传周期
                               outbuff->SendOK,        //发送完成回调
                               CJT188_AckTimeOut);     //重发完成回调  
 if(NO_ERR != err)
 {freeSendBuff(&(outbuff->buff));}  
 return err;  
}
#endif
/******************************************END********************************************************/

