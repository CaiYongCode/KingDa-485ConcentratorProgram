/*********************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
#ifndef  __MCPX_H
#define  __MCPX_H 
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include "..\..\BSP\inc\bsp.h"
#include "..\..\Projects\inc\SofTimer.h"
/*********************************************************************************
�궨����
*********************************************************************************/
#define  SPCP_OutputBuff_Number     16

#define  DomainNameLength           32

#define  AutomatchPort              0
#define  CoercePort_1CH             101
#define  CoercePort_2CH             102
#define  CoercePort_3CH             103
#define  CoercePort_4CH             104
#define  MCPX_InputBuffSize         256     
/*********************************************************************************
�������Ͷ���
*********************************************************************************/

typedef    struct McpxOutputStruct  MCPX_OutputStruct;

typedef void (*MCPX_RetransEndCallBack_Type)(void *pbuff);

//���ݰ����ݵ��߼�����
enum Package_Dir_Type{ASK =0, ACK = ~ASK} ;


//ͨ�Ž��ܻ������ṹ����
typedef struct {
 CommPortNumber_Type Port; 
 unsigned char length;
 unsigned char buff[MCPX_InputBuffSize];
}MCPX_InputStruct;
//ͨ�ŷ��ͻ������ṹ����
struct McpxOutputStruct{
  union {
    unsigned char Byte;
    struct {
      unsigned char idle :1; //����������
    }Bit;
  }CSR; //����״̬�Ĵ���
  union {
    unsigned char Byte;
    struct {
      unsigned char Retrans_SV :4; //�趨�ط�����
      unsigned char Retrans_PV :4; //��ʣ�ط�����
    }Bit;
  }CCR;  //���ƼĴ���
 CommPortNumber_Type OutPort;
 unsigned short length;
 void* buff;
 SofTimer_Struct*    RetransTimer;                    //�ط���ʱ��
 SerialPort_SendEndCallBack_Type  SendEndCallBack;    //������ɻص�����
 MCPX_RetransEndCallBack_Type     RetransEndCallBack; //�ط���������
};

//�Զ�����ʱ����
typedef  struct{
  unsigned char  day;
  unsigned char  hour ;
  unsigned char  minute ; 
}AutoCollectCycleStruct;
//������
typedef  struct {
  unsigned char ip[4] ;
  unsigned char DomainName[DomainNameLength];
  unsigned short port;
}DataServerStruct;
//ͨ���м�������
typedef  struct {
  unsigned char min ;
  unsigned char max ;
}ChannelConfigStruct;  
//���������ò�����ṹ

typedef  union{
 unsigned char Byte;
 struct {
 unsigned char ip_flg     :1;        //Ϊ��1�����ݷ�����ʹ��IP��Ϊ��0�����ݷ�����ʹ������
 unsigned char Updata_flg :1;        //�ϴ��������ڵ����־
 unsigned char Debug      :1;        // 1������ģʽ       0����������ģʽ
 unsigned char ReadAll    :1;        // 1��ȫ����������   0������
 unsigned char Appcon_EN  :1;        //�Ƿ�Ӱ���ͨģ��
 }Bit;
}CSR_ConcentratorParameter; 

typedef  union{
 unsigned char Byte;
 struct {
 unsigned char App_RdVal_end_01  :1;    //���ݶ����־
 unsigned char App_UpLod_end_01  :1;   //�ϴ����ݽ�����־
 
 unsigned char App_RdVal_end_02  :1;    //���ݶ����־
 unsigned char App_UpLod_end_02  :1;   //�ϴ����ݽ�����־
 unsigned char NewData_flg       :1;   //��������δ�ϴ�
 }Bit;
}SR1_ConcentratorParameter; 

typedef  struct{
  CSR_ConcentratorParameter  CSR;             //����״̬�Ĵ���
  SR1_ConcentratorParameter  SR1;
  unsigned char              id[3] ;          //��������
  unsigned char              AppconNetID[8];  //�����ֻ�����
  AutoCollectCycleStruct     cycle;           //������ �Զ�����ʱ���� 
  ChannelConfigStruct        channel[4];      //������ ����ͨ���м������� 
  DataServerStruct           server1;         //�����ݷ�����
  DataServerStruct           server2;         //���÷�����
  unsigned char             er_tele[12];     //�����ֻ�����
}ConcentratorParameterStruct;



typedef struct
  {
    union{
      unsigned char Byte;
      struct{
        unsigned char  CN     :1;    //���������ӱ�־   
        unsigned char  IF     :1;    //�������Ϸ������ױ�־  
        unsigned char  RC     :1;    //���������������� ��־  
        unsigned char  ST     :1;    //������ ���־  
        unsigned char  ACK    :1;    //������ Ӧ��
        unsigned char  GZ     :1;    //�������������
      }Bit; 
    }SR;
    CommPortNumber_Type    Port;       //�������˿�
    unsigned char        ServerConnect_CNT ;
    SofTimer_Struct        *Time;  //
  }ServerState_Struct;
/*********************************************************************************
�ⲿ����������
*********************************************************************************/
extern unsigned short              Concentrator_DebugTimer;  
extern ConcentratorParameterStruct   Concentrator;
extern ServerState_Struct            ServerState[2];    //������״̬
extern AutoCollectCycleStruct        ConcentratorUpdataTimer;     //������ �ϴ�ʱ��

/*********************************************************************************
����������
*********************************************************************************/
void MCPX_DeInit(void);
void MXPC_Thread (void);

void DeInitOutputBuff_MXPC(void);
static MCPX_OutputStruct* GetIdleOutputBuff_MXPC (void);
void FreeOutputBuff_MXPC(MCPX_OutputStruct* pbuff);
void FreeOutputBuffByDataACK_MXPC(void* pbuff, void *data, unsigned short length );

SystemErrName 
MXPC_Send_AutoRetrans ( CommPortNumber_Type  outport,     //Ŀ�Ķ˿�
                        void * pbuff,                    //���ݻ�����
                        unsigned short length,          //���ݳ���
                        unsigned char retrans,          //�ش�����
                        unsigned short cycle,           //�ش�������
                        SofTimer_TB tb,                   //�ش�����
                        SerialPort_SendEndCallBack_Type SendEnd_callback,  //������ɻص�
                        MCPX_RetransEndCallBack_Type RetransEnd_callback); //�ط���ɻص�                       
void FreeOutputBuffForData_MXPC (void* pbuff);  
MCPX_OutputStruct* GetOutputBuffForData_MXPC (void* pbuff);   

CommPortNumber_Type getServerPort( unsigned char Server_N0);
void ServerConnect_OK(unsigned char ServerNo, CommPortNumber_Type  Port);
void ServerClose_UEV(unsigned char ch);
void  CommPortClose_UEV (CommPortNumber_Type  Port);
void ReconnecServer( unsigned char ServerNo);
void BreakServerAction(unsigned char ServerNo);
/********************************************************************************/
#endif