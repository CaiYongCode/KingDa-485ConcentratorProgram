/*********************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include ".\KingDaV1.h"
#include ".\CRC8.h"
#include "..\APPCON\APPCON.h"
/*********************************************************************************
����������
*********************************************************************************/
const unsigned char  CMD_OK1[2]={'O','K'};
/*********************************************************************************
��������������
*********************************************************************************/

/*********************************************************************************
�ⲿ����������
*********************************************************************************/
//extern TimerData_Struct  SystemTimer;
/*********************************************************************************
˽�б���������
*********************************************************************************/ 

PProtocolSendBuff_Struct  KingDaV1send_buff[PProtoclSendBuff_Number];
static CommPortNumber_Type InPort_PV;

static CmdCode_KindgDaSPCP  LastCmd = NullCmd_KindgDaSPCP;
/*********************************************************************************
���Ա���������
*********************************************************************************/

/*********************************************************************************
�ڲ�����������
*********************************************************************************/
static void DeInitBuff_PProtocol(void);
static PProtocolSendBuff_Struct* GetIdleBuff_PProtocol (void);
static SystemErrName Protocol_frameVerify ( void *pbuff, 
                                             unsigned char length, 
                                             unsigned char* Preamble_OP,
                                             KindgDaSPCP_PackageDataStruct** data);
