/*********************************************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************************************/
#ifndef  __JINDA_H
#define  __JINDA_H
/*********************************************************************************************************
�ļ�������
*********************************************************************************************************/
#include "..\..\APP\inc\MCPX.h"
/*********************************************************************************************************
�궨����
*********************************************************************************************************/

#define  JingDa_OutPutBuff_Number     1        //�������������

#define  JD_MeterConfigFile_PathName               "FLASH:/JDMeter"       //�洢���������������ˮ����Ϣ
#define  JD_OperateRecordFile_PathName             "FLASH:/JDOperate"     //�洢������¼
#define  JD_AllMeterCurrentValueFile_PathName      "FLASH:/JDValue"       //�洢���б�Ƶ�ǰ����
#define  JD_AllMeterFreezingValueFile_PathName     "FLASH:/JDFreezing"    //�洢���������б���ն�������





#define  JINGDA_AreaCode             0xEE      //������ ������

#define  JINGDA_Preamble             0xFEFE     //֡ͷ
#define  JINGDA_StartCharacter       0x68     //֡ͷ
#define  JINGDA_Pause                0x16     //֡β

#define  ConcentratorAddr_Length     5        //��������ַ���� 
#define	 LSSB			     0x10     //��ˮˮ��

//��վ�뼯����֮�� �����붨��
#define      JD_Time_Wcmd		0x01	//1���ü�����ʱ��														
#define      JD_Time_Rcmd		0x02	//2��ȡ������ʱ��															
#define      JD_ConfigData_Wcmd		0x03	//3���ü������������													
#define      JD_ConfigData_Rcmd		0x04	//4��ȡ�������������													
#define      JD_Gprs_Wcmd		0x07	//5���ü�����GPRS����													
#define      JD_Gprs_Rcmd		0x08	//6��ȡ������GPRS����													
#define      JD_Meter_Wcmd		0x09	//7���ر�Ƶ�ַ															
#define      JD_Meter_Rcmd		0x0B	//8��ȡ���سɹ���Ƶ�ַ													
#define      JD_MeterErr_Rcmd		0x0C	//9��ȡ����ʧ�ܱ�Ƶ�ַ													
#define      JD_Operate_Wcmd		0x0E	//10�´��������															
#define      JD_Operate_Rcmd		0x0F	//11��ȡ��������															
#define      JD_Reading_Wcmd		0x14	//12������������															
#define      JD_AllCurrentData_Rcmd	0x15	//13��ȡĳһ�ɼ��������б�Ƶ�ǰ����									
#define      JD_AllFreezingData_Rcmd	0x16	//14��ȡĳһ�ɼ��������б���ն�������									
#define      JD_SingCurrentData_Rcmd	0x17	//15��ȡ����ĵ�ǰ����													
#define      JD_SingFreezingData_Rcmd	0x18	//16��ȡ������ն�������													
#define      JD_AlterMeter_Wcmd		0x1C	//17�滻���																
#define      JD_DeleteMeter_Wcmd	0x1D	//18ɾ�����																
//#define      JD_Beat_cmd		0x1D	//19��¼֡������֡
    
/*********************************************************************************************************
�������Ͷ���
*********************************************************************************************************/
#pragma pack(1)
//������ʱ��  ���ݸ�ʽ
typedef struct {
  unsigned char Year;   	//��ֻȡ����λ (BCD)
  unsigned char	Month;  	//�� (BCD)
  unsigned char	Day;    	//�� (BCD)
  unsigned char	Hour;		//ʱ (BCD)
  unsigned char Minute;		//�� (BCD)
  unsigned char Second;		//�� (BCD)
}JD_Time_Stuct;

//��Ƶ�ַ��ʽ
typedef struct {
  unsigned char  ID[5];	//���ַ1λ
  union{
    unsigned short Word; 
    struct{
      unsigned char Byte1;   //���ֽ�
      unsigned char Byte2;   //���ֽ�
    }Byte;
  }FactoryCode;  //���̴���
}JD_MeterAddr_Stuct;

//��Լ���ͼ�ͨ���� ���ݸ�ʽ
typedef  union{
  unsigned char Byte; 
  struct{
    unsigned char Protocol: 4;    /* ��Լ����   0000:	����188 */
    unsigned char Channel : 3;    /*ͨ����   0000:	δָ��ͨ��
                                             0001:	M-BUSͨ��1
                                             0010:	M-BUSͨ��2
                                             0011:	M-BUSͨ��3
                                             0100:	M-BUSͨ��4
                                             0101:	RS485ͨ��
                                             0110:	����ͨ�� */
    unsigned char Poll_EN : 1;   //�Ƿ�Ѳ��   0:��Ѳ��  1:Ѳ��
  }Bit;
}JD_LinkDeclarator;    

