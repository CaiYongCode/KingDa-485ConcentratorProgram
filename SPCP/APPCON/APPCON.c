/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include ".\APPCON.h"
#include "..\..\Projects\inc\Common_API.h"
#include "..\..\Projects\inc\SofTimer.h"
/*********************************************************************************
常量定义区
*********************************************************************************/
//AT 指令列表
const char*  const CmdNameList_APPCON[] = {
 "DELNET\r\n",     // 删除网络命令。删除模块，路由器数据
 "CMD ",        // 上位机与无线模块模块通讯命令
 "TST ",        // 测试无线模块命令和控阀命令
 "RDNODE",     // 读网络存在的无线表模块ID 的命令
 "RDROUTER\r\n",   // 读网络存在的无线路由模块ID 的命令
 "FREQ",       // 读写频率参数命令
 "NETID",      // 读写网络ID 命令
 "BATCH\r\n",      // 组网并抄表命令 
 "STATUS\r\n",     // 状态查询命令
 "RATE",      // 读写集中器串口速率命令
 "MRATE",     // 读写无线模块串口速率参数命令
 "VER\r\n",       // 集中器软硬件版本命令
 "HELP\r\n",      // 帮助命令
 "TXD",       // 集中器与掌机通讯命令
 "SETID",     // 集中器无线设置ID 参数命令  
};
const char*  const CmdAckList_APPCON[CmdAckListNumber] = {
 "OK\r\n",     //  0
 "ANS ",       //  1
 "NO ANS",     //  2
 "BUSY\r\n",   //  3
 "FREE\r\n",   //  4
 
 "ERR 1\r\n",  //  5   
 "ERR 2\r\n",  //  6  
 "ERR 3\r\n",   // 7
 "ERR 4\r\n",  //  8 
 
 "TOTAL ",      // 9
 "OK ",     //  0
};
/*********************************************************************************
公共变量定义区
*********************************************************************************/

/*********************************************************************************
外部变量声明区
*********************************************************************************/

/*********************************************************************************
私有变量定义区
*********************************************************************************/
static APPCON_OutPutBuff_Struct   APPCON_OutPutBuff[APPCON_OutPutBuff_Number];
static struct{
 PortStatus                       Status; 
 APPCON_OutPutBuff_Struct*       LastCmd;
 SofTimer_Struct*               WaitTime;   //等待应答计时器 应答超时则 取消等待
}APPCONS_Module; 
/*********************************************************************************
测试变量定义区
*********************************************************************************/

