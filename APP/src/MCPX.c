/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include ".\MCPX.h"
//#include "..\..\BSP\inc\bsp.h"
#include "..\inc\GPRS.h"
#include "..\..\SPCP\SPCP_Config.h"
#include <string.h>
/*********************************************************************************
常量定义区
*********************************************************************************/
/*********************************************************************************
公共变量定义区
*********************************************************************************/
unsigned short              Concentrator_DebugTimer  = 0;     //调试模式倒计时器 计时器时间为零则退出调试模式
ConcentratorParameterStruct   Concentrator;
ServerState_Struct            ServerState[2];    //集中器状态
AutoCollectCycleStruct        ConcentratorUpdataTimer ={99,99};     //集中器 上传时间
/*********************************************************************************
外部变量声明区
*********************************************************************************/
/*********************************************************************************
私有变量定义区
*********************************************************************************/ 
static MCPX_InputStruct  Input;
static MCPX_OutputStruct  MCPX_Output[SPCP_OutputBuff_Number];
/*********************************************************************************
测试变量定义区
*********************************************************************************/

/*********************************************************************************
内部函数定义区
*********************************************************************************/
static SystemErrName getInputData (void);
static void GetDataForBuff_GPRS (MCPX_InputStruct *buff);
/*********************************************************************************
功能代码定义区
*********************************************************************************/