//ͨѶ���� ���ݸ�ʽ	
typedef union{
  unsigned char Byte; 
  struct{
    unsigned char BaudRate: 3; /*������ 000: 1200bps
					001: 2400bps
					010: 4800bps
		 			011: 9600bps
		 			100: 19200bps
		 			101: 38400bps  ��ʱδ��
		 			110: 57600bps  ��ʱδ��	
                                        111: 115200bps*/
    unsigned char DataNumber : 2;   //����λ  00: 6λ����λ  01: 7λ����λ 10: 8λ����λ
    unsigned char StopBits : 1;   //ֹͣλ  0 :1λֹͣλ   1 :2λֹͣλ
    unsigned char Parity : 2;   //У��λ  00:��У��     01:żУ��       10:��У��
  }Bit;
}JD_CommDeclarator; 

//�������������  ���ݸ�ʽ
typedef struct {
  union{
    unsigned short Word; 
    struct{
      unsigned char Byte_L;   //���ֽ�
      unsigned char Byte_H;   //���ֽ�
    }Byte;
  }ReadingCycle;	
  unsigned char	DayFrozen;	//�ն���ʱ�� 0��23��������ͨ����Ч  HEX
  unsigned char	MonthFrozenH;	//�¶���ʱ�䣬1-31  HEX 	
  unsigned char	MonthFrozenL;	//�¶���ʱ�̣�0-23  HEX
  union{
    unsigned char Byte; 
    struct{
      unsigned char D0 : 1;     //1Ϊ�رճ�������
      unsigned char D1 : 1;     //1Ϊ�ر��ն��ᣬ���ǳ�������²��ر��ն��ᡣ
      unsigned char D2 : 1;	//1Ϊ�ر��¶��ᡣ
      unsigned char D3 : 1;	//1Ϊ�ر����㳭��
    }Bit;
  }FreezingSwitch;
}JD_ConfigData_Stuct;

//������GPRS����  ���ݸ�ʽ
typedef struct {
  union{
    unsigned char Byte; 
    struct{
      unsigned char D0 : 1;     //�����Ƿ�ʹ��DNS��0��ʹ��IP��ַ��1ʹ��DNS
      unsigned char D1 : 1;     //�����Ƿ�ʹ��APN��0����ʹ��APN��1ʹ��APN 
    }Bit;
  }ConnectionMode;	        //GPRS���ӷ�ʽ(HEX)
  unsigned char IP[4];		//ʮ�������� ��һ�ֽ�(HEX)
  union{
    unsigned short Word; 
    struct{
      unsigned char Byte_L;   //���ֽ�
      unsigned char Byte_H;   //���ֽ�
    }Byte;
  }Port;	              //���ֽ� ʮ������ ��λ��ǰ ��λ�ں� (HEX)       
  unsigned char APN[16];  //16�ֽڣ�������16�ֽڣ����ಿ����䡮\0�� (ASCII)
  unsigned char DNS[37];  //37�ֽڣ�������37�ֽڣ����ಿ����� ��\0��(ASCII)	
  unsigned char	BeatCycle; //1�ֽڣ���λ�����ӣ�(HEX)
}JD_Gprs_Stuct;

//�ڵ���������ʽ
typedef struct {
  unsigned char        MeterType;  //�ڵ�����ID (HEX)  
  JD_MeterAddr_Stuct   Addr;       //�ڵ��ַ
  JD_LinkDeclarator    CR1;        //�ڵ��Լ���ͼ�ͨ���� (HEX)
  JD_CommDeclarator    CR2;	   //�ڵ�ͨѶ����
}JD_Meter_Stuct;

//������ݸ�ʽ
typedef struct{
  JD_Meter_Stuct Meter;
   // Byte1;    //��ǰ�ۻ�����С�������λ(BCD)
   // Byte2;    //��ǰ�ۻ������������ֵĵ���λ(BCD)
   // Byte3;    //��ǰ�ۻ������������ֵ��м���λ(BCD)
   // Byte4;    //��ǰ�ۻ������������ֵĸ���λ(BCD)
  unsigned char Value[4];
  unsigned char    State1;     //���״̬1
  unsigned char    State2;     //���״̬2
}JD_CJT188_MeterValue_Stuct;