/*********************************************************************************
内部函数定义区
*********************************************************************************/
static APPCON_OutPutBuff_Struct* getSendBuff(void);
//static void freeSendBuff(void* frame );
static void StopAutoRetrans (APPCON_OutPutBuff_Struct* OutPut);
static void SetModuleToIdle (void);
static SystemErrName SendBuff(APPCON_OutPutBuff_Struct* p);
static void WaitTimeOut(void *p_tmr, void *p_arg);
/*********************************************************************************
功能代码定义区
*********************************************************************************/
/*********************************************************************************
 Function:      //
 Description:   //协议初始化
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void APPCON_SPCP_init(void)
{
 unsigned char itemp = 0;
 while (itemp < APPCON_OutPutBuff_Number)
 {
   APPCON_OutPutBuff[itemp ++].CSR.Bit.idle = 1;
 }
 
 APPCONS_Module.Status = OFF; 
 APPCONS_Module.LastCmd = P_NULL;
 APPCONS_Module.WaitTime = SofTimerCreate ( 30,                   //周期
                                            TimerTB_1S,           //时基
                                            SofTimer_PERIODIC,    //模式
                                            WaitTimeOut,          //回调
                                            P_NULL);              //回调参数
}
/*********************************************************************************
 Function:      //
 Description:   // 模块输入数据解析
Input:          // 
                // 
                //
                //   
 Output:        //
 Return:        //
 Others:        //              
*********************************************************************************/
void APPCON_InPut(void *pbuff, unsigned length,CommPortNumber_Type port)
{
  DataBuffSpec_Struct DataBuff;
  DataBuff.pbuff = pbuff;
  DataBuff.length = length;
  
  Decode_APPCON(&DataBuff);
}
/*********************************************************************************
 Function:      //
 Description:   // 模块输入数据解析
Input:          // 
                // 
                //
                //   
 Output:        //
 Return:        //
 Others:        //              
*********************************************************************************/
void Decode_APPCON(DataBuffSpec_Struct *Data)
{  
 unsigned char  Number;
 APPCON_AckType   AckType = OK_APPCON;
 APPCON_OutPutBuff_Struct*     Cmd;
 
 if(P_NULL == APPCONS_Module.LastCmd)
 {return;}
 else
 {Cmd = APPCONS_Module.LastCmd;}
 while (1)
 {
  if(IsLETTER(*(Data->pbuff)) 
     ||Isletter(*(Data->pbuff)))
  {break;}
  if(0 == --(Data->length)){return;}
  Data->pbuff ++;
 }
 //接收内容  比对识别
 Number = MatchinString (&(Data->pbuff),
                         (const char**)&CmdAckList_APPCON,
                         CmdAckListNumber);
 switch( Number )
 {
   case 0:  //"OK\r\n"
     {
       AckType = OK_APPCON;
       Data->pbuff -=4; 
       break;
     }
   case 1:  //"ANS " 
     {
       AckType = Ans_APPCON;
       Data->length -= 4;
       break;
     }
   case 2:  //"NO ANS",    
     {
       AckType = NoAns_APPCON;
       Data->pbuff -= 6;
       break;
     }
   case 3:    //"BUSY\r\n",   //  3
     {
       AckType = BUSY_APPCON;
       break;
     }
   case 4:    //"FREE\r\n",   //  4
     {
       AckType = FREE_APPCON;
       break;
     }
   case 5:   //"ERR1\r\n",  
     {
       AckType = Err1_APPCON;
       Data->pbuff -=7; 
       break;
     }
   case 6:   //"ERR2\r\n",  
     {
       AckType = Err2_APPCON;
       Data->pbuff -=7; 
       break;
     }
   case 7:   //"ERR3\r\n"    
     {
       AckType = Err3_APPCON;
       Data->pbuff -=7; 
       break;
     }
   case 8:   //"ERR4\r\n",  
     {
       AckType = Err4_APPCON;
       Data->pbuff -=7; 
       break;
     }
  case 9:   //"TOTAL ",  
     {
       AckType = TOTAL_APPCON;
       Data->length -= 6; 
       break;
     }
  case 10:  //"OK "
     {
       AckType = OK_APPCON;
       Data->length -= 3; 
       break;
     }   
  default :
   {
     if(0 == Cmd->CSR.Bit.ack1)
     {return ;}
     else
     {AckType = AnsDatas_APPCON;}
   }
 }
 switch(Cmd->Cmd)
 { 
  //直接 集中操作命令组 
  case DELNET:    //删除网络命令。删除模块，路由器数据
  //case RDNODE:    //读网络存在的无线表模块ID 的命令
  case RDROUTER:  //读网络存在的无线路由模块ID 的命令
  case FREQ:      //读写频率参数命令
  case NETID:     //读写网络ID 命令
  case BATCH:     //组网即群表命令 
  case STATUS:    //状态查询命令
  case RATE:      //读写集中器串口速率命令
  case VER:       //集中器软硬件版本命令
  case HELP:     //帮助命令
  case TXD:      //集中器与掌机通讯命令
  case SETID:    //集中器无线设置ID 参数命令 
    {
      SetModuleToIdle( );
      if(P_NULL != APPCONS_Module.LastCmd->AckFunc)
      {
        APPCONS_Module.LastCmd->AckFunc(APPCONS_Module.LastCmd,
                                        AckType,
                                        Data);
      }
      //freeSendBuff_APPCON(Cmd);
      break;
    }
  //直接节点表计操作命令组
  case CMD:    //上位机与无线模块模块通讯命令  实时抄表
  case TST:    //测试无线模块命令和控阀命令
  case MRATE:  //读写无线模块串口速率参数命令
  case RDNODE:    //读网络存在的无线表模块ID 的命令  
    {
      if(((0x01 == Cmd->CSR.Bit.ack1)&&(AnsDatas_APPCON == AckType))
         ||(TOTAL_APPCON == AckType))
      {
        SetModuleToIdle( );
        if(P_NULL != APPCONS_Module.LastCmd->AckFunc)  
        {APPCONS_Module.LastCmd->AckFunc(APPCONS_Module.LastCmd,AckType,Data);}
        SofTimerRset(APPCONS_Module.WaitTime);
      }
      else if(OK_APPCON == AckType)  //集中器以正确收到命令
      {
        Cmd->CSR.Bit.ack1 = 0x01;
        StopAutoRetrans (Cmd);
        SofTimerRset(APPCONS_Module.WaitTime);
        SofTimer_ChangePeriod (APPCONS_Module.WaitTime,   //操纵的定时器
                               30,                        //新的周期
                               1);                        //是否复位定时器 1:重新计时  0:继续计时
        SofTimerStart(APPCONS_Module.WaitTime);
      }  
      else if((Err1_APPCON == AckType)
              ||(Err2_APPCON == AckType) 
              ||(Err3_APPCON == AckType)
              ||(Ans_APPCON == AckType)
              ||(NoAns_APPCON == AckType))
      {
       SetModuleToIdle( );
       if(P_NULL != APPCONS_Module.LastCmd->AckFunc)  
       {
         APPCONS_Module.LastCmd->AckFunc(APPCONS_Module.LastCmd,
                                         AckType,
                                         Data);
       }
       else
       {freeSendBuff_APPCON(Cmd);}
       //freeSendBuff_APPCON(Cmd);
      }
      break;
    }
  default :
    {
     SetModuleToIdle( ); 
     freeSendBuff_APPCON(Cmd); 
     break; 
    }
 }   
}
/*********************************************************************************
 Function:      //
 Description:   //删除网络命令。删除模块，路由器数据
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName APPCON_DELNET_SendFunc(CommPortNumber_Type Source, 
                                     void* AckFuncParam,
                                      APPCON_AckFunc AckFunc)
{
 SystemErrName ErrName;  
 APPCON_OutPutBuff_Struct*  Buff;
 Buff = getSendBuff( ); 
 if(P_NULL == Buff)
 {return PortBusy;}
 Buff->SourcePort = Source; 
 Buff->AckFuncParam = AckFuncParam;
 Buff->AckFunc = AckFunc; 
 Buff->Cmd = DELNET;
 AddString(Buff->Buff,(unsigned char*) CmdNameList_APPCON[DELNET]);
 
  ErrName = SendBuff(Buff);
 if(NO_ERR != ErrName)
 {freeSendBuff_APPCON(Buff);}
 return NO_ERR;
 //return (SendBuff(Buff)); 
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
SystemErrName  APPCON_CMD_SendFunc (CommPortNumber_Type Source,
                                    void* id,
                                    void* AckFuncParam,
                                    APPCON_AckFunc  AckFunc)
{
 SystemErrName ErrName;  
 unsigned char *p;
 APPCON_OutPutBuff_Struct*  Buff;
 Buff = getSendBuff( ); 
 if(P_NULL == Buff)
 {return PortBusy;}
 Buff->SourcePort = Source; 
 Buff->AckFunc = AckFunc;
 Buff->Cmd = CMD;
 p = AddString(Buff->Buff,(unsigned char*)CmdNameList_APPCON[CMD]);
 CopyBuff (p,id, 8);
 p = AddString(p,"\r\n");
 
  ErrName = SendBuff(Buff);
 if(NO_ERR != ErrName)
 {freeSendBuff_APPCON(Buff);}
 return NO_ERR;
 //return (SendBuff(Buff)); 
}
/*********************************************************************************
 Function:      //
 Description:   //测试无线模块命令和控阀命令
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName  APPCON_TST_SendFunc(CommPortNumber_Type Source,
                                   unsigned char *id,
                                   unsigned char AnsNumber,
                                   void* AckFuncParam,
                                   APPCON_AckFunc  AckFunc)
{
 SystemErrName ErrName;  
 unsigned char *p;
 APPCON_OutPutBuff_Struct*  Buff;
 Buff = getSendBuff( ); 
 if(P_NULL == Buff)
 {return PortBusy;}
 Buff->SourcePort = Source; 
 Buff->AckFuncParam = AckFuncParam;
 Buff->AckFunc = AckFunc;
 Buff->Cmd = TST;
 p = AddString(Buff->Buff,(unsigned char*)CmdNameList_APPCON[TST]);
 CopyBuff (p,id, 8);
 p += 8;
 AddString(p," ");
 p = AddIntToString(p,AnsNumber);
 p = AddString(p,"\r\n");
 
  ErrName = SendBuff(Buff);
 if(NO_ERR != ErrName)
 {freeSendBuff_APPCON(Buff);}
 return NO_ERR;
 //return (SendBuff(Buff)); 
}                     
/*********************************************************************************
 Function:      //
 Description:   //无线模块控阀命令 
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName  APPCON_ValveCmd_SendFunc(CommPortNumber_Type Source,
                                        void *id,
                                        ValveCmd_APPCON  cmd,
                                        void* AckFuncParam,
                                        APPCON_AckFunc  AckFunc)
{
 SystemErrName ErrName;  
 unsigned char *p;
 APPCON_OutPutBuff_Struct*  Buff;
 Buff = getSendBuff( ); 
 if(P_NULL == Buff)
 {return PortBusy;}
 Buff->SourcePort = Source; 
 Buff->AckFuncParam = AckFuncParam;
 Buff->AckFunc = AckFunc;
 Buff->Cmd = TST;
 p = AddString(Buff->Buff,(unsigned char*)CmdNameList_APPCON[TST]);
 CopyBuff (p,id, 8);
 p += 8;
 AddString(p," ");
 p = AddIntToString(p,cmd);
 p = AddString(p,"\r\n");
 
  ErrName = SendBuff(Buff);
 if(NO_ERR != ErrName)
 {freeSendBuff_APPCON(Buff);}
 return NO_ERR;
 //return (SendBuff(Buff)); 
}
/*********************************************************************************
 Function:      //
 Description:   //读网络存在的无线表模块ID 的命令
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName  APPCON_RDNODE_SendFunc(CommPortNumber_Type Source,
                                      void* id,
                                      unsigned char Number,
                                      void* AckFuncParam,
                                      APPCON_AckFunc AckFunc)
{
 SystemErrName ErrName;  
 unsigned char *p;
 APPCON_OutPutBuff_Struct*  Buff;
 Buff = getSendBuff( ); 
 if(P_NULL == Buff)
 {return PortBusy;}
 Buff->SourcePort = Source; 
 Buff->AckFuncParam = AckFuncParam;
 Buff->AckFunc = AckFunc;
 Buff->Cmd = RDNODE;
 p = AddString(Buff->Buff,(unsigned char*)CmdNameList_APPCON[RDNODE]);
 if(P_NULL != id)
 {
  p = AddString(p," "); 
  CopyBuff (p,id, 8);
  p += 8;
  p = AddString(p," "); 
  if(0 != Number)
  p = AddIntToString(p,Number);
 }
 p = AddString(p,"\r\n");
 
  ErrName = SendBuff(Buff);
 if(NO_ERR != ErrName)
 {freeSendBuff_APPCON(Buff);}
 return NO_ERR;
 ///return (SendBuff(Buff)); 
}/*********************************************************************************
 Function:      //
 Description:   //读网络存在的无线路由模块ID 的命令
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName  APPCON_RDROUTER_SendFunc(CommPortNumber_Type Source,
                                        //unsigned char *id,
                                        unsigned char Number,
                                        void* AckFuncParam,
                                        APPCON_AckFunc AckFunc)
{
 SystemErrName ErrName;  
 //unsigned char *p;
 APPCON_OutPutBuff_Struct*  Buff;
 Buff = getSendBuff( ); 
 if(P_NULL == Buff)
 {return PortBusy;}
 Buff->SourcePort = Source; 
 Buff->AckFuncParam = AckFuncParam;
 Buff->AckFunc = AckFunc;
 Buff->Cmd = RDROUTER;
 AddString(Buff->Buff,(unsigned char*)CmdNameList_APPCON[RDROUTER]);
 //if(P_NULL != id)
 //{
 // CopyBuff (p,id, 8);
  //p += 8;
 // AddString(p," ");
 // p = AddIntToString(p,Number);
 //}
 //AddString(Buff->Buff,"\r\n");
 
  ErrName = SendBuff(Buff);
 if(NO_ERR != ErrName)
 {freeSendBuff_APPCON(Buff);}
 return NO_ERR;
 ///return (SendBuff(Buff));  
}
/*********************************************************************************
 Function:      //
 Description:   //读写频率参数命令
 Input:         //dir   0x00： 读取频率参数   0x01-0xff：写入频率参数
                //f 频率值
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName  APPCON_FREQ_SendFunc (CommPortNumber_Type Source,
                                     unsigned char* freq,
                                     void* AckFuncParam,
                                     APPCON_AckFunc AckFunc)
{
 SystemErrName ErrName;  
 unsigned char* p;
 APPCON_OutPutBuff_Struct*  Buff;
 Buff = getSendBuff( ); 
 if(P_NULL == Buff)
 {return PortBusy;}
 Buff->SourcePort = Source; 
 Buff->AckFuncParam = AckFuncParam;
 Buff->AckFunc = AckFunc;
 Buff->Cmd = FREQ;
 p = AddString(Buff->Buff,(unsigned char*)CmdNameList_APPCON[FREQ]);
 if(P_NULL != freq )
 {
   *p++ = ' ';
   *p++ = *freq++;
   *p++ = *freq++;
   *p++ = *freq++;
   *p++ = *freq++;
   *p++ = *freq++;
   *p++ = *freq++;
   //p = AddIntToString(p,f);
 }
 *p++ = '\r';
 *p = '\n';
 
  ErrName = SendBuff(Buff);
 if(NO_ERR != ErrName)
 {freeSendBuff_APPCON(Buff);}
 return NO_ERR;
 //return (SendBuff(Buff));
}
/*********************************************************************************
 Function:      //
 Description:   //读写网络ID 命令
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName  APPCON_NETID_SendFunc (CommPortNumber_Type Source,
                                      unsigned char* id,
                                      void* AckFuncParam,
                                      APPCON_AckFunc AckFunc)
{
 SystemErrName ErrName;  
 unsigned char* p;
 APPCON_OutPutBuff_Struct*  Buff;
 Buff = getSendBuff( ); 
 if(P_NULL == Buff)
 {return PortBusy;}
 Buff->SourcePort = Source; 
 Buff->AckFuncParam = AckFuncParam;
 Buff->AckFunc = AckFunc;
 Buff->Cmd = NETID;
 p = AddString(Buff->Buff,(unsigned char*)CmdNameList_APPCON[NETID]);
 if(P_NULL != id)
 {
   *p++ = ' ';
   //p = AddBinToAsciiString(p,*id);
   *p++ = *id++;
   *p++ = *id++;
   *p++ = *id++;
   *p++ = *id++;
 
 }
 *p++ = '\r';
 *p = '\n';
  ErrName = SendBuff(Buff);
 if(NO_ERR != ErrName)
 {freeSendBuff_APPCON(Buff);}
 return NO_ERR;
 //return (SendBuff(Buff));
}
/*********************************************************************************
 Function:      //
 Description:   //组网并抄表命令 
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName  APPCON_BATCH_SendFunc (CommPortNumber_Type Source,
                                      void* AckFuncParam,
                                      APPCON_AckFunc AckFunc)
{
 SystemErrName ErrName; 
 APPCON_OutPutBuff_Struct*  Buff;
 Buff = getSendBuff( ); 
 if(P_NULL == Buff)
 {return PortBusy;}
 Buff->SourcePort = Source; 
 Buff->AckFuncParam = AckFuncParam;
 Buff->AckFunc = AckFunc;
 Buff->Cmd = BATCH;
 AddString(Buff->Buff,(unsigned char*)CmdNameList_APPCON[BATCH]);
 ErrName = SendBuff(Buff);
 if(NO_ERR != ErrName)
 {freeSendBuff_APPCON(Buff);}
 return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //状态查询命令
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName  APPCON_STATUS_SendFunc (CommPortNumber_Type Source,
                                       void* AckFuncParam,
                                       APPCON_AckFunc AckFunc)
{
 SystemErrName ErrName;  
 APPCON_OutPutBuff_Struct*  Buff;
 Buff = getSendBuff( ); 
 if(P_NULL == Buff)
 {return PortBusy;}
 Buff->SourcePort = Source;                                     
 Buff->AckFuncParam = AckFuncParam;
 Buff->AckFunc = AckFunc;
 Buff->Cmd = STATUS;
 AddString(Buff->Buff,(unsigned char*)CmdNameList_APPCON[STATUS]);
 
 ErrName = SendBuff(Buff);
 if(NO_ERR != ErrName)
 {freeSendBuff_APPCON(Buff);}
 return NO_ERR;
 
 //return (SendBuff(Buff)); 
}
/*********************************************************************************
 Function:      //
 Description:   //读写集中器串口速率命令
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName  APPCON_RATE_SendFunc (CommPortNumber_Type Source,
                                     BaudRate_APPCON rate,
                                     void* AckFuncParam,
                                     APPCON_AckFunc AckFunc) 
{
 SystemErrName ErrName;  
 unsigned char* p; 
 APPCON_OutPutBuff_Struct*  Buff;
 if(rate < _9600bps)
 {return IllegalParam;}
 Buff = getSendBuff( ); 
 if(P_NULL == Buff)
 {return PortBusy;}
 Buff->SourcePort = Source; 
 Buff->AckFuncParam = AckFuncParam;
 Buff->AckFunc = AckFunc;
 Buff->Cmd = RATE;
 p = AddString(Buff->Buff,(unsigned char*)CmdNameList_APPCON[RATE]);
 *p ++ = ' ';
 p = AddIntToString(p,(unsigned long)rate);
 *p++ = '\r';
 *p = '\n';
 
 ErrName = SendBuff(Buff);
 if(NO_ERR != ErrName)
 {freeSendBuff_APPCON(Buff);}
 return NO_ERR;
 //return (SendBuff(Buff));
}
/*********************************************************************************
 Function:      //
 Description:   //读写无线模块串口速率参数命令
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName  APPCON_MRATE_SendFunc (CommPortNumber_Type Source,
                                      unsigned char dir, 
                                      BaudRate_APPCON rate,
                                      void* AckFuncParam,
                                      APPCON_AckFunc AckFunc) 
{
 SystemErrName ErrName;  
 unsigned char* p;
 APPCON_OutPutBuff_Struct*  Buff;
 if(rate > _57600bps)
 {return IllegalParam;}
 Buff = getSendBuff( ); 
 if(P_NULL == Buff)
 {return PortBusy;}
 Buff->SourcePort = Source; 
 Buff->AckFuncParam = AckFuncParam;
 Buff->AckFunc = AckFunc;
 Buff->Cmd = MRATE;
 p = AddString(Buff->Buff,(unsigned char*)CmdNameList_APPCON[MRATE]);
 if(0 != dir )
 {
   *p++ = ' ';
   p = AddIntToString(p,rate);
 }
 *p++ = '\r';
 *p = '\n';
 
  ErrName = SendBuff(Buff);
 if(NO_ERR != ErrName)
 {freeSendBuff_APPCON(Buff);}
 return NO_ERR;
 ///return (SendBuff(Buff));
}
/*********************************************************************************
 Function:      //
 Description:   //集中器软硬件版本命令
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName  APPCON_VER_SendFunc (CommPortNumber_Type Source,
                                    void* AckFuncParam,
                                    APPCON_AckFunc AckFunc ) 
{
 SystemErrName ErrName;  
 APPCON_OutPutBuff_Struct*  Buff;
 Buff = getSendBuff( ); 
 if(P_NULL == Buff)
 {return PortBusy;}
 Buff->SourcePort = Source; 
 Buff->AckFuncParam = AckFuncParam;
 Buff->AckFunc = AckFunc;
 Buff->Cmd = VER;
 AddString(Buff->Buff,(unsigned char*)CmdNameList_APPCON[VER]);
 
  ErrName = SendBuff(Buff);
 if(NO_ERR != ErrName)
 {freeSendBuff_APPCON(Buff);}
 return NO_ERR;
 //return (SendBuff(Buff));
}
/*********************************************************************************
 Function:      //
 Description:   //帮助命令
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName  APPCON_HELP_SendFunc (CommPortNumber_Type Source,
                                     void* AckFuncParam,
                                     APPCON_AckFunc AckFunc)
{
 SystemErrName ErrName;  
 APPCON_OutPutBuff_Struct*  Buff;
 Buff = getSendBuff( ); 
 if(P_NULL == Buff)
 {return PortBusy;}
 Buff->SourcePort = Source; 
 Buff->AckFuncParam = AckFuncParam;
 Buff->AckFunc = AckFunc;
 Buff->Cmd = HELP;
 AddString(Buff->Buff,(unsigned char*)CmdNameList_APPCON[HELP]);
 
  ErrName = SendBuff(Buff);
 if(NO_ERR != ErrName)
 {freeSendBuff_APPCON(Buff);}
 return NO_ERR;
 //return (SendBuff(Buff));  
}
/*********************************************************************************
 Function:      //
 Description:   //集中器与掌机通讯命令
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName APPCON_TXD_SendFunc (CommPortNumber_Type Source,
                                   void *buff,
                                   unsigned char length,
                                   void* AckFuncParam,
                                   APPCON_AckFunc AckFunc) 
{
 SystemErrName ErrName;  
 unsigned char* p; 
 APPCON_OutPutBuff_Struct*  Buff;
 if((length > 180)||(length < 1))
 {return PortBusy;}
 Buff = getSendBuff( ); 
 if(P_NULL == Buff)
 {return PortBusy;}
 Buff->SourcePort = Source; 
 Buff->AckFuncParam = AckFuncParam;
 Buff->AckFunc = AckFunc;
 Buff->Cmd = TXD;
 p = AddString(Buff->Buff,(unsigned char*)CmdNameList_APPCON[TXD]);
 while(length)
 {
  *p++ = ' ';
  p = AddIntToString(p,*((unsigned char*)buff));
  length --;
 }
 *p++ = '\r';
 *p = '\n';
 
  ErrName = SendBuff(Buff);
 if(NO_ERR != ErrName)
 {freeSendBuff_APPCON(Buff);}
 return NO_ERR;
 //return SendBuff(Buff);
}
/*********************************************************************************
 Function:      //
 Description:   //集中器无线设置ID 参数命令
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //SETID _口令(3 字节)_频率(3 字节)_网络 ID(2 字节)↙
*********************************************************************************/
SystemErrName  APPCON_SETID_SendFunc (unsigned long command ,//口令  
                                      unsigned long f,
                                      unsigned short id,
                                      void* AckFuncParam,
                                      APPCON_AckFunc AckFunc)   
{
  SystemErrName err;
  err = CmdValid;
  return err; 
}
/*********************************************************************************
 Function:      //WaitTimeOut
 Description:   //命令应答超时处理函数 取消等待
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static void WaitTimeOut(void *p_tmr, void *p_arg)
{
 SofTimerStop(p_tmr);
 SetModuleToIdle( );
 if(P_NULL == APPCONS_Module.LastCmd->AckFunc)  
 {APPCONS_Module.LastCmd->AckFunc(APPCONS_Module.LastCmd,NoAns_APPCON,P_NULL);}
 freeSendBuff_APPCON(APPCONS_Module.LastCmd);
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
static APPCON_OutPutBuff_Struct* getSendBuff(void)
{
 unsigned char itemp = 0;
 unsigned char itemp1;
 while (itemp < APPCON_OutPutBuff_Number)
 {
   if(1 == APPCON_OutPutBuff[itemp].CSR.Bit.idle)
   {
     APPCON_OutPutBuff[itemp].CSR.Byte = 0x00;
     APPCON_OutPutBuff[itemp].AckFunc = P_NULL;
     APPCON_OutPutBuff[itemp].AckFuncParam = P_NULL;
     itemp1 = 0;
     while(itemp1 < APPCON_OutPutBuffLength)  
     {APPCON_OutPutBuff[itemp].Buff[itemp1++] = '\0';}
     return (&(APPCON_OutPutBuff[itemp]));
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
void freeSendBuff_APPCON(void* frame )
{
 unsigned char itemp = 0;
 while (itemp < APPCON_OutPutBuff_Number)
 {   
  if((frame == &(APPCON_OutPutBuff[itemp]))
      ||(frame == APPCON_OutPutBuff[itemp].Buff)) 
  {
   FreeOutputBuffForData_MXPC (APPCON_OutPutBuff[itemp].Buff);
   APPCON_OutPutBuff[itemp].CSR.Bit.idle = 1;
   APPCON_OutPutBuff[itemp].Cmd = NOP; 
   if(APPCONS_Module.LastCmd == &(APPCON_OutPutBuff[itemp]))
   {
     SofTimerStop(APPCONS_Module.WaitTime);
     APPCONS_Module.Status = Idle;
     APPCONS_Module.LastCmd = P_NULL;
   }
  }
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
static void StopAutoRetrans (APPCON_OutPutBuff_Struct* OutPut)
{
  FreeOutputBuffForData_MXPC (OutPut->Buff);
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
static void SetModuleToIdle (void)
{
 APPCONS_Module.Status = Idle;
 //APPCONS_Module.LastCmd = P_NULL; 
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
static void APPCON_AckTimeOut (void *pbuff)
{
  APPCON_OutPutBuff_Struct*  SendBuff; 
  unsigned long itemp ;
  
  SetModuleToIdle ( );
  if(P_NULL == pbuff)
  {return;}
  SendBuff = (APPCON_OutPutBuff_Struct *) 0 ;
  itemp = (unsigned long)&(SendBuff->Buff);
  pbuff =  (((unsigned char*)pbuff) - itemp);
  SendBuff =  pbuff;
  if(P_NULL != SendBuff->AckFunc)
  {
    SendBuff->AckFunc (SendBuff,NoAck_APPCON, P_NULL); 
  }
  freeSendBuff_APPCON(SendBuff);
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
static SystemErrName SendBuff(APPCON_OutPutBuff_Struct* OutPut)
{
  SystemErrName err;

  //if(0x01 != Concentrator.CSR.Bit.Appcon_EN)
  //{return  Port_OFF;}
  
  if((WaitAck == APPCONS_Module.Status)
     ||(Busy == APPCONS_Module.Status))
  {return  PortBusy;}
  
  OutPut->BuffLength = 0;
  while('\0' != OutPut->Buff[OutPut->BuffLength])
  {OutPut->BuffLength ++;}
  
  err = MXPC_Send_AutoRetrans( Rs485_PortD,            //目的端口
                               (OutPut->Buff),         //数据缓冲区
                               OutPut->BuffLength,     //数据长度
                               2,                      //重传次数
                               10,                     //重传周期数
                               TimerTB_100MS,          //重传周期
                               P_NULL,                 //发送完成回调
                               APPCON_AckTimeOut);     //重发完成回调  
 if(NO_ERR != err)
 {freeSendBuff_APPCON(OutPut);}
 else
 {
  if(APPCONS_Module.LastCmd != OutPut)
  {freeSendBuff_APPCON(APPCONS_Module.LastCmd);}
  APPCONS_Module.LastCmd = OutPut;
  APPCONS_Module.Status = WaitAck;
 }
 return err;
}
/*********************************************************************************
 Function:      //
 Description:   //ASCII码串表示的16进制变换为实数
 Input:         //hex： ASCII码串首地址
                //length：ASCII码串长度
                  width：期望的实数16进制码元数
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
unsigned long  APPCON_AsciiHexToDigit ( unsigned char* hex, 
                                        unsigned char length, 
                                        unsigned char width )   
{
  unsigned long  ToDigit = 0;
  if((0 == width )||(P_NULL == hex))
  {return ToDigit; }
  while(length-- > 0)
  {
   if(IsDigit(*hex))
   {
    ToDigit <<= 4; 
    ToDigit |= *hex - '0';
    width --;
   }
   else if(IsABCDEF(*hex))
   {
    ToDigit <<= 4; 
    ToDigit |= *hex - 'A' + 0x0A; 
    width --;
   }
   else if(Isabcdef(*hex))
   {
    ToDigit <<= 4; 
    ToDigit |= *hex - 'a' + 0x0A; 
    width--;
   }
   if(0 == width )
   {break;}  
   hex++;
  }
  return ToDigit; 
}
/*************************************END****************************************/
