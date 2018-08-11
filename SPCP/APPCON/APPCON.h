/*********************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
#ifndef  __SPCP_APPCON_H
#define  __SPCP_APPCON_H 

#ifndef  IDLE 
#define  IDLE  0
#endif

#ifndef  INUSE
#define  INUSE  1
#endif
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include "..\..\Projects\inc\SystemDefine.h"
#include "..\..\Projects\inc\Common_API.h"
#include "..\..\APP\inc\MCPX.h"
//#include "..\..\Projects\inc\SofTimer.h"
/*********************************************************************************
�궨����
*********************************************************************************/


#define  APPCON_OutPutBuff_Number   4
#define  APPCON_OutPutBuffLength    32

#define  CmdAckListNumber   11

//��ȡ����ģ������Ϣ����
#define  APPCON_GetBat_SendFunc(id,AckFunc)   APPCON_TST_SendFunc(unsigned char *id,0,AckFunc);
//#define  MG2618_PowerFlg              (GPRS_ENpin)            
//#define  MG2618_PowerON( )            set_GPRS_ENpin         
//#define  MG2618_PowerOFF( )           clr_GPRS_ENpin

//#define  ConfigUsart_MG2616(BaudRate,Parity,StopBits,mod)           Launch_USART2(BaudRate, Parity, StopBits, mod)                           
//#define  GetDataForUsart_MG261( )                                   GetDataForBuff_USART2( )
//#define  FreeUsartInputBuff_MG261(buff)                             FreeBuff_USART2(buff)
//#define  SendDataToMG261_ByIT(buff,length,callBack,callBack_arg)    sendBuffByIT_USART2(buff,length,callBack,callBack_arg)
//#define  SendDataToMG261_ByDMA(buff,length,callBack,callBack_arg)   sendBuffByDMA_USART2(buff, length,callBack,callBack_arg)
//#define  SendStringToMG261_ByIT(String)                             sendStringByIT_USART2(String)
//#define  SendStringToMG261_ByDMA(String)                            sendStringByDMA_USART2(String)
//#define  SendDataToMG2618(buff,length,callBack)                     SendDataToMG261_ByDMA(buff,length,callBack,P_NULL)
//#define  SendStringToMG2618(String)                                 SendStringToMG261_ByDMA(String)    
  

/*********************************************************************************
�������Ͷ���
*********************************************************************************/

#pragma pack(1)

typedef union {
 unsigned char    Byte;
 struct{
   unsigned char NB0 : 1;
 }Bit;
}APPCON_MeterState_Struct;

typedef struct {
 unsigned char    Value[4];
 APPCON_MeterState_Struct  State;
 unsigned char    Bat;
}APPCON_MeterValue_Struct;


typedef struct {
 unsigned char    link;
 unsigned char    spa1;
 unsigned char    route;
 unsigned char    spa2;
 unsigned char    id[8];
 unsigned char    spa3;
 unsigned char    value[12];
 unsigned char    state[2];
 unsigned char    spa5;
 unsigned char    bat[2];
}APPCON_RDNODE_AckData_Struct;


typedef struct {
 unsigned char    value[4][3];
 unsigned char    state[2];
 unsigned char    spa;
 unsigned char    bat[2];
}APPCON_CMD_AckData_Struct;

typedef struct {
 unsigned char    state[2];
 unsigned char    spa;
 unsigned char    bat[2];
}APPCON_TST_Valve_AckData_Struct;

#pragma pack()

typedef enum{ 
 _1200bps    = 1200,
 _2400bps    = 2400, 
 _4800bps    = 4800, 
 _9600bps    = 9600,
 _19200bps    = 19200, 
 _38400bps    = 38400, 
 _57600bps    = 57600, 
 _115200bps    = 115200, 
}BaudRate_APPCON;

typedef enum{
  ValveUnState_APPCON         = 0XC0,  //״̬������������ִ�ж�����
  HalfValveState_APPCON       = 0XC1,  //���Ŵ��ڰ뿪״̬
  CloseValveState_APPCON      = 0XC2,  //���Ŵ��ڹر�״̬
  OpenValveState_APPCON       = 0XC3,  //���Ŵ��ڴ�״̬
}ValveState_APPCON;