//���Ƽ�¼��ʽ��
typedef struct{
  unsigned char         OPID;	    //����ԱID(HEX)��
  JD_Time_Stuct	        Time;
  unsigned char         MeterType;  //���ر�����ID(HEX)
  JD_MeterAddr_Stuct    Addr;       //���ַ
  JD_LinkDeclarator     CR1;        //�ڵ��Լ���ͼ�ͨ���� (HEX)
  JD_CommDeclarator     CR2;        //����ͨѶ����(HEX)
  unsigned char  MeterState;        //����״̬(HEX) 0X99���ط���0X55������
}JD_OperateRecord_Stuct;


//���ü�����ʱ������ ��վ����>������ ����������� (�����룺0X01)
typedef struct {
  unsigned char Year;   	//�� BCD
  unsigned char	Month;  	//�� BCD
  unsigned char	Day;    	//�� BCD
  unsigned char	Hour;		//ʱ BCD 
  unsigned char Minute;		//�� BCD
  unsigned char Second;		//�� BCD
  unsigned char WeekDay;	//�ܼ� BCD
  unsigned char SER;	        /*���к�(HEX)*/
}JD_TimeWcmd_askParam_Stuct; 
//���ü�����ʱ������ ����������>��վ ����������� (�����룺0X01)
typedef struct{
  unsigned char ReplyFlag;     /*Ӧ���־��(HEX)
                                            0x00 :����
                                            0xCC : ʱ�䲻�Ϸ� */
  unsigned char SER;	        /*���к�(HEX)*/
}JD_TimeWcmd_ackParam_Stuct;

//��ȡ������ʱ������ ��վ����>������ ����������� (�����룺0X02)
typedef struct{
  unsigned char SER;	        /*���к�(HEX)*/
}JD_TimeRcmd_askParam_Stuct;
//��ȡ������ʱ������ ����������>��վ ����������� (�����룺0X02)
typedef struct
{
  unsigned char Year;   	//�� BCD
  unsigned char	Month;  	//�� BCD
  unsigned char	Day;    	//�� BCD
  unsigned char	Hour;		//ʱ BCD 
  unsigned char Minute;		//�� BCD
  unsigned char Second;		//�� BCD
  unsigned char WeekDay;	//�ܼ� BCD
  unsigned char ReplyFlag;      //Ӧ���־(HEX)��0x00���������쳣Ӧ��
  unsigned char SER;	        /*���к�(HEX)*/  
}JD_TimeRcmd_ackParam_Stuct;


//���ü������������   ��վ����>������  ����������� (�����룺0X03)
typedef struct{
  JD_ConfigData_Stuct   data;
  unsigned char         SER;	/*���к�(HEX)*/
}JD_ConfigDataWcmd_askParam_Stuct;
//���ü������������   ����������> ��վ ����������� (�����룺0X03)
typedef struct{
  unsigned char  ReplyFlag;     /*Ӧ���־��(HEX)
                                            0x00 : ���� 
                                            0xCC : ����д��eeprom����ȷ*/
  unsigned char        SER;	/*���к�(HEX)*/
}JD_ConfigDataWcmd_ackParam_Stuct;

//��ȡ�������������   ��վ����>������  ����������� (�����룺0X04)
typedef struct{
  unsigned char         SER;	/*���к�(HEX)*/
}JD_ConfigDataRcmd_askParam_Stuct;
			
//��ȡ�������������   ����������> ��վ  ����������� (�����룺0X04) 	
typedef struct{
  JD_ConfigData_Stuct Data;
  unsigned char  ReplyFlag;   //Ӧ���־(HEX)��0x00���������쳣Ӧ��
  unsigned char  SER;         /*���к�(HEX)*/  
}JD_ConfigDataRcmd_ackParam_Stuct;

//���ü�����GPRS����   ��վ����>������  ����������� (�����룺0x07) 
typedef struct{
  JD_Gprs_Stuct   data;
  unsigned char   SER;	/*���к�(HEX)*/
}JD_GprsWcmd_askParam_Stuct;
//���ü�����GPRS����   ����������> ��վ ����������� (�����룺0X07)
typedef struct{
  unsigned char  ReplyFlag;   //Ӧ���־(HEX)��0x00���������쳣Ӧ��
  unsigned char  SER;	      /*���к�(HEX)*/
}JD_GprsWcmd_ackParam_Stuct;

//��ȡ�������������   ��վ����>������  ����������� (�����룺0X08)
typedef struct{
  unsigned char         SER;	/*���к�(HEX)*/
}JD_GprsRcmd_askParam_Stuct;

