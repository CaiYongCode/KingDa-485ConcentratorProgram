/*********************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include "..\inc\SIM800E.h"
#include "..\..\Projects\inc\Common_API.h"
#include "..\..\Projects\inc\SofTimer.h"
#include "..\..\SPCP\KingDa\KingDaServer.h"
/*********************************************************************************
����������
*********************************************************************************/
const char*  const CmdNameList_GPRS[] = { 
/* 1 */   "OK",                 //ģ��Ĭ������Ӧ������
/* 2 */   "ATE",               //�򿪻��� ���� 
/* 3 */   "Revision",          //ģ��汾
/* 4 */   "CBC",               //�鿴ģ��Ĺ����ѹ
/* 5 */   "COPS",              //��ѯģ��ע���������Ӫ��   
/* 6 */   "CMGS",              //���ö���Ϣ���ݵĳ��� 
/* 7 */   "CSQ",               //�鿴�ź�ǿ��
/* 8 */   "CPIN",              //SIM״̬
/* 9 */   "CREG",              //GSM����ע��״̬
/* 10*/   "CGREG",             //GPRSע��״̬
/* 11*/   "AT+GMRRevision",    //ģ��汾
/* 12*/   "ERROR",             //����
/* 13*/   "AT+CIFSR",          //��ȡ����IP��ַ
/* 14*/   "CONNECT",           //����״̬��ѯ�����ʾ
/* 15*/   "SEND",             //�յ�Զ�˷�����ACKȷ�Ϻ� ����SEND OK,��ʾTCP���ݷ��ͳɹ�
/* 16*/   "RECV",             //���ܵ�����
/* 17*/   "CIPSEND",
/*  */    " "  
  };

typedef enum { 
  OK_gprs            = 1,  //
  ATE_gprs           = 2,  //�򿪻��� ���� 
  Revision_gprs      = 3,  //ģ��汾
  CBC_gprs           = 4,  //ģ��Ĺ����ѹ
  COPS_gprs          = 5,  //��ѯģ��ע���������Ӫ��   
  CMGS_gprs          = 6,  //���ö���Ϣ���ݵĳ��� 
  CSQ_gprs           = 7,  //�鿴�ź�ǿ�� 
  CPIN_gprs          = 8,  //SIM״̬
  CREG_gprs          = 9,  //GSM����ע��״̬
  CGREG_gprs         = 10, //GPRSע��״̬
  AT_GMR_gprs        = 11, //ģ��汾
  ERROR_gprs         = 12, //����
  CIFSR_gprs         = 13, //��ȡ����IP��ַ
  CONNECT_gprs       = 14, //����״̬��ѯ�����ʾ
  SEND_gprs          = 15, //�յ�Զ�˷�����ACKȷ�Ϻ� ����SEND OK,��ʾTCP���ݷ��ͳɹ�
  RECV_gprs          = 16, //���ܵ�����
  CIPSEND_gprs    = 17,
}ArgNameVaule_gprs;
/*********************************************************************************
��������������
*********************************************************************************/
SIM800E_STATE_TYPE  Sim800eState_PV = Power_OFF;   //GPRS ģ�鵱ǰ״̬
PortStatus          GPRS_Flg = Idle;               //GPRS ͨ�Ŷ˿�״̬
ErrorType_GPRS      GPRSError_state = No_err;      //GPRS ģ�鱨����Ϣ
/*********************************************************************************
�ⲿ����������
*********************************************************************************/

/*********************************************************************************
˽�б���������
*********************************************************************************/
static unsigned char GPRS_Step  = 0;
//GPRSģ�� ���ļ�ʱ��
SofTimer_Struct*  GPRS_Timer = P_NULL;
static SofTimer_Struct*  IP_MaintainTimer = P_NULL;
//GPRSģ�� ��ǰ���ӵ� ����������
unsigned char GPRS_IP_PV[20];
unsigned char GPRS_IP_Port[6];

