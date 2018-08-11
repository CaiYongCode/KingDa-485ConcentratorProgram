/*********************************************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************************************/
#ifndef  __APP_PPROTOCOL_H
#define  __APP_PPROTOCOL_H
/*********************************************************************************************************
�ļ�������
*********************************************************************************************************/
#include "..\..\APP\inc\MCPX.h"
/*********************************************************************************************************
�궨����
*********************************************************************************************************/
#define  NodeType_KingDaV1     1 //1:������ 2���м���  3. ���   0.�����豸
#define  PProtocl_Preamble_Down   0xFE     //����֡ͷ
#define  PProtocl_Preamble_Up     0xDF     //����֡ͷ
#define  PProtocl_Sync_Down       0xA5     //����ͬ����
#define  PProtocl_Sync_Up         0x5A     //����ͬ����
#define  PProtoclSendBuff_Number  4

#define PProtoco_PackageTypeA     0x00    //���ݰ��ṹ����   00: A��     
#define PProtoco_PackageTypeB     0x01    //���ݰ��ṹ����   01: B��

//#define  DomainNameLength_KingDa  64
#ifndef  UP
#define  UP     1
#endif

#ifndef  DOWN 
#define  DOWN    0
#endif


/*********************************************************************************************************
�������Ͷ���
*********************************************************************************************************/

typedef  enum Package_Dir_Type   PProtocol_DIR_Type;

typedef unsigned char   PProtocolPreamble_type;

typedef struct KindgDaSPCPSendBuff  PProtocolSendBuff_Struct;


//KingDa �豸����
typedef enum{
  KingDa_WaterMeter  = 0x01,   //
  KingDa_Repeaters   = 0x02,
  KingDa_Concen      = 0x03,
}KingDaEquip_type;


typedef struct{
  KingDaEquip_type Type ;
  unsigned char ID[4];  //������� 
  unsigned char ADDR[3];  //�����ַ 
}KingDaEquipInfor_struct;

typedef enum{
  NullCmd_KindgDaSPCP  = 0x00,
// ���������
  ReadValue         = 0xD9,
  ReadData          = 0x9d,
  LVP               = 0xA8,
  ReadDDRS          = 0XE5,
  ReadDDR2          = 0XD6,
  WriteDDR2         = 0XB8,
  WriteDDR1         = 0XC4,
  Open_OV           = 0XF1,          //����
  Close_OV          = 0XF4,          //�ط�
  Get_OV            = 0XF6,          //����״̬
  Open_OV_ENF       = 0XF5,          //ǿ�ƿ���
  Close_OV_ENF      = 0XFA,          //ǿ�ƹط�
  MT_OV             = 0XF2,          //�������
  MT_OV_ALL         = 0XF3,          //�㲥�������
//��������������
  ReadId_Concentrator          = 0xB7,    //����������
  WriteId_Concentrator         = 0x7B,    //���ü�������
  ReadTime_Concentrator        = 0xB9,    //��ȡ��ǰʱ��
  WriteTime_Concentrator       = 0x9B,    //������ʱ��У׼����
  ReadCycle_Concentrator       = 0x87,    //��ȡ�Զ�����ʱ����
  WriteCycle_Concentrator      = 0x78,    //�����Զ�����ʱ����
  ReadChannel_Concentrator     = 0xE9,    //��ȡ��ͨ����ʼ�����м�����
  WriteChannel_Concentrator    = 0x9E,    //���ü�������ͨ���м�����ʼ������
  ReadIpEnd_Concentrator       = 0xD7,    //��ȡ�Ƿ����������ӷ�����
  WriteIpEnd_Concentrator      = 0x7D,    //���ÿ������ر�ʹ���������ӷ�����
  ReadServerInf_Concentrator   = 0xA7,    //��ȡ�������
  WriteServerInf_Concentrator  = 0x7A,    //����������� 
  ReadServerI_Concentrator     = 0xC7,    //��ȡ������������Ϣ  
  WriteServerI_Concentrator    = 0x7C,    //���ü���������
  ReadServerII_Concentrator    = 0xE7,    //��ȡ����������Ϣ
  WriteServerII_Concentrator   = 0x7E,    //���ñ�������
//�м�����������  
  ReadID_Repeaters                    = 0x87,   //���м�����
  WriteID_Repeaters                   = 0x86,   //�����м�����
  OPen_Repeaters                      = 0x99,   //���м�������
  Close_Repeaters                     = 0x98,   //�ر��м�������ʹ�м���˯��
  ReadChannelNodeNumber_Repeaters     = 0x9D,   //��ȡ�м���N�¸���ͨ��ˮ������
  WriteChannelNodeNumber_Repeaters    = 0x9C,   //�����м�����ͨ��ˮ������

//����ͨ���߲�������  
  DeleteNet_APPCON                    = 0x61,   //ɾ����������
  BATCH_APPCON                        = 0x62,   //ά�����粢��������
  STATUS_APPCON                       = 0x63,   //����״̬��ѯ����
  ReadNetId_APPCON                    = 0x64,   //��ȡ����ID����
  WriteNetId_APPCON                   = 0x65,   //д����ID����
  RDNODE_APPCON                       = 0x66,   //��ȡ���������߽ڵ�ID������
  ReadFreq_APPCON                    = 0x67,   //��Ƶ�ʲ�������
  WriteFreq_APPCON                   = 0x68,   //дƵ�ʲ�������
  WriteRate_APPCON                   = 0x69,   //д������������������
  ReadNodeValue_APPCON                = 0x70,   //��ȡ���߱�������
  OpenOV_APPCON                       = 0x71,   //����
  HalfOV_APPCON                       = 0x72,   //�뿪����
  MtOV_APPCON                         = 0x73,   //�������
  CloseOV_APPCON                      = 0x74,   //�ط�
  GetOV_APPCON                        = 0x75,   //����״̬
  RDROUTER_APPCON                     = 0x76,   ////��������ڵ�����·��ģ�������
  
}CmdCode_KindgDaSPCP;