//��ȡ������GPRS����   ����������>��վ  ����������� (�����룺0X08)
typedef struct{
  JD_Gprs_Stuct Data;
  unsigned char ReplyFlag;   //Ӧ���־(HEX)��0x00���������쳣Ӧ��
  unsigned char  SER;	     /*���к�(HEX)*/
}JD_GprsRcmd_ackParam_Stuct;

//���ر�Ƶ�ַ   ��վ����>������  �����������(�����룺0X09)
typedef struct {
  unsigned char  PackageSum;      //���зְ�(HEX)
  union{
    unsigned short Word;
    struct{
      unsigned char Byte_L;   //���ֽ�
      unsigned char Byte_H;   //���ֽ�
    }Byte;		
  }MeterSum;                     //���б���(HEX)         
  unsigned char  PackageNumber;	 //�ڼ�������(HEX)
  unsigned char  MetNumber; 	 //�ð���Ƶ�ַ����(HEX)
  JD_Meter_Stuct Meter[25];      //���˵������(HEX)
  unsigned char  SER;	     /*���к�(HEX)*/
}JD_MeterWcmd_askParam_Stuct;

//���ر�Ƶ�ַ   ����������>��վ ����������� (�����룺0X09)
typedef struct{
  unsigned char  ReplyFlag;   //Ӧ���־(HEX)��0x00 ���سɹ������쳣
  unsigned char  SER;	     /*���к�(HEX)*/
}JD_MeterWcmd_ackParam_Stuct;

//��ȡ���سɹ���Ƶ�ַ  ��վ����>������  �����������(�����룺0x0B)
typedef struct {
  unsigned char  Channel;   /*ͨ����:(HEX)0x01��0x06									
                                     00000001:	M-BUSͨ��1							
                                     00000010:	M-BUSͨ��2							
                                     00000011:	M-BUSͨ��3
                                     00000100:	M-BUSͨ��4
                                     00000101:	RS485ͨ��
                                     00000110:	����ͨ��*/
  unsigned char  PackageNumber;   //�ڼ�������(HEX)
  unsigned char  SER;	     /*���к�(HEX)*/
}JD_MeterRcmd_askParam_Stuct;

//��ȡ���سɹ���Ƶ�ַ  ����������>��վ  �����������(�����룺0x0B)
typedef struct {
  unsigned char  PackageSum;	 //���зְ�(HEX)
  union{
    unsigned short Word;
    struct{
      unsigned char Byte_L;      //���ֽ�
      unsigned char Byte_H;      //���ֽ�
    }Byte;
  }MeterSum;                     //���б���(HEX)       
  unsigned char  PackageNumber;	 //�ڼ�������(HEX)	
  unsigned char  MetNumber; 	 //�ð���Ƶ�ַ����
  JD_Meter_Stuct Meter[25];      //���˵������  
  unsigned char	 ReplyFlag;      //Ӧ���־(HEX):0x00:������0xCB:�޴˰�
  unsigned char  SER;	         /*���к�(HEX)*/
}JD_MeterRcmd_ackParam_Stuct;

//��ȡ����ʧ�ܱ�Ƶ�ַ  ��վ����>������  �����������(�����룺0x0C)
typedef struct {
  unsigned char  PackageNumber;	//�ڼ�������(HEX)
  unsigned char  SER;	        /*���к�(HEX)*/
}JD_MeterErrRcmd_askParam_Stuct;

//��ȡ���سɹ���Ƶ�ַ  ����������>��վ  �����������(�����룺0x0C)
typedef struct {
  unsigned char  PackageSum;    //���зְ�(HEX)
  union{
    unsigned short Word;
    struct{
      unsigned char Byte_L;     //���ֽ�
      unsigned char Byte_H;     //���ֽ�
    }Byte;		
  }MeterSum;                    //  ���б���(HEX)
  unsigned char  PackageNumber;	//�ڼ�������(HEX)	
  unsigned char  MetNumber; 	//�ð���Ƶ�ַ����(HEX)
  JD_Meter_Stuct Meter[25];     //���˵������(HEX)  
  unsigned char  ReplyFlag;     //Ӧ���־  0x00:����; 0xCB:�޴˰�
  unsigned char  SER;	        /*���к�(HEX)*/
}JD_MeterErrRcmd_ackParam_Stuct;

//�´�������� ��վ����>������  �����������(�����룺0X0E)  
typedef struct {
  JD_OperateRecord_Stuct  Operate;
  unsigned char           SER;	    /*���к�(HEX)*/
}JD_OperateWcmd_askParam_Stuct;

