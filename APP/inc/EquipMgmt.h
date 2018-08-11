/*********************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
#ifndef  __EQUIP_MGMT_H
#define  __EQUIP_MGMT_H 
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include "..\..\SPCP\SPCP_Config.h"
/*********************************************************************************
�궨����
*********************************************************************************/


/*********************************************************************************
�������Ͷ���
*********************************************************************************/
//�ж˽ڵ�Э������
typedef enum{
  UnFind   = 0xFF,
  KingDa   = 1,
  CJT_188  = 2,
  DLT_645  = 3,
  ModBus   = 4,
}Equip_SPCP_type;
 
typedef union {
    KingDaEquipInfor_struct   KingDaNode;
  } EquipRecordData_struct;
//�豸������Ϣ��¼��ʽ  
typedef struct{
  union{
    unsigned char Byte;
    struct{
      unsigned char Idel :1;  //�ü�¼�ռ��Ƿ� ����    1 ����    0 ռ��
      unsigned char Sync :1; //������¼�Ƿ�ͬ�����   0 δ���  1 ���
    }Bit;
  }CSR;
  Equip_SPCP_type Spcp;
  unsigned char  Channel1;  //�豸һ��ͨ���� 
  unsigned char  Channel2;  //�豸����ͨ����  
  EquipRecordData_struct Data;
}EquipRecord_struct;
/*********************************************************************************
�ⲿ����������
*********************************************************************************/

/*********************************************************************************
����������
*********************************************************************************/
SystemErrName MountMemory(void); //�����ⲿ�洢��
SystemErrName CreateBlank_Equip( unsigned long  *number); //�����հ��豸������
SystemErrName GetBlank_Equip( unsigned long  *number);    //��ȡ�հ��豸������

SystemErrName Config_Equip(unsigned long  number,     //�豸������¼�����
                            Equip_SPCP_type  spcp_tpye, //�豸��ͨ��Э���
                            unsigned char  channel1,  //�豸һ��ͨ���� 
                            unsigned char  channel2,  //�豸����ͨ����
                            void*           data,      //�豸��ϸ��Ϣ
                            unsigned char  length);   //��Ϣ����

SystemErrName ReadForNumber_Equip(unsigned long  number, EquipRecord_struct* equip);
SystemErrName  ReadnextOChannel_Equip( unsigned char channel1, 
                                       unsigned char channel2,
                                       unsigned char opt,
                                       EquipRecord_struct* equip);
/********************************************************************************/
#endif