#pragma pack(1)
//ˮ������
typedef struct{
  unsigned char addr[3];
  unsigned char NC;
}ReadValue_Node_ASKstruct;    //ˮ������  ��������
typedef struct{
  unsigned char addr[3];
  unsigned short value;
}ReadValue_Node_ACKstruct;   //ˮ������  Ӧ������

//��ˮ������,��ԭʼ��
typedef struct{
  unsigned char addr[3];
  unsigned char NC;
}ReadData_Node_ASKstruct;   //��ˮ������,��ԭʼ��  ��������
typedef struct{
  unsigned char addr[3];
  unsigned short value;
  unsigned char data[4];
}ReadData_Node_ACKstruct;   //��ˮ������,��ԭʼ�� Ӧ������


typedef struct {
  unsigned char addr[3];
  unsigned char flg[2];
}LVP_PRAM_ACKstruct;


typedef struct {
  unsigned char NC[2];
}ReadDDRS_PRAM_ASKstruct;

typedef struct {
  unsigned char DDR1[4];
  unsigned char DDR2[3];
}ReadDDRS_PRAM_ACKstruct;


typedef struct{
  unsigned char DDR1[4];
  unsigned char NC;
}ReadDDR2_Node_ASKstruct;

typedef struct{
  unsigned char DDR1[4];
  unsigned char DDR2[3];
}ReadDDR2_Node_ACKstruct;


typedef struct {
  unsigned char DDR1[4];
  unsigned char DDR2[3];
}WriteDDR2_Node_ASKstruct;// Node  WriteDDR2_PRAM_ASKstruct;

typedef struct{
  //unsigned char DDR1[4];
  unsigned char DDR2[3];
  unsigned char NC;
}WriteDDR2_Node_ACKstruct;  //WriteDDR2_PRAM_ACKstruct;

typedef struct{
  unsigned char DDR1[4];
  unsigned char NC;
}WriteDDR1_PRAM_ASKstruct;

typedef struct{
  unsigned char DDR1[4];
  unsigned char NC;
}WriteDDR1_PRAM_ACKstruct;

typedef struct{
  unsigned char addr[3];
  unsigned char NC;
}Open_OV_ASKstruct;         //����

typedef struct{
  unsigned char addr[3];
  unsigned char NC;
}Open_OV_ACKstruct;         //����

typedef struct{
  unsigned char addr[3];
  unsigned char NC;
}Close_OV_ASKstruct;        //�ط�

typedef struct{
  unsigned char addr[3];
  unsigned char NC;
}Close_OV_ACKstruct;        //�ط�

typedef struct{
  unsigned char addr[3];
  unsigned char NC;
}Get_OV_ASKstruct;          //����״̬

typedef struct{
  unsigned char addr[3];
  unsigned char State;
}Get_OV_ACKstruct;          //����״̬

typedef struct{
  unsigned char addr[3];
  unsigned char NC;
}Open_OV_ENF_ASKstruct;     //ǿ�ƿ���

typedef struct{
  unsigned char addr[3];
  unsigned char NC;
}Open_OV_ENF_ACKstruct;     //ǿ�ƿ���

typedef struct{
  unsigned char addr[3];
}Close_OV_ENF_ASKstruct;    //ǿ�ƹط�

typedef struct{
  unsigned char addr[3];
  unsigned char NC;
}Close_OV_ENF_ACKstruct;    //ǿ�ƹط�

typedef struct{
  unsigned char addr[3];
  unsigned char NC;
}MT_OV_ASKstruct;           //�������

typedef struct{
  unsigned char addr[3];
  unsigned char NC;
}MT_OV_ACKstruct;           //�������

typedef struct{
  unsigned char addr[3];
}MT_OV_ALL_ASKstruct;       //�㲥�������

//����������
typedef struct{
  unsigned char DI1;
  unsigned char DI2;
}ReadId_Concentrator_ASKstruct;       //���������� ��������
typedef struct{
  unsigned char DI1;
  unsigned char id[3];
}ReadId_Concentrator_ACKstruct;       //���������� Ӧ������

//���ü�������
typedef struct{
  unsigned char DI1;
  unsigned char id[3];
  unsigned char DI2;
}WriteId_Concentrator_ASKstruct;       //���ü������� ��������
typedef struct{
  unsigned char DI1;
  //unsigned char DI2; 
  unsigned char state[2];
}WriteId_Concentrator_ACKstruct;       //���ü������� Ӧ������

//��ȡ��ǰʱ��
typedef struct{
  unsigned char DI1;
  unsigned char DI2;
}ReadTime_Concentrator_ASKstruct;       //��ȡ��ǰʱ�� ��������
typedef struct{
   unsigned char DI1;
   //unsigned char DI2;
   unsigned char year;
   unsigned char month;
   unsigned char day;
   unsigned char hour;
   unsigned char minute;
   unsigned char second;
}ReadTime_Concentrator_ACKstruct;       //��ȡ��ǰʱ�� Ӧ������

//������ʱ��У׼����
typedef struct{
   unsigned char DI1;
   unsigned char year;
   unsigned char month;
   unsigned char day;
   unsigned char hour;
   unsigned char minute;
   unsigned char second;
}WriteTime_Concentrator_ASKstruct;       //������ʱ��У׼���� ��������
typedef struct{
   unsigned char DI1;
   unsigned char state[2];
}WriteTime_Concentrator_ACKstruct;       //������ʱ��У׼���� Ӧ������

//��ȡ�Զ�����ʱ����
typedef struct{
   unsigned char DI1;
   unsigned char DI2;
}ReadCycle_Concentrator_ASKstruct;       //��ȡ�Զ�����ʱ���� ��������

typedef struct{
   unsigned char DI1;
   //unsigned char DI2;
   unsigned char hour;
   unsigned char minute;
}ReadCycle_Concentrator_ACKstruct_A;       //��ȡ�Զ�����ʱ���� Ӧ������
typedef struct{
   unsigned char DI1;
   unsigned char Day;
   unsigned char hour;
   unsigned char minute;
}ReadCycle_Concentrator_ACKstruct_B;       //��ȡ�Զ�����ʱ���� Ӧ������

//�����Զ�����ʱ����
typedef struct{
   unsigned char DI1;
   unsigned char hour;
   unsigned char minute;
}WriteCycle_Concentrator_ASKstruct_TypeA;       //�����Զ�����ʱ���� ��������
//�����Զ�����ʱ����
typedef struct{
   unsigned char DI1;
   unsigned char day;
   unsigned char hour;
   unsigned char minute;
}WriteCycle_Concentrator_ASKstruct_TypeB;       //�����Զ�����ʱ���� ��������

typedef struct{
  unsigned char DI1;
  unsigned char state[2];
}WriteCycle_Concentrator_ACKstruct;       //�����Զ�����ʱ���� Ӧ������

//��ȡ��������ͨ����ʼ�����м�����
typedef struct{
   unsigned char DI1;
   unsigned char DI2;
}ReadChannel_Concentrator_ASKstruct;       //��ȡ��������ͨ����ʼ�����м�������������
typedef struct{
  unsigned char DI1;
  unsigned char channel[3][2];
}ReadChannel_Concentrator_ACKstruct;       //��ȡ��������ͨ����ʼ�����м����� Ӧ������

//���ü�������ͨ���м�����ʼ������
typedef struct{
  unsigned char DI1;
  unsigned char channel[3][2];
}WriteChannel_Concentrator_ASKstruct;       //���ü�������ͨ���м�����ʼ������ ��������
typedef struct{
  unsigned char DI1;
  unsigned char state[2];
}WriteChannel_Concentrator_ACKstruct;       //���ü�������ͨ���м�����ʼ������ Ӧ������
 
//��ȡ�Ƿ����������ӷ�����
typedef struct{
   unsigned char DI1;
   unsigned char DI2;
}ReadIpEnd_Concentrator_ASKstruct;       //��ȡ�Ƿ����������ӷ����� ��������
typedef struct{
  unsigned char DI1;
  //unsigned char DI2;
  unsigned char state;
}ReadIpEnd_Concentrator_ACKstruct;       //��ȡ�Ƿ����������ӷ����� Ӧ������

 //���ÿ������ر�ʹ���������ӷ�����
typedef struct{
  unsigned char DI1;
  unsigned char state;
}WriteIpEnd_Concentrator_ASKstruct;       //���ÿ������ر�ʹ���������ӷ����� ��������
typedef struct{
  unsigned char DI1;
  unsigned char state[2];
}WriteIpEnd_Concentrator_ACKstruct;       //���ÿ������ر�ʹ���������ӷ����� Ӧ������


//��ȡ������� 
typedef struct{
   unsigned char DI1;
   unsigned char DI2;
}ReadServerInf_Concentrator_ASKstruct;       //��ȡ�������  ��������
typedef struct{
  unsigned char DI1;
  //unsigned char DI2;
  unsigned char IP_M[4];
  unsigned short PORT_M;
  unsigned char IP_S[4];
  unsigned short PORT_S;
  unsigned char  TEL[5];
}ReadServerInf_Concentrator_ACKstruct;       //��ȡ�������  Ӧ������

//�����������
typedef struct{
  unsigned char DI1;
  unsigned char IP_M[4];
  unsigned short PORT_M;
  unsigned char IP_S[4];
  unsigned short PORT_S;
  unsigned char  TEL[5];
}WriteServerInf_Concentrator_ASKstruct;       //����������� ��������
typedef struct{
  unsigned char DI1;
  unsigned char state[2];
}WriteServerInf_Concentrator_ACKstruct;       //����������� Ӧ������

//��ȡ������������Ϣ  
typedef struct{
  unsigned char DI1;
  unsigned char DI2;
}ReadServer_Concentrator_ASKstruct;       //��ȡ������������Ϣ  ��������
typedef struct{
  unsigned char DI1;
  //unsigned char DI2;
  unsigned char DomainName[DomainNameLength];
  unsigned short PORT;
}ReadServer_Concentrator_ACKstruct;       //��ȡ������������Ϣ  Ӧ������ 

//���ü���������
typedef struct{
  unsigned char DI1;
  unsigned char DomainName[DomainNameLength];
  unsigned short PORT;
}WriteServer_Concentrator_ASKstruct;       //���ü��������� ��������
typedef struct{
  unsigned char DI1;
  unsigned char state[2];
}WriteServer_Concentrator_ACKstruct;       //���ü��������� Ӧ������ 