//�´��������  ����������>��վ  �����������(�����룺0X0E)
typedef struct {
  unsigned char  MeterType;	  	//�������ID   0x16   HEX	    
  JD_MeterAddr_Stuct  Addr;
  unsigned char  MetState;  /*����״̬(HEX)  0x00���ط��ɹ�
                                             0x01���ط�����λ
					     0x02���ط�ʧ��
					     0x03�������ɹ�
					     0x04����������λ
					     0x05������ʧ��*/
  unsigned char ReplyFlag;  /*Ӧ���־ (HEX) 0x00: �ɹ���
					     0x01: δ��ѯ����ƣ�
					     0x02: �������Ϸ���
					     0x05: MBUSͨ���쳣��
					     0x06: �������Ӧ��
                                             0x07: ��Ӧ��
					     0xCB: ����æ��
					     0xCE: ���в�����ʱ*/
  unsigned char SER;	    /*���к�(HEX)*/ 
}JD_OperateWcmd_ackParam_Stuct;

//ȡ���Ƽ�¼   ��վ����>������  �����������(�����룺0X0F	)  
typedef struct {
  unsigned char  PackageNumber;	//�ڼ�������  HEX
  unsigned char  SER;	        /*���к�(HEX)*/
}JD_OperateRcmd_askParam_Stuct;

//ȡ���Ƽ�¼   ����������>��վ  �����������(�����룺0X0F)
typedef struct {
  unsigned char  PackageSum;	       //����������(HEX)
  unsigned char  PackageNumber;	       //�ڼ�������(HEX)
  unsigned char  ConNumber;	       //���Ƽ�¼����N (HEX)
  JD_OperateRecord_Stuct  Record[15];  //���Ƽ�¼����;
  unsigned char  ReplyFlag;	       /*Ӧ���־(HEX)��0x00:����
                                                        0xCA:�޴˰�*/
  unsigned char  SER;	               /*���к�(HEX)*/
}JD_OperateRcmd_ackParam_Stuct;

//������������  ��վ����>������  �����������(�����룺0X14)  
typedef struct {
  unsigned char SER;	   /*���к�(HEX)*/  	
}JD_ReadingWcmd_askParam_Stuct;

//������������  ����������>��վ  �����������(�����룺0X14)  
typedef struct {
  unsigned char ReplyFlag; /*Ӧ���־ 0x00:������������
				      0x01:����æ�Ѿ��ڳ�����,�����ٴ�����
			              0x02:��������Ѳ����ַ,������������*/
  unsigned char SER;	   /*���к�(HEX)*/  	
}JD_ReadingWcmd_ackParam_Stuct;

//��ȡ���б�Ƶ�ǰ����  ��վ����>������  �����������(�����룺0X15)
typedef struct {
  unsigned char PackageNumber;	//�ڼ�������(HEX)
  unsigned char SER;	   /*���к�(HEX)*/  
}JD_AllCurrentDataRcmd_askParam_Stuct; 

//��ȡ���б�Ƶ�ǰ����  ����������>��վ  �����������(�����룺0X15)  
typedef struct {
  unsigned char FileSum;    //�ļ�����(HEX)	
  unsigned char Filecount;  //�ļ����(HEX)
  unsigned char RecordNum;  //��¼�������������� (HEX)
  JD_Time_Stuct Time;       //ʱ��
  union{
    JD_CJT188_MeterValue_Stuct CJT188[17];
    unsigned char buff[sizeof (JD_CJT188_MeterValue_Stuct )*17];
  }Value;
  unsigned char ReplyFlag;  /*Ӧ���־(HEX)��0x00:����; 0xCA:�޴˰�*/
  unsigned char SER;	    /*���к�(HEX)*/  
}JD_AllCurrentDataRcmd_ackParam_Stuct;

//��ȡȫ������ն�������   ��վ����>������  �����������(�����룺0X16)
typedef struct {
  unsigned char PackageNumber;	//�ڼ�������(HEX)
  unsigned char Year;	        //�� ���ֽ�(BCD)
  unsigned char Month;          //��(BCD)
  unsigned char Day;		//��(BCD)
  unsigned char SER;	        /*���к�(HEX)*/  
}JD_AllFreezingDataRcmd_askParam_Stuct;  

//��ȡȫ������ն�������   ����������>��վ  �����������(�����룺0X16)  
typedef struct {
  unsigned char FileSum;       //�ļ�����(HEX)	
  unsigned char Filecount;     //�ļ����(HEX)
  unsigned char RecordNum;     //��¼�������������� (HEX)
  JD_Time_Stuct Time;          //ʱ��
  union{
    JD_CJT188_MeterValue_Stuct  CJT188[17];
    unsigned char buff[sizeof(JD_CJT188_MeterValue_Stuct )*17];
  }Value;
  unsigned char ReplyFlag; /*Ӧ���־(HEX)��0x00:����;
					    0xCD:δ�ҵ������� 
					    0xCA�޴˰�*/
  unsigned char SER;	   /*���к�(HEX)*/  
}JD_AllFreezingDataRcmd_ackParam_Stuct;

