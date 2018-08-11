/*********************************************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************************************/
#ifndef  __CJT188_2004_H
#define  __CJT188_2004_H
/*********************************************************************************************************
�ļ�������
*********************************************************************************************************/
#include "..\..\BSP\inc\bsp.h"
#include "..\..\Projects\inc\SystemDefine.h"
#include "..\..\APP\inc\MCPX.h"
/*********************************************************************************************************
�궨����
*********************************************************************************************************/
#define  CJT188_OutPutBuff_Number      4

#define  Locall_MeterType     ColdWaterMeter
#define  Locall_FactoryID     ('K' *10) + 'D'

#define  Locall_Type                 0  //0 �����ݲɼ���   1�����ݴ�����

#define  PreambleNumber              2 

#define  CJT188_Preamble             0xFE
#define  CJT188_StartCharacter       0x68
#define  CJT188_Pause                0x16



#define  CJT188_ColdWaterMeter       0x10     //��ˮ��
#define  CJT188_HeatWaterMeter       0x11     //������ˮ��
#define  CJT188_DrinkWaterMeter      0x12     //ֱ��ˮ��
#define  CJT188_RecycledWaterMeter   0x13     //��ˮ��   


#define  CJT188_ReadData_CTR         0x01        //������ ������
#define  CJT188_ReadVerNo_CTR        0x09        //����Կ�汾�� ������
#define  CJT188_ReadAddress_CTR      0x03        //����ַ ������
#define  CJT188_WriteData_CTR        0x04        //д���� ������
#define  CJT188_WriteAddress_CTR     0x15        //д��ַ ������
#define  CJT188_WriteOffset_CTR      0x16        //д����ͬ�����ݣ�����

#define  CJT188_ReadData_DI          0x1F90      //��������
#define  CJT188_ReadHistData_1_DI    0x20D1      //��ʷ��������1
#define  CJT188_ReadHistData_2_DI    0x21D1      //��ʷ��������2
#define  CJT188_ReadHistData_3_DI    0x22D1      //��ʷ��������3
#define  CJT188_ReadHistData_4_DI    0x23D1      //��ʷ��������4
#define  CJT188_ReadHistData_5_DI    0x24D1      //��ʷ��������5
#define  CJT188_ReadHistData_6_DI    0x25D1      //��ʷ��������6
#define  CJT188_ReadHistData_7_DI    0x26D1      //��ʷ��������7
#define  CJT188_ReadHistData_8_DI    0x27D1      //��ʷ��������8
#define  CJT188_ReadHistData_9_DI    0x28D1      //��ʷ��������9
#define  CJT188_ReadHistData_10_DI   0x29D1      //��ʷ��������10
#define  CJT188_ReadHistData_11_DI   0x2AD1      //��ʷ��������11
#define  CJT188_ReadHistData_12_DI   0x2BD1      //��ʷ��������12
#define  CJT188_ReadPriceList_DI     0x0281      //���۸��
#define  CJT188_ReadSelDate_DI       0x0381      //��������
#define  CJT188_ReadRecordDate_DI    0x0481      //��������
#define  CJT188_ReadRMoney_DI        0x0581      //��������
#define  CJT188_ReadVerNo_DI         0x0681      //����Կ�汾��
#define  CJT188_ReadAddress_DI       0x0A81      //����ַ

#define  CJT188_WritePriceList_DI    0x10A0      //д�۸��
#define  CJT188_WriteSelDate_DI      0x11A0      //д��������
#define  CJT188_WriteRecordDate_DI   0x12A0      //д������
#define  CJT188_WriteRMoney_DI       0x13A0      //��������
#define  CJT188_WriteSecretKey_DI    0x14A0      //д��Կ�汾��
#define  CJT188_WriteTime_DI         0x15A0      //д��עʱ��
#define  CJT188_WriteValveState_DI   0x17A0      //д���ſ���
#define  CJT188_Lock_DI              0x19A0      //��������
#define  CJT188_WriteAddress_DI      0x18A0      //д��ַ
#define  CJT188_WriteOffset_DI       0x16A0      //д��ַ
/*********************************************************************************************************
�������Ͷ���
*********************************************************************************************************/
#pragma pack(1)
typedef unsigned char           CJT188_SER_Type;
typedef unsigned short          CJT188_DI_Type;

typedef enum{ASK_CJT188 = 0, ACK_CJT188 = 1  }CJT188_DirType;

typedef enum{
  Wh        = 0x02,   //��ˮ��
  KWh       = 0x05,   //������ˮ��
  MWh       = 0x08,
  MWh_100   = 0x0a,
  J         = 0x01,
  KJ        = 0x0B,
  MJ        = 0x0E, 
  GJ        = 0x11,
  GJ_100    = 0x13,
  W         = 0x14,
  KW        = 0x17,
  MW        = 0x1A,
  L         = 0x29,
  m3        = 0x2C,
  L_h       = 0x32,
  m3_h      = 0x35,
}CJT188_UnitType;

typedef struct{
  unsigned char MeterID[5];
  unsigned short FactoryID;
}CJT188_AddresStruct;

typedef union{
  unsigned char Byte;
  struct{
    unsigned char Infr : 6;
    unsigned char CommFlg : 1; //0 �� ͨѶ����; 1�� ͨ Ѷ �� �� 
    unsigned char DIR : 1;     //0 �� ����վ�����Ŀ���֡; 1�� �ɴ�վ������Ӧ��֡��
  }Bit;
}CJT188_CTRStruct;

