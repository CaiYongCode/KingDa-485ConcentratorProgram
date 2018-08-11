/*********************************************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************************************/
#ifndef  __DLT_645_1997_H
#define  __DLT_645_1997_H
/*********************************************************************************************************
�ļ�������
*********************************************************************************************************/
#include "..\..\BSP\inc\BSP.h"
/*********************************************************************************************************
�궨����
*********************************************************************************************************/
#define  MeterAddrLength        6
#define  DLT_PassWordLength     4



#define  DLT_645_Preamble                0xFE
#define  DLT_645_StartCharacter          0x68
#define  DLT_645_Pause                   0x16
    
#define  DLT_CCR_ReadData_ASK       0x01          //������ ������
#define  DLT_CCR_ReadData_ACK       0x81          //������ ��Ӧ��

#define  DLT_CCR_WriteAddress_ASK    0x04          //д��ַ ������
#define  DLT_CCR_WriteAddress_ACK    0x84          //д��ַ ��Ӧ��

#define DLT_CCR_SetPassWord_ASK       0x0F        //д���� ������
#define DLT_CCR_SetPassWordOk_ACK     0x8F      //д���� ������Ӧ��
#define DLT_CCR_SetPassWordErr_ACK    0xCF      //д���� ���쳣Ӧ��

#define  DLT_DI0_ReadData            0x10      //��������֡
#define  DLT_DI1_ReadData            0x90      //��������֡

//#define  DLT_DI0_WriteAddress        0xA018      //д��ַ
//#define  DLT_DI1_WriteAddress        0xA018      //д��ַ

#define  DLT_DI0_ReadAddress         0x32     //����ַ
#define  DLT_DI1_ReadAddress         0xC0      //����ַ

#define  DLT_DI0_ReadPassWord         0x44     //����ַ
#define  DLT_DI1_ReadPassWord         0xC0     //����ַ    

/*********************************************************************************************************
�������Ͷ���
*********************************************************************************************************/
typedef unsigned char           DLT_645_SER_Type;
typedef unsigned char           DLT_645_DI_Type;

typedef enum{ASK_DLT_645 = 0, ACK_DLT_645 = 1  }DLT_645_DirType;

typedef struct{
  unsigned char MeterAddr[MeterAddrLength];
}DLT_645_AddresStruct;

typedef union {
   unsigned short Word;
   struct{
     unsigned char  Custom ;
     unsigned char  M     : 1;       //�洢��״̬ (1:����,0:����)    
     unsigned char  VF    : 1;       //����״̬ (1:����,0:����)
     unsigned char  S     : 1;       //�ź�״̬ (1:����,0:����)
     unsigned char  Bat   : 1;       //���״̬ (1:����,0:����)     
     unsigned char  NC    : 2;       //����
     unsigned char  Comm  : 1;       //ˮ��ͨѶ״̬�� 1:����,0:������ 
     unsigned char  V     : 1;       //���ſ���״̬ (1:��,0:��)��
    }Bit;
}DLT_645_ST_Stuct;


typedef struct {
   DLT_645_DI_Type          DI0;
   DLT_645_DI_Type          DI1;
} DLT_SuperData_Struct;

typedef struct {
  DLT_645_DI_Type          DI0;
  DLT_645_DI_Type          DI1;
  unsigned char Value[4];
  DLT_645_ST_Stuct   Y;     
} DLT_645_GetValue_ACK_Struct;


typedef struct {
   DLT_645_DI_Type         DI0;
  DLT_645_DI_Type          DI1;
  unsigned char PassWord[DLT_PassWordLength];
  DLT_645_AddresStruct  NewAddr;     
} DLT_645_WriteAddr_ASK_Struct;



typedef struct {
   DLT_645_DI_Type         DI0;
  DLT_645_DI_Type          DI1;
  DLT_645_AddresStruct  Addr;    
} DLT_645_ReadAddr_ACK_Struct;

typedef struct {
  unsigned char CurrentPassWord[DLT_PassWordLength];
  unsigned char NewPassWord[DLT_PassWordLength];
} DLT_645_SetPassWord_ASK_Struct;

typedef struct {
  unsigned char CurrentPassWord[DLT_PassWordLength];
} DLT_645_SetPassWordOk_ACK_Struct;

typedef struct {
  unsigned char ErrCode;
} DLT_645_SetPassWordErr_ACK_Struct;

typedef struct {
  DLT_645_DI_Type         DI0;
  DLT_645_DI_Type         DI1;
  unsigned char CurrentPassWord[DLT_PassWordLength];
} DLT_645_ReadPassWord_ACK_Struct;

typedef struct {
  unsigned char       StartCharacter1;
  DLT_645_AddresStruct Addr;
  unsigned char       StartCharacter2;
  unsigned char       CCR;              
  unsigned char       Length;
}DLT_645_FrameHeadStruct;

//����֡��ʽ
typedef struct{
  DLT_645_FrameHeadStruct  Head;
  union{
   unsigned char                      Byte[32];
   DLT_SuperData_Struct               Super;
   DLT_645_WriteAddr_ASK_Struct       WriteAddr;
   DLT_645_SetPassWord_ASK_Struct     SetPassWord;
   DLT_645_GetValue_ACK_Struct        GetValue;
   DLT_645_ReadAddr_ACK_Struct        ReadAddr; 
   DLT_645_SetPassWordErr_ACK_Struct  SetPassWordErr;
   DLT_645_SetPassWordOk_ACK_Struct   SetPassWordOk;
   DLT_645_ReadPassWord_ACK_Struct    ReadPassWord;
  }Data;
}DLT_645_Frame_Struct;
/*********************************************************************************************************
�ⲿ����������
*********************************************************************************************************/

/*********************************************************************************************************
����������
*********************************************************************************************************/
SystemErrName DLT_645_1997_InPut (void *pbuff, unsigned length);
SystemErrName DLT_645_SendStart (void *pbuff);
void DLT_time ( void );
void BinToBCD_DLT (unsigned short data, unsigned char *pbuff, unsigned char w);
/********************************************************************************************************/
#endif
/******************************************END********************************************************/