typedef enum{
  GetValveCmd_APPCON        = 0XC0,  //��ѯ����״̬
  HalfValveCmd_APPCON       = 0XC1,  //���Ű뿪ָ��
  OpenValveCmd_APPCON       = 0XC3,  //���Ŵ�ָ��
  CloseValveCmd_APPCON      = 0XC2,  //���Źر�ָ��
  MT_ValveCmd_APPCON        = 0XC4,  //���ŷ���ָ��
}ValveCmd_APPCON;



typedef enum{ 
 DELNET    = 0,   //ɾ���������ɾ��ģ�飬·��������
 CMD       = 1,   //��λ��������ģ��ģ��ͨѶ����  ʵʱ����
 TST       = 2,   //��������ģ������Ϳط�����
 RDNODE    = 3,   //��������ڵ����߱�ģ��ID ������
 RDROUTER  = 4,   //��������ڵ�����·��ģ��ID ������
 FREQ      = 5,   //��дƵ�ʲ�������
 NETID     = 6,    //��д����ID ���� 
 BATCH     = 7,   //������Ⱥ������  
 STATUS    = 8,   //״̬��ѯ����
 RATE      = 9,  //��д������������������
 MRATE     = 10,  //��д����ģ�鴮�����ʲ�������
 VER       = 11,  //��������Ӳ���汾����
 HELP      = 12,  //��������
 TXD       = 13,  //���������ƻ�ͨѶ����
 SETID     = 14,  //��������������ID ��������
 
 NOP       = 15,
}Command_APPCON;

typedef enum{
  OK_APPCON                = 0x00,
  NoAck_APPCON             = 0x01,   //��������Ӧ��
  Ans_APPCON               = 0x02,   //�ڵ�Ӧ��
  NoAns_APPCON             = 0x03,   //�ڵ���Ӧ��
  BUSY_APPCON              = 0x04,   //���紦��ά��״̬
  FREE_APPCON              = 0x05,   //ά������
  TOTAL_APPCON             = 0x06,   //����ģ�������
  
  Err1_APPCON              = 0x11,   //���� 1
  Err2_APPCON              = 0x12,   //���� 2
  Err3_APPCON              = 0x13,   //���� 3
  Err4_APPCON              = 0x14,   //���� 4
  Err5_APPCON              = 0x15,   //���� 5
  Err6_APPCON              = 0x16,   //���� 6
  
  AnsDatas_APPCON          = 0x20,   //
  
  
  ReadNodeEnd              =0x60,          
}APPCON_AckType;


typedef   struct APPCON_OutPutBuff_struct   APPCON_OutPutBuff_Struct;

typedef void(*APPCON_AckFunc)(APPCON_OutPutBuff_Struct *cmd, 
                              APPCON_AckType AckType, 
                              DataBuffSpec_Struct* AckData);

struct APPCON_OutPutBuff_struct{
  union{
    unsigned char Byte;
    struct{
      unsigned char idle :1; //����������
      unsigned char ack1 :1; //��һ��Ӧ���־
      unsigned char ack2 :1; //�ڶ���Ӧ���־
    }Bit;
  }CSR; //����״̬�Ĵ���
 Command_APPCON       Cmd;    //���� 
 CommPortNumber_Type  SourcePort;
 APPCON_AckFunc       AckFunc;
 void*                AckFuncParam; 
 unsigned char        BuffLength;
 unsigned char        Buff[APPCON_OutPutBuffLength];
};
/*********************************************************************************
�ⲿ����������
*********************************************************************************/
/*********************************************************************************
����������
*********************************************************************************/
void APPCON_SPCP_init(void);

void freeSendBuff_APPCON(void* frame);

void APPCON_InPut(void *pbuff, unsigned length,CommPortNumber_Type port);

void Decode_APPCON(DataBuffSpec_Struct *Data);

//ɾ���������ɾ��ģ�飬·��������
SystemErrName APPCON_DELNET_SendFunc(CommPortNumber_Type Source, 
                                     void* AckFuncParam,
                                      APPCON_AckFunc AckFunc);