typedef struct {
  unsigned char       Preamble[PreambleNumber]; 
  unsigned char       StartCharacter;
  unsigned char       Type;
  CJT188_AddresStruct Addr;
  CJT188_CTRStruct    CTR;
  unsigned char        Length;
}CJT188_FrameHeadStruct;

typedef struct{
  CJT188_DI_Type   DI;
  CJT188_SER_Type  SER;
}CJT188_DataHeadStruct;

//֡β��ʽ
typedef struct {
  unsigned char CS;     //У����,��֡ͷ��ʼ��֡У���־֮ǰ�����ֽڵ��ۼӺ͵�8λ	
  unsigned char Pause;  //������0x16H
}CJT188_FrameStern_Struct;

//�ڵ�״̬
typedef union {
   unsigned short Word;
   struct{
      unsigned short F1: 1;          //�����Զ���1
      unsigned short F2: 1;          //�����Զ���2
      unsigned short F3: 1;          //�����Զ���3
      unsigned short CN: 2;          //����
      unsigned short Bat_Flg: 1;    //��ص�ѹ   0������  1��Ƿѹ
      unsigned short Valve_State : 2;    //����״̬ 00������ 01 �أ� 11���쳣
      unsigned short Custom :8;
    }Bit;
}CJT188_ST_Stuct;

//ʵʱʱ�Ӹ�ʽ
typedef struct{
  unsigned short  Year;
  unsigned char   Mon;
  unsigned char   Day;
  unsigned char   Hour;
  unsigned char   Min;
  unsigned char   Sec;
}CJT188_Time_Struct;

//������ ��ʽ
typedef struct{
  unsigned char          Flow_PV[4];              //��ǰ�ۻ�����
  //CJT188_UnitType        Flow_PV_Unit;            //��ǰ�ۻ�������λ
  //unsigned char          Flow_Accounts[4];        //�������ۻ�����
  // CJT188_UnitType        Flow_Accounts_Unit;      //�������ۻ�������λ
  //CJT188_Time_Struct     Time;                    //ʵʱʱ��
  CJT188_ST_Stuct        ST;                      //
}CJT188_MeterData_Struct;  

//
typedef enum{
  CJT188_ValveON      = 0x55,   //�������Ʋ���:
  CJT188_ValveOFF     = 0x99,   //�ط����Ʋ���:
}CJT188_ValveState_Type;

//����Ӧ֡��ʽ
typedef struct{
  CJT188_FrameHeadStruct  Head;
  CJT188_DataHeadStruct   DataHead;
  unsigned char Data[32];  
}CJT188_OkACK_Struct;

//����Ӧ��֡��ʽ
typedef struct{
  CJT188_FrameHeadStruct    Head;
  CJT188_SER_Type           SER;
  CJT188_ST_Stuct           ST;     //�ڵ�״̬
  CJT188_FrameStern_Struct  Stern;
}CJT8_188_ErrACK_Struct;

//
typedef union{        
   CJT188_FrameHeadStruct  Head;
   CJT188_OkACK_Struct     Normal;
   CJT8_188_ErrACK_Struct  ERR;
   unsigned char           Data[64]; 
 }CJT188_Frame_Struct; 

#pragma pack()


typedef struct CJT188_SendBuff  CJT188_SendBuff_Struct;

typedef void(*CJT188_AckFunc)(CJT188_SendBuff_Struct* SendData, CJT188_Frame_Struct* AckData);
typedef SerialPort_SendEndCallBack_Type  CJT188_SendOKFunc;

struct CJT188_SendBuff{
 union{
    unsigned char Byte;
    struct{
      unsigned char idle :1; //����������
    }Bit;
  }CSR; //����״̬�Ĵ���
 CJT188_SendOKFunc    SendOK;   //������ɴ�����
 CJT188_AckFunc       AckFunc;      //Ӧ������
 CommPortNumber_Type  SourcePort;
 CommPortNumber_Type  TargetPort;
 CJT188_Frame_Struct  buff;
};
//CJT188����������
typedef struct{
 CommPortNumber_Type SourcePort; 
 unsigned char       Type;
 CJT188_AddresStruct Addr;
 CJT188_CTRStruct    CTR;
 CJT188_SER_Type     SER;
 CJT188_DI_Type      DataDI;
 unsigned char  ParamLength;
 unsigned char  Param[32];
}CJT188_CmdDeclarator;
/*********************************************************************************************************
�ⲿ����������
*********************************************************************************************************/

/*********************************************************************************************************
����������
*********************************************************************************************************/
void CJT188_SPCP_init(void);

SystemErrName CJT188_2004_InPut (void *pbuff, unsigned length, CommPortNumber_Type port);

CJT188_SER_Type getSER (void);

SystemErrName CJT188_SendCmd (CJT188_CmdDeclarator* Cmd,
                              CommPortNumber_Type TargetPort,
                              CJT188_AckFunc AckFunc);

SystemErrName CJT188_ReadNodeValue (unsigned char  MeterType,
                                    CJT188_AddresStruct* Addr,
                                    unsigned short DI,
                                    CommPortNumber_Type SourcePort,
                                    CommPortNumber_Type TargetPort,
                                    CJT188_AckFunc AckFunc);

SystemErrName CJT188_OperateNodeValve(unsigned char  MeterType,
                                      CJT188_AddresStruct* Addr,
                                      unsigned char  ValveCmd,
                                      CommPortNumber_Type SourcePort,
                                      CommPortNumber_Type TargetPort,
                                      CJT188_AckFunc AckFunc);
/********************************************************************************************************/
#endif

/******************************************END********************************************************/