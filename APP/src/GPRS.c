/*********************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include "..\inc\GPRS.h"
#include "..\inc\MCPX.h"
#include "..\..\Projects\inc\Common_API.h"
#include "..\..\Projects\inc\SofTimer.h"
#include "..\..\SPCP\KingDa\KingDaServer.h"
/*********************************************************************************
����������
*********************************************************************************/
/*********************************************************************************
��������������
*********************************************************************************/
unsigned short    GprsStop_time   = 0;
unsigned char     LinkLed_time  = 2;
unsigned short   LinkErr_time  = 0;
/*********************************************************************************
�ⲿ����������
*********************************************************************************/
/*********************************************************************************
˽�б���������
*********************************************************************************/
/*********************************************************************************
���Ա���������
*********************************************************************************/

/*********************************************************************************
�ڲ�����������
*********************************************************************************/
static void Reboot_MG2618_STEP (void *p_tmr, void *p_arg);
static void GprsInitConnect(Command_MG2618 cmd, DataBuffSpec_Struct *AckData);
static void HeartBeat_Gprs(void *p_tmr, void *p_arg);

/*********************************************************************************
���ܴ��붨����
*********************************************************************************/
/*********************************************************************************
Function:      //Reboot_MG2618
Description:   //�������� MG2618ģ�� 
Input:         // OK  ���������Ϣ֪ͨ����
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
SystemErrName Reboot_MG2618 (GPRSLaunch_OK OK) 
{
  SofTimer_Struct *GPRS_Timer;
  unsigned char itemp = 0;
  
  PowerOff_GPRS ( );
  
  if((0 != Concentrator.server1.ip[0])
     ||(0 != Concentrator.server1.ip[1])
       ||(0 != Concentrator.server1.ip[2])
         ||(0 != Concentrator.server1.ip[3]))
  {itemp = 1;}
  
  if((0 != Concentrator.server2.ip[0])
     ||(0 != Concentrator.server2.ip[1])
       ||(0 != Concentrator.server2.ip[2])
         ||(0 != Concentrator.server2.ip[3]))
  {itemp = 2;}
  
  if(0 ==itemp )
  {return NO_ERR;}
  
  GPRS_Timer =  SofTimerCreate (2000,                      //����
                                TimerTB_10MS,           //ʱ��
                                SofTimer_PERIODIC,      //ģʽ
                                Reboot_MG2618_STEP,     //�ص�
                                (void *)OK);            //�ص�����
  if(P_NULL == GPRS_Timer)
  {return Nothing;}
  SET_MG2616_RSET_PIN;
  SET_MG2616_ONOF_PIN;
  SofTimerStart(GPRS_Timer );  
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
static void Reboot_MG2618_STEP (void *p_tmr, void *p_arg)
{
  static unsigned char step = 0;
  if(DISABLE == Get_MG2168PowerState( ))   
  {
    RSET_MG2616_RSET_PIN; 
    MG2618_PowerON( );
    SofTimer_ChangePeriod((SofTimer_Struct*) p_tmr,50,1);
    step = 1;
  }
  else
  {
    switch (step)
    {  
    case 1:  
      {
        RSET_MG2616_ONOF_PIN;  //������������
        SofTimer_ChangePeriod((SofTimer_Struct*) p_tmr,300,1);    
        step = 2;
        break;
      }
    case 2:  //
      {
        SET_MG2616_RSET_PIN;  //�ͷŸ�λ����
        SofTimer_ChangePeriod((SofTimer_Struct*) p_tmr,300,1);    
        step = 3;
        break;
      }
    case 3:
      {
        SET_MG2616_ONOF_PIN;  //��ɿ�������
        ConfigUsart_MG2616(38400, Parity_No, StopBits_1, ByIT);
        MG2618Infr.ModInf.Status = Idle;
        if(P_NULL != p_arg)  //�ж��Ƿ���Ҫ���Ϳ��������Ϣ
        {((GPRSLaunch_OK)p_arg)( );}
        SofTimerDel((SofTimer_Struct**)&p_tmr);  
        GprsInitConnect(CSQ, P_NULL);
        step = 0; 
        break;
      }
    default:
      {
        SofTimerDel((SofTimer_Struct**) &p_tmr) ;
        Reboot_MG2618 (P_NULL); 
        break;
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
void GprsConnect_OK(unsigned char CH, GprsInfr_Struct*  Infr)
{
  CommPortNumber_Type Port;
  unsigned char itemp;
  unsigned char *tp,*cp ;
  
  if(CH < 4)
  {
    MG2618Infr.CH_SR[CH].Bit.CN = 1; 
    itemp = 0;
    tp = (unsigned char *)&(MG2618Infr.GprsInfr[CH]);
    cp = (unsigned char *)Infr;
    while (itemp < sizeof(GprsInfr_Struct))
    {
      tp[itemp] = cp[itemp];
      itemp ++;
    }
  }
  if((MG2618Infr.Command.CmdPram.TcpUdp_Connect.IP[0] == Concentrator.server1.ip[0])
     &&(MG2618Infr.Command.CmdPram.TcpUdp_Connect.IP[1] == Concentrator.server1.ip[1])
       &&(MG2618Infr.Command.CmdPram.TcpUdp_Connect.IP[2] == Concentrator.server1.ip[2])
         &&(MG2618Infr.Command.CmdPram.TcpUdp_Connect.IP[3] == Concentrator.server1.ip[3])
           &&(MG2618Infr.Command.CmdPram.TcpUdp_Connect.PORT == Concentrator.server1.port)) 
  { itemp = 1;}
  else if((MG2618Infr.Command.CmdPram.TcpUdp_Connect.IP[0] == Concentrator.server2.ip[0])
          &&(MG2618Infr.Command.CmdPram.TcpUdp_Connect.IP[1] == Concentrator.server2.ip[1])
            &&(MG2618Infr.Command.CmdPram.TcpUdp_Connect.IP[2] == Concentrator.server2.ip[2])
              &&(MG2618Infr.Command.CmdPram.TcpUdp_Connect.IP[3] == Concentrator.server2.ip[3])
                &&(MG2618Infr.Command.CmdPram.TcpUdp_Connect.PORT == Concentrator.server2.port)) 
  {itemp = 2;}
  Port  = GPRS_CH0;
  Port += MG2618Infr.Command.CmdPram.TcpUdp_Connect.CH;
  ServerConnect_OK(itemp,Port);
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
static void GprsInitConnect(Command_MG2618 cmd, DataBuffSpec_Struct *AckData)
{
  static unsigned cnt = 0; 
  CommandDescriptor_MG2618  cmdbuff; 
  switch(cmd)  
  {
  case CSQ:  //�ź�ǿ�Ȳ�ѯ
    {
      if(P_NULL != AckData)
      {
        cnt = 0;
        if(MG2618Infr.ModInf.Rssi > 0)
        {
          //������ͬ����
          StopListen_MG2618( );
          cmdbuff.Cmd = CGATT;    //���GPRS ����״̬
          cmdbuff.Format = Query;  
          SendCmd_MG2618(&cmdbuff,GprsInitConnect,1,100);
        }
      }
      else if(cnt++ < 3)
      {
        StopListen_MG2618( );
        cmdbuff.Cmd = CSQ;
        cmdbuff.Format = Void;  
        SendCmd_MG2618(&cmdbuff,GprsInitConnect,0,50);
      }
      break;
    }
  case CGATT:  //���GPRS ����״̬
    {
      if(P_NULL != AckData)
      {
        cnt = 0;
        while(AckData->length--)
        {
          if(IsDigit(*(AckData->pbuff)))
          {break;}
          AckData->pbuff ++;
        }
        if('1' == *(AckData->pbuff))
        {
          while(AckData->length--)
          {
            if(IsLETTER(*(AckData->pbuff)))
            {break;}
            AckData->pbuff ++;
          }
          if(CompString(AckData->pbuff,"OK",P_NULL))
          {
            StopListen_MG2618( );
            cmdbuff.Cmd = CIPMUX;  
            cmdbuff.Format = Param; 
            cmdbuff.CmdPram.CIPMUX.mode = '1';
            SendCmd_MG2618(&cmdbuff,GprsInitConnect,1,50);
          }       
        }
      }
      else if(cnt++ < 3)
      {
        //������ͬ����
        StopListen_MG2618( );
        cmdbuff.Cmd = CGATT;    //���GPRS ����״̬
        cmdbuff.Format = Query;  
        SendCmd_MG2618(&cmdbuff,GprsInitConnect,1,100);
      }
      break;
    }
  case CIPMUX:
    {
      if(P_NULL != AckData)
      {
        cnt = 0;
        if(CompString(AckData->pbuff,"OK",P_NULL))
        {
          StopListen_MG2618( ); 
          cmdbuff.Cmd = CSTT;
          cmdbuff.Format = Param;
          cmdbuff.CmdPram.ZPNUM.APN = "CMNET";//"cmnet    UNIM2M.NJM2MAPN";
          SendCmd_MG2618(&cmdbuff,GprsInitConnect,1,100);
        }       
      }
      else if(cnt++ < 3)
      {
        StopListen_MG2618( );
        cmdbuff.Cmd = CIPMUX;  //���GPRS ����״̬
        cmdbuff.Format = Param; 
        cmdbuff.CmdPram.CIPMUX.mode = '1';
        SendCmd_MG2618(&cmdbuff,GprsInitConnect,1,50);
      }
      break;
    }
  case CSTT:  //���� APN�� �û���������
    {
      if(P_NULL!= AckData)
      {
        cnt = 0;
        if(CompString(AckData->pbuff,"OK",P_NULL))
        {
          cnt = 0;  
          StopListen_MG2618( ); 
          cmdbuff.Cmd = CIICR;  //����������·
          cmdbuff.Format = Void;
          SendCmd_MG2618(&cmdbuff,GprsInitConnect,1,500);;
        }
      }
      else if(cnt++ < 3)
      {
        StopListen_MG2618( ); 
        cmdbuff.Cmd = CSTT;
        cmdbuff.Format = Param;
        cmdbuff.CmdPram.ZPNUM.APN = "CMNET";//"cmnet    UNIM2M.NJM2MAPN";
        SendCmd_MG2618(&cmdbuff,GprsInitConnect,1,100);
      }
      break;
    }
  case CIICR:    // ����������·��GPRS����CSD�� 
    {
      if(P_NULL!= AckData)
      {
        if(CompString(AckData->pbuff,"OK",P_NULL))
        {
          cnt = 0;  
          StopListen_MG2618( );
          cmdbuff.Cmd = CIFSR;  //��ѯ��ǰģ�� IP ��ֵַ
          cmdbuff.Format = Void;
          SendCmd_MG2618(&cmdbuff,GprsInitConnect,1,100);
          break;
        }
      }
      else if(cnt++ < 3)
      {   
        cmdbuff.Cmd = CIICR;
        cmdbuff.Format = Void;
        SendCmd_MG2618(&cmdbuff,GprsInitConnect,1,500);
      }
      break;
    } 
  case CIFSR: //CIFSR:  //��ѯ��ǰģ�� IP ��ֵַ
    {
      if(P_NULL!= AckData)
      {
        cnt = 0;
        if((0 != MG2618Infr.ModInf.LOCAL_IP[0])
           ||(0 != MG2618Infr.ModInf.LOCAL_IP[1])
             ||(0 != MG2618Infr.ModInf.LOCAL_IP[2])
               ||(0 != MG2618Infr.ModInf.LOCAL_IP[3]))
        {
          StopListen_MG2618( ); 
          cnt = 0;  
          cmdbuff.Cmd = CIPSTART; //CIPSTART
          cmdbuff.Format = Param;
          if((0 != Concentrator.server1.ip[0])
             ||(0 != Concentrator.server1.ip[1])
               ||(0 != Concentrator.server1.ip[2])
                 ||(0 != Concentrator.server1.ip[3]))
          {
            cmdbuff.CmdPram.TcpUdp_Connect.CH = getIdleChannel_MG2618( ); 
            cmdbuff.CmdPram.TcpUdp_Connect.IP[0] = Concentrator.server1.ip[0];       //112;
            cmdbuff.CmdPram.TcpUdp_Connect.IP[1] = Concentrator.server1.ip[1];       //25;
            cmdbuff.CmdPram.TcpUdp_Connect.IP[2] = Concentrator.server1.ip[2];       //204;
            cmdbuff.CmdPram.TcpUdp_Connect.IP[3] = Concentrator.server1.ip[3];       //3;  
            cmdbuff.CmdPram.TcpUdp_Connect.PORT = Concentrator.server1.port;  //50050
            cmdbuff.CmdPram.TcpUdp_Connect.Mode = TCP;
            SendCmd_MG2618(&cmdbuff,GprsInitConnect,0,6000);
          }
          else if((0 != Concentrator.server2.ip[0])
                  ||(0 != Concentrator.server2.ip[1])
                    ||(0 != Concentrator.server2.ip[2])
                      ||(0 != Concentrator.server2.ip[3]))
          {
            cmdbuff.CmdPram.TcpUdp_Connect.CH = getIdleChannel_MG2618( ); 
            cmdbuff.CmdPram.TcpUdp_Connect.IP[0] = Concentrator.server2.ip[0];       //112;
            cmdbuff.CmdPram.TcpUdp_Connect.IP[1] = Concentrator.server2.ip[1];       //25;
            cmdbuff.CmdPram.TcpUdp_Connect.IP[2] = Concentrator.server2.ip[2];       //204;
            cmdbuff.CmdPram.TcpUdp_Connect.IP[3] = Concentrator.server2.ip[3];       //3;  
            cmdbuff.CmdPram.TcpUdp_Connect.PORT = Concentrator.server2.port;  //50050
            cmdbuff.CmdPram.TcpUdp_Connect.Mode = TCP;
            SendCmd_MG2618(&cmdbuff,GprsInitConnect,0,6000);
          }
          else
          {
            PowerOff_GPRS ( );
            cnt = 0;
          }
          break;
        }
      }
      else if(cnt++ < 3)
      {
        cnt ++;
        cmdbuff.Cmd = CIFSR;
        cmdbuff.Format = Void;
        SendCmd_MG2618(&cmdbuff,GprsInitConnect,1,100);
      }
      break;
    }
  case CIPSTART: //���� TCP ����������
    {
      if(P_NULL!= AckData)
      {
        cnt = 0;
        StopListen_MG2618( );
        if((0 != ServerState[0].SR.Bit.CN)&&(0 != ServerState[1].SR.Bit.CN))
        {
          //�������
          MG2618Infr.HeartBeatTime = SofTimerCreate( 60,                 //����
                                                    TimerTB_100MS,      //ʱ��
                                                    SofTimer_PERIODIC,  //ģʽ
                                                    HeartBeat_Gprs,  //�ص�
                                                    P_NULL);//�ص�����
          SofTimerStart(MG2618Infr.HeartBeatTime); 
        }
        else if(ServerState[0].SR.Bit.CN) 
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
            cmdbuff.CmdPram.TcpUdp_Connect.PORT = Concentrator.server2.port;         //50050
            cmdbuff.CmdPram.TcpUdp_Connect.Mode = TCP;
            SendCmd_MG2618(&cmdbuff,GprsInitConnect,0,6000);
          }
          else
          {
            //������� 
            MG2618Infr.HeartBeatTime = SofTimerCreate( 60,                 //����
                                                      TimerTB_100MS,      //ʱ��
                                                      SofTimer_PERIODIC,  //ģʽ
                                                      HeartBeat_Gprs,  //�ص�
                                                      P_NULL);//�ص�����
            SofTimerStart(MG2618Infr.HeartBeatTime); 
          }
        }else if(ServerState[1].SR.Bit.CN)
        {
          //�������
          MG2618Infr.HeartBeatTime = SofTimerCreate( 60,                 //����
                                                    TimerTB_100MS,      //ʱ��
                                                    SofTimer_PERIODIC,  //ģʽ
                                                    HeartBeat_Gprs,  //�ص�
                                                    P_NULL);//�ص�����
          SofTimerStart(MG2618Infr.HeartBeatTime); 
        }
      }
      else if(cnt++ < 3)
      {    
        StopListen_MG2618( );
        cmdbuff.Cmd = CIPSTART; //CIPSTART
        cmdbuff.Format = Param;
        if((0 == ServerState[0].SR.Bit.CN) 
           &&((0 != Concentrator.server1.ip[0])
              ||(0 != Concentrator.server1.ip[1])
                ||(0 != Concentrator.server1.ip[2])
                  ||(0 != Concentrator.server1.ip[3])))
        {
          cmdbuff.CmdPram.TcpUdp_Connect.CH = getIdleChannel_MG2618( ); 
          cmdbuff.CmdPram.TcpUdp_Connect.IP[0] = Concentrator.server1.ip[0];       //112;
          cmdbuff.CmdPram.TcpUdp_Connect.IP[1] = Concentrator.server1.ip[1];       //25;
          cmdbuff.CmdPram.TcpUdp_Connect.IP[2] = Concentrator.server1.ip[2];       //204;
          cmdbuff.CmdPram.TcpUdp_Connect.IP[3] = Concentrator.server1.ip[3];       //3;  
          cmdbuff.CmdPram.TcpUdp_Connect.PORT = Concentrator.server1.port;  //50050
          cmdbuff.CmdPram.TcpUdp_Connect.Mode = TCP;
          SendCmd_MG2618(&cmdbuff,GprsInitConnect,0,6000);
        }
        else if((0 == ServerState[1].SR.Bit.CN) 
                &&((0 != Concentrator.server2.ip[0])
                   ||(0 != Concentrator.server2.ip[1])
                     ||(0 != Concentrator.server2.ip[2])
                       ||(0 != Concentrator.server2.ip[3])))
          cmdbuff.CmdPram.TcpUdp_Connect.CH = getIdleChannel_MG2618( ); 
        cmdbuff.CmdPram.TcpUdp_Connect.IP[0] = Concentrator.server2.ip[0];       //112;
        cmdbuff.CmdPram.TcpUdp_Connect.IP[1] = Concentrator.server2.ip[1];       //25;
        cmdbuff.CmdPram.TcpUdp_Connect.IP[2] = Concentrator.server2.ip[2];       //204;
        cmdbuff.CmdPram.TcpUdp_Connect.IP[3] = Concentrator.server2.ip[3];       //3;  
        cmdbuff.CmdPram.TcpUdp_Connect.PORT = Concentrator.server2.port;  //50050
        cmdbuff.CmdPram.TcpUdp_Connect.Mode = TCP;
        SendCmd_MG2618(&cmdbuff,GprsInitConnect,0,6000);
      }
      break;
    }
  }
  
  if(cnt >= 3)
  { 
    cnt = 0;
    StopListen_MG2618( );  
    Reboot_MG2618(P_NULL);
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
void GprsClose_UEV(unsigned char CH)
{
  CommPortNumber_Type    Port = GPRS_CH0;       //�������˿�
  
  if( CH < 4)
  {
    Port += CH;
    MG2618Infr.CH_SR[CH].Byte = 0; 
    CommPortClose_UEV (Port);
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
SystemErrName TCP_SendStart_MG2618(unsigned char ch,
                                   unsigned char *buff,
                                   unsigned short length,
                                   SerialPort_SendEndCallBack_Type callBack,
                                   void *callBack_arg)
{
  SystemErrName err = NO_ERR;
  CommPortNumber_Type Port ;
  CommandDescriptor_MG2618  cmdbuff; 
  switch(ch)
  {
  case 0:
    {if(0 == MG2618Infr.CH_SR[0].Bit.CN) {err = Port_OFF;}break;}
  case 1:
    {if(0 == MG2618Infr.CH_SR[1].Bit.CN) {err = Port_OFF;}break;}
  case 2:
    {if(0 == MG2618Infr.CH_SR[2].Bit.CN) {err = Port_OFF;}break;}
  case 3:
    {if(0 == MG2618Infr.CH_SR[3].Bit.CN) {err = Port_OFF;}break;}
  default :
    { err = IllegalParam;break;}   
  }
  if(NO_ERR != err)
  {return err;}
  
  cmdbuff.Cmd = CIPSEND;
  cmdbuff.Format = Param;
  cmdbuff.CmdPram.TcpUdp_Send.CH = ch;
  cmdbuff.CmdPram.TcpUdp_Send.LENGTH = length;
  cmdbuff.CmdPram.TcpUdp_Send.buff = buff;
  cmdbuff.CmdPram.TcpUdp_Send.callBack = callBack;
  cmdbuff.CmdPram.TcpUdp_Send.callBack_arg = callBack_arg;
  err = SendCmd_MG2618(&cmdbuff,P_NULL,0,3000);
  
  if(NO_ERR == err)
  { 
    Port = GPRS_CH0 ;
    Port += ch;
    if(Port == ServerState[0].Port)
    {ServerState[0].SR.Bit.ST = 1;}
    else if(Port == ServerState[1].Port)
    {ServerState[1].SR.Bit.ST = 1;}
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
static void Server1_AckFunc(KingDaServer_PackageStruct* SendBuff, void* AckData)
{
  static unsigned char cnt = 0 ; 
  if(HandCmd_Server == SendBuff->head.Cmd)
  {
    if(P_NULL == AckData)
    {
      if(++cnt >=  3)
      {
        cnt = 0 ;
        ServerClose_UEV(1);
      }
    }
    else
    {
      ServerState[0].SR.Bit.ACK = 1;
      cnt = 0;
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
static void Server2_AckFunc(KingDaServer_PackageStruct* SendBuff, void* AckData)
{
  static unsigned char cnt = 0 ; 
  if(HandCmd_Server == SendBuff->head.Cmd)
  {
    if(P_NULL == AckData)
    {
      if(++cnt >= 3)
      {
        cnt = 0 ;
        ServerClose_UEV(2);
      }
    }
    else
    {
      cnt = 0;
      ServerState[1].SR.Bit.ACK = 1;
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
static void HeartBeat_Gprs(void *p_tmr, void *p_arg)
{
  static unsigned short step = 0;
  CommandDescriptor_MG2618  cmdbuff; 
  
  if(step == 8)
  {
    if((0 != Concentrator.server1.ip[0])
       ||(0 != Concentrator.server1.ip[1])
         ||(0 != Concentrator.server1.ip[2])
           ||(0 != Concentrator.server1.ip[3]))
    {
      if(0 !=ServerState[0].SR.Bit.CN)
      {
        if(1 == ServerState[0].SR.Bit.ST)
        {ServerState[0].SR.Bit.ST = 0;}
        else
        {SendHandCmdToKingDaServer(1,Server1_AckFunc );}  //������������
      }
      else
      {ServerClose_UEV(1);}
    }
    else
    {BreakServerAction(1);}
  }
  else if(15 == step)
  {
    step = 0; 
    if((0 != Concentrator.server2.ip[0])
       ||(0 != Concentrator.server2.ip[1])
         ||(0 != Concentrator.server2.ip[2])
           ||(0 != Concentrator.server2.ip[3]))
    {
      if(0 !=ServerState[1].SR.Bit.CN)
      {
        if(1 == ServerState[1].SR.Bit.ST)
        {ServerState[1].SR.Bit.ST = 0;}
        else
        {SendHandCmdToKingDaServer(2,Server2_AckFunc );}  //������������
      }
      else
      {ServerClose_UEV(2);}
    }
    else
    {BreakServerAction(2);}
  }
  else
  {
    if(0 != step )
    {
      if(ServerState[0].SR.Bit.IF)
      {
        ServerState[0].SR.Bit.IF = 0;
        SendHandCmdToKingDaServer(1, P_NULL );
      }
      else if(ServerState[1].SR.Bit.IF)
      {
        ServerState[1].SR.Bit.IF = 0;
        SendHandCmdToKingDaServer(2, P_NULL );
      }
      else
      {
        cmdbuff.Cmd = CSQ;
        cmdbuff.Format = Void;  
        SendCmd_MG2618(&cmdbuff,P_NULL,0,50);
      }
    }
    else
    {
      cmdbuff.Cmd = CSQ;
      cmdbuff.Format = Void;  
      SendCmd_MG2618(&cmdbuff,P_NULL,0,50);
    }
  }
  step ++;
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
unsigned char getIdleChannel_MG2618(void)
{
  unsigned char itemp = 0;
  while(itemp < 4)
  {
    if(0 == MG2618Infr.CH_SR[itemp].Bit.Flg)
    {
      MG2618Infr.CH_SR[itemp].Byte = 0;
      MG2618Infr.CH_SR[itemp].Bit.Flg = 1;
      break;
    }
    else
    {itemp ++;}
  }
  if(4 == itemp)
  {return 255;}  
  else
  {return itemp;}
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
void freeChannel_MG2618(unsigned char ch)
{
  CommandDescriptor_MG2618  cmdbuff;
  
  if(ch < 4)
  {
    MG2618Infr.CH_SR[ch].Bit.Flg = 0; 
    if(0 != MG2618Infr.CH_SR[ch].Bit.CN)
    {
      cmdbuff.Cmd = CIPCLOSE;
      cmdbuff.Format = Param;
      cmdbuff.CmdPram.CIPCLOSE.CH = ch;
      SendCmd_MG2618(&cmdbuff,P_NULL,6,100);
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
void  GprsGuarde_Process ( void )
{
  unsigned char itemp;
  if((0 == Concentrator.server1.ip[0])
     &&(0 == Concentrator.server1.ip[1])
       &&(0 == Concentrator.server1.ip[2])
         &&(0 == Concentrator.server1.ip[3]))
  {
    if((0 == Concentrator.server2.ip[0])
       &&(0 == Concentrator.server2.ip[1])
         &&(0 == Concentrator.server2.ip[2])
           &&(0 == Concentrator.server2.ip[3]))
    {
      PowerOff_GPRS ( );
      LinkLed_OFF( );
      return;
    }
  }
  
  if((0 == ServerState[0].SR.Bit.CN)
     &&(0 == ServerState[0].SR.Bit.RC)
       &&(0 == ServerState[1].SR.Bit.CN)
         &&(0 == ServerState[1].SR.Bit.RC))
  {
    if(LinkErr_time > 1200 )
    {
      BreakServerAction(1);
      BreakServerAction(2);
      Reboot_MG2618(P_NULL); //����ģ��
      LinkErr_time = 0;
    }
  }
  else
  {LinkErr_time = 0;}
  
  if(GprsStop_time >= 600)
  {
    BreakServerAction(1);
    BreakServerAction(2);
    Reboot_MG2618(P_NULL);
  } //����ģ��}
  
  LinkLed_time  %= 8;
  if((0 != ServerState[0].SR.Bit.CN) && (0 != ServerState[1].SR.Bit.CN))
  {itemp = 8;}
  else if(0 != ServerState[0].SR.Bit.CN)
  {itemp = 6;}
  else if(0 != ServerState[1].SR.Bit.CN)
  {itemp = 2;}
  else
  {itemp = 0;}
  
  if(LinkLed_time < itemp)
  { LinkLed_ON( );}
  else
  { LinkLed_OFF( );}   
}
/***********************************END******************************************/
