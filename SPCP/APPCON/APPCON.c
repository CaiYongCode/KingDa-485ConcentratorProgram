/*********************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include ".\APPCON.h"
#include "..\..\Projects\inc\Common_API.h"
#include "..\..\Projects\inc\SofTimer.h"
/*********************************************************************************
����������
*********************************************************************************/
//AT ָ���б�
const char*  const CmdNameList_APPCON[] = {
 "DELNET\r\n",     // ɾ���������ɾ��ģ�飬·��������
 "CMD ",        // ��λ��������ģ��ģ��ͨѶ����
 "TST ",        // ��������ģ������Ϳط�����
 "RDNODE",     // ��������ڵ����߱�ģ��ID ������
 "RDROUTER\r\n",   // ��������ڵ�����·��ģ��ID ������
 "FREQ",       // ��дƵ�ʲ�������
 "NETID",      // ��д����ID ����
 "BATCH\r\n",      // �������������� 
 "STATUS\r\n",     // ״̬��ѯ����
 "RATE",      // ��д������������������
 "MRATE",     // ��д����ģ�鴮�����ʲ�������
 "VER\r\n",       // ��������Ӳ���汾����
 "HELP\r\n",      // ��������
 "TXD",       // ���������ƻ�ͨѶ����
 "SETID",     // ��������������ID ��������  
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
��������������
*********************************************************************************/

/*********************************************************************************
�ⲿ����������
*********************************************************************************/

/*********************************************************************************
˽�б���������
*********************************************************************************/
static APPCON_OutPutBuff_Struct   APPCON_OutPutBuff[APPCON_OutPutBuff_Number];
static struct{
 PortStatus                       Status; 
 APPCON_OutPutBuff_Struct*       LastCmd;
 SofTimer_Struct*               WaitTime;   //�ȴ�Ӧ���ʱ�� Ӧ��ʱ�� ȡ���ȴ�
}APPCONS_Module; 
/*********************************************************************************
���Ա���������
*********************************************************************************/

/*********************************************************************************
�ڲ�����������
*********************************************************************************/
static APPCON_OutPutBuff_Struct* getSendBuff(void);
//static void freeSendBuff(void* frame );
static void StopAutoRetrans (APPCON_OutPutBuff_Struct* OutPut);
static void SetModuleToIdle (void);
static SystemErrName SendBuff(APPCON_OutPutBuff_Struct* p);
static void WaitTimeOut(void *p_tmr, void *p_arg);
/*********************************************************************************
���ܴ��붨����
*********************************************************************************/
/*********************************************************************************
 Function:      //
 Description:   //Э���ʼ��
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
 APPCONS_Module.WaitTime = SofTimerCreate ( 30,                   //����
                                            TimerTB_1S,           //ʱ��
                                            SofTimer_PERIODIC,    //ģʽ
                                            WaitTimeOut,          //�ص�
                                            P_NULL);              //�ص�����
}
/*********************************************************************************
 Function:      //
 Description:   // ģ���������ݽ���
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
 Description:   // ģ���������ݽ���
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
 //��������  �ȶ�ʶ��
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
  //ֱ�� ���в��������� 
  case DELNET:    //ɾ���������ɾ��ģ�飬·��������
  //case RDNODE:    //��������ڵ����߱�ģ��ID ������
  case RDROUTER:  //��������ڵ�����·��ģ��ID ������
  case FREQ:      //��дƵ�ʲ�������
  case NETID:     //��д����ID ����
  case BATCH:     //������Ⱥ������ 
  case STATUS:    //״̬��ѯ����
  case RATE:      //��д������������������
  case VER:       //��������Ӳ���汾����
  case HELP:     //��������
  case TXD:      //���������ƻ�ͨѶ����
  case SETID:    //��������������ID �������� 
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
  //ֱ�ӽڵ��Ʋ���������
  case CMD:    //��λ��������ģ��ģ��ͨѶ����  ʵʱ����
  case TST:    //��������ģ������Ϳط�����
  case MRATE:  //��д����ģ�鴮�����ʲ�������
  case RDNODE:    //��������ڵ����߱�ģ��ID ������  
    {
      if(((0x01 == Cmd->CSR.Bit.ack1)&&(AnsDatas_APPCON == AckType))
         ||(TOTAL_APPCON == AckType))
      {
        SetModuleToIdle( );
        if(P_NULL != APPCONS_Module.LastCmd->AckFunc)  
        {APPCONS_Module.LastCmd->AckFunc(APPCONS_Module.LastCmd,AckType,Data);}
        SofTimerRset(APPCONS_Module.WaitTime);
      }
      else if(OK_APPCON == AckType)  //����������ȷ�յ�����
      {
        Cmd->CSR.Bit.ack1 = 0x01;
        StopAutoRetrans (Cmd);
        SofTimerRset(APPCONS_Module.WaitTime);
        SofTimer_ChangePeriod (APPCONS_Module.WaitTime,   //���ݵĶ�ʱ��
                               30,                        //�µ�����
                               1);                        //�Ƿ�λ��ʱ�� 1:���¼�ʱ  0:������ʱ
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
 Description:   //ɾ���������ɾ��ģ�飬·��������
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
 Description:   //��������ģ������Ϳط�����
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
 Description:   //����ģ��ط����� 
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
 Description:   //��������ڵ����߱�ģ��ID ������
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
 Description:   //��������ڵ�����·��ģ��ID ������
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
 Description:   //��дƵ�ʲ�������
 Input:         //dir   0x00�� ��ȡƵ�ʲ���   0x01-0xff��д��Ƶ�ʲ���
                //f Ƶ��ֵ
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
 Description:   //��д����ID ����
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
 Description:   //�������������� 
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
 Description:   //״̬��ѯ����
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
 Description:   //��д������������������
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
 Description:   //��д����ģ�鴮�����ʲ�������
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
 Description:   //��������Ӳ���汾����
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
 Description:   //��������
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
 Description:   //���������ƻ�ͨѶ����
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
 Description:   //��������������ID ��������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //SETID _����(3 �ֽ�)_Ƶ��(3 �ֽ�)_���� ID(2 �ֽ�)�L
*********************************************************************************/
SystemErrName  APPCON_SETID_SendFunc (unsigned long command ,//����  
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
 Description:   //����Ӧ��ʱ������ ȡ���ȴ�
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
  
  err = MXPC_Send_AutoRetrans( Rs485_PortD,            //Ŀ�Ķ˿�
                               (OutPut->Buff),         //���ݻ�����
                               OutPut->BuffLength,     //���ݳ���
                               2,                      //�ش�����
                               10,                     //�ش�������
                               TimerTB_100MS,          //�ش�����
                               P_NULL,                 //������ɻص�
                               APPCON_AckTimeOut);     //�ط���ɻص�  
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
 Description:   //ASCII�봮��ʾ��16���Ʊ任Ϊʵ��
 Input:         //hex�� ASCII�봮�׵�ַ
                //length��ASCII�봮����
                  width��������ʵ��16������Ԫ��
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