unsigned char ServerIP_PV[16] = "000.000.000.000"; 
unsigned char ServerPort_PV[6] = "00000"; 

void *GPRS_SendPbuff = P_NULL;
unsigned short GPRS_SendBuffSize = 0;
SerialPort_SendEndCallBack_Type GPRS_SendEndCallBack = P_NULL;
void* GPRS_SendEndCallBack_Arg = P_NULL;
//
SofTimer_Struct*  GPRS_SendCmdTimer = P_NULL;

//GPRSģ�� Ԥ�ڵĹ���״̬
static SIM800E_STATE_TYPE  Sim800eState_SV = Power_OFF;
static GPRSLaunch_CallBack     LaunchCallBack = P_NULL;
//GPRS���ͻ�����
static struct {
  unsigned char flg ;
  unsigned char buff[64];
}Sim800eSendBuff  = {IDLE,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};
/*********************************************************************************
���Ա���������
*********************************************************************************/

/*********************************************************************************
�ڲ�����������
*********************************************************************************/
void USART_GPRS_Config( void );
static void LaunchOrder_GPRS(void *p_tmr, void *p_arg);
void* getCmdName (char *opcode,  unsigned short length, unsigned char *code);
SystemErrName CmdSparse_GPRS ( void* pbuff, unsigned short length, DataBuffSpec_Struct* data);
void GPRS_ATE_func(unsigned char* pdata,unsigned short length);
void GPRS_Revision_func(unsigned char* pdata,unsigned short length);
void GPRS_CBC_func(unsigned char* pdata,unsigned short length);
void GPRS_COPS_func(unsigned char* pdata,unsigned short length);
void GPRS_CMGS_func(unsigned char* pdata,unsigned short length);
void GPRS_CSQ_func(unsigned char* pdata,unsigned short length);
void GPRS_CPIN_func(unsigned char* pdata,unsigned short length); 
void GPRS_CREG_func(unsigned char* pdata,unsigned short length);
void GPRS_CGREG_func(unsigned char* pdata,unsigned short length);
void GPRS_CIFSR_func(unsigned char* pdata,unsigned short length);
void GPRS_CONNECT_func(unsigned char* pdata,unsigned short length); 
void GPRS_SEND_func(unsigned char* pdata,unsigned short length); 
void GPRS_RECV_func(unsigned char* pdata,unsigned short length, DataBuffSpec_Struct* data);
void GPRS_CIPSEND_func(unsigned char* pdata,unsigned short length); 
unsigned char  StringComparison ( char *String1,  char *String2);
void GPRS_SendEndCallBack11(CommPortNumber_Type port, void *pbuff);
SystemErrName GPRS_CONNECT_CmdSend ( void );
void GPRS_SendCmd_func (unsigned short length) ;
/*********************************************************************************
���ܴ��붨����
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
SystemErrName Launch_GPRS (SIM800E_STATE_TYPE  state,    //Ԥ�ڹ���״̬
                           unsigned char* server_ip,   //Ԥ�����ӷ�������IP��ַ
                           unsigned short server_port,   //Ԥ�����ӵ� 
                           GPRSLaunch_CallBack  CallBack) 
{
  unsigned char itemp =0;
  unsigned char itemp2 =0;
  //��¼��������IP��ַ
  if((state == CONNECT_OK)&&(P_NULL == server_ip))
  { return IllegalParam;}
  
  if(0 != GPRS_Step)     
  {return Unsuccessful;}
  
  LaunchCallBack = CallBack;
  while (0 != server_ip[itemp])
  {
    ServerIP_PV[itemp] = server_ip[itemp];
    if(++itemp > 15)
    { return IllegalParam;} 
  }
  while (itemp < 16)
  { ServerIP_PV[itemp++] = 0;}
  //ת���������Ķ˿ںŸ�ʽ
  itemp = 5;
  do{
     ServerPort_PV[--itemp] = server_port % 10 + '0';
     server_port /= 10;
  }while( itemp >0);
  itemp2 = 0;
  while('0'==ServerPort_PV[itemp2]){itemp2++;}
  itemp = 0;
  while(itemp <= 5)
  {
   if(itemp2 < 5)
   {ServerPort_PV[itemp++] = ServerPort_PV[itemp2++];}
   else
   {ServerPort_PV[itemp++] = 0;}
  } 
  
  Sim800eState_SV = state;
  PowerOff_GPRS ( );
  Sim800eState_PV = Power_OFF;
  GPRS_Timer =  SofTimerCreate (10,                        //����
                                TimerTB_100MS,               //ʱ��
                                SofTimer_PERIODIC,          //ģʽ
                                LaunchOrder_GPRS,           //�ص�
                                P_NULL);                    //�ص�����
  GPRS_Step  = 1;
  SofTimerStart( GPRS_Timer );   
  return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //�ر�GPSR
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName Clse_GPRS(void)
{
 SystemErrName err; 

 if(CONNECT_OK == Sim800eState_PV)
  {GPRS_Step = 51;}
 else
  {GPRS_Step = 100;}
  Sim800eState_SV = Power_OFF;
 SofTimer_ChangePeriod (GPRS_Timer, 100, 1);
 err = SofTimer_Compelling(GPRS_Timer);
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
void  ClseConNect_GPRS(void)
{
  if(CONNECT_OK == Sim800eState_PV)
  {
    GPRS_Step = 51;
    Sim800eState_SV = GPRS_OK;
    SofTimer_ChangePeriod (GPRS_Timer, 100, 1);
    SofTimer_Compelling(GPRS_Timer); 
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
void  OpenConNect_GPRS(void)
{
  if(GPRS_OK == Sim800eState_PV)
  {
    GPRS_Step = 6;
    Sim800eState_SV = CONNECT_OK;
    SofTimer_ChangePeriod (GPRS_Timer, 100, 1);
    SofTimer_Compelling(GPRS_Timer); 
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
static void LaunchAchieve_GPRS(void)
{
  Sim800eState_PV = Sim800eState_SV;
  
  if(P_NULL != LaunchCallBack )
  {LaunchCallBack( No_err);}
  
  if(GPRS_Step < 51)
  {GPRS_Step = 51;}
  else
  {GPRS_Step = 100;}
  Sim800eState_SV = Power_OFF;
  SofTimer_ChangePeriod (GPRS_Timer, 1000, 1);
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
static void ErrorHandler_GPRS(ErrorType_GPRS type)
{
  GPRSError_state |= type;
  Clse_GPRS( );
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
static void IP_Maintain_GPRS(void *p_tmr, void *p_arg)
{
  SendHandCmdToKingDaServer( P_NULL );
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
static void LaunchOrder_GPRS(void *p_tmr, void *p_arg)
{
  SystemErrName err = NO_ERR ;
  static unsigned  char  cnt = 0;
  switch(GPRS_Step)
  {
   case 0:   //GPRS �ϵ� 
     {
       SofTimerDel( GPRS_Timer );
       GPRS_Timer = P_NULL;
       break;
     }  
   case 1:   //GPRS �ϵ�
     {
       PowerOn_GPRS ();
       err = SofTimer_ChangePeriod (GPRS_Timer, 10, 1); 
       GPRS_Step ++;
       break;
     } 
   case 2:  //  ����������
     {
       set_GPRS_RESTpin; 
       err = SofTimer_ChangePeriod (GPRS_Timer, 20, 1);  
       GPRS_Step ++;
       break;
     }
   case 3:  //�������ø�  ��ɿ���
     {
      clr_GPRS_RESTpin;  
      GPRS_USART_Config(115200,Parity_No,StopBits_1,ByDMA);  
      Sim800eState_PV = Power_ON;
      if(Power_ON ==  Sim800eState_SV)
      {LaunchAchieve_GPRS( );}
      else
      { GPRS_Step ++;}
      err = SofTimer_ChangePeriod (GPRS_Timer, 5, 1); 
       break;
     }
  case 4:  //����  AT  ͬ�� ������
    {
      GPRS_sendStringByIT("AT\r\n");
      break;
    }
  case 5:
    {
      err = GPRS_sendStringByIT("AT+CGREG?\r\n");
      break;
    }   
  case 6:
    { 
      err = GPRS_sendStringByIT("AT+CIPMUX=0\r\n");  //��������Ϊ������
      if(NO_ERR == err)
      { GPRS_Step ++;}
      break;
    }
  case 7:
    {
      GPRS_sendStringByIT("AT+CIPMODE=0\r\n");  //��������Ϊ��͸��ģʽ
      if(NO_ERR == err)
      { GPRS_Step = 9;}
      break;
    }     
  case 9:
    {
      GPRS_sendStringByIT("AT+CSTT=\"CMNET\"\r\n");//����GPRS�����
      if(NO_ERR == err)
      {GPRS_Step ++;}
      break;
    }   
  case 10:
    {
      GPRS_sendStringByIT("AT+CIICR\r\n"); //�����ƶ�����
      if(NO_ERR == err)
      {GPRS_Step = 13; cnt = 0;}
      break;
    }    
  case 13:
    {
      err =  GPRS_CONNECT_CmdSend ( );   //��ʼ���ӷ�����
      if(NO_ERR == err)
      {SofTimer_ChangePeriod (GPRS_Timer, 100, 1);}
      else
      {SofTimer_ChangePeriod (GPRS_Timer, 5, 1);}
      if(cnt ++ > 200)
      {ErrorHandler_GPRS( CONNECT_err );cnt = 0;}
      break;
    }
  case 14:  
   {
     GPRS_sendStringByIT("AT+CIPHEAD=1\r\n"); //��ʾ���յ������ݰ���IPͷ
     SofTimer_ChangePeriod (GPRS_Timer, 5, 1);
     if(NO_ERR == err)
     {GPRS_Step ++;}
     break;
   }  
  case 15:  
   {
     GPRS_sendStringByIT("AT+CIPSRIP=1\r\n");  //��ʾ���յ������ݰ���IPͷ 
     if(NO_ERR == err)
     {
      if(P_NULL !=IP_MaintainTimer )
      {SofTimerDel(IP_MaintainTimer);}
      IP_MaintainTimer  =  SofTimerCreate ( 600,                    //����
                                            TimerTB_100MS,          //ʱ��
                                            SofTimer_PERIODIC,      //ģʽ
                                            IP_Maintain_GPRS,       //�ص�
                                            P_NULL);                //�ص���
       SofTimerStart( IP_MaintainTimer );      
       LaunchAchieve_GPRS( ); 
     }
     break;
   }
  case 51:
   {
     SofTimerDel(IP_MaintainTimer);
     SofTimer_ChangePeriod (GPRS_Timer, 5, 1);
     GPRS_sendStringByIT("AT+CIPCLOSE\r\n");  //�ر�TCP����
     if(NO_ERR == err)
     {GPRS_Step ++;}
     break;
   }
  case 52:
   {
     GPRS_sendStringByIT("AT+CIPSHUT\r\n");  //�ر��ƶ�����
     if(NO_ERR == err)
     {
      if(Power_OFF ==  Sim800eState_SV)
      {GPRS_Step = 100; } 
      else 
      {LaunchAchieve_GPRS( );}
      break;
     }
   } 
  case 100:
   {
    PowerOff_GPRS ( );
    GPRS_Step = 0; 
    break;
   }
  default :
    {GPRS_Step = 0; break;}
   
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
 unsigned short GetDataForBuff_GPRS (unsigned char* buff, unsigned short buffSize)
{
 SystemErrName err;
 unsigned short itemp;
 USART2_BuffStruct*  pbuff;
 DataBuffSpec_Struct data;
 
  
 pbuff = GetDataForBuff_USART2 ( );
 if(P_NULL == pbuff )
 {return 0;}
 if((CONNECT_OK == Sim800eState_PV)||(GPRS_OK == Sim800eState_PV))
 {SofTimerRset (GPRS_Timer);}  
 /*sendBuffByDMA_Rs232 ( pbuff->data, 
                       pbuff->length,
                       GPRS_SendEndCallBack11,
                       pbuff );*/

 err = CmdSparse_GPRS ((char*)pbuff->data, pbuff->length, &data );
 if(NO_ERR == err)
 {
   if(data.length > 0)
   {
     itemp = 0;
     while(itemp < data.length)
     {
       if(itemp > buffSize)
       {break;}
       buff[itemp] = ((unsigned char*)data.pbuff)[itemp];
       itemp ++;
     }
   FreeBuff_USART2(pbuff); 
    return itemp;
   }
 }
 FreeBuff_USART2(pbuff);
 return 0;
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
SystemErrName CmdSparse_GPRS ( void* pbuff, unsigned short length, DataBuffSpec_Struct* data)
{
  SystemErrName err = NO_ERR;
  unsigned char code;
  void *p_data;
  
  data->length = 0;
  p_data = getCmdName (pbuff,  length, &code);
  length -= (unsigned long )p_data - (unsigned long)pbuff; 
  switch (code)
  { 
   case OK_gprs:
     {
       if(4== GPRS_Step){GPRS_Step = 5;break;}
       break;
     }
  case ATE_gprs:
    {GPRS_ATE_func (p_data, length); break;} 
  case Revision_gprs:
  case AT_GMR_gprs:  
    {GPRS_Revision_func (p_data, length);break; }
  case CBC_gprs:
    { GPRS_CBC_func (p_data, length);break;}
  case COPS_gprs:
    {GPRS_COPS_func (p_data, length);break;} 
  case CMGS_gprs:
    {GPRS_CMGS_func (p_data, length); break;}
  case CSQ_gprs:
    {GPRS_CSQ_func (p_data, length);break;}  
  case CPIN_gprs:  //SIM״̬
    {GPRS_CPIN_func (p_data, length);break;}    
  case CREG_gprs://GSM����ע��״̬
    {GPRS_CREG_func (p_data, length);break;}  
  case CGREG_gprs: //GPRSע��״̬ 
    {GPRS_CGREG_func (p_data, length);break;} 
  case ERROR_gprs: 
    {/*GPRS_sendStringByIT("ATE1\r\n");*/ break;} 
  case CIFSR_gprs: 
    {GPRS_CIFSR_func (p_data, length);break;} 
  case CONNECT_gprs: 
    {GPRS_CONNECT_func (p_data, length);break;}
  case SEND_gprs: 
    {GPRS_SEND_func (p_data, length);break;} 
  case RECV_gprs :
    {GPRS_RECV_func (p_data, length, data);break;} 
  case CIPSEND_gprs: 
    {GPRS_CIPSEND_func (p_data, length);break;} 
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
void GPRS_ATE_func (unsigned char* pdata,unsigned short length)
{
 unsigned char itemp = 0;
  
 while (itemp++ < length)
 {
  if(IsDigit(*pdata))
  {break;}
  pdata ++;
 }
 if('1' == *pdata )
 {
  if(5== GPRS_Step)
  {GPRS_Step = 6;} 
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
void GPRS_Revision_func(unsigned char* pdata,unsigned short length)
{
 unsigned char itemp = 0;
 while (itemp++ < length)
 {
  if(IsDigit(*pdata)||Isletter(*pdata)||IsLETTER(*pdata))
  {break;}
  pdata ++;
 }
 
 if(6 == GPRS_Step)
 {GPRS_Step = 7;}
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
void GPRS_CBC_func(unsigned char* pdata,unsigned short length)
{
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
void GPRS_COPS_func(unsigned char* pdata,unsigned short length)
{
 unsigned char itemp = 0;
// unsigned char itemp2 = 0;
 while (itemp++ < length)
 {
  if(IsDigit(*pdata)||Isletter(*pdata)||IsLETTER(*pdata))
  {break;}
  pdata ++;
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
void GPRS_CMGS_func(unsigned char* pdata,unsigned short length)
{}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void GPRS_CSQ_func(unsigned char* pdata,unsigned short length)
{
 unsigned short i = 0;  
 while (1)
 {
  if(IsLETTER(*pdata) || Isletter(*pdata) || IsDigit(*pdata) )
  {break;}
  if(i++ > length){return ; }
  pdata ++;
 }
 if('C' == *pdata)
 {
   i =  StringComparison ((char*)pdata, "CSQ:"); 
   if(i)
   {pdata += i;}
   else
   {return;}
 }
 while (1)
 {
  if(IsDigit(*pdata))
  {break;}
  if(i++ > length){return ; }
  pdata ++;
 }

 if('1' == *pdata)
 { } 
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
void GPRS_CPIN_func(unsigned char* pdata,unsigned short length)
{
 const unsigned char  org[] = "READY"; 
 unsigned short  itemp  = 0; 
 unsigned char c;
 unsigned char tc;
 const unsigned char* sp;
 unsigned char* tp;
 
 while (itemp++ < length)
 {
  if(Isletter(*pdata)||IsLETTER(*pdata))
  {break;}
  pdata ++;
 }
 
 sp = org;
 tp = pdata;
 do 
 {
  c = *sp++ ; tc = *tp++;
 } while (c == tc);

 if((0 == c )&&('\r' == tc ))
 {
   if(7 == GPRS_Step)
   {GPRS_Step = 8;}
 }
 else
 {
   //SIM ������
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
void GPRS_CREG_func (unsigned char* pdata,unsigned short length)
{}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void GPRS_CGREG_func (unsigned char* pdata,unsigned short length)
{
 unsigned short i = 0;  
 while (1)
 {
  if(IsDigit(*pdata))
  {break;}
  if(i++ > length){return ; }
  pdata ++;
 }
 if('1' == pdata[2])
 {
   Sim800eState_PV = GPRS_OK; 
   if(Sim800eState_SV == GPRS_OK)
   { LaunchAchieve_GPRS( ); }
   else if(CONNECT_OK == Sim800eState_SV )
   {
    if(5 == GPRS_Step)
    {GPRS_Step = 6;}
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
void GPRS_CIFSR_func(unsigned char* pdata,unsigned short length)
{
  
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
void GPRS_CONNECT_func(unsigned char* pdata,unsigned short length)
{
 unsigned char itemp = 0; 
 while (itemp++ < length)
 {
  if(Isletter(*pdata)||IsLETTER(*pdata))
  {break;}
  pdata ++;
 }
 if(itemp <= length -2)
 {
  if(('O'== pdata[0])&&('K'== pdata[1]))
  {
    if( 13 == GPRS_Step)
    {
      GPRS_Step++; 
      SofTimer_Compelling(GPRS_Timer);
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
void GPRS_SEND_func(unsigned char* pdata,unsigned short length)
{
  
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
void GPRS_RECV_func(unsigned char* pdata,unsigned short length, DataBuffSpec_Struct* data)
{
 unsigned short i; 
 unsigned short itemp = 0;
 
 data->length  = 0;
 
 while (itemp < length)
 {
  if(Isletter(pdata[itemp])||IsLETTER(pdata[itemp])){break;}
  itemp++;
 }
 if(itemp < length)
 {
  if('F' == pdata[itemp])
  {
   i = StringComparison ((char*)&(pdata[itemp]), "FROM:"); 
   if(i)
   {
    itemp += i;
    if(itemp >= length){return;}
    i = 0;
    while(IsDigit(pdata[itemp]) || ('.'== pdata[itemp]))
    {
      if(i < 15)
      {GPRS_IP_PV[i++] = pdata[itemp];}
      else
      {break;}
      itemp ++ ;
      if(itemp > length){return;}
    }
   while (itemp < length)
   {
    if(IsDigit(pdata[itemp])){break;}
    itemp++;
    }
   i = 0; 
   while(IsDigit(pdata[itemp]))
   {
     if(i < 6)
     {GPRS_IP_Port[i++] = pdata[itemp];}
     else
     {break;}
     itemp ++ ;
     if(itemp >= length){return;}
    }
   while (itemp < length)
   {
     if(Isletter(pdata[itemp])||IsLETTER(pdata[itemp])){break;}
     itemp++;
    }
   }
  }
 }
 if(itemp >= length){return;}
 if('I' == pdata[itemp])
 {
   i = StringComparison ((char*)&(pdata[itemp]), "IPD");
   if(i)
   {
     itemp += i; 
     if(itemp >= length){return;}
     while (itemp < length)
     {
      if(IsDigit(pdata[itemp])){break;}
      itemp++;
     }
     if(itemp >= length){return;}
     while (IsDigit(pdata[itemp]))
     {
      data->length  *= 10; 
      data->length  += pdata[itemp++] - '0';
      if(itemp >= length){return;}
     }
     while (itemp < length)
     {if(':' == pdata[itemp++]){break;}}

     if(  itemp + data->length <  length)
     {data->pbuff = &(pdata[itemp]);}
     else
     {data->length = 0;}
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
void GPRS_CIPSEND_func(unsigned char* pdata,unsigned short length)
{
 unsigned short i = 0; 
 unsigned short itemp = 0;
 
 while (itemp < length)
 {
   if(IsDigit(pdata[itemp])){break;}
  itemp++;
 }
 while (IsDigit(pdata[itemp]))
 {
   i *= 10;
   i += pdata[itemp] - '0';
   itemp++;
 }
 
 if(i == GPRS_SendBuffSize)
 {
   GPRS_sendBuffByIT(GPRS_SendPbuff,
                         GPRS_SendBuffSize,
                         GPRS_SendEndCallBack,
                         GPRS_SendEndCallBack_Arg);
 }
 else
 {GPRS_SendCmd_func (GPRS_SendBuffSize);}
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
void* getCmdName ( char *opcode,  unsigned short length , unsigned char *code)
{
 const   char *sp; 
 char *tp; 
 unsigned short i = 0; 
 unsigned char  c = 0;
 unsigned char  tc = 0;
 
 //Ѱ�Ҳ�����ʼ�ֽ�
 while (1)
 {
  if(IsLETTER(*opcode) || Isletter(*opcode))
  {break;}
  if(i++ > length)
  { *code = 0 ;return opcode; }
  opcode ++;
 }
 //���������
 if(('A' == opcode[0])&&('T' == opcode[1]))
 {
   opcode += 2;
   if('+' == *opcode)
   {opcode ++;}
 }
 //�ȶԲ��Ҳ�����
 i = 0;
 do 
 {
  sp = CmdNameList_GPRS[i];
  if(' ' == *sp){*code = 0;return opcode; } 
  tp = opcode;
  do 
  {
   c = *sp++ ; tc = *tp++;
  } while (c == tc);
  ++i;
 } while (c != 0);
 *code = i;
 return --tp;
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
unsigned char  StringComparison ( char *String1,  char *String2)
{
 unsigned char i= 0;
 do 
 {
  if(String1[i] != String2[i])
  {break;}
  i ++;
 } while((0 != String1[i]) && ( 0 != String2[i] ));
 
 if((0 == String1[i]) || ( 0 == String2[i] ))
 {return i;}
 else
 {return 0;}
}
/*********************************************************************************
 Function:      //
 Description:   //���ݷ������� ��ʱ������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static void  GPRS_SendCmd_TimeOut_func (void *p_tmr, void *p_arg)
{
  GPRS_Flg = Idle;
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
void GPRS_SendCmd_func (unsigned short length) 
{
 const unsigned char  cmd[] = "AT+CIPSEND=";
 static unsigned char  cmdbuff [20];
 unsigned char  L[6];
 unsigned char itemp = 0;
 unsigned char itemp2 = 5;
  
 if(0 == length)
 {return;}
 
 while(0 != cmd[itemp])
 {cmdbuff[itemp] = cmd[itemp]; itemp ++;}
 
 do
 {
  L[itemp2] = length % 10;
  length /= 10;
 }while(itemp2-- > 0);
 itemp2 = 0;
 while(itemp2 < 6)
 {
  if(L[itemp2] != 0){break ;}
  itemp2 ++;
 }
 while(itemp2 < 6)
 {cmdbuff[itemp++] =  L[itemp2++] + '0';}
 cmdbuff[itemp++] = '\r';
 cmdbuff[itemp++] = '\n';
 GPRS_sendBuffByIT(cmdbuff,itemp, P_NULL,P_NULL); 

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
SystemErrName GPRS_SendBbuff (void *buff, unsigned short length,
                               SerialPort_SendEndCallBack_Type callBack,
                               void *callBack_arg) 
{
  if((P_NULL == buff) || (0 == length))
  {return IllegalParam;}
   if(Idle == GPRS_Flg)
  {GPRS_Flg = Busy;}
  else
  {return PortBusy;}
  
  SofTimerRset (IP_MaintainTimer);
  
  GPRS_SendCmdTimer =  SofTimerCreate (50,                          //����
                                       TimerTB_10MS,               //ʱ��
                                       SofTimer_ONE_SHOT,          //ģʽ
                                       GPRS_SendCmd_TimeOut_func,  //�ص�
                                       P_NULL);                    //�ص�����
 if(P_NULL == GPRS_SendCmdTimer)
 {return Nothing;}
 SofTimerStart( GPRS_SendCmdTimer );  
 GPRS_SendPbuff = buff;
 GPRS_SendBuffSize = length;
 GPRS_SendEndCallBack = callBack;
 GPRS_SendEndCallBack_Arg = callBack_arg;
 GPRS_SendCmd_func (GPRS_SendBuffSize); 
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
void GPRS_CONNECT_CmdSend_CallBack (CommPortNumber_Type port, void *pbuff)
{
  if(pbuff == Sim800eSendBuff.buff)
  {Sim800eSendBuff.flg = IDLE;}
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
SystemErrName GPRS_CONNECT_CmdSend ( void )
{
 unsigned char itemp;
 unsigned char itemp2 ;
 const unsigned char cmd1[]= "AT+CIPSTART=\"TCP\",";
 
 if(IDLE == Sim800eSendBuff.flg)
 {Sim800eSendBuff.flg = INUSE;}
 else
 {return Nothing;}
 itemp = 0;
 while('\0' !=cmd1[itemp])
 {
   Sim800eSendBuff.buff[itemp] = cmd1[itemp];
   itemp ++;
 }
  Sim800eSendBuff.buff [itemp++] = '\"';
 itemp2 = 0;
 while(0 != ServerIP_PV[itemp2] )
 {
   Sim800eSendBuff.buff[itemp++] = ServerIP_PV [itemp2++];
   if(itemp2 > 15 )
   {break;}
 }
 Sim800eSendBuff.buff [itemp++] = '\"';
 Sim800eSendBuff.buff [itemp++] = ',';
 itemp2 = 0;
 while(0 != ServerPort_PV[itemp2] )
 {
   Sim800eSendBuff.buff [itemp++] = ServerPort_PV [itemp2++];
   if(itemp2 > 5 )
   {break;}
 }
 Sim800eSendBuff.buff [itemp++] = '\r';
 Sim800eSendBuff.buff [itemp++] = '\n';

 return GPRS_sendBuffByDMA(Sim800eSendBuff.buff, 
                            itemp,
                            GPRS_CONNECT_CmdSend_CallBack,
                            Sim800eSendBuff.buff);

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
void GPRS_SendEndCallBack11(CommPortNumber_Type port, void *pbuff)
{
  FreeBuff_USART2(pbuff);
}























