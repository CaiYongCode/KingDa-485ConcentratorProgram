/*********************************************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************************************/
#ifndef  __KINGDASERVER_H
#define  __KINGDASERVER_H
/*********************************************************************************************************
�ļ�������
*********************************************************************************************************/
#include "..\..\APP\inc\MCPX.h"
#include ".\CRC8.h"
/*********************************************************************************************************
�궨����
*********************************************************************************************************/

#define  KingDaServerSync_ToServer               0xABAB     //����ͬ����
#define  KingDaServerSync_ToConcentrator         0xBABA     //����ͬ����


#define  RetransNumber_ToServer_Default           2         //���ݰ�Ĭ���ش�����


#define  KingDaServerSendBuff_Number              4         //���ͻ���������

#ifndef  UP
#define  UP     1
#endif

#ifndef  DOWN 
#define  DOWN    0
#endif
/*********************************************************************************************************
�������Ͷ���
*********************************************************************************************************/
typedef struct KingDaServer_SendBuff  KingDaServerSendBuff_Struct;

typedef enum{
// ���������
 ReadMeasureValue     = 0xA1,    //�������ݶ�ȡ
 ReadMeasureValue_OFF = 0x63,    //�������ݶ�ȡ  (����ر�ͨ��) 
//��������������
 HandCmd_Server       = 0x6B,     //������ ��������
 TuneUp_RTC           = 0x68,     //������������Уʱ���� 
 ReadALLMeasureValue  = 0x6D,     //����ָ��   ������������ȡ
 MeasureValue_UpLoad  = 0x6C,     //����ָ��   ����������
//�м�����������
 GetRepeatersState    = 0x67,     //��ѯ�м���״̬  
 
 ServerOpen_OV_ENF       = 0X71,          //ǿ�ƿ���
 ServerClose_OV_ENF      = 0X72,          //ǿ�ƹط�

 
 
 
 
 ServerDeleteNode_APPCON      = 0x80,   //ɾ���ڵ�����
 ServerBATCH_APPCON           = 0x81,   //����������
 ServerSTATUS_APPCON          = 0x82,   //��ѯ�齨����״̬
 ServerOpenOV_APPCON          = 0x65,   //����
 ServerHalfOV_APPCON          = 0x74,   //�뿪����
 ServerMtOV_APPCON            = 0x73,   //�������
 ServerCloseOV_APPCON         = 0x64,   //�ط�
 ServerGetOV_APPCON           = 0x61,   //��ȡ����״̬
 
}KingDaServer_CmdCode;

#pragma pack(1)
//�������ݶ�ȡ  ����������������� ����������
typedef struct{
  unsigned char Concentrator_ID [3];
  unsigned char Node_ID[3];
}ReadMeasureValue_ToKingDaConcentrator_Type;   //�������ݶ�ȡ  ���������������� 
typedef struct{
  unsigned char  Concentrator_ID[3];
  unsigned char  Node_ID[3];
  unsigned short Value;
}ReadMeasureValue_ToKingDaServer_Type;        //�������ݶ�ȡ  ����������������� 

//���Ų���  ����������������� ����������
typedef struct{
  unsigned char Concentrator_ID [3];
  unsigned char Node_ID[3];
}OperateValve_ToKingDaConcentrator_Type;   //���Ų��� ���������������� 
typedef struct{
  unsigned char  Concentrator_ID[3];
  unsigned char  Node_ID[3];
  unsigned short  state;
}OperateValve_ToKingDaServer_Type;        //���Ų���  ����������������� 

//�������ݶ�ȡ  (����ر�ͨ��)
typedef ReadMeasureValue_ToKingDaConcentrator_Type   ReadMeasureValue_OFF_ToKingDaConcentrator_Type;  //�������ݶ�ȡ  ����������������  (����ر�ͨ��)
typedef ReadMeasureValue_ToKingDaServer_Type         ReadMeasureValue_OFF_ToKingDaServer_Type;        //�������ݶ�ȡ  �����������������  (����ر�ͨ��)

//������ ��������
typedef struct{
  unsigned char id [3];
  unsigned char DIO[5];
  unsigned char CSQ;
}HandsData_ToKingDaServer;               //�������������� �����������������
typedef union{
 struct{
  unsigned char id [3];
  unsigned char DI[2];
 }OK;
 struct{
  unsigned char id [5];
 }err;
}HandsData_ToKingDaConcentrator;   //�������������� ����������

//������������Уʱ����
typedef struct{
  unsigned char Concentrator_ID [3];
  unsigned char DIO[2];
}TuneUpRTC_ToKingDaServer;           //������������Уʱ���� �����������������
typedef struct{
  unsigned char Concentrator_ID [3];
  unsigned char Year;
  unsigned char Month;
  unsigned char Day;
  unsigned char Hour;
  unsigned char Minute;
  unsigned char Second;
}TuneUpRTC_ToKingDaConcentrator;    //������������Уʱ���� ����������������
        
//��ѯ�м���״̬
typedef struct{
  unsigned char Concentrator_ID [3];
  unsigned char State;
}GetRepeatersState_ToKingDaServer;         //��ѯ�м���״̬ �����������������
typedef struct{
  unsigned char Concentrator_ID [3];
  unsigned char Repeaters_ID;
}GetRepeatersState_ToKingDaConcentrator;   //��ѯ�м���״̬ ����������������

//��������ȡ���ݵ�����
typedef struct{
  unsigned char ChannelNode_ID [4];    //�м�����ͨ��ˮ������
  unsigned char Concentrator_ID [3];    //��������
  unsigned char RepeatersNet_ID [3];    //�м��������
  unsigned char Repeaters_ID;
  unsigned char RepeatersVoltage;
  unsigned char DIO[2];
  unsigned char Data[400 * 2];
}ReadALLMeasureValue_ToKingDaServer;   //��������ȡ���ݵ����� �����������������



typedef union{
  struct{
  unsigned char Concentrator_ID [3];
  unsigned char DIO[2];
  }OK;        
 struct{
  unsigned char DIO[5];
 }ERR;
}ReadALLMeasureValue_ToKingDaConcentrator;   //��������ȡ���ݵ����� ����������������



//����ͨˮ�����ݸ�ʽ
typedef struct{
 unsigned char id[4];
 unsigned char value[4];
 unsigned char vbat;
 unsigned char link;
 unsigned char route;
}NodeDatas_APPCON_Struct;

//����ͨˮ�����ݸ�ʽ
//typedef struct{
// unsigned long NodeNumber;  //Ϊˮ������
// unsigned char CID[3];         //�������ı��
// unsigned char NETID[4];      //�����
// NodeDatas_APPCON_Struct   Nodes[50];
//}ReadNodeDatas_APPCON_ToKingDaServer_Struct;
  

//��������ȡ���ݵ�����
typedef struct{
  unsigned char ChannelNode_ID [4];    //�м�����ͨ��ˮ������
  unsigned char Concentrator_ID [3];    //��������
  unsigned char RepeatersNet_ID [3];    //�м��������
  unsigned char Repeaters_ID;
  struct{
    unsigned char Year;
    unsigned char Month;
    unsigned char Day;
    unsigned char Hour;
    unsigned char Minute;
    unsigned char Second;
  }Time;
  NodeDatas_APPCON_Struct  Datas[52];
  unsigned char crc;
}ReadALLMeasureValue_ToKingDaServer_APPCON;   //��������ȡ���ݵ����� �����������������



 typedef struct{
  unsigned char Concentrator_ID [3];
  unsigned char Node_ID[8];
}APPCON_ReadMeasureValue_ToKingDaConcentrator_Type;   //�������ݶ�ȡ  ���������������� 
   
typedef struct{
  unsigned char  Concentrator_ID[3];
  unsigned char  Node_ID[8];
  unsigned char  Value[4];
  unsigned char  Bat;
}APPCON_ReadMeasureValue_ToKingDaServer_Type;        //�������ݶ�ȡ  �����������������



//ɾ���ڵ�����
  //BA+BA+80+���ݳ��ȣ����ֽڣ�+�������ţ�3���ֽڣ�+����ţ�2���ֽڣ�+У��
typedef struct{
 unsigned char  Concentrator_ID[3];
  unsigned short  Net_ID;
}APPCON_DeleteNode_ToKingDaConcentrator_Type; 
  //���أ�AB+AB+80+���ݳ��ȣ�1���ֽڣ�+�������ţ�3���ֽڣ�+����ţ�2���ֽڣ�+OK��ER��+У��
typedef struct{
 unsigned char  Concentrator_ID[3];
 unsigned char  Net_ID[2];
 unsigned char  Resu[2];
 }APPCON_DeleteNode_ToKingDaServer_Type; 

 //����������
   //BA+BA+81+���ݳ��ȣ����ֽڣ�+�������ţ�3���ֽڣ�+����ţ�2���ֽڣ�+У��
typedef struct{
  unsigned char  Concentrator_ID[3]; 
  unsigned short  Net_ID;
 }APPCON_BATCH_ToKingDaConcentrator_Type;      
   //���أ�AB+AB+81+���ݳ��ȣ�1���ֽڣ�+�������ţ�3���ֽڣ�+����ţ�2���ֽڣ�+OK��ER��+У��
 typedef struct{
   unsigned char  Concentrator_ID[3];
   unsigned char  Net_ID[2];
   unsigned char  Resu[2];
 }APPCON_BATCH_ToKingDaServer_Type;          
   
//��ѯ�齨����״̬
  //BA+BA+82+���ݳ��ȣ����ֽڣ�+�������ţ�3���ֽڣ�+����ţ�2���ֽڣ�+У��
typedef struct{
  unsigned char  Concentrator_ID[3];
  unsigned short  Net_ID;
 }APPCON_STATUS_ToKingDaConcentrator_Type;      
  //���أ�AB+AB+81+���ݳ��ȣ�1���ֽڣ�+�������ţ�3���ֽڣ�+����ţ�2���ֽڣ�+BUSY��FREE��ERRR��+У��   
typedef struct{
  unsigned char  Concentrator_ID[3];
  unsigned char  Net_ID[2];
  unsigned char  Resu[4];
 }APPCON_STATUS_ToKingDaServer_Type;       
 
//����
  //BA+BA+65+���ݳ��ȣ����ֽڣ�+�������ţ�3���ֽڣ�+����ţ�2���ֽڣ�+У��
typedef struct{
  unsigned char  Concentrator_ID[3];
  unsigned char  Node_ID[8];  
 }APPCON_OpenOV_ToKingDaConcentrator_Type; 
  //���أ�AB+AB+65+���ݳ��ȣ�1���ֽڣ�+�������ţ�3���ֽڣ�+������루2���ֽڣ�+У��
typedef struct{
  unsigned char  Concentrator_ID[3];
  unsigned char  Node_ID[8];
  unsigned short Status;
 }APPCON_OpenOV_ToKingDaServer_Type;   
 
//�뿪����
  //BA+BA+74+���ݳ��ȣ����ֽڣ�+�������ţ�3���ֽڣ�+����ţ�2���ֽڣ�+У��
typedef struct{
  unsigned char  Concentrator_ID[3];
  unsigned char  Node_ID[8];
 }APPCON_HalfOV_ToKingDaConcentrator_Type;
  //���أ�AB+AB+74+���ݳ��ȣ�1���ֽڣ�+�������ţ�3���ֽڣ�+������루2���ֽڣ�+У��
typedef struct{ 
  unsigned char  Concentrator_ID[3];
  unsigned char  Node_ID[8];
  unsigned short Status;
 }APPCON_HalfOV_ToKingDaServer_Type;
 
//�����
  //BA+BA+73+���ݳ��ȣ����ֽڣ�+�������ţ�3���ֽڣ�+����ţ�2���ֽڣ�+У��
typedef struct{
  unsigned char  Concentrator_ID[3]; 
  unsigned char  Node_ID[8];
 }APPCON_MtOV_ToKingDaConcentrator_Type;            
  //���أ�AB+AB+73+���ݳ��ȣ�1���ֽڣ�+�������ţ�3���ֽڣ�+������루2���ֽڣ�+У��
typedef struct{
  unsigned char  Concentrator_ID[3];
  unsigned char  Node_ID[8];
  unsigned short Status;
 }APPCON_MtOV_ToKingDaServer_Type; 
 
//�ط�
  //BA+BA+64+���ݳ��ȣ����ֽڣ�+�������ţ�3���ֽڣ�+����ţ�2���ֽڣ�+У��
typedef struct{
  unsigned char  Concentrator_ID[3];
  unsigned char  Node_ID[8];
 }APPCON_CloseOV_ToKingDaConcentrator_Type;   
  //���أ�AB+AB+64+���ݳ��ȣ�1���ֽڣ�+�������ţ�3���ֽڣ�+������루2���ֽڣ�+У��
typedef struct{
  unsigned char  Concentrator_ID[3];
  unsigned char  Node_ID[8];
  unsigned short Status;
 }APPCON_CloseOV_ToKingDaServer_Type;
 
//10����ѯ����״̬
  //BA+BA+61+���ݳ��ȣ����ֽڣ�+�������ţ�3���ֽڣ�+����ţ�2���ֽڣ�+У��
typedef struct{
  unsigned char  Concentrator_ID[3];
  unsigned char  Node_ID[8];
 }APPCON_GetOV_ToKingDaConcentrator_Type;
//���أ�AB+AB+61+���ݳ��ȣ�1���ֽڣ�+�������ţ�3���ֽڣ�+������루2���ֽڣ�+У��
typedef struct{
  unsigned char  Concentrator_ID[3];
  unsigned char  Node_ID[8];
  unsigned short Status;
 }APPCON_GetOV_ToKingDaServer_Type;


typedef struct{
  unsigned char  Concentrator_ID[3];
  unsigned char  Node_ID[8];
  unsigned short Status;
 }APPCON_OVSTATUS_ToKingDaServer_Type;


//ͨ�����ݰ�ͷ
typedef struct{
 //unsigned char  Sync1;
 //unsigned char  Sync2;
 unsigned short  Sync;
 KingDaServer_CmdCode Cmd;
 unsigned char   Length_H; 
 unsigned char   Length_L;  
}KindgDaServer_PackageHead;
  
typedef union{
 unsigned char                                    data[64];
 HandsData_ToKingDaServer                          HandsData_Send;              //��������������  �����������������
 HandsData_ToKingDaConcentrator                    HandsData_Receive;           //�������������� ����������
 
 ReadMeasureValue_ToKingDaServer_Type              ReadMeasureValue_Send;       //�������ݶ�ȡ  ����������������� 
 ReadMeasureValue_ToKingDaConcentrator_Type        ReadMeasureValue_Receive;    //�������ݶ�ȡ  ���������������� 
 
 OperateValve_ToKingDaServer_Type                  OperateValve_Send;           //���Ų���  �����������������
 OperateValve_ToKingDaConcentrator_Type            OperateValveReceive;         //���Ų��� ����������������  
 
 ReadMeasureValue_OFF_ToKingDaServer_Type          ReadMeasureValue_OFF_Send;   //�������ݶ�ȡ  �����������������  (����ر�ͨ��)
 ReadMeasureValue_OFF_ToKingDaConcentrator_Type    ReadMeasureValue_OFF_Receive;//�������ݶ�ȡ  ����������������  (����ر�ͨ��)
 TuneUpRTC_ToKingDaServer                          TuneUpRTC_Send;              //������������Уʱ���� �����������������
 TuneUpRTC_ToKingDaConcentrator                    TuneUpRTC_Receive;           //������������Уʱ���� ����������������       
 GetRepeatersState_ToKingDaServer                  GetRepeatersState_Send;      //��ѯ�м���״̬ �����������������
 GetRepeatersState_ToKingDaConcentrator            GetRepeatersState_Receive;   //��ѯ�м���״̬ ����������������
 ReadALLMeasureValue_ToKingDaServer                ReadALLMeasureValue_Send;    //��������ȡ���ݵ����� �����������������
 ReadALLMeasureValue_ToKingDaConcentrator          ReadALLMeasureValue_Receive; //��������ȡ���ݵ����� ����������������
 //ReadNodeDatas_APPCON_ToKingDaServer_Struct        NodeDatas_APPCON;
 ReadALLMeasureValue_ToKingDaServer_APPCON          NodeDatas_APPCON_ToKingDaServer;
 APPCON_ReadMeasureValue_ToKingDaConcentrator_Type  ReadMeasureValue_Receive_APPCON;   //�������ݶ�ȡ  ���������������� 
 APPCON_ReadMeasureValue_ToKingDaServer_Type        ReadMeasureValue_Send_APPCON;

 //ɾ���ڵ�����
 APPCON_DeleteNode_ToKingDaConcentrator_Type        APPCON_DeleteNode_Receive; 
 APPCON_DeleteNode_ToKingDaServer_Type              APPCON_DeleteNode_Send;
 //����������
 APPCON_BATCH_ToKingDaConcentrator_Type             APPCON_BATCH_Receive; 
 APPCON_BATCH_ToKingDaServer_Type                   APPCON_BATCH_Send;
 //��ѯ�齨����״̬ 
 APPCON_STATUS_ToKingDaConcentrator_Type            APPCON_STATUS_Receive; 
 APPCON_STATUS_ToKingDaServer_Type                  APPCON_STATUS_Send;
 //����
 APPCON_OpenOV_ToKingDaConcentrator_Type            APPCON_OpenOV_Receive; 
 APPCON_OpenOV_ToKingDaServer_Type                  APPCON_OpenOV_Send;
 //�뿪����
 APPCON_HalfOV_ToKingDaConcentrator_Type            APPCON_HalfOV_Receive; 
 APPCON_HalfOV_ToKingDaServer_Type                  APPCON_HalfOV_Send;
 //�����
 APPCON_MtOV_ToKingDaConcentrator_Type              APPCON_MtOV_Receive;    
 APPCON_MtOV_ToKingDaServer_Type                    APPCON_MtOV_Send;
 //�ط�
 APPCON_CloseOV_ToKingDaConcentrator_Type           APPCON_CloseOV_Receive; 
 APPCON_CloseOV_ToKingDaServer_Type                 APPCON_CloseOV_Send;
 //��ѯ����״̬
 APPCON_GetOV_ToKingDaConcentrator_Type             APPCON_GetOV_Receive; 
 APPCON_GetOV_ToKingDaServer_Type                   APPCON_GetOV_Send;
 
 APPCON_OVSTATUS_ToKingDaServer_Type                APPCON_OVSTATUS_Send;  
}KindgDaServer_PackagePram_type;