void MCPX_DeInit(void)
{
  DeInitOutputBuff_MXPC( );
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
void MXPC_Thread (void)
{
 SystemErrName err = NO_ERR;  
 while (1)
 {
  err = getInputData ( ); 
  if(NO_ERR != err)
  {return;}
  //err = JingDa_InPut (Input.buff, Input.length,Input.Port);
  //if(NO_ERR != err)
  {err = KingDaV1Protocol_input (Input.buff, Input.length, Input.Port);}
  if(NO_ERR != err)
  {err = KingDaServer_input (Input.buff, Input.length, Input.Port);}
  if(NO_ERR != err) 
  {err = JingDa_InPut (Input.buff, Input.length, Input.Port); }
  if(NO_ERR != err)
  {APPCON_InPut(Input.buff, Input.length, Input.Port); }
  //if(NO_ERR != err)
  //{err =  CJT188_2004_InPut (Input.buff, Input.length, Input.Port);}
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
static SystemErrName  getInputData (void)
{
 //SystemErrName err = NO_ERR; 
 static unsigned char step = 1;
 unsigned char cnt;
 unsigned char i;
 void* data;
 unsigned char *p_data;
 cnt = 0;
 while (cnt++ <4)
 {
   switch (step)
   {
    case 1:
      {
       data = GetDataForBuff_Rs232 ( );
       if(data != P_NULL)
       {
        Input.length = ((Rs232_BuffStruct*)data)->length;
        p_data = ((Rs232_BuffStruct*)data)->data;
        i = 0 ;
        while(i < Input.length)
        {Input.buff[i] = p_data[i]; i++;}
        Input.Port = Rs232_Port;
        FreeBuff_Rs232((Rs232_BuffStruct*)data);
        return NO_ERR;
       }
       else
       {step ++; break;}
      } 
    case 2:    //从Rs485或扩展模块获取数
      {
       data = GetDataForBuff_Rs485 ( );
       if(data != P_NULL)
       {
        Input.length = ((Rs485_BuffStruct*)data)->length;
        p_data = ((Rs485_BuffStruct*)data)->data;
        i = 0;
        while(i < Input.length)
        {Input.buff[i] = p_data[i]; i++;}
        Input.Port = ((Rs485_BuffStruct*)data)->port;
        FreeBuff_Rs485((Rs485_BuffStruct*)data);
        return NO_ERR;
       }
       else
       {step ++;break;}
      }
    case 3:   //从Gprs
      {
        step ++;
        GetDataForBuff_GPRS (&Input);
        if(Input.length > 0)
        {return NO_ERR;}
        else
        {return Nodata;}
      }
    case 4:    //从EtherNet获取数据 
      {step = 1;break;}
    default :
      {step = 1; break;}  
   }
 }
 return Nodata;
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
SystemErrName MXPC_SendSingle(CommPortNumber_Type port,
                              void * pbuff, 
                              unsigned short length,
                              SerialPort_SendEndCallBack_Type callBack,
                              void *callBack_arg  )
{
  SystemErrName err = NO_ERR;
  unsigned char itemp;

  switch (port)
  {
   case Rs485_PortA:
   case Rs485_PortB:
   case Rs485_PortC:
     {
      //RS485_IdleTime = 0; 
      if((ENABLE == getPowerOut_State (port))
        &&(ENABLE == getPowerRs485_State(port)))
      {err = sendBuffByDMA_Rs485 (pbuff, length, callBack, callBack_arg );}
      else
      {
       if((DISABLE == getPowerOut_State (Rs485_ALL))
          &&(DISABLE == getPowerRs485_State(Rs485_ALL)))
       {
        changeChannel_Rs485(port);
        err = PortBusy;
       }
       else
       {
         itemp = 0;
         while(itemp < SPCP_OutputBuff_Number)
         {
          if(0 == MCPX_Output[itemp].CSR.Bit.idle)
          {
            if(getChannel_Rs485( ) == MCPX_Output[itemp].OutPort)
            {
              if(pbuff != MCPX_Output[itemp].buff)
              {break;}  
            }           
            //if((Rs485_PortA == MCPX_Output[itemp].OutPort)
            //   ||(Rs485_PortB == MCPX_Output[itemp].OutPort)
            //     ||(Rs485_PortC == MCPX_Output[itemp].OutPort)
            //       ||(Rs485_PortD == MCPX_Output[itemp].OutPort))
            // {
            // if(pbuff != MCPX_Output[itemp].buff)
            //  {break;} 
            // }
          }
          itemp++;
         }
         if(itemp < SPCP_OutputBuff_Number)
         {err = PortBusy;}
         else
         {
          changeChannel_Rs485(port);
          err = PortBusy;
         }
       }
      }
      break; 
     }
   case Rs485_PortD:
     {
      if((ENABLE == getPowerRs485_State(port)))
      {err = sendBuffByDMA_Rs485 (pbuff, length, callBack, callBack_arg );}
      else
      {
       if((DISABLE == getPowerRs485_State(Rs485_ALL))) 
       {
        changeChannel_Rs485(port);
        err = PortBusy;
       }
       else
       {
         itemp = 0;
         while(itemp < SPCP_OutputBuff_Number)
         {
          if(0 == MCPX_Output[itemp].CSR.Bit.idle)
          {
            if((Rs485_PortA == MCPX_Output[itemp].OutPort)
               ||(Rs485_PortB == MCPX_Output[itemp].OutPort)
                 ||(Rs485_PortC == MCPX_Output[itemp].OutPort)
                   ||(Rs485_PortD == MCPX_Output[itemp].OutPort))
            {
              if(pbuff != MCPX_Output[itemp].buff)
              {break;} 
            }
          }
          itemp++;
         } 
         if(itemp < SPCP_OutputBuff_Number)
         {err = PortBusy;}
         else
         {
          changeChannel_Rs485(port);
          err = PortBusy;
         }
       }
      }
      break; 
     }
   case Rs232_Port:
     {
      err = sendBuffByDMA_Rs232 (pbuff, length, callBack, callBack_arg );  
      break; 
     }
   case GPRS_CH0:
   case GPRS_CH1:
   case GPRS_CH2:
   case GPRS_CH3:  
    {
     err = TCP_SendStart_MG2618(port - GPRS_CH0,pbuff,length,callBack,callBack_arg);
     break; 
     }
   case ErthNet_Port:
    {
      break; 
     }
   default :
     {return IllegalParam;}
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
void MXPC_Retrans(void *p_tmr, void *p_arg)
{
  SystemErrName err = NO_ERR;
  MCPX_OutputStruct*  OutputBuff;
  OutputBuff = (MCPX_OutputStruct*)p_arg;
 if(OutputBuff->CCR.Bit.Retrans_PV >0)
 {
   err = MXPC_SendSingle(OutputBuff->OutPort, 
                     OutputBuff->buff,  
                     OutputBuff->length,
                     OutputBuff->SendEndCallBack,
                     OutputBuff->buff); 
   
   if((GPRS_CH1 ==  OutputBuff->OutPort)
      ||(GPRS_CH2 ==  OutputBuff->OutPort)
        ||(Rs232_Port ==  OutputBuff->OutPort))
   { 
     if(NO_ERR == err)
     {OutputBuff->CCR.Bit.Retrans_PV --;}
   }
   else
   {OutputBuff->CCR.Bit.Retrans_PV --;}  
 }
 else
 {
  if(P_NULL !=  OutputBuff->RetransEndCallBack)
  {
    OutputBuff->RetransEndCallBack(OutputBuff->buff);
    //FreeOutputBuffForData_MXPC (OutputBuff->buff);
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
SystemErrName 
MXPC_Send_AutoRetrans ( CommPortNumber_Type  outport,   //目的端口
                        void * pbuff,                   //数据缓冲区
                        unsigned short length,          //数据长度
                        unsigned char retrans,          //重传次数
                        unsigned short cycle,           //重传周期数
                        SofTimer_TB tb,                 //重传周期
                        SerialPort_SendEndCallBack_Type  SendEnd_callback,     //发送完成回调
                        MCPX_RetransEndCallBack_Type     RetransEnd_callback)  //重发完成回调
{
  SystemErrName err = NO_ERR;
  MCPX_OutputStruct*  OutputBuff;
  err = MXPC_SendSingle(outport,pbuff,length,SendEnd_callback,pbuff);
  if(IllegalParam == err){return err;}
  if(NO_ERR == err) 
  {if((0  == retrans)&&(0 == cycle )){return NO_ERR;}}
  else
  {
   if(Port_OFF == err)
   {
    if((Rs485_PortA == outport)||(Rs485_PortB == outport)
       ||(Rs485_PortC == outport)||(Rs485_PortD == outport))
    {changeChannel_Rs485( outport);}
    }
   if(0  == retrans)
   {
    retrans = 1;
    cycle = 20;
    tb = TimerTB_100MS;
    }
   else
   { retrans ++;}
  }
  
  OutputBuff = GetIdleOutputBuff_MXPC ( );
  if(P_NULL == OutputBuff)
  { return Nothing;}
  OutputBuff->RetransTimer =  SofTimerCreate ( cycle,                 //周期
                                               tb,                    //时基
                                               SofTimer_PERIODIC,     //模式
                                               MXPC_Retrans,          //回调
                                               OutputBuff);           //回调参数                      
  if(P_NULL == OutputBuff->RetransTimer)
  {
   FreeOutputBuff_MXPC(OutputBuff);
   return Nothing;
  }
  OutputBuff->buff = pbuff;
  OutputBuff->length = length;
  OutputBuff->OutPort = outport;
  //OutputBuff->SourcePort = inport;
  OutputBuff->CCR.Bit.Retrans_SV = retrans;
  OutputBuff->CCR.Bit.Retrans_PV = retrans;
  OutputBuff->SendEndCallBack  =     SendEnd_callback;
  OutputBuff->RetransEndCallBack  =  RetransEnd_callback;
  SofTimerStart (OutputBuff->RetransTimer);
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
void DeInitOutputBuff_MXPC(void)
{
  unsigned char itemp =0;    
  while ( itemp < SPCP_OutputBuff_Number)
  {
   MCPX_Output[itemp].CSR.Byte = 0; 
   MCPX_Output[itemp].CSR.Bit.idle = 1;
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
MCPX_OutputStruct* GetIdleOutputBuff_MXPC (void)
{
  unsigned char itemp =0;
  while ( itemp < SPCP_OutputBuff_Number)
  {
    if(1 == MCPX_Output[itemp].CSR.Bit.idle)
    {
     MCPX_Output[itemp].CSR.Bit.idle = 0;
     return &(MCPX_Output[itemp]);
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
MCPX_OutputStruct* GetOutputBuffForData_MXPC (void* pbuff)
{
  unsigned char itemp =0;
  while ( itemp < SPCP_OutputBuff_Number)
  {
    if(pbuff == MCPX_Output[itemp].buff)
    {return &(MCPX_Output[itemp]);}
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
void FreeOutputBuff_MXPC(MCPX_OutputStruct* pbuff)
{
  if(P_NULL != pbuff)
  {
    if(P_NULL !=  pbuff->RetransTimer)
    {SofTimerDel(&(pbuff->RetransTimer));}
    pbuff->CSR.Byte = 0; 
    pbuff->CSR.Bit.idle = 1;
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
void FreeOutputBuffForData_MXPC (void* pbuff)
{
  unsigned char itemp =0;
  while ( itemp < SPCP_OutputBuff_Number)
  {
    if(pbuff == MCPX_Output[itemp].buff)
    {
      if(P_NULL !=  MCPX_Output[itemp].RetransTimer)
      {SofTimerDel(&(MCPX_Output[itemp].RetransTimer));}
      MCPX_Output[itemp].CSR.Byte = 0; 
      MCPX_Output[itemp].CSR.Bit.idle = 1;
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
static void GetDataForBuff_GPRS (MCPX_InputStruct *buff)
{
 unsigned char ch = 255;
 char* str = NULL;
 USART2_BuffStruct* package;
 static DataBuffSpec_Struct Data ;
 package = GetDataForBuff_USART2 ( );
 if(P_NULL == package)
 {buff->length = 0;return ;}
 Data.length = package->length;
 Data.pbuff = package->data;
 Decode_MG2618(&ch,&Data);
  
 if(ch < 4)
 {
  buff->length = Data.length; 
  buff->Port = GPRS_CH0;
  buff->Port += ch; 
  while(Data.length -- )
  {buff->buff[Data.length] = Data.pbuff[Data.length];}
 }
 else if( (NULL != strstr((char*)Data.pbuff,"+RECEIVE"))&&(Data.length > 0) )
 {
   str = strstr((char*)Data.pbuff,"+RECEIVE");
   if((str[9]>='0')&&(str[9]<='3'))
   {
     ch = str[9]-0x30;
     buff->Port = GPRS_CH0;
     buff->Port += ch; 
     buff->length = Data.length; 

      while(Data.length -- )
      {buff->buff[Data.length] = Data.pbuff[Data.length];}
   }
 }
 FreeBuff_USART2(package);  
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
CommPortNumber_Type getServerPort( unsigned char Server_N0)
{ 
 if(1 ==Server_N0 ) 
 {return ServerState[0].Port;}
 else if(2 ==Server_N0 ) 
 {return ServerState[1].Port;}
 return Null_Port;
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
void ServerConnect_OK(unsigned char ServerNo, CommPortNumber_Type  Port)
{
  if((ServerNo == 1)||(2 == ServerNo))
  {
    ServerNo --;
    ServerState[ServerNo].SR.Bit.CN = 1;
    ServerState[ServerNo].SR.Bit.IF = 1;
    ServerState[ServerNo].SR.Bit.RC = 0;
    ServerState[ServerNo].SR.Bit.ST = 0; 
    ServerState[ServerNo].SR.Bit.GZ = 0;
    ServerState[ServerNo].Port =  Port;
    SofTimerDel(&(ServerState[ServerNo].Time));
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
/*
static void ServerConnect(Command_MG2618 cmd, DataBuffSpec_Struct *AckData)
{
 GprsInfr_Struct  GprsInfr;
 
 if(CIPSTART == cmd )
 {
  if(P_NULL!= AckData)
  {
   StopListen_MG2618( );   
   if(CompString(AckData->pbuff,"CONNECT OK",P_NULL))
   {
     GprsInfr.IP[0] = MG2618Infr.Command.CmdPram.TcpUdp_Connect.IP[0];
     GprsInfr.IP[1] = MG2618Infr.Command.CmdPram.TcpUdp_Connect.IP[1];
     GprsInfr.IP[2] = MG2618Infr.Command.CmdPram.TcpUdp_Connect.IP[2];
     GprsInfr.IP[3] = MG2618Infr.Command.CmdPram.TcpUdp_Connect.IP[3];
     GprsInfr.PORT = MG2618Infr.Command.CmdPram.TcpUdp_Connect.PORT;
     GprsConnect_OK(MG2618Infr.Command.CmdPram.TcpUdp_Connect.CH, &GprsInfr);
   }
   else
   {
     StopListen_MG2618( );
     freeChannel_MG2618(MG2618Infr.Command.CmdPram.TcpUdp_Connect.CH);
   }
  }
  else
  {
    StopListen_MG2618( ); 
    freeChannel_MG2618(MG2618Infr.Command.CmdPram.TcpUdp_Connect.CH);
  }
 }
 else
 {
   StopListen_MG2618( );
 }
}*/
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static void Reconnec(void *p_tmr, void *p_arg)
{
  unsigned char ServerNo;  
  CommandDescriptor_MG2618  cmdbuff; 
  
  if(P_NULL == p_arg )
  {
    SofTimerDel ((SofTimer_Struct**)&p_tmr);
    BreakServerAction(1);
    BreakServerAction(2); 
    Reboot_MG2618(P_NULL); //重启模块
    return;
  } 
  ServerNo = *(( unsigned char* )p_arg);
  if((1 == ServerNo)||(2 == ServerNo))
  {
    if(p_tmr != ServerState[ServerNo-1].Time)
    {SofTimerDel((SofTimer_Struct**)&p_tmr);}
    
    if(ServerState[ServerNo-1].ServerConnect_CNT >= 5)
    { 
      SofTimer_ChangePeriod ( p_tmr,           //操纵的定时器
                             GRPS_PauseTime,  //新的周期
                             1);             //是否复位定时器 1:重新计时  0:继续计时    
      ServerState[ServerNo-1].ServerConnect_CNT = 0;  
      ServerState[ServerNo-1].SR.Bit.GZ = 1;
    }
    else
    {
      SofTimer_ChangePeriod ( p_tmr,           //操纵的定时器
                             GRPS_ReconnectionTime,  //新的周期
                             1);             //是否复位定时器 1:重新计时  0:继续计时 
    }
    
    if((0 != ServerState[0].SR.Bit.GZ)
       &&(0 != ServerState[1].SR.Bit.GZ))
    {
      BreakServerAction(1);
      BreakServerAction(2);
      Reboot_MG2618(P_NULL); //重启模块
      return;
    }
    
    else if((0 != ServerState[0].SR.Bit.GZ)
            &&(0 == ServerState[1].SR.Bit.CN)
              &&(0 == ServerState[1].SR.Bit.RC))
    {
      BreakServerAction(1);
      BreakServerAction(2);
      Reboot_MG2618(P_NULL); //重启模块
      return;
    }
    
    else if((0 != ServerState[1].SR.Bit.GZ)
            &&(0 == ServerState[0].SR.Bit.CN)
              &&(0 == ServerState[0].SR.Bit.RC))
    {
      BreakServerAction(1);
      BreakServerAction(2);
      Reboot_MG2618(P_NULL); //重启模块
      return;
    }
  }
  else
  {
    SofTimerDel ((SofTimer_Struct**)&p_tmr); 
    return;
  }
  
  if(1 == ServerNo)   
  {
    if((0 != Concentrator.server1.ip[0])
       ||(0 != Concentrator.server1.ip[1])
         ||(0 != Concentrator.server1.ip[2])
           ||(0 != Concentrator.server1.ip[3]))
    {
      cmdbuff.Cmd = CIPSTART;
      cmdbuff.Format = Param;
      cmdbuff.CmdPram.TcpUdp_Connect.CH = getIdleChannel_MG2618( ); 
      cmdbuff.CmdPram.TcpUdp_Connect.IP[0] = Concentrator.server1.ip[0];       //112;
      cmdbuff.CmdPram.TcpUdp_Connect.IP[1] = Concentrator.server1.ip[1];       //25;
      cmdbuff.CmdPram.TcpUdp_Connect.IP[2] = Concentrator.server1.ip[2];       //204;
      cmdbuff.CmdPram.TcpUdp_Connect.IP[3] = Concentrator.server1.ip[3];       //3;  
      cmdbuff.CmdPram.TcpUdp_Connect.PORT = Concentrator.server1.port;         //50050
      cmdbuff.CmdPram.TcpUdp_Connect.Mode = TCP;
    }
    else
    {
      ServerState[0].Port = Null_Port;
      ServerState[1].SR.Byte = 0;
      ServerState[0].ServerConnect_CNT = 0;
      SofTimerDel(&(ServerState[0].Time));
      SofTimerDel ((SofTimer_Struct**)&p_tmr); 
      return; 
    }
  }
  else if(2 == ServerNo)
  {
    if((0 != Concentrator.server2.ip[0])
       ||(0 != Concentrator.server2.ip[1])
         ||(0 != Concentrator.server2.ip[2])
           ||(0 != Concentrator.server2.ip[3]))
    {
      cmdbuff.Cmd = CIPSTART;
      cmdbuff.Format = Param;
      cmdbuff.CmdPram.TcpUdp_Connect.CH = getIdleChannel_MG2618( ); 
      cmdbuff.CmdPram.TcpUdp_Connect.IP[0] = Concentrator.server2.ip[0];       //112;
      cmdbuff.CmdPram.TcpUdp_Connect.IP[1] = Concentrator.server2.ip[1];       //25;
      cmdbuff.CmdPram.TcpUdp_Connect.IP[2] = Concentrator.server2.ip[2];       //204;
      cmdbuff.CmdPram.TcpUdp_Connect.IP[3] = Concentrator.server2.ip[3];       //3;  
      cmdbuff.CmdPram.TcpUdp_Connect.PORT = Concentrator.server2.port;  //50050
      cmdbuff.CmdPram.TcpUdp_Connect.Mode = TCP;
    }
    else
    {
      ServerState[1].Port = Null_Port;
      ServerState[1].SR.Byte = 0;
      ServerState[1].ServerConnect_CNT = 0;
      SofTimerDel(&(ServerState[1].Time));
      SofTimerDel ((SofTimer_Struct**)&p_tmr); 
      return; 
    }
  }
  else //参数不合法
  { 
    SofTimerDel ((SofTimer_Struct**)&p_tmr); 
    return;
  }
  if(NO_ERR == SendCmd_MG2618(&cmdbuff,P_NULL,0,6000))
  {
    if(ServerState[ServerNo-1].ServerConnect_CNT < 100)
    {ServerState[ServerNo-1].ServerConnect_CNT ++;}
    return;
  }
  else
  {freeChannel_MG2618(cmdbuff.CmdPram.TcpUdp_Connect.CH); }   
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
void ServerClose_UEV(unsigned char ServerNo)
{
  if((1 == ServerNo) || (2 == ServerNo))
  {
    ServerNo --;
    if(0 == ServerState[ServerNo].SR.Bit.RC)
    {
      BreakServerAction( ServerNo + 1 );  
      ServerState[ServerNo].Time = SofTimerCreate(GRPS_ReconnectionTime, //周期
                                                  TimerTB_1M,            //时基
                                                  SofTimer_PERIODIC,     //模式
                                                  Reconnec,      //回调
                                                 (void *)&(Integer[ServerNo + 1]));   //回调参数
      if(P_NULL != ServerState[ServerNo].Time )
      {
        ServerState[ServerNo].SR.Bit.RC = 1; 
        SofTimerStart(ServerState[ServerNo].Time); 
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
/*void ReconnecServer( unsigned char ServerNo)
{
  BreakServerAction(ServerNo);
  Reconnec(P_NULL, (void *)&(Integer[ServerNo]));
}*/
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void BreakServerAction(unsigned char ServerNo)
{
  unsigned char itemp;
  if((1 == ServerNo)||(2 == ServerNo))
  {
    ServerNo --;
    ServerState[ServerNo].SR.Byte = 0;
    ServerState[ServerNo].ServerConnect_CNT = 0;  
    SofTimerDel(&(ServerState[ServerNo].Time)); 
    
    if((GPRS_CH0 == ServerState[ServerNo].Port)
      ||(GPRS_CH1 == ServerState[ServerNo].Port)
        ||(GPRS_CH2 == ServerState[ServerNo].Port)
          ||(GPRS_CH3 == ServerState[ServerNo].Port))
    { 
      itemp = ServerState[ServerNo].Port - GPRS_CH0;
      freeChannel_MG2618(itemp);
    }
    ServerState[ServerNo].Port = Null_Port;
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
void  CommPortClose_UEV (CommPortNumber_Type  Port)
{
 if(ServerState[0].Port == Port) 
 {
  ServerClose_UEV(1);
 }
 if(ServerState[1].Port == Port) 
 {
  ServerClose_UEV(2);
 }
}
/******************************************END********************************************************/