//������ĵ�ǰ����   ��վ����>������  �����������(�����룺0X17)
typedef struct {
  JD_Meter_Stuct  Meter; //�������
  unsigned char   SER;	 /*���к�(HEX)*/  
}JD_SingCurrentDataRcmd_askParam_Stuct;  

//������ĵ�ǰ����   ����������>��վ  �����������(�����룺0X17)  
typedef struct {
  JD_CJT188_MeterValue_Stuct Data;
  unsigned char ReplyFlag; /*Ӧ���־(HEX)��0x00:������0xCF:δ�ҵ��� */
  unsigned char SER;	   /*���к�(HEX)*/  
}JD_SingCurrentDataRcmd_CJT188_ackParam_Stuct;

//��ȡ������ն�������   ��վ����>������  �����������(�����룺0X18)
typedef struct {
  unsigned char	  Year;	   //�� ���ֽ�(BCD)
  unsigned char	  Month;   //��(BCD)
  unsigned char	  Day;	   //��(BCD)
  JD_Meter_Stuct  Meter;   //�������
  unsigned char   SER;	   /*���к�(HEX)*/  
}JD_SingFreezingDataRcmd_askParam_Stuct;

//��ȡ������ն�������   ����������>��վ  �����������(�����룺0X18)  
typedef struct {
  JD_Time_Stuct Time;      //ʱ��
  union{
    JD_CJT188_MeterValue_Stuct CJT188;
    unsigned char buff[sizeof (JD_CJT188_MeterValue_Stuct )];
  }Value;
  unsigned char ReplyFlag; /*Ӧ���־(HEX)��0x00:����;
				            0xCD:δ�ҵ������� 
				            0xCF:δ�ҵ��� */
  unsigned char SER;	  /*���к�(HEX)*/  
}JD_SingFreezingDataRcmd_ackParam_Stuct;

//�滻���   ��վ����>������  �����������(�����룺0X1C)
typedef struct {
  JD_Meter_Stuct  OldMeter; 	//�������
  JD_Meter_Stuct  NewMeter; 	//�������
  unsigned char   SER;	        /*���к�(HEX)*/  
}JD_AlterMeterWcmd_askParam_Stuct;  
//�滻���   ����������>��վ  �����������(�����룺0X1C) 
typedef struct {
  unsigned char ReplyFlag; /*Ӧ���־(HEX): 0x00: �滻�ɹ�
                                            0x01: δ��ѯ�����
                                            0x02: �������Ϸ�
					    0x04: ��ַ�ظ�
                                            0x05: MBUSͨ���쳣
					    0x06: �������Ӧ
					    0xCB: ����æ
					    0xCE: ���в�����ʱ*/
  unsigned char   SER;	        /*���к�(HEX)*/ 
}JD_AlterMeterWcmd_ackParam_Stuct;   

//ɾ�����   ��վ����>������  �����������(�����룺0X1c)
typedef struct {
  unsigned char   TotalNumber;    //ɾ����¼����(HEX)  
  JD_Meter_Stuct  Meter[25]; 	  //�������
  unsigned char   SER;	          /*���к�(HEX)*/ 
}JD_DeleteMeterWcmd_askParam_Stuct; 

//ɾ�����   ����������>��վ  �����������(�����룺0X1c) 
typedef struct {
  unsigned char TotalNumber;      //ɾ����¼����(HEX)
  unsigned char DeleteResult[25]; // ��N�� 00�ɹ���01ɾ��ʧ��  
  unsigned char ReplyFlag;        /*Ӧ���־(HEX): 0x00: ɾ���ɹ�; 
                                                   0xCB: ����æ; 
                                                   0xCE: ���в�����ʱ*/
  unsigned char SER;	          /*���к�(HEX)*/ 
}JD_DeleteMeterWcmd_ackParam_Stuct; 

typedef unsigned short   JingDa_Preamble;
//֡֡ͷ��ʽ
typedef struct {	
  unsigned char StartCharacter;  //֡ͷֵΪ0x68H
  unsigned char LAI;             //����ʶ����	1Byte	����ʶ�����ݴ��������
  unsigned char Concentrator[ConcentratorAddr_Length]; //��������ַ 5�ֽ�BCD��
  unsigned char Code;            //������
  union{
    unsigned short Word;
    struct{
      unsigned char Byte_L;
      unsigned char Byte_H;
    }Byte;
  }DataLength;                   //���ݶγ���
}JingDa_FrameHead_Struct;

