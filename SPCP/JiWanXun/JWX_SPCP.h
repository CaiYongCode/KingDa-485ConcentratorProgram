/*********************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
#ifndef  JWX_SPCP
#define  JWX_SPCP
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include "..\..\BSP\inc\bsp.h"
/*********************************************************************************
�궨����
*********************************************************************************/
# define Preamble_JWX_SPCP     0xFE
# define SYNC_JWX_SPCP         0x68
# define Tailed_JWX_SPCP       0x16



# define DataLengthLimit_JWX_SPCP       0x64
 
# define JWX_SPCP_TxFunc(p,l)   sendBuffByDMA_Rs485(p,l,P_NULL, P_NULL)
/*********************************************************************************
�������Ͷ���
*********************************************************************************/
typedef enum{
  ColdWaterMeter_JWX  = 0x10,   //��ˮ��
  HeatWaterMeter_JWX  = 0x11,   //������ˮ��
  DrinkWaterMeter_JWX = 0x12,   //ֱ��ˮ��
  RecycledWaterMeter_JWX = 0x13   //��ˮ�� 
}JWX_SPCP_MeterType;

typedef enum{
  JWX_ReadData_ASK     = 0x01,          //������ ������
  JWX_ReadData_ACK     = 0x81,          //������ ��Ӧ��
  JWX_ReadData_ERR     = 0xC1,          //������ ������
  
  JWX_ReadAddress_ASK  = 0x03,          //����ַ ������
  JWX_ReadAddress_ACK  = 0x83,          //����ַ ��Ӧ��
  JWX_ReadAddress_ERR  = 0xC3,          //����ַ ������ 

  JWX_WriteAddress_ASK = 0x15,          //д��ַ ������
  JWX_WriteAddress_ACK = 0x95,          //д��ַ ��Ӧ��
  JWX_WriteAddress_ERR = 0xD5,          //д��ַ ������  
  
  JWX_ValveOperation_ASK = 0x2A,          //���Ų��� ������
  JWX_ValveOperation_ACK = 0xAA,          //���Ų��� ��Ӧ��)
    
  JWX_Disable_ASK = 0x24,          //д�ָ�����״̬����[ȡ����������] ������
  JWX_Disable_ACK = 0xA4,          //д�ָ�����״̬����[ȡ����������] ��Ӧ��
  
  JWX_Enable_ASK = 0x04,          //д������������ ������
  JWX_Enable_ACK = 0x84,          //д������������ ��Ӧ��
  
}JWX_SPCP_CMD_Type;

#pragma pack(1)
typedef  struct
{
  unsigned char Sync ;
  unsigned char NodeType;
  unsigned char Addr[7];
  unsigned char CMD;
  unsigned char DataLength;
}JWX_SPCP_PackageHeader_Type;
#pragma pack()

#pragma pack(1)
typedef  struct
{
  unsigned char Preamble1;
  unsigned char Preamble2;
  JWX_SPCP_PackageHeader_Type  Header ;
  unsigned char Data[DataLengthLimit_JWX_SPCP]; 
}JWX_SPCP_Package_Type;
#pragma pack()

#pragma pack(1)
typedef  struct
{
  unsigned char CS ;
  unsigned char Tailed;
}JWX_SPCP_PackageTailed_Type;
#pragma pack()

//typedef enum{
 // NoErr   = 0,
//  Busy = 1,
 // Length_err   = 2,
 // FramePreamble_err = 3,
 // FrameSync_err = 4,
 // Verify_err = 5,
//  OperateCmd_err    = 6,
 // DataRomWrite_err  = 7,
 // FrameInvalid   = 8,        //��Ч����֡
 // CounterFault =9 ,      //����������
 // PassWordErr = 10
//}JWX_SPCP_ErrCodeType;

#pragma pack(1)
typedef  struct{
  unsigned char Decimals;
  unsigned char Integer_L;
  unsigned char Integer_M;
  unsigned char Integer_H;  
  unsigned char Unit;  
}JWX_SPCP_QuantityFlow_Format;
#pragma pack()

#pragma pack(1)
typedef  struct{
  unsigned char Second;
  unsigned char Minute;
  unsigned char Hour;
  unsigned char Day;  
  unsigned char Month; 
  unsigned short Year;
}JWX_SPCP_Timer_Format;
#pragma pack()

#pragma pack(1)
typedef  union{
  unsigned short  Word;
  struct{
    unsigned char ValveState : 2;
    unsigned char PVD_Flg : 1;
    unsigned char NC1 : 5;
    
    unsigned char ErrState : 1;
    unsigned char Mag_Flg : 1;
    unsigned char NC2 : 2;
    unsigned char NC3 : 2;
    unsigned char Mode : 1;
    unsigned char NC4 : 1;
  }Bit; 
}JWX_SPCP_Status_Format;
#pragma pack()
  
  
#pragma pack(1)
typedef  struct{
  unsigned char  DI0;
  unsigned char   DI1;
  unsigned char   SER;
  JWX_SPCP_QuantityFlow_Format   CURDATA;
  JWX_SPCP_QuantityFlow_Format   SurplusDATA;
  JWX_SPCP_Timer_Format          Timer;
  JWX_SPCP_Status_Format         ST;
}JWX_SPCP_ReadData_AckData_Struct;
#pragma pack()
  
  
#pragma pack(1)
typedef  struct{
  unsigned char   DI0;
  unsigned char   DI1;
  unsigned char   SER;
  JWX_SPCP_Status_Format   ST;
}JWX_SPCP_ValveOperation_AckData_Struct; 
#pragma pack() 


#pragma pack(1)
typedef  struct{
  unsigned char   DI0;
  unsigned char   DI1;
  unsigned char   SER;
}JWX_SPCP_Disable_Enable_AckData_Struct; 
#pragma pack() 


typedef enum{
 JWX_OpenValve  = 0x55,
 JWX_CloseValve  = 0x99, 
 JWX_AutoValve  = 0x11,  
}JWX_ValveOperationCode_Type;
/*********************************************************************************
�ⲿ����������
*********************************************************************************/
extern const unsigned char   BroadcastAddress_JWX_SPCP[];
/*********************************************************************************
����������
*********************************************************************************/
void readVaule_JWX (unsigned char *addr);
void ReadAddress__JWX ( void );
SystemErrName JWX_InPut (void *pbuff, unsigned length);
void ValveOperation_ASK_JWX (unsigned char *addr, JWX_ValveOperationCode_Type  cmd);
/********************************************************************************/

/*********************************************************************************************************/
#endif
