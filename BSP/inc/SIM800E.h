/*********************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
#ifndef  __BSP_SIM800E_H
#define  __BSP_SIM800E_H 

#ifndef  IDLE 
#define  IDLE  0
#endif

#ifndef  INUSE
#define  INUSE  1
#endif
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include "PowerConfig.h"
/*********************************************************************************
�궨����
*********************************************************************************/

#define   GPRS_Revision_ByteNumber   32



#define  GPRS_USART_Config(BaudRate, Parity, StopBits, mod)      Launch_USART2 (BaudRate, Parity, StopBits, mod )                           
#define  GPRS_GetDataForBuf( )                                   GetDataForBuff_USART2 ( )
#define  GPRS_FreeBuff(buff)                                     FreeBuff_USART2(buff)
#define  GPRS_sendBuffByIT(buff,length,callBack,callBack_arg)    sendBuffByIT_USART2 (buff,length,callBack,callBack_arg )
#define  GPRS_sendStringByIT(buff)                               sendStringByIT_USART2 (buff)
#define  GPRS_sendBuffByDMA(buff, length,callBack,callBack_arg)  sendBuffByDMA_USART2 (buff, length,callBack,callBack_arg )
#define  GPRS_sendStringByDMA(buff )                             sendStringByDMA_USART2 (buff )
/*********************************************************************************
�������Ͷ���
*********************************************************************************/
typedef enum{
  Power_OFF = 0,
  Power_ON  = 1,
  GPRS_OK = 2,
  CONNECT_OK = 3,
}SIM800E_STATE_TYPE;

typedef enum{
  No_err           = 0x00,
  CPIN_err         = 0x01<<0,    //SIM ������
  CGREG_err        = 0x01<<1,    //GPRSע�����
  CREG_err         = 0x01<<2,    //GSM����ע�����
  CONNECT_err      = 0x01<<3,    //���������Ӵ������  
}ErrorType_GPRS;

typedef struct{
  unsigned char length;
  unsigned char* pbuff;
}GPRS_BuffStruct;

typedef void (*GPRSLaunch_CallBack)(ErrorType_GPRS  err);
/*********************************************************************************
�ⲿ����������
*********************************************************************************/

/*********************************************************************************
����������
*********************************************************************************/
SystemErrName Launch_GPRS (SIM800E_STATE_TYPE  state,    //Ԥ�ڹ���״̬
                           unsigned char* server_ip,   //Ԥ�����ӷ�������IP��ַ
                           unsigned short server_port,   //Ԥ�����ӵ� 
                           GPRSLaunch_CallBack  CallBack); 
SystemErrName SendString_GPRS (void *p);
 unsigned short GetDataForBuff_GPRS (unsigned char* pbuff, unsigned short pbuffSize);
SystemErrName GPRS_SendBbuff ( void *buff, 
                               unsigned short length,
                               SerialPort_SendEndCallBack_Type callBack,
                               void *callBack_arg) ;
/********************************************************************************/
#endif