//��ȡ�ź�ǿ��
typedef struct{
  unsigned char DI1;
  unsigned char DI2;
}ReadGprsRssi_ASKstruct;       //���ü��������� ��������
typedef struct{
  unsigned char DI1;
  unsigned char Rssi;
}ReadGprsRssi_ACKstruct;       //���ü��������� Ӧ������ 

typedef struct{
 unsigned char  Sync;
 CmdCode_KindgDaSPCP  Cmd;
 unsigned short  Length; 
}PPro_PackageHead_struct2;

typedef struct{
 PPro_PackageHead_struct2  Head;
 ReadGprsRssi_ACKstruct  data;
 unsigned char  crc;  
}ReadGprsRssi_AckPacage_struct;



//���м�����
typedef struct{
  unsigned char DI1;
  unsigned char DI2;
}ReadID_Repeaters_ASKstruct;       //��ȡ�м����� ��������
typedef struct{
  unsigned char DI1;
  unsigned char DI2;
}ReadID_Repeaters_ACKstruct;       //��ȡ�м����� ��������


//�����м�����
typedef struct{
  unsigned char DI1;
  unsigned char DI2;
}WriteID_Repeaters_ASKstruct;       //�����м����� ��������
typedef struct{
  unsigned char DI1;
  unsigned char DI2;
}WriteID_Repeaters_ACKstruct;       //�����м����� Ӧ������ 

//���м�������  
typedef struct{
  unsigned char RepeatersId;
  unsigned char ChannelNumber;
}OPen_Repeaters_ASKstruct;       //���м������� ��������
typedef struct{
  //unsigned char DI1;
  //unsigned char DI2;
  unsigned char RepeatersId;
  unsigned char ChannelNumber;
}OPen_Repeaters_ACKstruct;       //���м������� Ӧ������ 

//�ر��м�������  
typedef struct{
  unsigned char RepeatersId;
  unsigned char Zero;
}Close_Repeaters_ASKstruct;       //�ر��м������� ��������
typedef struct{
  //unsigned char DI1;
  //unsigned char DI2;
  unsigned char RepeatersId;
  unsigned char Zero;
}Close_Repeaters_ACKstruct;       //�ر��м�������  Ӧ������ 

//��ȡ�м���N�¸���ͨ��ˮ������  
typedef struct{
  unsigned char id;
  unsigned char NC;
}ReadChannelNodeNumber_Repeaters_ASKstruct;       //��ȡ�м���N�¸���ͨ��ˮ������ ��������
typedef struct{
  unsigned char id;
  unsigned char NodeNumber_channel[4];
}ReadChannelNodeNumber_Repeaters_ACKstruct;       //��ȡ�м���N�¸���ͨ��ˮ������ Ӧ������ 

//�����м�����ͨ��ˮ������
typedef struct{
  unsigned char id;
  unsigned char NodeNumber_channel[4];
}WriteChannelNodeNumber_Repeaters_ASKstruct;       //�����м�����ͨ��ˮ������ ��������
typedef struct{
  //unsigned char DI1;
  //unsigned char DI2;  
  unsigned char id;
  unsigned char NodeNumber_channel[4];
}WriteChannelNodeNumber_Repeaters_ACKstruct;       //�����м�����ͨ��ˮ������ Ӧ������ 

//������ ��������
typedef struct{
  unsigned char id;
  unsigned char NodeNumber_channel[4];
}HandsData_ToServer;       //�����м�����ͨ��ˮ������ ��������
typedef struct{
  //unsigned char DI1;
  //unsigned char DI2;  
  unsigned char id;
  unsigned char NodeNumber_channel[4];
}WriteChannelNodeNumber_Repeaters_ToServer;       //�����м�����ͨ��ˮ������ Ӧ������ 



//ͨ�����ݰ�ͷ
typedef struct{
 unsigned char  Sync;
 CmdCode_KindgDaSPCP  Cmd;
 unsigned char  Length; 
}PPro_PackageHead_TypeA;

typedef struct{
 unsigned char  Sync;
 CmdCode_KindgDaSPCP  Cmd;
 unsigned short  Length; 
}PPro_PackageHead_TypeB;
  