typedef struct{

 KindgDaServer_PackageHead       head;
 KindgDaServer_PackagePram_type  pram;
}KingDaServer_PackageStruct;

#pragma pack()

typedef void(*KingDaServer_CmdAckFunc)(KingDaServer_PackageStruct* SendBuff, void* AckData);   //AckData  ��AckData = P_NULL  ��˵�����ݰ�Ӧ��ʱ�� 

struct KingDaServer_SendBuff{
  union{
    unsigned char Byte;
    struct {
      unsigned char idle :1; //����������
      unsigned char AckEn : 1;  //�Ƿ���ҪӦ��
      //unsigned char Preamble :1; //�����Ƿ�����  1������  0��������
    }Bit;
  }CCR;
  unsigned char                  RetransNumber;    //�ط�����
  CommPortNumber_Type             TargetPort;
  KingDaServer_CmdAckFunc         AckFunc;  
  KingDaServer_PackageStruct      buff;    
};

typedef struct{
  union{
    unsigned char Byte;
    struct{
      unsigned char DIR :1;
      //unsigned char Preamble :1; //�����Ƿ�����  1������  0��������
    }Bit;
  }CSR;
  KingDaServer_CmdCode Cmd;            //������
  void* Param;                        //�������
  unsigned char length;              //�����������
  KingDaServer_CmdAckFunc AckFunc;    //����Ӧ��ص�����
}KingDaServer_Struct;


typedef struct 
{
 unsigned char ID;           //�м������
 unsigned char CH_NO[4];     //�м�����ͨ��ˮ������
 unsigned char Voltage;
}Repeaters_DescrType;       //�м���������
/*********************************************************************************************************
�ⲿ����������
*********************************************************************************************************/

/*********************************************************************************************************
����������
*********************************************************************************************************/
void KingDaServer_DeInit(void);

KingDaServerSendBuff_Struct* Get_KingDaServerBuff (void);
void Free_KingDaServerBuff(KingDaServerSendBuff_Struct* pbuff);

//void Free_KingDaServerBuff(KingDaServerSendBuff_Struct* pbuff)

SystemErrName KingDaServer_input (void *pbuff, unsigned char length, CommPortNumber_Type port);

SystemErrName KingDaServer_BuffSend (KingDaServerSendBuff_Struct *pbuff);

SystemErrName SendHandCmdToKingDaServer(unsigned char ServerNO, KingDaServer_CmdAckFunc  AckFunc);

SystemErrName Send_ALLMeasureValue_ToKingDaServer(unsigned char Repeaters_ID, KingDaServer_CmdCode Code);

SystemErrName UpdatMeasureValue_ToKingDaServer(unsigned char Repeaters_NO, 
                                               CommPortNumber_Type Port,
                                               KingDaServer_CmdAckFunc AckFunc);

                                                      
SystemErrName UpdatAppconMeasureValue_ToKingDaServer(unsigned char ServerNo,
                                                     KingDaServer_CmdAckFunc AckFunc);


SystemErrName ReadALLMeasureValue_ToKingDaServer_ACK(unsigned char Repeaters_NO, 
                                               CommPortNumber_Type Port,
                                               KingDaServer_CmdAckFunc AckFunc);

/*SystemErrName ReadAllAppconMeasureValue_ToKingDaServer_ACK(unsigned char Repeaters_NO, 
                                                     CommPortNumber_Type Port,
                                                     KingDaServer_CmdAckFunc AckFunc);*/
/********************************************************************************************************/
#endif

/******************************************END********************************************************/