//��λ��������ģ��ģ��ͨѶ����
SystemErrName  APPCON_CMD_SendFunc (CommPortNumber_Type Source,
                                    void* id,
                                    void* AckFuncParam,
                                    APPCON_AckFunc  AckFunc);
//��������ģ������Ϳط�����
SystemErrName  APPCON_TST_SendFunc(CommPortNumber_Type Source,
                                   unsigned char *id,
                                   unsigned char AnsNumber,
                                   void* AckFuncParam,
                                   APPCON_AckFunc  AckFunc);
//����ģ��ط����� 
SystemErrName  APPCON_ValveCmd_SendFunc(CommPortNumber_Type Source,
                                        void *id,
                                        ValveCmd_APPCON  cmd,
                                        void* AckFuncParam,
                                        APPCON_AckFunc  AckFunc);

//��������ڵ����߱�ģ��ID ������
SystemErrName  APPCON_RDNODE_SendFunc(CommPortNumber_Type Source,
                                      void* id,
                                      unsigned char Number,
                                      void* AckFuncParam,
                                      APPCON_AckFunc AckFunc);
//��������ڵ�����·��ģ��ID ������
SystemErrName  APPCON_RDROUTER_SendFunc(CommPortNumber_Type Source,
                                        //unsigned char *id,
                                        unsigned char Number,
                                        void* AckFuncParam,
                                        APPCON_AckFunc AckFunc);
//��дƵ�ʲ�������
SystemErrName  APPCON_FREQ_SendFunc (CommPortNumber_Type Source,
                                     unsigned char* freq,
                                     void* AckFuncParam,
                                     APPCON_AckFunc AckFunc);
//��д����ID ����
SystemErrName  APPCON_NETID_SendFunc (CommPortNumber_Type Source,
                                      unsigned char* id,
                                      void* AckFuncParam,
                                      APPCON_AckFunc AckFunc);
//�������������� 
SystemErrName  APPCON_BATCH_SendFunc (CommPortNumber_Type Source,
                                      void* AckFuncParam,
                                      APPCON_AckFunc AckFunc);
//״̬��ѯ����
SystemErrName  APPCON_STATUS_SendFunc (CommPortNumber_Type Source,
                                       void* AckFuncParam,
                                       APPCON_AckFunc AckFunc);
//��д������������������
SystemErrName  APPCON_RATE_SendFunc (CommPortNumber_Type Source,
                                     BaudRate_APPCON rate,
                                     void* AckFuncParam,
                                     APPCON_AckFunc AckFunc); 
//��д����ģ�鴮�����ʲ�������
SystemErrName  APPCON_MRATE_SendFunc (CommPortNumber_Type Source,
                                      unsigned char dir, 
                                      BaudRate_APPCON rate,
                                      void* AckFuncParam,
                                      APPCON_AckFunc AckFunc); 
//��������Ӳ���汾����
SystemErrName  APPCON_VER_SendFunc (CommPortNumber_Type Source,
                                    void* AckFuncParam,
                                    APPCON_AckFunc AckFunc );
//��������
SystemErrName  APPCON_HELP_SendFunc (CommPortNumber_Type Source,
                                     void* AckFuncParam,
                                     APPCON_AckFunc AckFunc);
 //���������ƻ�ͨѶ����
SystemErrName APPCON_TXD_SendFunc (CommPortNumber_Type Source,
                                   void *buff,
                                   unsigned char length,
                                   void* AckFuncParam,
                                   APPCON_AckFunc AckFunc); 
//��������������ID ��������
SystemErrName  APPCON_SETID_SendFunc (unsigned long command ,//����  
                                      unsigned long f,
                                      unsigned short id,
                                      void* AckFuncParam,
                                      APPCON_AckFunc AckFunc) ;  
//ASCII���16���Ʊ任Ϊʵ��
unsigned long   APPCON_AsciiHexToDigit (unsigned char* hex, 
                                       unsigned char length, 
                                       unsigned char width ) ;
/********************************************************************************/
#endif