typedef union{
  unsigned char              data[64];
  WriteDDR2_Node_ASKstruct   WriteDDR2_ask;  //дˮ���ַ  ��������
  WriteDDR2_Node_ACKstruct   WriteDDR2_ack;  //��ˮ���ַ  Ӧ������ 

  ReadDDR2_Node_ASKstruct    ReadDDR2_ask;   //��ˮ���ַ  ��������
  ReadDDR2_Node_ACKstruct    ReadDDR2_ack;   //��ˮ���ַ  ��������
  
  ReadValue_Node_ASKstruct   ReadValue_ask;  //ˮ������  ��������
  ReadValue_Node_ACKstruct   ReadValue_ack;  //ˮ������  Ӧ������

  ReadData_Node_ASKstruct    ReadData_ask;   //��ˮ������,��ԭʼ��  ��������
  ReadData_Node_ACKstruct    ReadData_ack;   //��ˮ������,��ԭʼ��  Ӧ������
  
  Open_OV_ASKstruct          Open_OV_ask;    //����
  Open_OV_ACKstruct          Open_OV_ack;    //����
  
  Close_OV_ASKstruct         Close_OV_ask;      //�ط�
  Close_OV_ACKstruct         Close_OV_ack;      //�ط�
  
  Get_OV_ASKstruct           Get_OV_ask;        //����״̬
  Get_OV_ACKstruct           Get_OV_ack;        //����״̬

  Open_OV_ENF_ASKstruct      Open_OV_ENF_ask;   //ǿ�ƿ���
  Open_OV_ENF_ACKstruct      Open_OV_ENF_ack;   //ǿ�ƿ���

  Close_OV_ENF_ASKstruct     Close_OV_ENF_ask;  //ǿ�ƹط�
  Close_OV_ENF_ACKstruct     Close_OV_ENF_ack;  //ǿ�ƹط�
  
  MT_OV_ASKstruct            MT_OV_ask;         //�������
  MT_OV_ACKstruct            MT_OV_ack;         //�������
  
  MT_OV_ALL_ASKstruct        MT_OV_ALL_ask;     //�㲥�������

  ReadId_Concentrator_ASKstruct          ReadId_ask;            //���������� ��������
  ReadId_Concentrator_ACKstruct          ReadId_ack;            //���������� Ӧ������
  WriteId_Concentrator_ASKstruct         WriteId_ask;           //���ü������� ��������
  WriteId_Concentrator_ACKstruct         WriteId_ack;           //���ü������� Ӧ������
  ReadTime_Concentrator_ASKstruct        ReadTime_ask;          //��ȡ��ǰʱ�� ��������
  ReadTime_Concentrator_ACKstruct        ReadTime_ack;          //��ȡ��ǰʱ�� Ӧ������   
  WriteTime_Concentrator_ASKstruct       WriteTime_ask;         //������ʱ��У׼���� ��������
  WriteTime_Concentrator_ACKstruct       WriteTime_ack;         //������ʱ��У׼���� Ӧ������   
  ReadCycle_Concentrator_ASKstruct       ReadCycle_ask;         //��ȡ�Զ�����ʱ���� ��������
  ReadCycle_Concentrator_ACKstruct_A     ReadCycle_ack_A;
  ReadCycle_Concentrator_ACKstruct_B     ReadCycle_ack_B;         //��ȡ�Զ�����ʱ���� Ӧ������   
  WriteCycle_Concentrator_ASKstruct_TypeA     WriteCycle_ask_TypeA;        //�����Զ�����ʱ���� ��������
  WriteCycle_Concentrator_ASKstruct_TypeB     WriteCycle_ask_TypeB;        //�����Զ�����ʱ���� ��������
  WriteCycle_Concentrator_ACKstruct      WriteCycle_ack;        //�����Զ�����ʱ���� Ӧ������    
  ReadChannel_Concentrator_ASKstruct     ReadChannel_ask;       //��ȡ��������ͨ����ʼ�����м�������������
  ReadChannel_Concentrator_ACKstruct     ReadChannel_ack;       //��ȡ��������ͨ����ʼ�����м����� Ӧ������   
  WriteChannel_Concentrator_ASKstruct    WriteChannel_ask;      //���ü�������ͨ���м�����ʼ������ ��������
  WriteChannel_Concentrator_ACKstruct    WriteChannel_ack;      //���ü�������ͨ���м�����ʼ������ Ӧ������   
  ReadIpEnd_Concentrator_ASKstruct       ReadIpEnd_ask;         //��ȡ�Ƿ����������ӷ����� ��������
  ReadIpEnd_Concentrator_ACKstruct       ReadIpEnd_ack;         //��ȡ�Ƿ����������ӷ����� Ӧ������   
  WriteIpEnd_Concentrator_ASKstruct      WriteIpEnd_ask;        //���ÿ������ر�ʹ���������ӷ����� ��������
  WriteIpEnd_Concentrator_ACKstruct      WriteIpEnd_ack;        //���ÿ������ر�ʹ���������ӷ����� Ӧ������     
  ReadServerInf_Concentrator_ASKstruct   ReadServerInf_ask;     //��ȡ�������  ��������
  ReadServerInf_Concentrator_ACKstruct   ReadServerInf_ack;     //��ȡ�������  Ӧ������
  WriteServerInf_Concentrator_ASKstruct  WriteServerInf_ask;    //����������� ��������
  WriteServerInf_Concentrator_ACKstruct  WriteServerInf_ack;    //����������� Ӧ������   
  ReadServer_Concentrator_ASKstruct     ReadServer_ask;       //��ȡ������������Ϣ  ��������
  ReadServer_Concentrator_ACKstruct     ReadServer_ack;       //��ȡ������������Ϣ  Ӧ������ 
  WriteServer_Concentrator_ASKstruct    WriteServer_ask;      //���ü��������� ��������
  WriteServer_Concentrator_ACKstruct    WriteServer_ack;      //���ü��������� Ӧ������
  ReadGprsRssi_ASKstruct                ReadGprsRssi_ask;       //���ü��������� ��������
  ReadGprsRssi_ACKstruct                ReadGprsRssi_ack;//���ü��������� Ӧ������
 
  
  ReadChannelNodeNumber_Repeaters_ASKstruct   ReadChannelNodeNumber_Repeaters_ask;       //��ȡ�м���N�¸���ͨ��ˮ������ ��������
  ReadChannelNodeNumber_Repeaters_ACKstruct   ReadChannelNodeNumber_Repeaters_ack;       //��ȡ�м���N�¸���ͨ��ˮ������ Ӧ������ 
  WriteChannelNodeNumber_Repeaters_ASKstruct  WriteChannelNodeNumber_Repeaters_ask;      //�����м�����ͨ��ˮ������ ��������
  WriteChannelNodeNumber_Repeaters_ACKstruct  WriteChannelNodeNumber_Repeaters_ack;      //�����м�����ͨ��ˮ������ Ӧ������ 
  OPen_Repeaters_ASKstruct                    OPen_Repeaters_ask;                        //���м������� ��������
  OPen_Repeaters_ACKstruct                    OPen_Repeaters_ack;                        //���м������� Ӧ������ 
  Close_Repeaters_ASKstruct                   Close_Repeaters_ask;                       //�ر��м������� ��������
  Close_Repeaters_ACKstruct                   Close_Repeaters_ack;                       //�ر��м�������  Ӧ������ 
  ReadID_Repeaters_ASKstruct                  ReadID_Repeaters_ask;                      //��ȡ�м����� ��������
  ReadID_Repeaters_ACKstruct                  ReadID_Repeaters_ack;                      //��ȡ�м����� ��������
  WriteID_Repeaters_ASKstruct                 WriteID_Repeaters_ask;                     //�����м����� ��������
  WriteID_Repeaters_ACKstruct                 WriteID_Repeaters_ack;                     //�����м����� Ӧ������ 
}PProtocolPackagePram_type;