static void ReadValue_func(KindgDaSPCP_PackageDataStruct *pbuff);
static void ReadData_func (KindgDaSPCP_PackageDataStruct* pbuff );
static void ReadDDRS_func (KindgDaSPCP_PackageDataStruct *pbuff);
static void ReadDDR2_func (KindgDaSPCP_PackageDataStruct *pbuff);
static void WriteDDR2_func(KindgDaSPCP_PackageDataStruct *pbuff);
static void WriteDDR1_func(KindgDaSPCP_PackageDataStruct *pbuff);
static void Open_OV_func(KindgDaSPCP_PackageDataStruct* pbuff);         //����   
static void Close_OV_func(KindgDaSPCP_PackageDataStruct* pbuff);        //�ط�      
static void Get_OV_func(KindgDaSPCP_PackageDataStruct* pbuff);          //����״̬
static void Open_OV_ENF_func(KindgDaSPCP_PackageDataStruct* pbuff);     //ǿ�ƿ���
static void Close_OV_ENF_func(KindgDaSPCP_PackageDataStruct* pbuff);    //ǿ�ƹط�  
static void MT_OV_func(KindgDaSPCP_PackageDataStruct* pbuff);           //�������   
static void MT_OV_ALL_func(KindgDaSPCP_PackageDataStruct* pbuff);       //�㲥������� 
static void ReadId_Concentrator_func(KindgDaSPCP_PackageDataStruct* pbuff);  
static void WriteId_Concentrator_func(KindgDaSPCP_PackageDataStruct* pbuff); //
static void ReadTime_Concentrator_func(KindgDaSPCP_PackageDataStruct* pbuff);    //
static void WriteTime_Concentrator_func(KindgDaSPCP_PackageDataStruct* pbuff);
static void ReadCycle_Concentrator_func(KindgDaSPCP_PackageDataStruct* pbuff);
static void WriteCycle_Concentrator_func(KindgDaSPCP_PackageDataStruct* pbuff);
static void ReadChannel_Concentrator_func  (KindgDaSPCP_PackageDataStruct* pbuff); //��ȡ��������ͨ����ʼ�����м�����
static void WriteChannel_Concentrator_func (KindgDaSPCP_PackageDataStruct* pbuff);//���ü�������ͨ���м�����ʼ������
static void ReadIpEnd_Concentrator_func  (KindgDaSPCP_PackageDataStruct* pbuff);   //��ȡ�Ƿ����������ӷ�����
static void WriteIpEnd_Concentrator_func (KindgDaSPCP_PackageDataStruct* pbuff);  //���ÿ������ر�ʹ���������ӷ�����  
static void ReadServerInf_Concentrator_func  (KindgDaSPCP_PackageDataStruct* pbuff);  //��ȡ������� 
static void WriteServerInf_Concentrator_func (KindgDaSPCP_PackageDataStruct* pbuff);  //�����������
static void ReadServer_Concentrator_func  (KindgDaSPCP_PackageDataStruct* pbuff);  //��ȡ������������Ϣ  
static void WriteServer_Concentrator_func (KindgDaSPCP_PackageDataStruct* pbuff);//���ü���������
static void ReadID_Repeaters_func (KindgDaSPCP_PackageDataStruct* pbuff); //��ȡ�м�����
static void WriteID_Repeaters_func (KindgDaSPCP_PackageDataStruct* pbuff); //�����м�����                
static void OPen_Repeaters_func (KindgDaSPCP_PackageDataStruct* pbuff);  //���м�������            
static void Close_Repeaters_func (KindgDaSPCP_PackageDataStruct* pbuff);  //�ر��м�������ʹ�м���˯��
static void ReadChannelNodeNumber_Repeaters_func (KindgDaSPCP_PackageDataStruct* pbuff);   //��ȡ�м���N�¸���ͨ��ˮ������
static void WriteChannelNodeNumber_Repeaters_func (KindgDaSPCP_PackageDataStruct* pbuff);  //�����м�����ͨ��ˮ������
static SystemErrName PProtocol_send(CommPortNumber_Type port, PProtocolSendBuff_Struct *pbuff);//, unsigned char length);
static CommPortNumber_Type getPortForAddr ( unsigned char addr);
static unsigned short  BinChangeToBCD (unsigned char bin);
static void PProtocol_ACK_TimerOutCallBack (void *pbuff);
static void PProtocol_SendEnd_CallBack(CommPortNumber_Type port, void *pbuff);
static SystemErrName FreeBuff_PProtocol(PProtocolSendBuff_Struct* pbuff);
static void ReadGprsRssi_func (KindgDaSPCP_PackageDataStruct* pbuff); //��ȡ�ź�ǿ��
//����ͨ���߲�������  
static void DeleteNet_APPCON_func (KindgDaSPCP_PackageDataStruct* pbuff);  //ɾ����������
static void BATCH_APPCON_func (KindgDaSPCP_PackageDataStruct* pbuff);    //ά�����粢��������
static void STATUS_APPCON_func (KindgDaSPCP_PackageDataStruct* pbuff);    //����״̬��ѯ����
static void ReadNetId_APPCON_func (KindgDaSPCP_PackageDataStruct* pbuff);   //��ȡ����ID����
static void WriteNetId_APPCON_func (KindgDaSPCP_PackageDataStruct* pbuff);   //д����ID����
static void RDNODE_APPCON_func (KindgDaSPCP_PackageDataStruct* pbuff);   //��ȡ���������߽ڵ�ID������
static void GetROUTER_APPCON_func (KindgDaSPCP_PackageDataStruct* pbuff);
static void ReadFreq_APPCON_func (KindgDaSPCP_PackageDataStruct* pbuff);   //��Ƶ�ʲ�������
static void WriteFreq_APPCON_func (KindgDaSPCP_PackageDataStruct* pbuff);   //дƵ�ʲ�������
static void WriteRate_APPCON_func (KindgDaSPCP_PackageDataStruct* pbuff);   //д������������������
static void ReadNodeValue_APPCON_func (KindgDaSPCP_PackageDataStruct* pbuff);  //��ȡ���߱�������
static void OpenOV_APPCON_func (KindgDaSPCP_PackageDataStruct* pbuff);   //����
static void HalfOV_APPCON_func (KindgDaSPCP_PackageDataStruct* pbuff);   //�뿪����
static void MtOV_APPCON_func (KindgDaSPCP_PackageDataStruct* pbuff);   //�������
static void CloseOV_APPCON_func (KindgDaSPCP_PackageDataStruct* pbuff);   //�ط�
static void GetOV_APPCON_func (KindgDaSPCP_PackageDataStruct* pbuff);   //����״̬
/*********************************************************************************
���ܴ��붨����
*********************************************************************************/
void KingDaV1Protocol_DeInit(void)
{
  InPort_PV = Null_Port;
  DeInitBuff_PProtocol( );
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
CommPortNumber_Type getPortForAddr ( unsigned char addr)
{
  CommPortNumber_Type port;
  if((addr >= Concentrator.channel[0].min)&&(addr <= Concentrator.channel[0].max))
  {port = Rs485_PortA;}
  else if((addr >= Concentrator.channel[1].min)&&(addr <= Concentrator.channel[1].max))
  {port = Rs485_PortB;}
  else if((addr >= Concentrator.channel[2].min)&&(addr <= Concentrator.channel[2].max))
  {port = Rs485_PortC;}
  else if((addr >= Concentrator.channel[3].min)&&(addr <= Concentrator.channel[3].max))
  {port = Rs485_PortD;}
  else if((addr >= 64)&&(addr <= 99))
  {port = Rs485_PortD;}
  else if(CoercePort_1CH == addr)
  {port = Rs485_PortA;}
  else if(CoercePort_2CH == addr)
  {port = Rs485_PortB;}
  else if(CoercePort_3CH == addr)
  {port = Rs485_PortC;}
  else if(CoercePort_4CH == addr)
  {port = Rs485_PortD;}
  else {port = Null_Port;}
  return port;
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
SystemErrName SendCmd_KindDaSPCP(KindgDaCmd_Struct* Cmd)                        
{
  SystemErrName err;
  unsigned char i;
  unsigned crc;
  unsigned char* tc;
  unsigned char* tp;
  PProtocolSendBuff_Struct*  Buff;
  Buff = GetIdleBuff_PProtocol ( );
  if(P_NULL  == Buff)
  {return Nothing;}
  Buff->AckFunc = Cmd->AckFunc;
  Buff->SourcePort = Cmd->SourcePort;
  Buff->CCR.Bit.Preamble = Cmd->CSR.Bit.Preamble; 
  if(UP == Cmd->CSR.Bit.DIR)
  {
   Buff->buff.Prm1 = PProtocl_Preamble_Up;
   Buff->buff.Prm2 = PProtocl_Preamble_Up;
   Buff->buff.Data.TypeA.head.Sync= PProtocl_Sync_Up;
  }
  else
  {
   Buff->buff.Prm1 = PProtocl_Preamble_Down;
   Buff->buff.Prm2 = PProtocl_Preamble_Down;
   Buff->buff.Data.TypeA.head.Sync= PProtocl_Sync_Down;
  }
  Buff->buff.Data.TypeA.head.Cmd = Cmd->Cmd;
  Buff->buff.Data.TypeA.head.Length =  Cmd->length; 
  tp = (unsigned char*) &(Buff->buff.Data.TypeA.pram);
  tc = Cmd->Param;
  i =0;
  while(i < Cmd->length)
  {
    tp[i] = tc[i];
    i++;
  }
  i = sizeof (PPro_PackageHead_TypeA) + Cmd->length;
  crc = CRC8((unsigned char *) &(Buff->buff.Data.TypeA.head),i);  
  ((unsigned char *) &(Buff->buff.Data.TypeA.head))[i] = crc;
  Buff->TargetPort = getPortForAddr(Cmd->TargetPort);
  err = PProtocol_send (Buff->TargetPort, Buff);
  if(NO_ERR != err)
  {FreeBuff_PProtocol(Buff);}
  return err;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        // 
                             
Others:         // ��Ϊ��֧����ָ���ִ�� ����ֻ����һ��  ������������
*********************************************************************************/
PProtocolSendBuff_Struct* getSendBuffForAckCmd (CmdCode_KindgDaSPCP cmd)
{
  unsigned char i =0;
  while (i < PProtoclSendBuff_Number)
  {
    if((0 == KingDaV1send_buff[i].CCR.Bit.idle) && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == cmd))
    {
      if(KingDaV1send_buff[i].TargetPort == InPort_PV)
      {return &KingDaV1send_buff[i];}
    }
    i++;
  }
  return P_NULL;
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
SystemErrName Transpond_KingDaSPCP (Transpond_KingDaSPCP_struct* data)
{
  const unsigned char list[] ={Open_OV,Close_OV,Get_OV,Open_OV_ENF,Close_OV_ENF,MT_OV,MT_OV_ALL};  
 SystemErrName err = NO_ERR; 
 unsigned char i;
 unsigned char j;
 unsigned char* tp;
 unsigned char* tc;
 PProtocolSendBuff_Struct*  buff;
 buff = GetIdleBuff_PProtocol ( );
 if(P_NULL == buff)
 {return Nothing;}
 buff->CCR.Bit.Preamble =data->CSR.Bit.Preamble;
 buff->SourcePort = data->SourcePort;
 buff->TargetPort = data->TargetPort; 
 tc = (unsigned char*)data->pbuff;
 tp = (unsigned char*)&buff->buff.Data.TypeA.head;
 
 j = 0;
 while ( j < 7)
 {
   if(data->pbuff->TypeA.head.Cmd == list[j])
   {break;}
   j ++;
 }
 if(j < 7)
 {  j = 8; }
 else
 {j = sizeof (PPro_PackageHead_TypeA) + 1 + data->pbuff->TypeA.head.Length;}
 i = 0;
 while (i < j)
 {
   tp[i] = tc[i];
   i++;
 }

 if(0 != buff->CCR.Bit.Preamble)
 {
  if(PProtocl_Sync_Down == buff->buff.Data.TypeA.head.Sync) 
  {
   buff->buff.Prm1 = PProtocl_Preamble_Down;
   buff->buff.Prm2 = PProtocl_Preamble_Down;
  }
  else
  {
   buff->buff.Prm1 = PProtocl_Preamble_Up;
   buff->buff.Prm2 = PProtocl_Preamble_Up;
   }
   j +=  2 ;
   tp = (unsigned char*)&(buff->buff);
 }
if(PProtocl_Sync_Down == buff->buff.Data.TypeA.head.Sync) 
{
  err = MXPC_Send_AutoRetrans ( buff->TargetPort,             //Ŀ�Ķ˿�
                         tp,                                  //���ݻ�����
                         j,                                   //���ݳ���
                         2,                                   //�ش�����
                         15,                                 //�ش�������
                         TimerTB_100MS ,                     //�ش�����
                         P_NULL ,                            //������ɻص�
                         PProtocol_ACK_TimerOutCallBack);    //�ط���ɻص� 
}
else
{
 err = MXPC_Send_AutoRetrans ( buff->TargetPort,             //Ŀ�Ķ˿�
                         tp,                                 //���ݻ�����
                         j,                                  //���ݳ���
                         0,                                  //�ش�����
                         0,                                  //�ش�������
                         TimerTB_100MS ,                     //�ش�����
                         PProtocol_SendEnd_CallBack,         //������ɻص�
                         P_NULL);  //�ط���ɻص�  
}
if(NO_ERR != err)
{FreeBuff_PProtocol(buff);}
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
SystemErrName KingDaV1Protocol_OK_Ackfunc (KindgDaSPCP_PackageDataStruct* package)
{
  const CmdCode_KindgDaSPCP CmdList[3] = {OPen_Repeaters, Close_Repeaters, WriteChannelNodeNumber_Repeaters};
  
  unsigned char  i;
  unsigned char  j;
  i = 0;
  while (i < PProtoclSendBuff_Number)
  {
   j = 0;
   if((0 == KingDaV1send_buff[i].CCR.Bit.idle) && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
   {
    while (j < 3)
    {
      if(KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == CmdList[j])
      {break;}
      j ++;
    }
    if (j < 3)
    {
     if(P_NULL != KingDaV1send_buff[i].AckFunc)
     {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), package);} // Ӧ�����ݴ������
     else
     {
     if(Null_Port!= KingDaV1send_buff[i].SourcePort)
      {
       MXPC_Send_AutoRetrans ( KingDaV1send_buff[i].SourcePort,      //Ŀ�Ķ˿�
                              (void*)CMD_OK1,                //���ݻ�����
                               2,                            //���ݳ���
                               0,                            //�ش�����
                               0,                            //�ش�������
                              TimerTB_10MS ,                 //�ش�����
                              P_NULL,                        //������ɻص�
                              P_NULL);                       //�ط���ɻص� 
      }
    FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //�ͷŻ����� 
    return NO_ERR;
    }
    FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //�ͷŻ����� 
   }
  }
  i ++;
 }
 return OperateCmd_err;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        // 1�����ȴ���
                   2: ֡ͷͬ������
                   3��ͬ���ִ���
                   4��CRC У����� 
                   5: ��ʶ������
Others:         // 
*********************************************************************************/
SystemErrName KingDaV1Protocol_input (void *pbuff, unsigned char length, CommPortNumber_Type port)
{
 SystemErrName err;
 unsigned  char Preamble_flg = 0; 
 KindgDaSPCP_PackageDataStruct *package;
 InPort_PV  = port;
 if(('O' == ((unsigned char*)pbuff)[0]) && ('K' == ((unsigned char*)pbuff)[1]))
 {return KingDaV1Protocol_OK_Ackfunc (pbuff);}
 err =  Protocol_frameVerify (pbuff,length, &Preamble_flg, &package); 
 if(NO_ERR != err)
 {return err;}
 
 if(Rs232_Port == port)
 {
   Concentrator.CSR.Bit.Debug  = 1;
   Concentrator_DebugTimer = 300;
 }
 
 switch( package->TypeA.head.Cmd )
 {
  case ReadValue:
   {ReadValue_func (package); break;}
  case ReadDDRS:
   {ReadDDRS_func (package);  break;}
  case ReadDDR2:
   {ReadDDR2_func (package);  break;}
  case WriteDDR2:
   {WriteDDR2_func (package); break;}
  case WriteDDR1:
   {WriteDDR1_func (package); break;}
  case Open_OV:        //����
   {Open_OV_func (package); break;}   
  case Close_OV:       //�ط�
   {Close_OV_func (package); break;}
  case Get_OV:         //����״̬
   {Get_OV_func (package); break;}   
  case Open_OV_ENF:    //ǿ�ƿ���
   {Open_OV_ENF_func (package); break;}
  case Close_OV_ENF:  //ǿ�ƹط�
   {Close_OV_ENF_func (package); break;}
  case MT_OV:         //�������
   {MT_OV_func (package); break;}
  case MT_OV_ALL:     //�㲥������� 
   {MT_OV_ALL_func (package); break;}
  case ReadId_Concentrator:        //���������� 
   {ReadId_Concentrator_func (package); break;} 
  case WriteId_Concentrator:       //���ü������� 
   {WriteId_Concentrator_func (package); break;} 
  case ReadTime_Concentrator:     //��ȡ��ǰʱ�� 
   {ReadTime_Concentrator_func (package); break;}   
  case WriteTime_Concentrator:     //��ȡ��ǰʱ�� 
   {WriteTime_Concentrator_func (package); break;}   
  case ReadCycle_Concentrator:     //��ȡ�Զ�����ʱ���� 
   {
     if((0xAB == package->TypeA.pram.ReadCycle_ask.DI1)||(0xB6 == package->TypeA.pram.ReadCycle_ask.DI2))
     {ReadCycle_Concentrator_func (package);} //��ȡ�Զ�����ʱ���� 
     else
     {ReadID_Repeaters_func (package);} //��ȡ�м�����
     break;
   }  
  case WriteCycle_Concentrator:                          //�����Զ�����ʱ���� 
   {WriteCycle_Concentrator_func (package); break;}
  case  ReadChannel_Concentrator:                       //��ȡ��ͨ����ʼ�����м�����
    {ReadChannel_Concentrator_func (package); break;}    
  case  WriteChannel_Concentrator:                      //���ü�������ͨ���м�����ʼ������
    {WriteChannel_Concentrator_func (package); break;}
  case  ReadIpEnd_Concentrator:                         //��ȡ�Ƿ����������ӷ�����
    {ReadIpEnd_Concentrator_func (package); break;} 
  case  WriteIpEnd_Concentrator:                        //���ÿ������ر�ʹ���������ӷ�����
    {WriteIpEnd_Concentrator_func (package); break;}   
  case  ReadServerInf_Concentrator:                     //��ȡ�������
    {ReadServerInf_Concentrator_func (package); break;} 
  case  WriteServerInf_Concentrator:                     //�����������
    {WriteServerInf_Concentrator_func (package); break;}  
  case  ReadServerI_Concentrator:                        //��ȡ������������Ϣ 
  case  ReadServerII_Concentrator:                       //��ȡ����������Ϣ 
     {ReadServer_Concentrator_func (package);break;}
  case  WriteServerI_Concentrator:                       //���ü���������
  case  WriteServerII_Concentrator:                       //���ñ������� 
     {WriteServer_Concentrator_func (package); break;}
  case  WriteID_Repeaters:                               //�����м�����
    {WriteID_Repeaters_func (package); break;}
  case  OPen_Repeaters:                                  //���м�������
    {OPen_Repeaters_func (package); break;}
  case  Close_Repeaters:                                 //�ر��м�������ʹ�м���˯��
    {Close_Repeaters_func (package); break;}
  case  ReadChannelNodeNumber_Repeaters:                 //��ȡ�м���N�¸���ͨ��ˮ������
    {
      if(0 == Preamble_flg)
      {ReadChannelNodeNumber_Repeaters_func (package);}
      else
      {ReadData_func (package);}  
      break;
    }
  case  WriteChannelNodeNumber_Repeaters:                //�����м�����ͨ��ˮ������
    {
     if(2 == package->TypeA.head.Length)
     {ReadGprsRssi_func (package);}
     else
     {WriteChannelNodeNumber_Repeaters_func (package); } 
     break;
    }   
   //����ͨ���߲�������   
  case  DeleteNet_APPCON :  //ɾ����������
    {DeleteNet_APPCON_func (package); break;} 
  case  BATCH_APPCON :   //ά�����粢��������
    {BATCH_APPCON_func (package); break;}
  case  STATUS_APPCON :   //����״̬��ѯ����
    {STATUS_APPCON_func (package); break;}
  case  ReadNetId_APPCON :  //��ȡ����ID����
    {ReadNetId_APPCON_func (package); break;}
  case  WriteNetId_APPCON :  //д����ID����
    {WriteNetId_APPCON_func (package); break;}
  case  RDNODE_APPCON :  //��ȡ���������߽ڵ�ID������
    {RDNODE_APPCON_func (package); break;}
  case  ReadFreq_APPCON :  //��Ƶ�ʲ�������
    {ReadFreq_APPCON_func (package); break;}
  case  WriteFreq_APPCON :  //дƵ�ʲ�������
    {WriteFreq_APPCON_func (package); break;}
  case  WriteRate_APPCON :  //д������������������
    {WriteRate_APPCON_func (package); break;}
  case  ReadNodeValue_APPCON :  //��ȡ���߱�������
    {ReadNodeValue_APPCON_func (package); break;}
  case  OpenOV_APPCON :  //����
    {OpenOV_APPCON_func (package); break;}
  case  HalfOV_APPCON :  //�뿪����
    {HalfOV_APPCON_func (package); break;}
  case  MtOV_APPCON :  //�������
    {MtOV_APPCON_func (package); break;}
  case  CloseOV_APPCON :  //�ط�
    {CloseOV_APPCON_func (package); break;}
  case  GetOV_APPCON :  //����״̬
    {GetOV_APPCON_func (package); break;}
  case  RDROUTER_APPCON :  //����״̬
    {GetROUTER_APPCON_func (package); break;}  
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
void DeInitBuff_PProtocol(void)
{
  unsigned char itemp =0;
  while ( itemp < PProtoclSendBuff_Number)
  {
   KingDaV1send_buff[itemp].CCR.Byte = 0; 
   KingDaV1send_buff[itemp].CCR.Bit.idle = 1;
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
static SystemErrName PProtocolBuffVerify (PProtocolSendBuff_Struct* pbuff) 
{
 unsigned char itemp = 0;
 while (itemp < PProtoclSendBuff_Number)
 {
   if(pbuff == &(KingDaV1send_buff[itemp]))
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
static PProtocolSendBuff_Struct* GetIdleBuff_PProtocol (void)
{
  unsigned char itemp =0;
  while ( itemp < PProtoclSendBuff_Number)
  {
    if(KingDaV1send_buff[itemp].CCR.Bit.idle)
    {
     KingDaV1send_buff[itemp].CCR.Byte = 0; 
     //send_buff[itemp].CCR.Bit.idle = 0;
     KingDaV1send_buff[itemp].AckFunc = P_NULL;
     KingDaV1send_buff[itemp].SourcePort = Null_Port;
     KingDaV1send_buff[itemp].TargetPort = Null_Port;
     return &(KingDaV1send_buff[itemp]);
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
static SystemErrName FreeBuff_PProtocol(PProtocolSendBuff_Struct* pbuff)
{
  if(NO_ERR == PProtocolBuffVerify (pbuff))
  {
   if(0 != pbuff->CCR.Bit.Preamble)
   { FreeOutputBuffForData_MXPC (&(pbuff->buff)); }
   else
   { FreeOutputBuffForData_MXPC (&(pbuff->buff.Data.TypeA.head));}
   pbuff->CCR.Byte = 0; 
   pbuff->CCR.Bit.idle = 1;
   return NO_ERR ;
  }
  return IllegalParam ; 
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        // 1�����ȴ���
                   2: ֡ͷͬ������
                   3��ͬ���ִ���
                   4��CRC У����� 
                   5: ��ʶ������
Others:         // 
*********************************************************************************/
static SystemErrName Protocol_frameVerify ( void *pbuff, 
                                             unsigned char length, 
                                             unsigned char* Preamble_OP,
                                             KindgDaSPCP_PackageDataStruct** data)
{
 const unsigned char list[] ={Open_OV,Close_OV,Get_OV,Open_OV_ENF,Close_OV_ENF,MT_OV,MT_OV_ALL}; 
 union {
   unsigned char* p;
   PPro_PackageHead_TypeA  *headp;
 }buff;
 unsigned char itemp;
 unsigned char itemp2;
 itemp = 0;
 buff.p = pbuff;
 itemp2 = 0;
 while (itemp < length)
 {
   if((PProtocl_Preamble_Down  != *(buff.p)) && (PProtocl_Preamble_Up != *(buff.p)))
   {break;} 
   buff.p ++;
   itemp2 &= 0x0F;
   itemp2 ++;
 }
 if(itemp2 >= 2)
 {*Preamble_OP = 1;}
 else
 {*Preamble_OP = 0;}
 
 //if(itemp >= length)
 //{ return FramePreamble_err;}

 if(length < sizeof(PPro_PackageHead_TypeA))
 {return Length_err;}

 if((PProtocl_Sync_Down != buff.headp->Sync)&&(PProtocl_Sync_Up != buff.headp->Sync))
 {return FrameSync_err;}
 if(length < sizeof(PPro_PackageHead_TypeA) + buff.headp->Length + 1)
 {return Length_err;}

 itemp = sizeof (PPro_PackageHead_TypeA);
 itemp2 = 0;
 while ( itemp2 < 7)
 {
   if(buff.headp->Cmd == list[itemp2])
   {break;}
   itemp2 ++;
 }
 if(itemp2 < 7)
 { itemp += 4;}
 else
 {itemp += buff.headp->Length;}
 pbuff = &(buff.headp->Sync); 
 if(((unsigned char *)pbuff)[itemp] != CRC8((unsigned char *) pbuff,itemp)) 
 {return Verify_err;}
 
 *data = (KindgDaSPCP_PackageDataStruct*)buff.headp;
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
static void ReadValue_func (KindgDaSPCP_PackageDataStruct *pbuff) 
{
 unsigned char i;
 Transpond_KingDaSPCP_struct  Trans;
 
 if(PProtocl_Sync_Up == pbuff->TypeA.head.Sync)
 {
  i = 0;
  while (i < PProtoclSendBuff_Number)
  {
   if((0 == KingDaV1send_buff[i].CCR.Bit.idle)
      && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == ReadValue) 
      && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
   {
     if((KingDaV1send_buff[i].buff.Data.TypeA.pram.ReadValue_ask.addr[0] == pbuff->TypeA.pram.ReadValue_ack.addr[0]) 
        &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.ReadValue_ask.addr[1] == pbuff->TypeA.pram.ReadValue_ack.addr[1])
        &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.ReadValue_ask.addr[2] == pbuff->TypeA.pram.ReadValue_ack.addr[2]) )
     {break;}
   }
   i ++;
  }
  if(i < PProtoclSendBuff_Number)
  {
   if(P_NULL != KingDaV1send_buff[i].AckFunc)
   {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), pbuff);} // Ӧ�����ݴ������
   else
   {
    if(Null_Port!= KingDaV1send_buff[i].SourcePort)
    {
      Trans.CSR.Bit.Preamble =  1;
      Trans.pbuff = pbuff;
      Trans.SourcePort = Null_Port;
      Trans.TargetPort = KingDaV1send_buff[i].SourcePort;
      Transpond_KingDaSPCP (&Trans); 
    }
   }
   FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //�ͷŻ����� 
  }  
 }
 else
 {
   Trans.CSR.Bit.Preamble =  1;
   Trans.pbuff = pbuff;
   Trans.SourcePort = InPort_PV;
   Trans.TargetPort = getPortForAddr (pbuff->TypeA.pram.ReadValue_ask.addr[0]);
   Transpond_KingDaSPCP (&Trans); 
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
static void ReadData_func (KindgDaSPCP_PackageDataStruct* pbuff )
{
 unsigned char i;
 Transpond_KingDaSPCP_struct  Trans;
 
 if(PProtocl_Sync_Up == pbuff->TypeA.head.Sync)
 {
  i = 0;
  while (i < PProtoclSendBuff_Number)
  {
   if((0 == KingDaV1send_buff[i].CCR.Bit.idle)
      && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == ReadData) 
      && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
   {
     if((KingDaV1send_buff[i].buff.Data.TypeA.pram.ReadData_ask.addr[0] == pbuff->TypeA.pram.ReadData_ack.addr[0]) 
        &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.ReadData_ask.addr[1] == pbuff->TypeA.pram.ReadData_ack.addr[1])
        &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.ReadData_ask.addr[2] == pbuff->TypeA.pram.ReadData_ack.addr[2]) )
     {break;}
   }
   i ++;
  }
  if(i < PProtoclSendBuff_Number)
  {
   if(P_NULL != KingDaV1send_buff[i].AckFunc)
   {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), pbuff);} // Ӧ�����ݴ������
   else
   {
    if(Null_Port!= KingDaV1send_buff[i].SourcePort)
    {
      Trans.CSR.Bit.Preamble =  1;
      Trans.pbuff = pbuff;
      Trans.SourcePort = Null_Port;
      Trans.TargetPort = KingDaV1send_buff[i].SourcePort;
      Transpond_KingDaSPCP (&Trans); 
    }
   }
   FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //�ͷŻ����� 
  }  
 }
 else
 {
   Trans.CSR.Bit.Preamble =  1;
   Trans.pbuff = pbuff;
   Trans.SourcePort = InPort_PV;
   Trans.TargetPort = getPortForAddr (pbuff->TypeA.pram.ReadData_ask.addr[0]);
   Transpond_KingDaSPCP (&Trans); 
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
static void ReadDDRS_func (KindgDaSPCP_PackageDataStruct* pbuff )
{
  unsigned char i;
 Transpond_KingDaSPCP_struct  Trans;
 
 if(PProtocl_Sync_Up == pbuff->TypeA.head.Sync)
 {
  i = 0;
  while (i < PProtoclSendBuff_Number)
  {
   if((0 == KingDaV1send_buff[i].CCR.Bit.idle)
      && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == ReadDDRS) 
      && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
   {
     if((KingDaV1send_buff[i].buff.Data.TypeA.pram.ReadValue_ask.addr[0] == pbuff->TypeA.pram.ReadValue_ack.addr[0]) 
        &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.ReadValue_ask.addr[1] == pbuff->TypeA.pram.ReadValue_ack.addr[1])
        &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.ReadValue_ask.addr[2] == pbuff->TypeA.pram.ReadValue_ack.addr[2]) )
     {break;}
   }
   i ++;
  }
  if(i < PProtoclSendBuff_Number)
  {
   if(P_NULL != KingDaV1send_buff[i].AckFunc)
   {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), pbuff);} // Ӧ�����ݴ������
   else
   {
    if(Null_Port!= KingDaV1send_buff[i].SourcePort)
    {
      Trans.CSR.Bit.Preamble =  1;
      Trans.pbuff = pbuff;
      Trans.SourcePort = Null_Port;
      Trans.TargetPort = KingDaV1send_buff[i].SourcePort;
      Transpond_KingDaSPCP (&Trans); 
    }
   }
   FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //�ͷŻ����� 
  }  
 }
 else
 {
   Trans.CSR.Bit.Preamble =  1;
   Trans.pbuff = pbuff;
   Trans.SourcePort = InPort_PV;
   Trans.TargetPort = get_RS458ChannelNumber();
   Transpond_KingDaSPCP (&Trans); 
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
static void ReadDDR2_func (KindgDaSPCP_PackageDataStruct* pbuff)
{ 
  unsigned char i;
 Transpond_KingDaSPCP_struct  Trans;
 
 if(PProtocl_Sync_Up == pbuff->TypeA.head.Sync)
 {
  i = 0;
  while (i < PProtoclSendBuff_Number)
  {
   if((0 == KingDaV1send_buff[i].CCR.Bit.idle)
      && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == ReadDDR2) 
      && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
   {
     if((KingDaV1send_buff[i].buff.Data.TypeA.pram.ReadDDR2_ask.DDR1[0] == pbuff->TypeA.pram.ReadDDR2_ack.DDR1[0]) 
        &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.ReadDDR2_ask.DDR1[1] == pbuff->TypeA.pram.ReadDDR2_ack.DDR1[1])
        &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.ReadDDR2_ask.DDR1[2] == pbuff->TypeA.pram.ReadDDR2_ack.DDR1[2])
        &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.ReadDDR2_ask.DDR1[3] == pbuff->TypeA.pram.ReadDDR2_ack.DDR1[3]))
     {break;}
   }
   i ++;
  }
  if(i < PProtoclSendBuff_Number)
  {
   if(P_NULL != KingDaV1send_buff[i].AckFunc)
   {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), pbuff);} // Ӧ�����ݴ������
   else
   {
    if(Null_Port!= KingDaV1send_buff[i].SourcePort)
    {
      Trans.CSR.Bit.Preamble =  1;
      Trans.pbuff = pbuff;
      Trans.SourcePort = Null_Port;
      Trans.TargetPort = KingDaV1send_buff[i].SourcePort;
      Transpond_KingDaSPCP (&Trans); 
    }
   }
   FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //�ͷŻ����� 
  }  
 }
 else
 {
   Trans.CSR.Bit.Preamble =  1;
   Trans.pbuff = pbuff;
   Trans.SourcePort = InPort_PV;
   Trans.TargetPort = get_RS458ChannelNumber();
   Transpond_KingDaSPCP (&Trans); 
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
static void WriteDDR2_func (KindgDaSPCP_PackageDataStruct* pbuff)
{
 unsigned char i;
 Transpond_KingDaSPCP_struct  Trans;
 
 if(PProtocl_Sync_Up == pbuff->TypeA.head.Sync)
 {
  i = 0;
  while (i < PProtoclSendBuff_Number)
  {
   if((0 == KingDaV1send_buff[i].CCR.Bit.idle)
      && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == WriteDDR2) 
      && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
   {
     if((KingDaV1send_buff[i].buff.Data.TypeA.pram.WriteDDR2_ask.DDR2[0] == pbuff->TypeA.pram.WriteDDR2_ack.DDR2[0]) 
        &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.WriteDDR2_ask.DDR2[1] == pbuff->TypeA.pram.WriteDDR2_ack.DDR2[1])
        &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.WriteDDR2_ask.DDR2[2] == pbuff->TypeA.pram.WriteDDR2_ack.DDR2[2]) )
     {break;}
   }
   i ++;
  }
  if(i < PProtoclSendBuff_Number)
  {
   if(P_NULL != KingDaV1send_buff[i].AckFunc)
   {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), pbuff);} // Ӧ�����ݴ������
   else
   {
    if(Null_Port!= KingDaV1send_buff[i].SourcePort)
    {
      Trans.CSR.Bit.Preamble =  1;
      Trans.pbuff = pbuff;
      Trans.SourcePort = Null_Port;
      Trans.TargetPort = KingDaV1send_buff[i].SourcePort;
      Transpond_KingDaSPCP (&Trans); 
    }
   }
   FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //�ͷŻ����� 
  }  
 }
 else
 {
   Trans.CSR.Bit.Preamble =  1;
   Trans.pbuff = pbuff;
   Trans.SourcePort = InPort_PV;
   Trans.TargetPort = getPortForAddr (pbuff->TypeA.pram.WriteDDR2_ask.DDR2[0]);
   Transpond_KingDaSPCP (&Trans); 
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
static void WriteDDR1_func (KindgDaSPCP_PackageDataStruct* pbuff)
{
  
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
static void Open_OV_func (KindgDaSPCP_PackageDataStruct* pbuff)        //����
{ 
 unsigned char i;
 Transpond_KingDaSPCP_struct  Trans;
 
 if(PProtocl_Sync_Up == pbuff->TypeA.head.Sync)
 {
  i = 0;
  while (i < PProtoclSendBuff_Number)
  {
   if((0 == KingDaV1send_buff[i].CCR.Bit.idle)
      && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == Open_OV) 
      && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
   {
     if((KingDaV1send_buff[i].buff.Data.TypeA.pram.Open_OV_ask.addr[0] == pbuff->TypeA.pram.Open_OV_ack.addr[0]) 
        &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.Open_OV_ask.addr[1] == pbuff->TypeA.pram.Open_OV_ack.addr[1])
        &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.Open_OV_ask.addr[2] == pbuff->TypeA.pram.Open_OV_ack.addr[2]) )
     {break;}
   }
   i ++;
  }
  if(i < PProtoclSendBuff_Number)
  {
   if(P_NULL != KingDaV1send_buff[i].AckFunc)
   {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), pbuff);} // Ӧ�����ݴ������
   else
   {
    if(Null_Port!= KingDaV1send_buff[i].SourcePort)
    {
      Trans.CSR.Bit.Preamble =  1;
      Trans.pbuff = pbuff;
      Trans.SourcePort = Null_Port;
      Trans.TargetPort = KingDaV1send_buff[i].SourcePort;
      Transpond_KingDaSPCP (&Trans); 
    }
   }
   FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //�ͷŻ����� 
  }  
 }
 else
 {
   Trans.CSR.Bit.Preamble =  1;
   Trans.pbuff = pbuff;
   Trans.SourcePort = InPort_PV;
   Trans.TargetPort = getPortForAddr (pbuff->TypeA.pram.Open_OV_ask.addr[0]);
   Transpond_KingDaSPCP (&Trans); 
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
static void Close_OV_func (KindgDaSPCP_PackageDataStruct* pbuff)        //�ط� 
{
 unsigned char i;
 Transpond_KingDaSPCP_struct  Trans;
 
 if(PProtocl_Sync_Up == pbuff->TypeA.head.Sync)
 {
  i = 0;
  while (i < PProtoclSendBuff_Number)
  {
   if((0 == KingDaV1send_buff[i].CCR.Bit.idle)
      && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == Close_OV) 
      && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
   {
     if((KingDaV1send_buff[i].buff.Data.TypeA.pram.Close_OV_ask.addr[0] == pbuff->TypeA.pram.Close_OV_ack.addr[0]) 
        &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.Close_OV_ask.addr[1] == pbuff->TypeA.pram.Close_OV_ack.addr[1])
        &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.Close_OV_ask.addr[2] == pbuff->TypeA.pram.Close_OV_ack.addr[2]) )
     {break;}
   }
   i ++;
  }
  if(i < PProtoclSendBuff_Number)
  {
   if(P_NULL != KingDaV1send_buff[i].AckFunc)
   {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), pbuff);} // Ӧ�����ݴ������
   else
   {
    if(Null_Port!= KingDaV1send_buff[i].SourcePort)
    {
      Trans.CSR.Bit.Preamble =  1;
      Trans.pbuff = pbuff;
      Trans.SourcePort = Null_Port;
      Trans.TargetPort = KingDaV1send_buff[i].SourcePort;
      Transpond_KingDaSPCP (&Trans); 
    }
   }
   FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //�ͷŻ����� 
  }  
 }
 else
 {
   Trans.CSR.Bit.Preamble =  1;
   Trans.pbuff = pbuff;
   Trans.SourcePort = InPort_PV;
   Trans.TargetPort = getPortForAddr (pbuff->TypeA.pram.Close_OV_ask.addr[0]);
   Transpond_KingDaSPCP (&Trans); 
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
static void Get_OV_func (KindgDaSPCP_PackageDataStruct* pbuff)           //����״̬
{
 unsigned char i;
 Transpond_KingDaSPCP_struct  Trans;
 
 if(PProtocl_Sync_Up == pbuff->TypeA.head.Sync)
 {
  i = 0;
  while (i < PProtoclSendBuff_Number)
  {
   if((0 == KingDaV1send_buff[i].CCR.Bit.idle)
      && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == Get_OV) 
      && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
   {
     if((KingDaV1send_buff[i].buff.Data.TypeA.pram.Get_OV_ask.addr[0] == pbuff->TypeA.pram.Get_OV_ack.addr[0]) 
        &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.Get_OV_ask.addr[1] == pbuff->TypeA.pram.Get_OV_ack.addr[1])
        &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.Get_OV_ask.addr[2] == pbuff->TypeA.pram.Get_OV_ack.addr[2]) )
     {break;}
   }
   i ++;
  }
  if(i < PProtoclSendBuff_Number)
  {
   if(P_NULL != KingDaV1send_buff[i].AckFunc)
   {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), pbuff);} // Ӧ�����ݴ������
   else
   {
    if(Null_Port!= KingDaV1send_buff[i].SourcePort)
    {
      Trans.CSR.Bit.Preamble =  1;
      Trans.pbuff = pbuff;
      Trans.SourcePort = Null_Port;
      pbuff->TypeA.head.Length = 4;
      Trans.TargetPort = KingDaV1send_buff[i].SourcePort;
      Transpond_KingDaSPCP (&Trans); 
    }
   }
   FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //�ͷŻ����� 
  }  
 }
 else
 {
   
   Trans.CSR.Bit.Preamble =  1;
   
   //pbuff->head.Length = 4;
   
   Trans.pbuff = pbuff;
   Trans.SourcePort = InPort_PV;
   Trans.TargetPort = getPortForAddr (pbuff->TypeA.pram.Get_OV_ask.addr[0]);
   Transpond_KingDaSPCP (&Trans); 
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
static void Open_OV_ENF_func (KindgDaSPCP_PackageDataStruct* pbuff)     //ǿ�ƿ���
{
 unsigned char i;
 Transpond_KingDaSPCP_struct  Trans;
 
 if(PProtocl_Sync_Up == pbuff->TypeA.head.Sync)
 {
  i = 0;
  while (i < PProtoclSendBuff_Number)
  {
   if((0 == KingDaV1send_buff[i].CCR.Bit.idle)
      && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == Get_OV) 
      && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
   {
     if((KingDaV1send_buff[i].buff.Data.TypeA.pram.Open_OV_ENF_ask.addr[0] == pbuff->TypeA.pram.Open_OV_ENF_ack.addr[0]) 
        &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.Open_OV_ENF_ask.addr[1] == pbuff->TypeA.pram.Open_OV_ENF_ack.addr[1])
        &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.Open_OV_ENF_ask.addr[2] == pbuff->TypeA.pram.Open_OV_ENF_ack.addr[2]) )
     {break;}
   }
   i ++;
  }
  if(i < PProtoclSendBuff_Number)
  {
   if(P_NULL != KingDaV1send_buff[i].AckFunc)
   {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), pbuff);} // Ӧ�����ݴ������
   else
   {
    if(Null_Port!= KingDaV1send_buff[i].SourcePort)
    {
      Trans.CSR.Bit.Preamble =  1;
      Trans.pbuff = pbuff;
      Trans.SourcePort = Null_Port;
      Trans.TargetPort = KingDaV1send_buff[i].SourcePort;
      Transpond_KingDaSPCP (&Trans); 
    }
   }
   FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //�ͷŻ����� 
  }  
 }
 else
 {
   Trans.CSR.Bit.Preamble =  1;
   Trans.pbuff = pbuff;
   Trans.SourcePort = InPort_PV;
   Trans.TargetPort = getPortForAddr (pbuff->TypeA.pram.Open_OV_ENF_ask.addr[0]);
   Transpond_KingDaSPCP (&Trans); 
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
static void Close_OV_ENF_func (KindgDaSPCP_PackageDataStruct* pbuff)   //ǿ�ƹط� 
{
   unsigned char i;
 Transpond_KingDaSPCP_struct  Trans;
 
 if(PProtocl_Sync_Up == pbuff->TypeA.head.Sync)
 {
  i = 0;
  while (i < PProtoclSendBuff_Number)
  {
   if((0 == KingDaV1send_buff[i].CCR.Bit.idle)
      && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == Close_OV_ENF) 
      && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
   {
     if((KingDaV1send_buff[i].buff.Data.TypeA.pram.Close_OV_ENF_ask.addr[0] == pbuff->TypeA.pram.Close_OV_ENF_ack.addr[0]) 
        &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.Close_OV_ENF_ask.addr[1] == pbuff->TypeA.pram.Close_OV_ENF_ack.addr[1])
        &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.Close_OV_ENF_ask.addr[2] == pbuff->TypeA.pram.Close_OV_ENF_ack.addr[2]) )
     {break;}
   }
   i ++;
  }
  if(i < PProtoclSendBuff_Number)
  {
   if(P_NULL != KingDaV1send_buff[i].AckFunc)
   {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), pbuff);} // Ӧ�����ݴ������
   else
   {
    if(Null_Port!= KingDaV1send_buff[i].SourcePort)
    {
      Trans.CSR.Bit.Preamble =  1;
      Trans.pbuff = pbuff;
      Trans.SourcePort = Null_Port;
      Trans.TargetPort = KingDaV1send_buff[i].SourcePort;
      Transpond_KingDaSPCP (&Trans); 
    }
   }
   FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //�ͷŻ����� 
  }  
 }
 else
 {
   Trans.CSR.Bit.Preamble =  1;
   Trans.pbuff = pbuff;
   Trans.SourcePort = InPort_PV;
   Trans.TargetPort = getPortForAddr (pbuff->TypeA.pram.Close_OV_ENF_ask.addr[0]);
   Transpond_KingDaSPCP (&Trans); 
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
static void MT_OV_func (KindgDaSPCP_PackageDataStruct* pbuff)             //������� 
{
 unsigned char i;
 Transpond_KingDaSPCP_struct  Trans;
 
 if(PProtocl_Sync_Up == pbuff->TypeA.head.Sync)
 {
  i = 0;
  while (i < PProtoclSendBuff_Number)
  {
   if((0 == KingDaV1send_buff[i].CCR.Bit.idle)
      && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == MT_OV) 
      && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
   {
     if((KingDaV1send_buff[i].buff.Data.TypeA.pram.MT_OV_ask.addr[0] == pbuff->TypeA.pram.MT_OV_ack.addr[0]) 
        &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.MT_OV_ask.addr[1] == pbuff->TypeA.pram.MT_OV_ack.addr[1])
        &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.MT_OV_ask.addr[2] == pbuff->TypeA.pram.MT_OV_ack.addr[2]) )
     {break;}
   }
   i ++;
  }
  if(i < PProtoclSendBuff_Number)
  {
   if(P_NULL != KingDaV1send_buff[i].AckFunc)
   {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), pbuff);} // Ӧ�����ݴ������
   else
   {
    if(Null_Port!= KingDaV1send_buff[i].SourcePort)
    {
      Trans.CSR.Bit.Preamble =  1;
      Trans.pbuff = pbuff;
      Trans.SourcePort = Null_Port;
      Trans.TargetPort = KingDaV1send_buff[i].SourcePort;
      Transpond_KingDaSPCP (&Trans); 
    }
   }
   FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //�ͷŻ����� 
  }  
 }
 else
 {
   Trans.CSR.Bit.Preamble =  1;
   Trans.pbuff = pbuff;
   Trans.SourcePort = InPort_PV;
   Trans.TargetPort = getPortForAddr (pbuff->TypeA.pram.MT_OV_ask.addr[0]);
   Transpond_KingDaSPCP (&Trans); 
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
static void MT_OV_ALL_func (KindgDaSPCP_PackageDataStruct* pbuff)     //�㲥�������
{
 Transpond_KingDaSPCP_struct  Trans;
 
 if(PProtocl_Sync_Down == pbuff->TypeA.head.Sync)
 {
   Trans.CSR.Bit.Preamble =  1;
   Trans.pbuff = pbuff;
   Trans.SourcePort = InPort_PV;
   Trans.TargetPort = get_RS458ChannelNumber();
   if(Null_Port == Trans.TargetPort)
   {Transpond_KingDaSPCP (&Trans);}
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
static void ReadId_Concentrator_func(KindgDaSPCP_PackageDataStruct* pbuff)
{
  PProtocolSendBuff_Struct*  buff;
  
  if((0xAB != pbuff->TypeA.pram.ReadId_ask.DI1)||(0 != pbuff->TypeA.pram.ReadId_ask.DI2))
  {return;}
  buff = GetIdleBuff_PProtocol ( );
  if(P_NULL == buff){return;}
  
  buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;
  buff->SourcePort = Null_Port;
  buff->buff.Prm1 = PProtocl_Preamble_Up;
  buff->buff.Prm2 = PProtocl_Preamble_Up;
  buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
  buff->buff.Data.TypeB.head.Cmd = ReadId_Concentrator;
  buff->buff.Data.TypeB.head.Length =  sizeof (ReadId_Concentrator_ACKstruct);
  buff->buff.Data.TypeB.pram.ReadId_ack.DI1 = 0xAB ;
  buff->buff.Data.TypeB.pram.ReadId_ack.id[0] = Concentrator.id[0] ;
  buff->buff.Data.TypeB.pram.ReadId_ack.id[1] = Concentrator.id[1];
  buff->buff.Data.TypeB.pram.ReadId_ack.id[2] = Concentrator.id[2]; 
  PProtocol_send (InPort_PV, buff);
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
static void WriteId_Concentrator_func(KindgDaSPCP_PackageDataStruct* pbuff) //
{
 SystemErrName err = NO_ERR; 
 PProtocolSendBuff_Struct*  buff;
 unsigned long p;
  
 if(0xAB != pbuff->TypeA.pram.WriteId_ask.DI1)
 {return;}
 
 p =(unsigned long)(((ConcentratorParameterStruct*) 0)->id);
 p += ConcentratorParameter_AddrEeprom;
 err = EEPROM_I2C_MultipleWrite(pbuff->TypeA.pram.WriteId_ask.id,p, 3);
 if(NO_ERR != err){return ;}  
 
 Concentrator.id[0] = pbuff->TypeA.pram.WriteId_ask.id[0];
 Concentrator.id[1] = pbuff->TypeA.pram.WriteId_ask.id[1];
 Concentrator.id[2] = pbuff->TypeA.pram.WriteId_ask.id[2];
 buff = GetIdleBuff_PProtocol ( );
 if(P_NULL == buff){return;}
 buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;
 buff->SourcePort = Null_Port;
 buff->buff.Prm1 = PProtocl_Preamble_Up;
 buff->buff.Prm2 = PProtocl_Preamble_Up;
 buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
 buff->buff.Data.TypeB.head.Cmd = WriteId_Concentrator;
 buff->buff.Data.TypeB.head.Length =  sizeof (WriteId_Concentrator_ACKstruct);
 buff->buff.Data.TypeB.pram.WriteId_ack.DI1 = 0xAB ;
 buff->buff.Data.TypeB.pram.WriteId_ack.state[0] = 'O';
 buff->buff.Data.TypeB.pram.WriteId_ack.state[1] = 'K';
 PProtocol_send (InPort_PV, buff); 
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
static void ReadTime_Concentrator_func(KindgDaSPCP_PackageDataStruct* pbuff)
{
 PProtocolSendBuff_Struct*  buff;
 TimerData_Struct timer;
 
 if((0xAB != pbuff->TypeA.pram.ReadTime_ask.DI1)||(0x00 != pbuff->TypeA.pram.ReadTime_ask.DI2))
 {return;}
  
 buff = GetIdleBuff_PProtocol ( );
 if(P_NULL == buff){return;}
 
  buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;
  buff->SourcePort = Null_Port;
  buff->buff.Prm1 = PProtocl_Preamble_Up;
  buff->buff.Prm2 = PProtocl_Preamble_Up;
  buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
  buff->buff.Data.TypeB.head.Cmd = ReadTime_Concentrator;
  buff->buff.Data.TypeB.head.Length =  sizeof (ReadTime_Concentrator_ACKstruct);
  buff->buff.Data.TypeB.pram.ReadTime_ack.DI1 = 0xAB ;
  get_SystemTimer(&timer);
  buff->buff.Data.TypeB.pram.ReadTime_ack.year = BinChangeToBCD(timer.Year - 2000);
  buff->buff.Data.TypeB.pram.ReadTime_ack.month = BinChangeToBCD(timer.Month);
  buff->buff.Data.TypeB.pram.ReadTime_ack.day = BinChangeToBCD(timer.Day);
  buff->buff.Data.TypeB.pram.ReadTime_ack.hour = BinChangeToBCD(timer.Hour);
  buff->buff.Data.TypeB.pram.ReadTime_ack.minute = BinChangeToBCD(timer.Minute);
  buff->buff.Data.TypeB.pram.ReadTime_ack.second = BinChangeToBCD(timer.Second);
  PProtocol_send (InPort_PV, buff); 
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
static void WriteTime_Concentrator_func(KindgDaSPCP_PackageDataStruct* pbuff)
{
 TimerData_Struct time;
 PProtocolSendBuff_Struct*  buff;
  
 if(0xAB != pbuff->TypeA.pram.WriteTime_ask.DI1)
 {return;}
 
 time.Year = (unsigned short)(pbuff->TypeA.pram.WriteTime_ask.year) + 2000;
 time.Month = pbuff->TypeA.pram.WriteTime_ask.month;
 time.Day = pbuff->TypeA.pram.WriteTime_ask.day;
 time.Hour = pbuff->TypeA.pram.WriteTime_ask.hour;
 time.Minute = pbuff->TypeA.pram.WriteTime_ask.minute;
 time.Second = pbuff->TypeA.pram.WriteTime_ask.second;
 
 buff = GetIdleBuff_PProtocol ( );
 if(P_NULL == buff){return;}
 
 buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;
 
 PCF8563_Set_Time(&time);
  
 buff->SourcePort = Null_Port;
 buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
 buff->buff.Data.TypeB.head.Cmd = WriteTime_Concentrator;
 buff->buff.Data.TypeB.head.Length =  sizeof (WriteTime_Concentrator_ACKstruct);
 
 buff->buff.Data.TypeB.pram.WriteTime_ack.DI1 = 0xAB; 
 buff->buff.Data.TypeB.pram.WriteTime_ack.state[0] = 'O';
 buff->buff.Data.TypeB.pram.WriteTime_ack.state[1] = 'K';
 PProtocol_send (InPort_PV, buff); 
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
static void ReadCycle_Concentrator_func(KindgDaSPCP_PackageDataStruct* pbuff)
{
 PProtocolSendBuff_Struct*  buff;
 buff = GetIdleBuff_PProtocol ( );
 if(P_NULL == buff){return;}
 buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;
 buff->SourcePort = Null_Port;
 buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
 buff->buff.Data.TypeB.head.Cmd = ReadCycle_Concentrator;
 buff->buff.Data.TypeB.pram.ReadCycle_ack_B.DI1 = 0xAB;
 
 if(0x01 == Concentrator.CSR.Bit.Appcon_EN)
 {
   buff->buff.Data.TypeB.head.Length =  sizeof (ReadCycle_Concentrator_ACKstruct_B);
   buff->buff.Data.TypeB.pram.ReadCycle_ack_B.Day = Concentrator.cycle.day ;
   buff->buff.Data.TypeB.pram.ReadCycle_ack_B.hour = Concentrator.cycle.hour ;
   buff->buff.Data.TypeB.pram.ReadCycle_ack_B.minute = Concentrator.cycle.minute; 
 }
 else
 {
   buff->buff.Data.TypeB.head.Length =  sizeof (ReadCycle_Concentrator_ACKstruct_A);
   buff->buff.Data.TypeB.pram.ReadCycle_ack_A.hour = Concentrator.cycle.hour ;
   buff->buff.Data.TypeB.pram.ReadCycle_ack_A.minute = Concentrator.cycle.minute;  
 }
 PProtocol_send (InPort_PV, buff); 
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
static void WriteCycle_Concentrator_func(KindgDaSPCP_PackageDataStruct* pbuff)
{
 SystemErrName err = NO_ERR; 
 PProtocolSendBuff_Struct*  buff;
 unsigned long p;
 AutoCollectCycleStruct  Cycle;
 
 p =(unsigned long)&(((ConcentratorParameterStruct*) 0)->cycle);
 p += ConcentratorParameter_AddrEeprom;
 if(3 == pbuff ->TypeA.head.Length) 
 {
   if(0xAB != pbuff->TypeA.pram.WriteCycle_ask_TypeA.DI1)
   {return;}
   Cycle.day = 1;
   Cycle.hour = pbuff->TypeA.pram.WriteCycle_ask_TypeA.hour;
   Cycle.minute = pbuff->TypeA.pram.WriteCycle_ask_TypeA.minute; 
   err = EEPROM_I2C_MultipleWrite(&(Cycle), p,sizeof(AutoCollectCycleStruct));
   if(NO_ERR != err){return ;}  
   err = EEPROM_I2C_MultipleRead(&(Cycle), p, sizeof(AutoCollectCycleStruct));
   if(NO_ERR != err){return ;} 
   if((0 != Cycle.day)
      ||(Cycle.hour != pbuff->TypeA.pram.WriteCycle_ask_TypeA.hour)
        ||(Cycle.minute != pbuff->TypeA.pram.WriteCycle_ask_TypeA.minute))
   {return ;}
 }
 else if(4 == pbuff ->TypeA.head.Length)
 {
   if(0xAB != pbuff->TypeA.pram.WriteCycle_ask_TypeB.DI1)
   {return;}
   Cycle.day = pbuff->TypeA.pram.WriteCycle_ask_TypeB.day;
   Cycle.hour = pbuff->TypeA.pram.WriteCycle_ask_TypeB.hour;
   Cycle.minute = pbuff->TypeA.pram.WriteCycle_ask_TypeB.minute; 
   err = EEPROM_I2C_MultipleWrite(&(Cycle), p,sizeof(AutoCollectCycleStruct));
   if(NO_ERR != err){return ;}  
   err = EEPROM_I2C_MultipleRead(&(Cycle), p, sizeof(AutoCollectCycleStruct));
   if(NO_ERR != err){return ;} 
   if((Cycle.day != pbuff->TypeA.pram.WriteCycle_ask_TypeB.day)
      ||(Cycle.hour != pbuff->TypeA.pram.WriteCycle_ask_TypeB.hour)
        ||(Cycle.minute != pbuff->TypeA.pram.WriteCycle_ask_TypeB.minute))
   {return ;}
 }
 else
 {return;}
 
 Concentrator.cycle.day = Cycle.day;
 Concentrator.cycle.hour = Cycle.hour;
 Concentrator.cycle.minute = Cycle.minute;  
 buff = GetIdleBuff_PProtocol ( );
 if(P_NULL == buff){return;}
 buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB; 
 buff->SourcePort = Null_Port;
 buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
 buff->buff.Data.TypeB.head.Cmd = WriteCycle_Concentrator;
 buff->buff.Data.TypeB.head.Length =  sizeof (WriteCycle_Concentrator_ACKstruct);
 buff->buff.Data.TypeB.pram.WriteCycle_ack.DI1 = 0xAB ;
 buff->buff.Data.TypeB.pram.WriteCycle_ack.state[0] = 'O';
 buff->buff.Data.TypeB.pram.WriteCycle_ack.state[1] = 'K';
 PProtocol_send (InPort_PV, buff);
}
/*********************************************************************************
 Function:      //
 Description:   //��ȡ��ͨ����ʼ�����м�����
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void ReadChannel_Concentrator_func (KindgDaSPCP_PackageDataStruct* pbuff) 
{
 PProtocolSendBuff_Struct*  buff;
  
 if(0xAB != pbuff->TypeA.pram.ReadChannel_ask.DI1)
 {return;}

 buff = GetIdleBuff_PProtocol ( );
 if(P_NULL == buff){return;}
 buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB; 
 buff->SourcePort = Null_Port;
 buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
 buff->buff.Data.TypeB.head.Cmd = ReadChannel_Concentrator;
 buff->buff.Data.TypeB.head.Length =  sizeof (ReadChannel_Concentrator_ACKstruct);
 buff->buff.Data.TypeB.pram.ReadChannel_ack.DI1 = 0xAB ;
 buff->buff.Data.TypeB.pram.ReadChannel_ack.channel[0][0]= Concentrator.channel[0].min;
 buff->buff.Data.TypeB.pram.ReadChannel_ack.channel[0][1]= Concentrator.channel[0].max;
 buff->buff.Data.TypeB.pram.ReadChannel_ack.channel[1][0]= Concentrator.channel[1].min;
 buff->buff.Data.TypeB.pram.ReadChannel_ack.channel[1][1]= Concentrator.channel[1].max;
 buff->buff.Data.TypeB.pram.ReadChannel_ack.channel[2][0]= Concentrator.channel[2].min;
 buff->buff.Data.TypeB.pram.ReadChannel_ack.channel[2][1]= Concentrator.channel[2].max;
 PProtocol_send (InPort_PV, buff);
}
/*********************************************************************************
 Function:      //
 Description:   //���ü�������ͨ���м�����ʼ������
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void WriteChannel_Concentrator_func (KindgDaSPCP_PackageDataStruct* pbuff)
{
 SystemErrName err = NO_ERR; 
 PProtocolSendBuff_Struct*  buff;
 unsigned long p = 0;
 ChannelConfigStruct        channel[3];    //������ ����ͨ���м������� 
  
 if(0xAB != pbuff->TypeA.pram.WriteChannel_ask.DI1)
 {return;}
 
 channel[0].min = pbuff->TypeA.pram.WriteChannel_ask.channel[0][0];
 channel[0].max = pbuff->TypeA.pram.WriteChannel_ask.channel[0][1];
 channel[1].min = pbuff->TypeA.pram.WriteChannel_ask.channel[1][0];
 channel[1].max = pbuff->TypeA.pram.WriteChannel_ask.channel[1][1];
 channel[2].min = pbuff->TypeA.pram.WriteChannel_ask.channel[2][0];
 channel[2].max = pbuff->TypeA.pram.WriteChannel_ask.channel[2][1];
 
 p =(unsigned long)(((ConcentratorParameterStruct*) 0)->channel);
 p += ConcentratorParameter_AddrEeprom;
 err = EEPROM_I2C_MultipleWrite(channel, p, sizeof(ChannelConfigStruct)*3);
 if(NO_ERR != err){return ;}  
 Concentrator.channel[0].min = pbuff->TypeA.pram.WriteChannel_ask.channel[0][0];
 Concentrator.channel[0].max = pbuff->TypeA.pram.WriteChannel_ask.channel[0][1];
 Concentrator.channel[1].min = pbuff->TypeA.pram.WriteChannel_ask.channel[1][0];
 Concentrator.channel[1].max = pbuff->TypeA.pram.WriteChannel_ask.channel[1][1];
 Concentrator.channel[2].min = pbuff->TypeA.pram.WriteChannel_ask.channel[2][0];
 Concentrator.channel[2].max = pbuff->TypeA.pram.WriteChannel_ask.channel[2][1];
 
 buff = GetIdleBuff_PProtocol ( );
 if(P_NULL == buff){return;}
 buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;  
 buff->SourcePort = Null_Port;
 buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
 buff->buff.Data.TypeB.head.Cmd = WriteChannel_Concentrator;
 buff->buff.Data.TypeB.head.Length =  sizeof (WriteChannel_Concentrator_ACKstruct);
 buff->buff.Data.TypeB.pram.WriteChannel_ack.DI1 = 0xAB ;
 buff->buff.Data.TypeB.pram.WriteChannel_ack.state[0] = 'O';
 buff->buff.Data.TypeB.pram.WriteChannel_ack.state[1] = 'K';
 PProtocol_send (InPort_PV, buff);
}
/*********************************************************************************
 Function:      //
 Description:   //��ȡ�Ƿ����������ӷ�����
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void ReadIpEnd_Concentrator_func (KindgDaSPCP_PackageDataStruct* pbuff)
{
 PProtocolSendBuff_Struct*  buff;
  
 if(0xAB != pbuff->TypeA.pram.ReadChannel_ask.DI1)
 {return;}

 buff = GetIdleBuff_PProtocol ( );
 if(P_NULL == buff){return;}
 buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;  
 buff->SourcePort = Null_Port;
 buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
 buff->buff.Data.TypeB.head.Cmd = ReadIpEnd_Concentrator;
 buff->buff.Data.TypeB.head.Length =  sizeof (ReadIpEnd_Concentrator_ACKstruct);
 buff->buff.Data.TypeB.pram.ReadIpEnd_ack.DI1 = 0xAB ;
 if(Concentrator.CSR.Bit.ip_flg)
 {buff->buff.Data.TypeB.pram.ReadIpEnd_ack.state = 0;}
 else
 {buff->buff.Data.TypeB.pram.ReadIpEnd_ack.state = 1;}
 PProtocol_send (InPort_PV, buff);
}
/*********************************************************************************
 Function:      //
 Description:   //���ÿ������ر�ʹ���������ӷ����� 
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void WriteIpEnd_Concentrator_func (KindgDaSPCP_PackageDataStruct* pbuff)
{
 SystemErrName err = NO_ERR; 
 CSR_ConcentratorParameter  csr;
 PProtocolSendBuff_Struct*  buff;
 unsigned long p = 0;
 if(0xAB != pbuff->TypeA.pram.WriteChannel_ask.DI1)
 {return;}
 
 csr.Byte = Concentrator.CSR.Byte;
 if(pbuff->TypeA.pram.WriteIpEnd_ask.state)
 {csr.Bit.ip_flg = 0;}
 else
 {csr.Bit.ip_flg = 1;}
 p =(unsigned long)&(((ConcentratorParameterStruct*) 0)->CSR);
 p += ConcentratorParameter_AddrEeprom;
 err = EEPROM_I2C_MultipleWrite(&csr, p, sizeof (CSR_ConcentratorParameter));
 if(NO_ERR != err){return ;}  
 Concentrator.CSR.Byte =csr.Byte;
 
 buff = GetIdleBuff_PProtocol ( );
 if(P_NULL == buff){return;}
 buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;  
 buff->SourcePort = Null_Port;
 buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
 buff->buff.Data.TypeB.head.Cmd = WriteIpEnd_Concentrator;
 buff->buff.Data.TypeB.head.Length =  sizeof (WriteIpEnd_Concentrator_ACKstruct);
 buff->buff.Data.TypeB.pram.WriteChannel_ack.DI1 = 0xAB ;
 buff->buff.Data.TypeB.pram.WriteChannel_ack.state[0] = 'O';
 buff->buff.Data.TypeB.pram.WriteChannel_ack.state[1] = 'K';
 PProtocol_send (InPort_PV, buff);
}
/*********************************************************************************
 Function:      //
 Description:   //��ȡ�������
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void ReadServerInf_Concentrator_func (KindgDaSPCP_PackageDataStruct* pbuff)
{
 unsigned short itemp;
 unsigned char crc;
 PProtocolSendBuff_Struct*  buff;
 
 if(0xAB != pbuff->TypeA.pram.WriteServerInf_ask.DI1)
 {return;}

 buff = GetIdleBuff_PProtocol ( );
 if(P_NULL == buff){return;}
 buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;  
 buff->SourcePort = Null_Port;

 buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
 buff->buff.Data.TypeB.head.Cmd = ReadServerInf_Concentrator;
 buff->buff.Data.TypeB.head.Length =  sizeof (ReadServerInf_Concentrator_ACKstruct);
 buff->buff.Data.TypeB.pram.ReadServerInf_ack.DI1= 0xAB;

 buff->buff.Data.TypeB.pram.ReadServerInf_ack.IP_M[0]= Concentrator.server1.ip[0];
 buff->buff.Data.TypeB.pram.ReadServerInf_ack.IP_M[1]= Concentrator.server1.ip[1];
 buff->buff.Data.TypeB.pram.ReadServerInf_ack.IP_M[2]= Concentrator.server1.ip[2];
 buff->buff.Data.TypeB.pram.ReadServerInf_ack.IP_M[3]= Concentrator.server1.ip[3];
 
 buff->buff.Data.TypeB.pram.ReadServerInf_ack.IP_S[0]= Concentrator.server2.ip[0];
 buff->buff.Data.TypeB.pram.ReadServerInf_ack.IP_S[1]= Concentrator.server2.ip[1];
 buff->buff.Data.TypeB.pram.ReadServerInf_ack.IP_S[2]= Concentrator.server2.ip[2];
 buff->buff.Data.TypeB.pram.ReadServerInf_ack.IP_S[3]= Concentrator.server2.ip[3];
 
 buff->buff.Data.TypeB.pram.ReadServerInf_ack.PORT_M = (Concentrator.server1.port>> 8) | (Concentrator.server1.port << 8);
 buff->buff.Data.TypeB.pram.ReadServerInf_ack.PORT_S = (Concentrator.server2.port>> 8) | (Concentrator.server2.port << 8);
 
 itemp = 0;
 crc = 0;
 while(crc < 5)
 {
   itemp *= 10;
   itemp += Concentrator.er_tele[crc++];
 }
 buff->buff.Data.TypeB.pram.ReadServerInf_ack.TEL[0] = itemp >> 8;
 buff->buff.Data.TypeB.pram.ReadServerInf_ack.TEL[1] = itemp ;
 itemp = 0;
 while(crc < 9)
 {
   itemp *= 10;
   itemp += Concentrator.er_tele[crc++];
 }
 buff->buff.Data.TypeB.pram.ReadServerInf_ack.TEL[2] = itemp >> 8;
 buff->buff.Data.TypeB.pram.ReadServerInf_ack.TEL[3] = itemp ;
 itemp = 0;
 while(crc < 11)
 {
   itemp *= 10;
   itemp += Concentrator.er_tele[crc++];
 }
 buff->buff.Data.TypeB.pram.ReadServerInf_ack.TEL[4] = itemp;
 PProtocol_send (InPort_PV, buff);
}
/*********************************************************************************
 Function:      //
 Description:   // �����������
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void WriteServerInf_Concentrator_func (KindgDaSPCP_PackageDataStruct* pbuff)
{
 SystemErrName err = NO_ERR; 
 unsigned short itemp ;
 unsigned long itemp2;

 PProtocolSendBuff_Struct*  buff;
 if(0xAB != pbuff->TypeA.pram.WriteServerInf_ask.DI1)
 {return;}
 itemp = 0;
 Concentrator.server1.ip[0] =  pbuff->TypeA.pram.WriteServerInf_ask.IP_M[0];
 Concentrator.server1.ip[1] =  pbuff->TypeA.pram.WriteServerInf_ask.IP_M[1];
 Concentrator.server1.ip[2] =  pbuff->TypeA.pram.WriteServerInf_ask.IP_M[2];
 Concentrator.server1.ip[3] =  pbuff->TypeA.pram.WriteServerInf_ask.IP_M[3];
 Concentrator.server2.ip[0] =  pbuff->TypeA.pram.WriteServerInf_ask.IP_S[0];
 Concentrator.server2.ip[1] =  pbuff->TypeA.pram.WriteServerInf_ask.IP_S[1];
 Concentrator.server2.ip[2] =  pbuff->TypeA.pram.WriteServerInf_ask.IP_S[2];
 Concentrator.server2.ip[3] =  pbuff->TypeA.pram.WriteServerInf_ask.IP_S[3];
 Concentrator.server1.port =  (pbuff->TypeA.pram.WriteServerInf_ask.PORT_M >> 8) | (pbuff->TypeA.pram.WriteServerInf_ask.PORT_M<< 8);
 Concentrator.server2.port =  (pbuff->TypeA.pram.WriteServerInf_ask.PORT_S >> 8) | (pbuff->TypeA.pram.WriteServerInf_ask.PORT_S<< 8);
 itemp2 = pbuff->TypeA.pram.WriteServerInf_ask.TEL[0];
 itemp2 <<= 8;
 itemp2 += pbuff->TypeA.pram.WriteServerInf_ask.TEL[1];
 itemp = 5;
 while (itemp-- > 0 )
 {
   Concentrator.er_tele[itemp] = itemp2 % 10;
   itemp2 /= 10;
 }
 itemp2 = pbuff->TypeA.pram.WriteServerInf_ask.TEL[2];
 itemp2 <<= 8;
 itemp2 += pbuff->TypeA.pram.WriteServerInf_ask.TEL[3];
 itemp = 4;
 while (itemp-- > 0 )
 {
   Concentrator.er_tele[itemp+5] = itemp2 % 10;
   itemp2 /= 10;
 }
 itemp2 = pbuff->TypeA.pram.WriteServerInf_ask.TEL[4];
 itemp = 2;
 while (itemp-- > 0 )
 {
   Concentrator.er_tele[itemp+9] = itemp2 % 10;
   itemp2 /= 10;
 }
 itemp = (unsigned long)&(((ConcentratorParameterStruct*)0)->er_tele[11]);
 itemp2 =(unsigned long)&(((ConcentratorParameterStruct*)0)->server1);
 itemp -= itemp2;
 itemp2 += ConcentratorParameter_AddrEeprom;
 err = EEPROM_I2C_MultipleWrite(&(Concentrator.server1), itemp2, itemp);
 if(NO_ERR != err){return ;}  
 buff = GetIdleBuff_PProtocol ( );
 if(P_NULL == buff){return;} 
 buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB; 
 
 buff->SourcePort = Null_Port;
 buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
 buff->buff.Data.TypeB.head.Cmd = WriteServerInf_Concentrator;
 buff->buff.Data.TypeB.head.Length =  sizeof (WriteServerInf_Concentrator_ACKstruct);
 buff->buff.Data.TypeB.pram.WriteChannel_ack.DI1 = 0xAB ;
 buff->buff.Data.TypeB.pram.WriteChannel_ack.state[0] = 'O';
 buff->buff.Data.TypeB.pram.WriteChannel_ack.state[1] = 'K';
 PProtocol_send (InPort_PV, buff); 
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
static void ReadServer_Concentrator_func (KindgDaSPCP_PackageDataStruct* pbuff)  //��ȡ������������Ϣ 
{
 unsigned char itemp;
 PProtocolSendBuff_Struct*  buff;
 
 if(0xAB != pbuff->TypeA.pram.WriteServerInf_ask.DI1)
 {return;}

 buff = GetIdleBuff_PProtocol ( );
 if(P_NULL == buff){return;}
  buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;  
 buff->SourcePort = Null_Port;
 buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
 buff->buff.Data.TypeB.head.Cmd =pbuff->TypeA.head.Cmd;
 buff->buff.Data.TypeB.pram.ReadServer_ack.DI1 = 0xAB;
 itemp =  0;
 if(ReadServerI_Concentrator == pbuff->TypeA.head.Cmd)
 {
  while(itemp < DomainNameLength)
  {
   if(0 != Concentrator.server1.DomainName[itemp])
   {
     buff->buff.Data.TypeB.pram.ReadServer_ack.DomainName [itemp]= Concentrator.server1.DomainName[itemp];
     itemp ++;
   }
   else
   {break;}
  }
  buff->buff.Data.TypeB.pram.ReadServer_ack.DomainName [itemp++] = (Concentrator.server1.port>> 8);
  buff->buff.Data.TypeB.pram.ReadServer_ack.DomainName [itemp++] =  Concentrator.server1.port; 
 }
 else
 {
  while(itemp < DomainNameLength)
  {
    if(0 != Concentrator.server2.DomainName[itemp])
    {
      buff->buff.Data.TypeB.pram.ReadServer_ack.DomainName [itemp]= Concentrator.server2.DomainName[itemp];
      itemp ++;
    }
    else
    {break;}
   }
  buff->buff.Data.TypeB.pram.ReadServer_ack.DomainName [itemp++] = (Concentrator.server2.port>> 8);
  buff->buff.Data.TypeB.pram.ReadServer_ack.DomainName [itemp++] =  Concentrator.server2.port; 
 }
 buff->buff.Data.TypeB.head.Length =  itemp + 1;
 PProtocol_send (InPort_PV, buff); 
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
static void WriteServer_Concentrator_func (KindgDaSPCP_PackageDataStruct* pbuff)//���ü���������
{
 SystemErrName err = NO_ERR; 
 unsigned char crc;
 unsigned char itemp ;
 DataServerStruct           server;      

 PProtocolSendBuff_Struct*  buff;
 if(0xAB != pbuff->TypeA.pram.WriteServer_ask.DI1)
 {return;}
 itemp = pbuff->TypeA.head.Length - 3;
 crc = 0;
 while(crc < itemp)
 {
   server.DomainName[crc] = pbuff->TypeA.pram.WriteServer_ask.DomainName[crc];
   crc ++;
 }
 while (crc < DomainNameLength)
 {
   server.DomainName[crc] = 0;
   crc ++;
 }
 server.DomainName[DomainNameLength - 1] = 0;
 server.port =  pbuff->TypeA.pram.WriteServer_ask.DomainName[itemp++] ;
 server.port <<= 8;
 server.port |= pbuff->TypeA.pram.WriteServer_ask.DomainName[itemp++]; 
 
 if(WriteServerI_Concentrator == pbuff->TypeA.head.Cmd)
 {
   server.ip[0] = Concentrator.server1.ip[0];
   server.ip[1] = Concentrator.server1.ip[1];
   server.ip[2] = Concentrator.server1.ip[2];
   server.ip[3] = Concentrator.server1.ip[3];
 
   itemp =(unsigned long)&(((ConcentratorParameterStruct*)0)->server1);
   itemp += ConcentratorParameter_AddrEeprom;
   err = EEPROM_I2C_MultipleWrite(&(server), itemp, sizeof (DataServerStruct));
   if(NO_ERR != err){return ;} 
    crc = 0;
   while (crc < DomainNameLength)
   {
    Concentrator.server1.DomainName [crc] = server.DomainName[crc];
    crc ++;
   }
   Concentrator.server1.port  = server.port;
 }
 else if(WriteServerII_Concentrator == pbuff->TypeA.head.Cmd)
 {
   server.ip[0] = Concentrator.server2.ip[0];
   server.ip[1] = Concentrator.server2.ip[1];
   server.ip[2] = Concentrator.server2.ip[2];
   server.ip[3] = Concentrator.server2.ip[3];
 
   itemp =(unsigned long)&(((ConcentratorParameterStruct*)0)->server2);
   itemp += ConcentratorParameter_AddrEeprom;
   err = EEPROM_I2C_MultipleWrite(&(server), itemp, sizeof (DataServerStruct));
   if(NO_ERR != err){return ;} 
   crc = 0;
   while (crc < DomainNameLength)
   {
    Concentrator.server2.DomainName [crc] = server.DomainName[crc];
    crc ++;
   }
   Concentrator.server2.port  = server.port;
 }
 buff = GetIdleBuff_PProtocol ( );
 if(P_NULL == buff){return;}
 buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;  
 buff->SourcePort = Null_Port;
 buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
 buff->buff.Data.TypeB.head.Cmd = pbuff->TypeA.head.Cmd;
 buff->buff.Data.TypeB.head.Length =  sizeof (WriteServer_Concentrator_ACKstruct);
 buff->buff.Data.TypeB.pram.WriteServer_ack.DI1 = 0xAB ;
 buff->buff.Data.TypeB.pram.WriteServer_ack.state[0] = 'O';
 buff->buff.Data.TypeB.pram.WriteServer_ack.state[1] = 'K';
 PProtocol_send (InPort_PV, buff); 
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
static void ReadID_Repeaters_func (KindgDaSPCP_PackageDataStruct* pbuff) //��ȡ�м�����
{
 unsigned char i;
 Transpond_KingDaSPCP_struct  Trans;
 
 if(PProtocl_Sync_Up == pbuff->TypeA.head.Sync)
 {
  i = 0;
  while (i < PProtoclSendBuff_Number)
  {
   if((0 == KingDaV1send_buff[i].CCR.Bit.idle)
      && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == ReadID_Repeaters) 
      && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
   {break;}
   i ++;
  }
  if(i < PProtoclSendBuff_Number)
  {
   if(P_NULL != KingDaV1send_buff[i].AckFunc)
   {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), pbuff);} // Ӧ�����ݴ������
   else
   {
    if(Null_Port!= KingDaV1send_buff[i].SourcePort)
    {
      Trans.CSR.Bit.Preamble =  0;
      Trans.pbuff = pbuff;
      Trans.SourcePort = Null_Port;
      Trans.TargetPort = KingDaV1send_buff[i].SourcePort;
      Transpond_KingDaSPCP (&Trans); 
    }
   }
   FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //�ͷŻ����� 
  }  
 }
 else
 {
   Trans.CSR.Bit.Preamble =  0;
   Trans.pbuff = pbuff;
   Trans.SourcePort = InPort_PV;
   Trans.TargetPort = get_RS458ChannelNumber ( );
   if(Null_Port == Trans.TargetPort )
   {Trans.TargetPort = Rs485_PortA;}
   Transpond_KingDaSPCP (&Trans); 
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
static void WriteID_Repeaters_func (KindgDaSPCP_PackageDataStruct* pbuff) //�����м�����
{
 unsigned char i;
 Transpond_KingDaSPCP_struct  Trans;
 
 if(PProtocl_Sync_Up == pbuff->TypeA.head.Sync)
 {
  i = 0;
  while (i < PProtoclSendBuff_Number)
  {
   if((0 == KingDaV1send_buff[i].CCR.Bit.idle)
      && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == WriteID_Repeaters) 
      && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
   {break;}
   i ++;
  }
  if(i < PProtoclSendBuff_Number)
  {
   if(P_NULL != KingDaV1send_buff[i].AckFunc)
   {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), pbuff);} // Ӧ�����ݴ������
   else
   {
    if(Null_Port!= KingDaV1send_buff[i].SourcePort)
    {
      Trans.CSR.Bit.Preamble =  0;
      Trans.pbuff = pbuff;
      Trans.SourcePort = Null_Port;
      Trans.TargetPort = KingDaV1send_buff[i].SourcePort;
      Transpond_KingDaSPCP (&Trans); 
    }
   }
   FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //�ͷŻ����� 
  }  
 }
 else
 {
   Trans.CSR.Bit.Preamble =  0;
   Trans.pbuff = pbuff;
   Trans.SourcePort = InPort_PV;
   Trans.TargetPort = get_RS458ChannelNumber ( );
   if(Null_Port == Trans.TargetPort )
   {Trans.TargetPort = Rs485_PortA;}
   Transpond_KingDaSPCP (&Trans); 
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
static void OPen_Repeaters_func (KindgDaSPCP_PackageDataStruct* pbuff)//���м������� 
{
 unsigned char i;
 Transpond_KingDaSPCP_struct  Trans;
 
 if(PProtocl_Sync_Up == pbuff->TypeA.head.Sync)
 {
  i = 0;
  while (i < PProtoclSendBuff_Number)
  {
   if((0 == KingDaV1send_buff[i].CCR.Bit.idle)
      && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == OPen_Repeaters) 
      && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
   { 
     if((KingDaV1send_buff[i].buff.Data.TypeA.pram.OPen_Repeaters_ask.RepeatersId== pbuff->TypeA.pram.OPen_Repeaters_ack.RepeatersId)
        &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.OPen_Repeaters_ask.ChannelNumber== pbuff->TypeA.pram.OPen_Repeaters_ack.ChannelNumber))
     break; 
   }
   i ++;
  }
  if(i < PProtoclSendBuff_Number)
  {
   if(P_NULL != KingDaV1send_buff[i].AckFunc)
   {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), pbuff);} // Ӧ�����ݴ������
   else
   {
    if(Null_Port!= KingDaV1send_buff[i].SourcePort)
    {
      Trans.CSR.Bit.Preamble =  0;
      Trans.pbuff = pbuff;
      Trans.SourcePort = Null_Port;
      Trans.TargetPort = KingDaV1send_buff[i].SourcePort;
      Transpond_KingDaSPCP (&Trans); 
    }
   }
   FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //�ͷŻ����� 
  }  
 }
 else
 {
   Trans.CSR.Bit.Preamble =  0;
   Trans.pbuff = pbuff;
   Trans.SourcePort = InPort_PV;
   i = pbuff->TypeA.pram.OPen_Repeaters_ask.RepeatersId >> 4;
   i *= 10;
   i += pbuff->TypeA.pram.OPen_Repeaters_ask.RepeatersId & 0x0F; 
   Trans.TargetPort = getPortForAddr (i);
   Transpond_KingDaSPCP (&Trans); 
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
static void Close_Repeaters_func (KindgDaSPCP_PackageDataStruct* pbuff) //�ر��м�������ʹ�м���˯��
{
 unsigned char i;
 Transpond_KingDaSPCP_struct  Trans;
 
 if(PProtocl_Sync_Up == pbuff->TypeA.head.Sync)
 {
  i = 0;
  while (i < PProtoclSendBuff_Number)
  {
   if((0 == KingDaV1send_buff[i].CCR.Bit.idle)
      && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == Close_Repeaters) 
      && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
   {
     if(KingDaV1send_buff[i].buff.Data.TypeA.pram.Close_Repeaters_ask.RepeatersId== pbuff->TypeA.pram.Close_Repeaters_ack.RepeatersId)
     {break;}
   }
   i ++;
  }
  if(i < PProtoclSendBuff_Number)
  {
   if(P_NULL != KingDaV1send_buff[i].AckFunc)
   {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), pbuff);} // Ӧ�����ݴ������
   else
   {
    if(Null_Port!= KingDaV1send_buff[i].SourcePort)
    {
      Trans.CSR.Bit.Preamble =  0;
      Trans.pbuff = pbuff;
      Trans.SourcePort = Null_Port;
      Trans.TargetPort = KingDaV1send_buff[i].SourcePort;
      Transpond_KingDaSPCP (&Trans); 
    }
   }
   FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //�ͷŻ����� 
  }  
 }
 else
 {
   Trans.CSR.Bit.Preamble =  0;
   Trans.pbuff = pbuff;
   Trans.SourcePort = InPort_PV;
   Trans.TargetPort = getPortForAddr (pbuff->TypeA.pram.Close_Repeaters_ask.RepeatersId);
   Transpond_KingDaSPCP (&Trans); 
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
static void ReadChannelNodeNumber_Repeaters_func (KindgDaSPCP_PackageDataStruct* pbuff) //��ȡ�м���N�¸���ͨ��ˮ������
{
 unsigned char i;
 Transpond_KingDaSPCP_struct  Trans;
 
 if(PProtocl_Sync_Up == pbuff->TypeA.head.Sync)
 {
  i = 0;
  while (i < PProtoclSendBuff_Number)
  {
   if((0 == KingDaV1send_buff[i].CCR.Bit.idle)
      && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == ReadChannelNodeNumber_Repeaters) 
      && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
   {
     if(KingDaV1send_buff[i].buff.Data.TypeA.pram.ReadChannelNodeNumber_Repeaters_ask.id 
        == pbuff->TypeA.pram.ReadChannelNodeNumber_Repeaters_ack.id)
     {break;}
   }
   i ++;
  }
  if(i < PProtoclSendBuff_Number)
  {
   if(P_NULL != KingDaV1send_buff[i].AckFunc)
   {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), pbuff);} // Ӧ�����ݴ������
   else
   {
    if(Null_Port!= KingDaV1send_buff[i].SourcePort)
    {
      Trans.CSR.Bit.Preamble =  0;
      Trans.pbuff = pbuff;
      Trans.SourcePort = Null_Port;
      Trans.TargetPort = KingDaV1send_buff[i].SourcePort;
      Transpond_KingDaSPCP (&Trans); 
    }
   }
   FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //�ͷŻ����� 
  }  
 }
 else
 {
   Trans.CSR.Bit.Preamble =  0;
   Trans.pbuff = pbuff;
   Trans.SourcePort = InPort_PV;
   i = pbuff->TypeA.pram.ReadChannelNodeNumber_Repeaters_ask.id >> 4;
   i *= 10;
   i += pbuff->TypeA.pram.ReadChannelNodeNumber_Repeaters_ask.id & 0x0F; 
   Trans.TargetPort = getPortForAddr (i );
   Transpond_KingDaSPCP (&Trans); 
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
static void ReadGprsRssi_func (KindgDaSPCP_PackageDataStruct* pbuff) //��ȡ�ź�ǿ��
{
 unsigned char itemp;
 unsigned char crc;
 PProtocolSendBuff_Struct*  buff;
 
 if(0xAB != pbuff->TypeA.pram.WriteServerInf_ask.DI1)
 {return;}

 buff = GetIdleBuff_PProtocol ( );
 if(P_NULL == buff){return;}
 buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;
 buff->SourcePort = Null_Port;
 buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
 buff->buff.Data.TypeB.head.Cmd = pbuff->TypeA.head.Cmd;
 buff->buff.Data.TypeB.head.Length =  sizeof(ReadGprsRssi_ACKstruct);
 buff->buff.Data.TypeB.pram.ReadGprsRssi_ack.DI1 = 0xAB;
 buff->buff.Data.TypeB.pram.ReadGprsRssi_ack.Rssi = MG2618Infr.ModInf.Rssi;
 itemp = sizeof (PPro_PackageHead_TypeB) +  sizeof(ReadGprsRssi_ACKstruct);;
 crc = CRC8((unsigned char *) &(buff->buff.Data.TypeB.head),itemp);  
 ((unsigned char *) &(buff->buff.Data.TypeB.head))[itemp] = crc;
 PProtocol_send (InPort_PV, buff); 
}
/*********************************************************************************
 Function:      //
 Description:   //ɾ����������
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void DeleteNet_APPCON_AckFunc (APPCON_OutPutBuff_Struct *cmd, 
                                      APPCON_AckType AckType, 
                                      DataBuffSpec_Struct* AckData)
{
 unsigned char itemp;
 unsigned char crc;
 PProtocolSendBuff_Struct*  buff;
 
 if(DELNET != cmd->Cmd)
 {return ;}
 
 buff = GetIdleBuff_PProtocol ( );
 if(P_NULL == buff){return;}
 buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;
 buff->SourcePort = Null_Port;
 buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
 buff->buff.Data.TypeB.head.Cmd = DeleteNet_APPCON;
 if(OK_APPCON == AckType)
 {
   buff->buff.Data.TypeB.head.Length = 2;
   buff->buff.Data.TypeB.pram.data[0]= 'O';
   buff->buff.Data.TypeB.pram.data[1]= 'K';
 }
 else if(NoAck_APPCON == AckType)
 {
   buff->buff.Data.TypeB.head.Length = 5;
   buff->buff.Data.TypeB.pram.data[0]= 'N';
   buff->buff.Data.TypeB.pram.data[1]= 'o';
   buff->buff.Data.TypeB.pram.data[2]= 'A';
   buff->buff.Data.TypeB.pram.data[3]= 'c';
   buff->buff.Data.TypeB.pram.data[4]= 'k';
 }
 else
 {
   buff->buff.Data.TypeB.head.Length = 3;
   buff->buff.Data.TypeB.pram.data[0]= 'E';
   buff->buff.Data.TypeB.pram.data[1]= 'R';
   buff->buff.Data.TypeB.pram.data[2]= 'R';
 }
 itemp = sizeof (PPro_PackageHead_TypeB) + buff->buff.Data.TypeB.head.Length;
 crc = CRC8((unsigned char *) &(buff->buff.Data.TypeB.head),itemp);  
 ((unsigned char *) &(buff->buff.Data.TypeB.head))[itemp] = crc;
 freeSendBuff_APPCON(cmd);
 PProtocol_send (cmd->SourcePort, buff); 
}
/*********************************************************************************
 Function:      //
 Description:   //ɾ����������
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void DeleteNet_APPCON_func (KindgDaSPCP_PackageDataStruct* pbuff)
{
 APPCON_DELNET_SendFunc (InPort_PV,P_NULL,DeleteNet_APPCON_AckFunc); 
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
static void BATCH_APPCON_AckFunc (APPCON_OutPutBuff_Struct *cmd, 
                                   APPCON_AckType AckType, 
                                   DataBuffSpec_Struct* AckData)
{
 unsigned char itemp;
 unsigned char crc;
 PProtocolSendBuff_Struct*  buff;
 
 if(BATCH != cmd->Cmd)
 {return ;}
 
 buff = GetIdleBuff_PProtocol ( );
 if(P_NULL == buff){return;}
 buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;
 buff->SourcePort = Null_Port;
 buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
 buff->buff.Data.TypeB.head.Cmd = BATCH_APPCON;
 itemp = 0;
 switch(AckType)
 {
   case OK_APPCON:
     {
       buff->buff.Data.TypeB.pram.data[itemp++]= 'O';
       buff->buff.Data.TypeB.pram.data[itemp++]= 'K';
       break;
     }
   default:
     {
       buff->buff.Data.TypeB.pram.data[itemp++]= 'E';
       buff->buff.Data.TypeB.pram.data[itemp++]= 'R';
       buff->buff.Data.TypeB.pram.data[itemp++]= 'R';
       break;
     }
 }
 buff->buff.Data.TypeB.head.Length = itemp;
 itemp = sizeof (PPro_PackageHead_TypeB) + buff->buff.Data.TypeB.head.Length;
 crc = CRC8((unsigned char *) &(buff->buff.Data.TypeB.head),itemp);  
 ((unsigned char *) &(buff->buff.Data.TypeB.head))[itemp] = crc;
 freeSendBuff_APPCON(cmd);
 PProtocol_send (cmd->SourcePort, buff); 
}
/*********************************************************************************
 Function:      //
 Description:   //ά�����粢��������
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void BATCH_APPCON_func (KindgDaSPCP_PackageDataStruct* pbuff)
{
  APPCON_BATCH_SendFunc (InPort_PV,P_NULL,BATCH_APPCON_AckFunc);
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
static void STATUS_APPCON_AckFunc (APPCON_OutPutBuff_Struct *cmd, 
                                   APPCON_AckType AckType, 
                                   DataBuffSpec_Struct* AckData)
{
 unsigned char itemp;
 unsigned char crc;
 PProtocolSendBuff_Struct*  buff;
 
 if(STATUS != cmd->Cmd)
 {return ;}
 
 buff = GetIdleBuff_PProtocol ( );
 if(P_NULL == buff){return;}
 buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;
 buff->SourcePort = Null_Port;
 buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
 buff->buff.Data.TypeB.head.Cmd = STATUS_APPCON;
 itemp = 0;
 switch(AckType)
 {
   case BUSY_APPCON:
     {
       buff->buff.Data.TypeB.pram.data[itemp++]= 'B';
       buff->buff.Data.TypeB.pram.data[itemp++]= 'U';
       buff->buff.Data.TypeB.pram.data[itemp++]= 'S';
       buff->buff.Data.TypeB.pram.data[itemp++]= 'Y';
       break;
     }
   case FREE_APPCON:
     {
       buff->buff.Data.TypeB.pram.data[itemp++]= 'F';
       buff->buff.Data.TypeB.pram.data[itemp++]= 'R';
       buff->buff.Data.TypeB.pram.data[itemp++]= 'E';
       buff->buff.Data.TypeB.pram.data[itemp++]= 'E';
       break;
     }
   case NoAns_APPCON:
     {
       buff->buff.Data.TypeB.pram.data[itemp++]= 'N';
       buff->buff.Data.TypeB.pram.data[itemp++]= 'o';
       buff->buff.Data.TypeB.pram.data[itemp++]= 'A';
       buff->buff.Data.TypeB.pram.data[itemp++]= 'n';
       buff->buff.Data.TypeB.pram.data[itemp++]= 's';
       break;
     }
   default:
     {
       buff->buff.Data.TypeB.pram.data[itemp++]= 'E';
       buff->buff.Data.TypeB.pram.data[itemp++]= 'R';
       buff->buff.Data.TypeB.pram.data[itemp++]= 'R';
       break;
     }
 }
 buff->buff.Data.TypeB.head.Length = itemp;
 itemp = sizeof (PPro_PackageHead_TypeB) + buff->buff.Data.TypeB.head.Length;
 crc = CRC8((unsigned char *) &(buff->buff.Data.TypeB.head),itemp);  
 ((unsigned char *) &(buff->buff.Data.TypeB.head))[itemp] = crc;
 freeSendBuff_APPCON(cmd);
 PProtocol_send (cmd->SourcePort, buff); 
}
/*********************************************************************************
 Function:      //
 Description:   //����״̬��ѯ����
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void STATUS_APPCON_func (KindgDaSPCP_PackageDataStruct* pbuff)
{
  APPCON_STATUS_SendFunc (InPort_PV,P_NULL,STATUS_APPCON_AckFunc);
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
static void ReadNetId_APPCON_AckFunc (APPCON_OutPutBuff_Struct *cmd, 
                                      APPCON_AckType AckType, 
                                      DataBuffSpec_Struct* AckData)
{
 unsigned char itemp;
 unsigned char crc;
 //unsigned short id;
 PProtocolSendBuff_Struct*  buff;
 
 if(NETID != cmd->Cmd)
 {return ;}
 
 buff = GetIdleBuff_PProtocol ( );
 if(P_NULL == buff){return;}
 buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;
 buff->SourcePort = Null_Port;
 buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
 buff->buff.Data.TypeB.head.Cmd = ReadNetId_APPCON;
 itemp = 0;
 switch(AckType)        
 {
   case OK_APPCON:
     {
       while(itemp < AckData->length)
       {
         if(('\r' == AckData->pbuff[itemp])
            ||('\n' == AckData->pbuff[itemp]))
         {break;}
         else
         {buff->buff.Data.TypeB.pram.data[itemp] = AckData->pbuff[itemp];}
         itemp ++;
       }
       break;
     }
   case NoAns_APPCON:
     {
       buff->buff.Data.TypeB.pram.data[itemp++]= 'N';
       buff->buff.Data.TypeB.pram.data[itemp++]= 'o';
       buff->buff.Data.TypeB.pram.data[itemp++]= 'A';
       buff->buff.Data.TypeB.pram.data[itemp++]= 'n';
       buff->buff.Data.TypeB.pram.data[itemp++]= 's';
       break;
     }
   default:
     {
       buff->buff.Data.TypeB.pram.data[itemp++]= 'E';
       buff->buff.Data.TypeB.pram.data[itemp++]= 'R';
       buff->buff.Data.TypeB.pram.data[itemp++]= 'R';
       break;
     }
 }
 buff->buff.Data.TypeB.head.Length = itemp;
 itemp = sizeof (PPro_PackageHead_TypeB) + buff->buff.Data.TypeB.head.Length;
 crc = CRC8((unsigned char *) &(buff->buff.Data.TypeB.head),itemp);  
 ((unsigned char *) &(buff->buff.Data.TypeB.head))[itemp] = crc;
 freeSendBuff_APPCON(cmd);
 PProtocol_send (cmd->SourcePort, buff);  
}
/*********************************************************************************
 Function:      //
 Description:   //��ȡ����ID����
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void ReadNetId_APPCON_func (KindgDaSPCP_PackageDataStruct* pbuff)
{
 APPCON_NETID_SendFunc (InPort_PV,
                        P_NULL,
                        P_NULL,            
                        ReadNetId_APPCON_AckFunc);
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
static void WriteNetId_APPCON_AckFunc (APPCON_OutPutBuff_Struct *cmd, 
                                      APPCON_AckType AckType, 
                                      DataBuffSpec_Struct* AckData)
{
 unsigned char itemp;
 unsigned char crc;
 PProtocolSendBuff_Struct*  buff;
 
 if(NETID != cmd->Cmd)
 {return ;}
 
 buff = GetIdleBuff_PProtocol ( );
 if(P_NULL == buff){return;}
 buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;
 buff->SourcePort = Null_Port;
 buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
 buff->buff.Data.TypeB.head.Cmd = WriteNetId_APPCON;
 itemp = 0;
 switch(AckType)
 {
   case OK_APPCON:
     {
       buff->buff.Data.TypeB.pram.data[itemp++]= 'O';
       buff->buff.Data.TypeB.pram.data[itemp++]= 'K';
       break;
     }
   default:
     {
       buff->buff.Data.TypeB.pram.data[itemp++]= 'E';
       buff->buff.Data.TypeB.pram.data[itemp++]= 'R';
       buff->buff.Data.TypeB.pram.data[itemp++]= 'R';
       break;
     }
 }
 buff->buff.Data.TypeB.head.Length = itemp;
 itemp = sizeof (PPro_PackageHead_TypeB) + buff->buff.Data.TypeB.head.Length;
 crc = CRC8((unsigned char *) &(buff->buff.Data.TypeB.head),itemp);  
 ((unsigned char *) &(buff->buff.Data.TypeB.head))[itemp] = crc;
 freeSendBuff_APPCON(cmd);
 PProtocol_send (cmd->SourcePort, buff); 
}
/*********************************************************************************
 Function:      //
 Description:   //д����ID����
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void WriteNetId_APPCON_func (KindgDaSPCP_PackageDataStruct* pbuff)
{ 
  APPCON_NETID_SendFunc (InPort_PV,
                         pbuff->TypeB.pram.data,
                         P_NULL,            
                         WriteNetId_APPCON_AckFunc); 
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
static void RDNODE_APPCON_AckFunc (APPCON_OutPutBuff_Struct *cmd, 
                                      APPCON_AckType AckType, 
                                      DataBuffSpec_Struct* AckData)
{
 //static unsigned long  iiiiiiiii = 0;
 unsigned char itemp;
 unsigned char crc;
 unsigned long con;
 PProtocolSendBuff_Struct*  buff;
  
  if((RDNODE != cmd->Cmd)
     ||(TOTAL_APPCON != AckType))
  {return;}
  
 
 buff = GetIdleBuff_PProtocol ( );
 if(P_NULL == buff){return;}
  
  while(!(IsDigit(*(AckData->pbuff))))
  {
    if(0 == --AckData->length)
    {
      FreeBuff_PProtocol(buff);
      return;
    }
    AckData->pbuff ++;
  }
  con = 0;
  while(IsDigit(*(AckData->pbuff)))
  {
    con *= 10;
    con += *(AckData->pbuff) - '0';  
    if(0 == --AckData->length)
    {
      FreeBuff_PProtocol(buff);
      return;
    }
    AckData->pbuff ++;
  }
 
 buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;
 buff->SourcePort = Null_Port;
 buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
 buff->buff.Data.TypeB.head.Cmd = RDNODE_APPCON;
 itemp = 0;
 //con = iiiiiiiii;
 buff->buff.Data.TypeB.pram.data[itemp++]= con>> 16;
 buff->buff.Data.TypeB.pram.data[itemp++]= con>> 8;
 buff->buff.Data.TypeB.pram.data[itemp++]= con;

 buff->buff.Data.TypeB.head.Length = itemp;
 itemp = sizeof (PPro_PackageHead_TypeB) + buff->buff.Data.TypeB.head.Length;
 crc = CRC8((unsigned char *) &(buff->buff.Data.TypeB.head),itemp);  
 ((unsigned char *) &(buff->buff.Data.TypeB.head))[itemp] = crc;
 freeSendBuff_APPCON(cmd);
 PProtocol_send (cmd->SourcePort, buff); 
  
}
/*********************************************************************************
 Function:      //
 Description:   //��ȡ���������߽ڵ�ID������
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void RDNODE_APPCON_func (KindgDaSPCP_PackageDataStruct* pbuff)
{ 
 APPCON_RDNODE_SendFunc(InPort_PV,
                        P_NULL,
                        0,
                        P_NULL,
                        RDNODE_APPCON_AckFunc);
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
static void GetROUTER_APPCON_AckFunc (APPCON_OutPutBuff_Struct *cmd, 
                                      APPCON_AckType AckType, 
                                      DataBuffSpec_Struct* AckData)
{
 unsigned char itemp;
 unsigned char crc;
 unsigned long con;
 PProtocolSendBuff_Struct*  buff;
  
  if((RDROUTER != cmd->Cmd)
     ||(TOTAL_APPCON != AckType))
  {return;}
  
 
 buff = GetIdleBuff_PProtocol ( );
 if(P_NULL == buff){return;}
  
  while(!(IsDigit(*(AckData->pbuff))))
  {
    if(0 == --AckData->length)
    {
      FreeBuff_PProtocol(buff);
      return;
    }
    AckData->pbuff ++;
  }
  con = 0;
  while(IsDigit(*(AckData->pbuff)))
  {
    con *= 10;
    con += *(AckData->pbuff) - '0';  
    if(0 == --AckData->length)
    {
      FreeBuff_PProtocol(buff);
      return;
    }
    AckData->pbuff ++;
  }
 
 buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;
 buff->SourcePort = Null_Port;
 buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
 buff->buff.Data.TypeB.head.Cmd = RDNODE_APPCON;
 itemp = 0;
 buff->buff.Data.TypeB.pram.data[itemp++]= con>> 16;
 buff->buff.Data.TypeB.pram.data[itemp++]= con>> 8;
 buff->buff.Data.TypeB.pram.data[itemp++]= con;

 buff->buff.Data.TypeB.head.Length = itemp;
 itemp = sizeof (PPro_PackageHead_TypeB) + buff->buff.Data.TypeB.head.Length;
 crc = CRC8((unsigned char *) &(buff->buff.Data.TypeB.head),itemp);  
 ((unsigned char *) &(buff->buff.Data.TypeB.head))[itemp] = crc;
 freeSendBuff_APPCON(cmd);
 PProtocol_send (cmd->SourcePort, buff); 
  
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
static void GetROUTER_APPCON_func (KindgDaSPCP_PackageDataStruct* pbuff)
{
 SystemErrName err; 
  
 err = APPCON_RDROUTER_SendFunc(InPort_PV,
                                //P_NULL,
                                0,
                                P_NULL,
                                GetROUTER_APPCON_AckFunc); 
 if(NO_ERR == err)
 {LastCmd = GetOV_APPCON;}
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
static void ReadFreq_APPCON_AckFunc (APPCON_OutPutBuff_Struct *cmd, 
                                      APPCON_AckType AckType, 
                                      DataBuffSpec_Struct* AckData)
{ 
 unsigned char itemp;
 unsigned char crc;
 PProtocolSendBuff_Struct*  buff;
 
 if(FREQ != cmd->Cmd)
 {return ;}
 
 buff = GetIdleBuff_PProtocol ( );
 if(P_NULL == buff){return;}
 buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;
 buff->SourcePort = Null_Port;
 buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
 buff->buff.Data.TypeB.head.Cmd = ReadFreq_APPCON;
 itemp = 0;
 switch(AckType)        
 {
   case OK_APPCON:
     {
       while(itemp < AckData->length)
       {
         if(('\r' == AckData->pbuff[itemp])
            ||('\n' == AckData->pbuff[itemp]))
         {break;}
         else
         {buff->buff.Data.TypeB.pram.data[itemp] = AckData->pbuff[itemp];}
         itemp ++;
       }
       break;
     }
   case NoAck_APPCON:
     {
       buff->buff.Data.TypeB.pram.data[itemp++]= 'N';
       buff->buff.Data.TypeB.pram.data[itemp++]= 'o';
       buff->buff.Data.TypeB.pram.data[itemp++]= 'A';
       buff->buff.Data.TypeB.pram.data[itemp++]= 'c';
       buff->buff.Data.TypeB.pram.data[itemp++]= 'k';
       break;
     }
   default:
     {
       buff->buff.Data.TypeB.pram.data[itemp++]= 'E';
       buff->buff.Data.TypeB.pram.data[itemp++]= 'R';
       buff->buff.Data.TypeB.pram.data[itemp++]= 'R';
       break;
     }
 }
 buff->buff.Data.TypeB.head.Length = itemp;
 itemp = sizeof (PPro_PackageHead_TypeB) + buff->buff.Data.TypeB.head.Length;
 crc = CRC8((unsigned char *) &(buff->buff.Data.TypeB.head),itemp);  
 ((unsigned char *) &(buff->buff.Data.TypeB.head))[itemp] = crc;
 freeSendBuff_APPCON(cmd);
 PProtocol_send (cmd->SourcePort, buff); 
}
/*********************************************************************************
 Function:      //
 Description:   //��Ƶ�ʲ�������
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void ReadFreq_APPCON_func (KindgDaSPCP_PackageDataStruct* pbuff)
{
  APPCON_FREQ_SendFunc (InPort_PV,
                       P_NULL,
                       P_NULL,
                       ReadFreq_APPCON_AckFunc); 
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
static void WriteFreq_APPCON_AckFunc (APPCON_OutPutBuff_Struct *cmd, 
                                      APPCON_AckType AckType, 
                                      DataBuffSpec_Struct* AckData)
{
 unsigned char itemp;
 unsigned char crc;
 PProtocolSendBuff_Struct*  buff;
 
 if(FREQ != cmd->Cmd)
 {return ;}
 
 buff = GetIdleBuff_PProtocol ( );
 if(P_NULL == buff){return;}
 buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;
 buff->SourcePort = Null_Port;
 buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
 buff->buff.Data.TypeB.head.Cmd = WriteFreq_APPCON;
 itemp = 0;
 switch(AckType)
 {
   case OK_APPCON:
     {
       buff->buff.Data.TypeB.pram.data[itemp++]= 'O';
       buff->buff.Data.TypeB.pram.data[itemp++]= 'K';
       break;
     }
   default:
     {
       buff->buff.Data.TypeB.pram.data[itemp++]= 'E';
       buff->buff.Data.TypeB.pram.data[itemp++]= 'R';
       buff->buff.Data.TypeB.pram.data[itemp++]= 'R';
       break;
     }
 }
 buff->buff.Data.TypeB.head.Length = itemp;
 itemp = sizeof (PPro_PackageHead_TypeB) + buff->buff.Data.TypeB.head.Length;
 crc = CRC8((unsigned char *) &(buff->buff.Data.TypeB.head),itemp);  
 ((unsigned char *) &(buff->buff.Data.TypeB.head))[itemp] = crc;
 freeSendBuff_APPCON(cmd);
 PProtocol_send (cmd->SourcePort, buff); 
}
/*********************************************************************************
 Function:      //
 Description:   //дƵ�ʲ�������
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void WriteFreq_APPCON_func (KindgDaSPCP_PackageDataStruct* pbuff)
{ 
 APPCON_FREQ_SendFunc (InPort_PV,
                       pbuff->TypeB.pram.data,
                       P_NULL,
                       WriteFreq_APPCON_AckFunc); 
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
/*static void WriteRate_APPCON_AckFunc (APPCON_OutPutBuff_Struct *cmd, 
                                      APPCON_AckType AckType, 
                                      DataBuffSpec_Struct* AckData)
{
  
}*/
/*********************************************************************************
 Function:      //
 Description:   //д������������������
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void WriteRate_APPCON_func (KindgDaSPCP_PackageDataStruct* pbuff)
{
  
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
 unsigned char itemp;
 unsigned char crc;
 unsigned long value;
 APPCON_CMD_AckData_Struct* Datas;
 PProtocolSendBuff_Struct*  buff;
 
 
 if(CMD != cmd->Cmd)
 {return ;}
 
 buff = GetIdleBuff_PProtocol ( );
 if(P_NULL == buff){return;}
 buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;
 buff->SourcePort = Null_Port;
 buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
 buff->buff.Data.TypeB.head.Cmd = ReadNodeValue_APPCON;
 itemp = 0;
 switch(AckType)
 {
   case Ans_APPCON:
     {
       if(AckData->length < sizeof (APPCON_CMD_AckData_Struct))
       {
         FreeBuff_PProtocol(buff);
         return;
       }
       Datas = (APPCON_CMD_AckData_Struct*)AckData->pbuff;
       value = APPCON_AsciiHexToDigit((unsigned char*)Datas->value,12,8);
       buff->buff.Data.TypeB.pram.data[itemp++]= value >>24;
       buff->buff.Data.TypeB.pram.data[itemp++]= value >>16;
       buff->buff.Data.TypeB.pram.data[itemp++]= value >>8;
       buff->buff.Data.TypeB.pram.data[itemp++]= value ;
       buff->buff.Data.TypeB.pram.data[itemp++]= APPCON_AsciiHexToDigit(Datas->bat,2,2) ;
       
       break;
     }
   default:
     {
       while (itemp < AckData->length)
       {
         if(('\r' == AckData->pbuff[itemp])
            ||('\n' == AckData->pbuff[itemp]))
         { break;}
         else
         {
           buff->buff.Data.TypeB.pram.data[itemp]= AckData->pbuff[itemp];
           itemp ++;
         }
       }
       break;
     }
 }
 buff->buff.Data.TypeB.head.Length = itemp;
 itemp = sizeof (PPro_PackageHead_TypeB) + buff->buff.Data.TypeB.head.Length;
 crc = CRC8((unsigned char *) &(buff->buff.Data.TypeB.head),itemp);  
 ((unsigned char *) &(buff->buff.Data.TypeB.head))[itemp] = crc;
 freeSendBuff_APPCON(cmd);
 PProtocol_send (cmd->SourcePort, buff); 
}
/*********************************************************************************
 Function:      //
 Description:   //��ȡ���߱�������
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void ReadNodeValue_APPCON_func (KindgDaSPCP_PackageDataStruct* pbuff) 
{
  APPCON_CMD_SendFunc (InPort_PV,
                        pbuff->TypeA.pram.data,
                        P_NULL,
                        ReadNodeValue_APPCON_AckFunc); 
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
static void ValveCmd_APPCON_AckFunc (APPCON_OutPutBuff_Struct *cmd, 
                                   APPCON_AckType AckType, 
                                   DataBuffSpec_Struct* AckData)
{
 unsigned char itemp;
 unsigned char crc;
 unsigned char Ack;
 PProtocolSendBuff_Struct*  buff;

 buff = GetIdleBuff_PProtocol ( );
 if(P_NULL == buff){return;}
 buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;
 buff->SourcePort = Null_Port;
 buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;

 switch (LastCmd)
 {
  case OpenOV_APPCON :    //����
  case HalfOV_APPCON :    //�뿪����
  case MtOV_APPCON :      //�������
  case CloseOV_APPCON :   //�ط�
  case GetOV_APPCON :     //����״̬
    {
      buff->buff.Data.TypeB.head.Cmd = LastCmd;
      break ;
    }
  default:
    { 
      FreeBuff_PProtocol(buff);
      return;
    }
 }
 itemp = 0;
 if(Ans_APPCON == AckType)
 {
   Ack = APPCON_AsciiHexToDigit(AckData->pbuff,2,2);
   switch(Ack)
   {
    case ValveUnState_APPCON:  //���Ų���״̬
      {
        buff->buff.Data.TypeB.pram.data[itemp++] = 0x50;
        buff->buff.Data.TypeB.pram.data[itemp++] = 0x04;
        break;
      }
    case HalfValveState_APPCON:  //���Ű뿪״̬
      {
        buff->buff.Data.TypeB.pram.data[itemp++] = 0x50;
        buff->buff.Data.TypeB.pram.data[itemp++] = 0x08;
        break;
      }
    case CloseValveState_APPCON:  //���Źر�״̬
      {
        buff->buff.Data.TypeB.pram.data[itemp++] = 0x50;
        buff->buff.Data.TypeB.pram.data[itemp++] = 0x06;
        break;
      } 
    case OpenValveState_APPCON:  //���Ŵ�״̬
      {
        buff->buff.Data.TypeB.pram.data[itemp++] = 0x50;
        buff->buff.Data.TypeB.pram.data[itemp++] = 0x05;
        break;
      } 
   }
 }
 else if(NoAck_APPCON == AckType)
 {
   buff->buff.Data.TypeB.pram.data[itemp++] = 'N';
   buff->buff.Data.TypeB.pram.data[itemp++] = 'o';
   buff->buff.Data.TypeB.pram.data[itemp++] = 'A';
   buff->buff.Data.TypeB.pram.data[itemp++] = 'c';
   buff->buff.Data.TypeB.pram.data[itemp++] = 'k';
 }
 else
 {
   while (itemp < AckData->length)
   {
     if(('\r' == AckData->pbuff[itemp])
        ||('\n' == AckData->pbuff[itemp]))
     { break;}
     else
     {
       buff->buff.Data.TypeB.pram.data[itemp]= AckData->pbuff[itemp];
       itemp ++;
     }
   } 
 }


 buff->buff.Data.TypeB.head.Length = itemp;
 itemp = sizeof (PPro_PackageHead_TypeB) + buff->buff.Data.TypeB.head.Length;
 crc = CRC8((unsigned char *) &(buff->buff.Data.TypeB.head),itemp);  
 ((unsigned char *) &(buff->buff.Data.TypeB.head))[itemp] = crc;
 freeSendBuff_APPCON(cmd);
 PProtocol_send (cmd->SourcePort, buff);  
}
/*********************************************************************************
 Function:      //
 Description:   //����
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void OpenOV_APPCON_func (KindgDaSPCP_PackageDataStruct* pbuff )
{
  SystemErrName err; 
  err = APPCON_ValveCmd_SendFunc( InPort_PV,
                                  pbuff->TypeA.pram.data,
                                  OpenValveCmd_APPCON,
                                  P_NULL,
                                  ValveCmd_APPCON_AckFunc);
 if(NO_ERR == err)
 {LastCmd = OpenOV_APPCON;}
}
/*********************************************************************************
 Function:      //
 Description:   //�뿪����
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void HalfOV_APPCON_func (KindgDaSPCP_PackageDataStruct* pbuff)
{
  SystemErrName err; 
  err = APPCON_ValveCmd_SendFunc( InPort_PV,
                                  pbuff->TypeA.pram.data,
                                  HalfValveCmd_APPCON,
                                  P_NULL,
                                  ValveCmd_APPCON_AckFunc); 
 if(NO_ERR == err)
 {LastCmd = HalfOV_APPCON;}
}

/*********************************************************************************
 Function:      //�������
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void MtOV_APPCON_func (KindgDaSPCP_PackageDataStruct* pbuff)
{
  SystemErrName err; 
  err = APPCON_ValveCmd_SendFunc(InPort_PV,
                                 pbuff->TypeA.pram.data,
                                 MT_ValveCmd_APPCON,
                                 P_NULL,
                                 ValveCmd_APPCON_AckFunc);
 if(NO_ERR == err)
 {LastCmd = MtOV_APPCON;}
}
/*********************************************************************************
 Function:      //
 Description:   //�ط�
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void CloseOV_APPCON_func (KindgDaSPCP_PackageDataStruct* pbuff)
{
 SystemErrName err;  
 err = APPCON_ValveCmd_SendFunc(InPort_PV,
                                pbuff->TypeA.pram.data,
                                CloseValveCmd_APPCON,
                                P_NULL,
                                ValveCmd_APPCON_AckFunc);
 if(NO_ERR == err)
 {LastCmd = CloseOV_APPCON;}
}
/*********************************************************************************
 Function:      //
 Description:   //����״̬
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static void GetOV_APPCON_func (KindgDaSPCP_PackageDataStruct* pbuff)
{
 SystemErrName err; 
 err = APPCON_ValveCmd_SendFunc(InPort_PV,
                                pbuff->TypeA.pram.data,
                                GetValveCmd_APPCON,
                                P_NULL,
                                ValveCmd_APPCON_AckFunc); 
 if(NO_ERR == err)
 {LastCmd = GetOV_APPCON;}
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
static void ReadGprsRssi_func (KindgDaSPCP_PackageDataStruct* pbuff) //��ȡ�ź�ǿ��
{
 static ReadGprsRssi_AckPacage_struct AckPacage;  
 unsigned char itemp;

 if(0xAB != pbuff->TypeA.pram.WriteServerInf_ask.DI1)
 {return;}

 AckPacage.Head.Sync = PProtocl_Sync_Up;
 AckPacage.Head.Cmd = pbuff->TypeA.head.Cmd;
 AckPacage.Head.Length =  sizeof(ReadGprsRssi_ACKstruct);

 AckPacage.data.DI1 = 0xAB;
 AckPacage.data.Rssi = MG2618Infr.ModInf.Rssi;
 
 itemp = sizeof (PPro_PackageHead_struct2) +  sizeof(ReadGprsRssi_ACKstruct);
 AckPacage.crc = CRC8_Table((unsigned char *) &(AckPacage),itemp); 

 
  MXPC_Send_AutoRetrans (  InPort_PV,                     //Ŀ�Ķ˿�
                           &AckPacage,                   //���ݻ�����
                           sizeof(ReadGprsRssi_AckPacage_struct),                           //���ݳ���
                           0,                                //�ش�����
                           80,                               //�ش�������
                           TimerTB_10MS ,                    //�ش�����
                           PProtocol_SendEnd_CallBack,       //������ɻص�
                           P_NULL);                          //�ط���ɻص�
 
 //PProtocol_send (InPort_PV, buff); 
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
static void WriteChannelNodeNumber_Repeaters_func (KindgDaSPCP_PackageDataStruct* pbuff) //�����м�����ͨ��ˮ������
{
 unsigned char i;
 Transpond_KingDaSPCP_struct  Trans;
 
 if(PProtocl_Sync_Up == pbuff->TypeA.head.Sync)
 {
  i = 0;
  while (i < PProtoclSendBuff_Number)
  {
   if((0 == KingDaV1send_buff[i].CCR.Bit.idle)
      && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == WriteChannelNodeNumber_Repeaters) 
      && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
   {
     if(KingDaV1send_buff[i].buff.Data.TypeA.pram.WriteChannelNodeNumber_Repeaters_ask.id 
        == pbuff->TypeA.pram.WriteChannelNodeNumber_Repeaters_ack.id)
     {break;}
   }
   i ++;
  }
  if(i < PProtoclSendBuff_Number)
  {
   if(P_NULL != KingDaV1send_buff[i].AckFunc)
   {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), pbuff);} // Ӧ�����ݴ������
   else
   {
    if(Null_Port!= KingDaV1send_buff[i].SourcePort)
    {
      Trans.CSR.Bit.Preamble =  0;
      Trans.pbuff = pbuff;
      Trans.SourcePort = Null_Port;
      Trans.TargetPort = KingDaV1send_buff[i].SourcePort;
      Transpond_KingDaSPCP (&Trans); 
    }
   }
   FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //�ͷŻ����� 
  }  
 }
 else
 {
   Trans.CSR.Bit.Preamble =  0;
   Trans.pbuff = pbuff;
   Trans.SourcePort = InPort_PV;
   
   i = pbuff->TypeA.pram.WriteChannelNodeNumber_Repeaters_ask.id >> 4;
   i *= 10;
   i += pbuff->TypeA.pram.WriteChannelNodeNumber_Repeaters_ask.id & 0x0F; 
   
   
   Trans.TargetPort = getPortForAddr (i);
   Transpond_KingDaSPCP (&Trans); 
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

/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //              
Others:         // 
*********************************************************************************/
static unsigned short  BinChangeToBCD (unsigned char bin)
{
  unsigned char itemp = 0;
  unsigned char data[5];
  unsigned short re = 0;
  
  while (itemp < 4)
  {
    data[itemp++] = bin % 10;
    bin /= 10;
  }
  while(itemp -- >0)
  {
   re <<= 4; 
   re |= data[itemp]; 
  }
 return re;
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
/*unsigned char  BCDChangeToBin (unsigned char bcd)
{
 unsigned char itemp;
 itemp = bcd >> 4;
 itemp *= 10;
 itemp += bcd & 0x0f;
 return itemp;
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
static void PProtocol_SendEnd_CallBack(CommPortNumber_Type port, void *pbuff)
{
  unsigned long itemp;
  PProtocolSendBuff_Struct* p =  (PProtocolSendBuff_Struct*)0;
  itemp = (unsigned long)pbuff;
  if((*(unsigned char*)pbuff == PProtocl_Preamble_Down)
     ||(*(unsigned char*)pbuff == PProtocl_Preamble_Up))
  {itemp  -= (unsigned long)(&(p->buff));}
  else if((*(unsigned char*)pbuff == PProtocl_Sync_Down)
           ||(*(unsigned char*)pbuff == PProtocl_Sync_Up))
  {itemp  -= (unsigned long)(&(p->buff.Data.TypeA.head)); }
  else {return ;}
  FreeBuff_PProtocol((PProtocolSendBuff_Struct*) itemp);
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
static void PProtocol_ACK_TimerOutCallBack (void *pbuff)
{
  SystemErrName err = NO_ERR;
  unsigned long itemp;
  PProtocolSendBuff_Struct* p =  (PProtocolSendBuff_Struct*)0;
  itemp = (unsigned long)pbuff;
  if((*(unsigned char*)pbuff == PProtocl_Preamble_Down)
     ||(*(unsigned char*)pbuff == PProtocl_Preamble_Up))
  {itemp  -= (unsigned long)(&(p->buff));}
  else if((*(unsigned char*)pbuff == PProtocl_Sync_Down)
           ||(*(unsigned char*)pbuff == PProtocl_Sync_Up))
  {itemp  -= (unsigned long)(&(p->buff.Data.TypeA.head)); }
  else {return ;}
  p = (PProtocolSendBuff_Struct*) itemp;
  err = PProtocolBuffVerify (p);
  if(NO_ERR == err)
  {
   if(P_NULL !=  p->AckFunc)
   {p->AckFunc(p,P_NULL);}
  }
  FreeBuff_PProtocol( p );
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
static SystemErrName PProtocol_send (CommPortNumber_Type port, PProtocolSendBuff_Struct *pbuff)
{
 void* p; 
 unsigned short itemp;
 unsigned short length ;
 if((P_NULL == pbuff) || (Null_Port == port))
 {return IllegalParam;}
 
 if(PProtoco_PackageTypeA == pbuff->CCR.Bit.TypeA_B)
 {
   if(0 != pbuff->CCR.Bit.Preamble)
   {
     if(PProtocl_Sync_Down == pbuff->buff.Data.TypeA.head.Sync) 
     {
       pbuff->buff.Prm1 = PProtocl_Preamble_Down;
       pbuff->buff.Prm2 = PProtocl_Preamble_Down;
     }
     else
     {
       pbuff->buff.Prm1 = PProtocl_Preamble_Up;
       pbuff->buff.Prm2 = PProtocl_Preamble_Up;
     }
     length =  2 + sizeof (PPro_PackageHead_TypeA) + 1;
     p = &(pbuff->buff);
   }
   else
   {
     length =  sizeof (PPro_PackageHead_TypeA) + 1;
     p = &(pbuff->buff.Data.TypeA.head);
   } 
   length += pbuff->buff.Data.TypeA.head.Length; 
 }
 else
 {
   if(0 != pbuff->CCR.Bit.Preamble)
   {
     if(PProtocl_Sync_Down == pbuff->buff.Data.TypeB.head.Sync) 
     {
       pbuff->buff.Prm1 = PProtocl_Preamble_Down;
       pbuff->buff.Prm2 = PProtocl_Preamble_Down;
     }
     else
     {
       pbuff->buff.Prm1 = PProtocl_Preamble_Up;
       pbuff->buff.Prm2 = PProtocl_Preamble_Up;
     }
     length =  2 + sizeof (PPro_PackageHead_TypeB) ;
     p = &(pbuff->buff);
   }
   else
   {
     length =  sizeof (PPro_PackageHead_TypeB);
     p = &(pbuff->buff.Data.TypeB.head);
   } 
   length += pbuff->buff.Data.TypeB.head.Length; 
   itemp = pbuff->buff.Data.TypeB.head.Length >> 8;
   itemp |= pbuff->buff.Data.TypeB.head.Length << 8;
   pbuff->buff.Data.TypeB.head.Length = itemp; 
   itemp = CRC8((unsigned char *) &(pbuff->buff.Data),length);
   ((unsigned char *) &(pbuff->buff.Data))[length ++] = itemp; //crc;
 }
 if(PProtocl_Sync_Down == pbuff->buff.Data.TypeA.head.Sync)
 {
  return MXPC_Send_AutoRetrans ( port,                             //Ŀ�Ķ˿�
                          p,                   //���ݻ�����
                          length,                           //���ݳ���
                          7,                                //�ش�����
                          30,                               //�ش�������
                          TimerTB_100MS ,                    //�ش�����
                          P_NULL,                           //������ɻص�
                          PProtocol_ACK_TimerOutCallBack);  //�ط���ɻص�
  }
  else
 {
   return MXPC_Send_AutoRetrans ( port,                             //Ŀ�Ķ˿�
                           p,                   //���ݻ�����
                           length,                           //���ݳ���
                           0,                                //�ش�����
                           80,                               //�ش�������
                           TimerTB_10MS ,                    //�ش�����
                           PProtocol_SendEnd_CallBack,       //������ɻص�
                           P_NULL);                          //�ط���ɻص�
  } 
}
/*********************************************************************************
 Function:      //
 Description:   //��ȡָ���ڵ����ֵ
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName ReadNodeValue_KindDa(unsigned char* NoderAddr,
                                   unsigned char SendPort,  
                                   CommPortNumber_Type SourcePort,
                                   KindgDaSPCPCmdCode_AckFunc AckFunc)
{
 ReadValue_Node_ASKstruct data;
 KindgDaCmd_Struct cmd;

 data.addr[0] = NoderAddr[0];
 data.addr[1] = NoderAddr[1];
 data.addr[2] = NoderAddr[2];
 data.NC = 0;
   
 cmd.Cmd = ReadValue;               //������
 cmd.SourcePort = SourcePort;
 if(AutomatchPort == SendPort)
 {cmd.TargetPort = NoderAddr[0];}  //���͵�Ԥ�ڵ�ַ
 else 
 { cmd.TargetPort = SendPort;}   //���͵�Ԥ�ڵ�ַ       
 cmd.Param = &data;                //�������
 cmd.length = sizeof(ReadValue_Node_ASKstruct);   //�����������
 cmd.AckFunc = AckFunc;
 cmd.CSR.Bit.DIR = DOWN;
 cmd.CSR.Bit.Preamble = 1;
 return SendCmd_KindDaSPCP(&cmd);
}
/*********************************************************************************
 Function:      //
 Description:   //����ָ���ڵ㷧��
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName  OperateValve_KindDa(CmdCode_KindgDaSPCP Operate,
                                   unsigned char* NoderAddr,
                                   unsigned char SendPort,
                                   CommPortNumber_Type SourcePort,                                   
                                   KindgDaSPCPCmdCode_AckFunc AckFunc)
{
 ReadValue_Node_ASKstruct data;
 KindgDaCmd_Struct cmd;
 
 if((Open_OV == Operate) ||(Open_OV_ENF == Operate))
 {Operate = Open_OV;}
 else if((Close_OV == Operate) ||(Close_OV_ENF == Operate))
 {Operate = Close_OV;}
 else if((Get_OV != Operate)&&((MT_OV != Operate)))
 {return CmdValid;}


 data.addr[0] = NoderAddr[0];
 data.addr[1] = NoderAddr[1];
 data.addr[2] = NoderAddr[2];
 data.NC = 0;
   
 cmd.Cmd = Operate;               //������
 cmd.SourcePort = SourcePort;
 if(AutomatchPort == SendPort)
 {cmd.TargetPort = NoderAddr[0];}  //���͵�Ԥ�ڵ�ַ
 else 
 { cmd.TargetPort = SendPort;}   //���͵�Ԥ�ڵ�ַ   
 cmd.Param = &data;                //�������
 cmd.length = sizeof(ReadValue_Node_ASKstruct);   //�����������
 cmd.AckFunc = AckFunc;
 cmd.CSR.Bit.DIR = DOWN;
 cmd.CSR.Bit.Preamble = 1;
 return SendCmd_KindDaSPCP(&cmd);
}
/*********************************************************************************
 Function:      //
 Description:   //��ָ���м�����ָ��ͨ��
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName OpenNETChannel_KindDa ( unsigned char RepeatersId, 
                                     unsigned char ChannelNumber,
                                     CommPortNumber_Type SourcePort,
                                      KindgDaSPCPCmdCode_AckFunc AckFunc)
{
 unsigned char itemp;
 OPen_Repeaters_ASKstruct data;
 KindgDaCmd_Struct cmd;

 data.RepeatersId   = RepeatersId % 10; 
 itemp = RepeatersId /10;
 itemp %= 10;
 itemp <<= 4;
 data.RepeatersId  |= itemp;
 data.ChannelNumber = ChannelNumber;
 cmd.SourcePort = SourcePort;
 cmd.Cmd = OPen_Repeaters;      //������
 cmd.TargetPort = RepeatersId;           //���͵�Ԥ�ڵ�ַ        
 cmd.Param = &data;                //�������
 cmd.length = sizeof(OPen_Repeaters_ASKstruct);   //�����������
 cmd.AckFunc = AckFunc;
 cmd.CSR.Bit.DIR = DOWN;
 return SendCmd_KindDaSPCP(&cmd);
}
/*********************************************************************************
 Function:      //
 Description:   //�ر�ָ���м�����ָ��ͨ��
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName CloseNETChannel_KindDa ( unsigned char RepeatersId, 
                                       unsigned char ChannelNumber,
                                       CommPortNumber_Type SourcePort,
                                       KindgDaSPCPCmdCode_AckFunc AckFunc)
{
 unsigned char itemp;
 Close_Repeaters_ASKstruct data;
 KindgDaCmd_Struct cmd;
 
 data.RepeatersId   = RepeatersId % 10; 
 itemp = RepeatersId /10;
 itemp %= 10;
 itemp <<= 4;
 data.RepeatersId |= itemp;
 data.Zero = 0;
 cmd.SourcePort = SourcePort;
 cmd.Cmd = Close_Repeaters;      //������
 cmd.TargetPort = RepeatersId;   //���͵�Ԥ�ڵ�ַ        
 cmd.Param = &data;             //�������
 cmd.length = sizeof(Close_Repeaters_ASKstruct);   //�����������
 cmd.AckFunc = AckFunc;
 cmd.CSR.Bit.DIR = DOWN;
 return SendCmd_KindDaSPCP(&cmd);
}
/*********************************************************************************
 Function:      //
 Description:   //�ر�ָ���м�����ָ��ͨ��
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName CloseRepeatersId_KindDa ( unsigned char RepeatersId, 
                                       CommPortNumber_Type SourcePort,
                                       KindgDaSPCPCmdCode_AckFunc AckFunc)
{
 unsigned char itemp;
 Close_Repeaters_ASKstruct data;
 KindgDaCmd_Struct cmd;
 
 data.RepeatersId   = RepeatersId % 10; 
 itemp = RepeatersId /10;
 itemp %= 10;
 itemp <<= 4;
 data.RepeatersId  |= itemp;
 data.Zero = 0;
 cmd.SourcePort = SourcePort;
 cmd.Cmd = Close_Repeaters;      //������
 cmd.TargetPort = RepeatersId;   //���͵�Ԥ�ڵ�ַ        
 cmd.Param = &data;             //�������
 cmd.length = sizeof(Close_Repeaters_ASKstruct);   //�����������
 cmd.AckFunc = AckFunc;
 cmd.CSR.Bit.DIR = DOWN;
 return SendCmd_KindDaSPCP(&cmd);
}
/*********************************************************************************
 Function:      //
 Description:   //��ȡ�ƶ��м�����ͨ���ڵ�����
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName ReadRepeatersNodeNumber_KindDa ( unsigned char RepeatersId,
                                               CommPortNumber_Type SourcePort,
                                               KindgDaSPCPCmdCode_AckFunc AckFunc)
{
 unsigned char itemp;
 ReadChannelNodeNumber_Repeaters_ASKstruct data;
 KindgDaCmd_Struct cmd;
 
 data.id   = RepeatersId % 10; 
 itemp = RepeatersId /10;
 itemp %= 10;
 itemp <<= 4;
 data.id  |= itemp;
 data.NC  = 0; 
 cmd.SourcePort = SourcePort;
 cmd.Cmd = ReadChannelNodeNumber_Repeaters;      //������
 cmd.TargetPort = RepeatersId;                   //���͵�Ԥ�ڵ�ַ        
 cmd.Param = &data;                              //�������
 cmd.length = sizeof(ReadChannelNodeNumber_Repeaters_ASKstruct);   //�����������
 cmd.AckFunc = AckFunc;
 cmd.CSR.Bit.DIR = DOWN;
 cmd.CSR.Bit.Preamble = 0;
 return SendCmd_KindDaSPCP(&cmd); 
}


         
/******************************************END********************************************************/