//֡β��ʽ
typedef struct {
  unsigned char CS;     //У����,��֡ͷ��ʼ��֡У���־֮ǰ�����ֽڵ��ۼӺ͵�8λ	
  unsigned char Pause;  //������0x16H
}JingDa_FrameStern_Struct;


//����֡��ʽ
typedef struct{
  JingDa_FrameHead_Struct  Head;
  union{ 
    //���ü�����ʱ������ ��վ����>������ ����������� (�����룺0X01)
    JD_TimeWcmd_askParam_Stuct              TimeWcmd_ask; 
    //���ü�����ʱ������ ����������>��վ ����������� (�����룺0X01)
    JD_TimeWcmd_ackParam_Stuct              TimeWcmd_ack;
    //��ȡ������ʱ������ ��վ����>������ ����������� (�����룺0X02)
    JD_TimeRcmd_askParam_Stuct              TimeRcmd_ask;
    //��ȡ������ʱ������ ����������>��վ ����������� (�����룺0X02)
    JD_TimeRcmd_ackParam_Stuct              TimeRcmd_ack;
    //���ü������������   ��վ����>������  ����������� (�����룺0X03)
    JD_ConfigDataWcmd_askParam_Stuct        ConfigDataWcmd_ask;
    //���ü������������   ����������> ��վ ����������� (�����룺0X03)
    JD_ConfigDataWcmd_ackParam_Stuct        ConfigDataWcmd_ack;
    //��ȡ�������������   ��վ����>������  ����������� (�����룺0X04)
    JD_ConfigDataRcmd_askParam_Stuct        ConfigDataRcmd_ask; 	
    //��ȡ�������������   ����������> ��վ  ����������� (�����룺0X04) 	
    JD_ConfigDataRcmd_ackParam_Stuct        ConfigDataRcmd_ack;
    //���ü�����GPRS����   ��վ����>������  ����������� (�����룺0x07) 
    JD_GprsWcmd_askParam_Stuct              GprsWcmd_ask;
    //���ü�����GPRS����   ����������> ��վ ����������� (�����룺0X07)
    JD_GprsWcmd_ackParam_Stuct              GprsWcmd_ack;
    //��ȡ�������������   ��վ����>������  ����������� (�����룺0X08)
    JD_GprsRcmd_askParam_Stuct              GprsRcmd_ask;
    //��ȡ������GPRS����   ����������>��վ  ����������� (�����룺0X08)
    JD_GprsRcmd_ackParam_Stuct              GprsRcmd_ack;
    //���ر�Ƶ�ַ   ��վ����>������  �����������(�����룺0X09)
    JD_MeterWcmd_askParam_Stuct             MeterWcmd_ask;
    //���ر�Ƶ�ַ   ����������>��վ ����������� (�����룺0X09)
    JD_MeterWcmd_ackParam_Stuct             MeterWcmd_ack;
    //��ȡ���سɹ���Ƶ�ַ  ��վ����>������  �����������(�����룺0x0B)
    JD_MeterRcmd_askParam_Stuct             MeterRcmd_ask;
    //��ȡ���سɹ���Ƶ�ַ  ����������>��վ  �����������(�����룺0x0B)
    JD_MeterRcmd_ackParam_Stuct             MeterRcmd_ack;
    //��ȡ����ʧ�ܱ�Ƶ�ַ  ��վ����>������  �����������(�����룺0x0C)
    JD_MeterErrRcmd_askParam_Stuct          MeterErrRcmd_ask;
    //��ȡ���سɹ���Ƶ�ַ  ����������>��վ  �����������(�����룺0x0C)
    JD_MeterErrRcmd_ackParam_Stuct          MeterErrRcmd_ack;
    //�´�������� ��վ����>������  �����������(�����룺0X0E)  
    JD_OperateWcmd_askParam_Stuct          OperateWcmd_ask;
    //�´��������  ����������>��վ  �����������(�����룺0X0E)
    JD_OperateWcmd_ackParam_Stuct          OperateWcmd_ack;
    //ȡ���Ƽ�¼   ��վ����>������  �����������(�����룺0X0F	)  
    JD_OperateRcmd_askParam_Stuct          OperateRcmd_ask;
    //ȡ���Ƽ�¼   ����������>��վ  �����������(�����룺0X0F)
    JD_OperateRcmd_ackParam_Stuct          OperateRcmd_ack;
    //������������  ��վ����>������  �����������(�����룺0X14)  
    JD_ReadingWcmd_askParam_Stuct          ReadingWcmd_ask;
    //������������  ����������>��վ  �����������(�����룺0X14)  
    JD_ReadingWcmd_ackParam_Stuct          ReadingWcmd_ack;
    //��ȡ���б�Ƶ�ǰ����  ��վ����>������  �����������(�����룺0X15)
    JD_AllCurrentDataRcmd_askParam_Stuct   AllCurrentDataRcmd_ask; 
    //��ȡ���б�Ƶ�ǰ����  ����������>��վ  �����������(�����룺0X15)  
    JD_AllCurrentDataRcmd_ackParam_Stuct   AllCurrentDataRcmd_ack;
    //��ȡȫ������ն�������   ��վ����>������  �����������(�����룺0X16)
    JD_AllFreezingDataRcmd_askParam_Stuct   AllFreezingDataRcmd_ask;  
    //��ȡȫ������ն�������   ����������>��վ  �����������(�����룺0X16)  
    JD_AllFreezingDataRcmd_ackParam_Stuct     AllFreezingDataRcmd_ack;
    //������ĵ�ǰ����   ��վ����>������  �����������(�����룺0X17)
    JD_SingCurrentDataRcmd_askParam_Stuct         SingCurrentDataRcmd_ask;  
    //������ĵ�ǰ����   ����������>��վ  �����������(�����룺0X17)  
    JD_SingCurrentDataRcmd_CJT188_ackParam_Stuct  SingCurrentDataRcmd_CJT188_ack;
    //��ȡ������ն�������   ��վ����>������  �����������(�����룺0X18)
    JD_SingFreezingDataRcmd_askParam_Stuct       SingFreezingDataRcmd_ask;
    //��ȡ������ն�������   ����������>��վ  �����������(�����룺0X18)  
    JD_SingFreezingDataRcmd_ackParam_Stuct       SingFreezingDataRcmd_ack;
    //�滻���   ��վ����>������  �����������(�����룺0X1C)
    JD_AlterMeterWcmd_askParam_Stuct    AlterMeterWcmd_ask; 
    //�滻���   ����������>��վ  �����������(�����룺0X1C) 
    JD_AlterMeterWcmd_ackParam_Stuct    AlterMeterWcmd_ack;   
    //ɾ�����   ��վ����>������  �����������(�����룺0X1c)
    JD_DeleteMeterWcmd_askParam_Stuct   DeleteMeterWcmd_ask;
    //ɾ�����   ����������>��վ  �����������(�����룺0X1c) 
    JD_DeleteMeterWcmd_ackParam_Stuct   DeleteMeterWcmd_ack;
    unsigned char                       Data[256]; 
  }Param;
}JingDa_FrameDatas_Struct;