typedef union{
  struct{
     PPro_PackageHead_TypeA     head;
     PProtocolPackagePram_type  pram;
  }TypeA;
  struct{
     PPro_PackageHead_TypeB     head;
     PProtocolPackagePram_type  pram;
  }TypeB; 
}KindgDaSPCP_PackageDataStruct;


typedef struct{
 PProtocolPreamble_type Prm1;
 PProtocolPreamble_type Prm2;
 KindgDaSPCP_PackageDataStruct Data;  
}KindgDaSPCP_PackageStruct;
#pragma pack()

typedef void(*KindgDaSPCPCmdCode_AckFunc)(PProtocolSendBuff_Struct* SendBuff, void* AckData);   //AckData  ��AckData = P_NULL  ��˵�����ݰ�Ӧ��ʱ�� 

struct KindgDaSPCPSendBuff{
  union{
    unsigned char Byte;
    struct {
      unsigned char idle :1; //����������
      unsigned char Preamble :1; //�����Ƿ�����  1������  0��������
      unsigned char OFF : 1;     //������Ϻ��Ƿ�رշ���ͨ��   1�������ر�  0��������
      unsigned char TypeA_B : 2;   //���ݰ��ṹ����   00: A��     01 :B��
    }Bit;
  }CCR;
  CommPortNumber_Type             SourcePort;
  CommPortNumber_Type             TargetPort;
  KindgDaSPCPCmdCode_AckFunc      AckFunc;
  KindgDaSPCP_PackageStruct       buff;    
};