//����֡��ʽ
typedef struct{
  JingDa_Preamble          Preamble; //ǰ���ֽ�  ����2���ֽ�FEH��������У�飩��
  JingDa_FrameDatas_Struct Datas;
}JingDa_Frame_Struct;

#pragma pack()

//typedef void(*JingDa_AckFunc)(CJT188_SendBuff_Struct* SendData, CJT188_Frame_Struct* AckData)
//��������������
typedef struct{
 CommPortNumber_Type SourcePort;
 unsigned char Code;
 unsigned char ParamLength;
 unsigned char *Param;
}JingDa_CmdDeclarator;


typedef struct {
  union{
    unsigned char Byte;
    struct{
      unsigned char idle :1; //����������
    }Bit;
  }CSR; //����״̬�Ĵ���
 CommPortNumber_Type   SourcePort;
 JingDa_Frame_Struct   Frame; 
}JingDa_OutPutBuff_Struct;
/*********************************************************************************************************
�ⲿ����������
*********************************************************************************************************/

/*********************************************************************************************************
����������
*********************************************************************************************************/
void JingDaSPCP_init(void);

JingDa_OutPutBuff_Struct* getJingDaSendBuff(void);

void freeJingDaSendBuff(void* frame );

SystemErrName JingDa_InPut (void *pbuff, unsigned length,CommPortNumber_Type port);

SystemErrName sendJingDaCmd (CommPortNumber_Type Port,
                             unsigned char Code,
                             unsigned short ParamLength,
                             void *Param);

SystemErrName sendJingDa_WLMQSY (CommPortNumber_Type Port);

SystemErrName JingDa_Send(CommPortNumber_Type Port, JingDa_Frame_Struct* farame);
/********************************************************************************************************/
#endif
/******************************************END********************************************************/