typedef struct{
  union{
    unsigned char Byte;
    struct{
      unsigned char DIR :1;
      unsigned char Preamble :1; //�����Ƿ�����  1������  0��������
    }Bit;
  }CSR;
  CommPortNumber_Type SourcePort;     //ԭʼ�˿�
  CmdCode_KindgDaSPCP Cmd;           //������
  unsigned char TargetPort;           //���͵�Ԥ�ڵ�ַ 
  void* Param;                        //�������
  unsigned char length;              //�����������
  KindgDaSPCPCmdCode_AckFunc AckFunc;//����Ӧ��ص�����
}KindgDaCmd_Struct;


typedef struct{
  union{
    unsigned char Byte;
    struct{
      unsigned char Preamble :1; //�����Ƿ�����  1������  0��������
    }Bit;
  }CSR;
  CommPortNumber_Type SourcePort;     //ԭʼ�˿�
  CommPortNumber_Type TargetPort;     //ת����Ŀ��˿�
  KindgDaSPCP_PackageDataStruct* pbuff;  //�������
}Transpond_KingDaSPCP_struct;

/*********************************************************************************************************
�ⲿ����������
*********************************************************************************************************/

/*********************************************************************************************************
����������
*********************************************************************************************************/
void KingDaV1Protocol_DeInit(void);

CommPortNumber_Type getPortForAddr ( unsigned char addr);

SystemErrName SendCmd_KindDaSPCP(KindgDaCmd_Struct* Cmd);

SystemErrName KingDaV1Protocol_input (void *pbuff, 
                                      unsigned char length,
                                      CommPortNumber_Type port);

SystemErrName ReadNodeValue_KindDa(unsigned char* NoderAddr,
                                   unsigned char SendPort,  
                                   CommPortNumber_Type SourcePort,
                                   KindgDaSPCPCmdCode_AckFunc AckFunc);

SystemErrName  OperateValve_KindDa(CmdCode_KindgDaSPCP Operate,
                                   unsigned char* NoderAddr,
                                   unsigned char SendPort,
                                   CommPortNumber_Type SourcePort,                                   
                                   KindgDaSPCPCmdCode_AckFunc AckFunc);

SystemErrName OpenNETChannel_KindDa(unsigned char RepeatersId, 
                                    unsigned char ChannelNumber,
                                    CommPortNumber_Type SourcePort,
                                    KindgDaSPCPCmdCode_AckFunc AckFunc);

SystemErrName CloseNETChannel_KindDa (unsigned char RepeatersId, 
                                      unsigned char ChannelNumber,
                                      CommPortNumber_Type SourcePort,
                                      KindgDaSPCPCmdCode_AckFunc AckFunc);

SystemErrName CloseRepeatersId_KindDa ( unsigned char RepeatersId, 
                                       CommPortNumber_Type SourcePort,
                                       KindgDaSPCPCmdCode_AckFunc AckFunc);

SystemErrName ReadRepeatersNodeNumber_KindDa ( unsigned char RepeatersId,
                                               CommPortNumber_Type SourcePort,
                                               KindgDaSPCPCmdCode_AckFunc AckFunc);
/********************************************************************************************************/
#endif

/******************************************END********************************************************/